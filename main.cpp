#include "consoleEngine3D.h"
#include <string>
#include <print>
#include <thread>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;
// make&&clear&&./main.out 

#define TERM_COLS 76
#define TERM_ROWS 40


Engine engine;
string symbols = "..++@@**--==";
bool isFinished = false;

/* defining the 3D cube, which consists of triangles & vertices */
struct Vec3 {
    float x, y, z;
    Vec3() {}
    Vec3(const float &xi, const float &yi, const float &zi)
	: x(xi), y(yi), z(zi) {}
};

struct Vec2 {
    float x, y;
    Vec2() {}
    Vec2(const float &xi, const float &yi)
	: x(xi), y(yi) {}
};

vector<Vec3> cubeVertices {
    Vec3(-1, -1, -1), // 0
    Vec3(-1,  1, -1), // 1
    Vec3( 1,  1, -1), // 2
    Vec3( 1, -1, -1), // 3
    Vec3( 1,  1,  1), // 4
    Vec3( 1, -1,  1), // 5
    Vec3(-1, -1,  1), // 6
    Vec3(-1,  1,  1), // 7
};

vector<vector<int>> cubeTriangles = {
    /* each int represents a vertex, 
     * indexed exactly as the members 
     * of "vertices" above */

    // front
    {0, 1, 2}, 
    {0, 2, 3},
    // right
    {3, 2, 4},
    {3, 4, 5},
    // back
    {5, 4, 7},
    {5, 7, 6},
    // left
    {6, 7, 1},
    {6, 1, 0},
    // top
    {6, 0, 3},
    {6, 3, 5},
    // bottom
    {1, 7, 4},
    {1, 4, 2}
};

Vec2 project3DTo2D(Vec3 v)
{
    /* note that not only should the x & y coordinates be scaled by z, 
     * we should also push the cube into the middle of the screen */
    return Vec2(
	round(v.x/v.z + TERM_COLS/2), 
	round(v.y/v.z + TERM_ROWS/2)
    );
}




/* draws a straight horizontal line between two points, x0 and x1. */
void drawScanLine(const int &y, const int &x0, const int &x1, char symbol)
{
    // first we need to figure out the start and the end
    int left {x0};
    int right {x1};
    if (left > right)
    {
	left = x1; 
	right = x0;
    }
    // then draw the line
    for (int i = left; i <= right; i++)
    {
	string s(1, symbol); 
	engine.print(s, i, y);
    }
}


void drawFlatBottomTriangle(Vec2 top, Vec2 bottom1, Vec2 bottom2, char symbol)
{
    Vec2 pStart = top;
    Vec2 pEnd = top;
    while (pStart.y != bottom1.y) {

	drawScanLine(pStart.y, pStart.x, pEnd.x, symbol);

	/* modify pStart */
	pStart.y += 1;
	pStart.x += (bottom1.x - top.x) / (bottom1.y - top.y);

	/* modify pEnd */
	pEnd.y += 1;
	pEnd.x += (bottom2.x - top.x) / (bottom2.y - top.y);
    }
    drawScanLine(pStart.y, pStart.x, pEnd.x, symbol);
}

void drawFlatTopTriangle(Vec2 bottom, Vec2 top1, Vec2 top2, char symbol)
{
    Vec2 pStart = bottom;
    Vec2 pEnd = bottom;
    while (pStart.y != top1.y) {

	drawScanLine(pStart.y, pStart.x, pEnd.x, symbol);

	/* modify pStart */
	pStart.y -= 1;
	pStart.x -= (bottom.x - top1.x) / (bottom.y - top1.y);

	/* modify pEnd */
	pEnd.y -= 1;
	pEnd.x -= (bottom.x - top2.x) / (bottom.y - top2.y);
    }
    drawScanLine(pStart.y, pStart.x, pEnd.x, symbol);
}

void drawTriangle(Vec2 inputV0, Vec2 inputV1, Vec2 inputV2, char symbol)
{
    /* first sort the vertices by ascending Y */
    vector<Vec2> sortedVertices = {inputV0, inputV1, inputV2};
    sort(sortedVertices.begin(), sortedVertices.end(), 
	[](const auto &L, const auto &R) {return L.y < R.y;}
    );
    Vec2 v0 = sortedVertices[0];
    Vec2 v1 = sortedVertices[1];
    Vec2 v2 = sortedVertices[2];

    /* compute the middle point using right angled triangles */
    Vec2 midpoint = {0, 0};
    midpoint.y = v1.y;

    midpoint.x = v0.x + (v2.x - v0.x) / (v2.y - v0.y) * (v1.y - v0.y);

    /* split the triangle into 2 and use both functions to draw it */
    drawFlatBottomTriangle(v0, midpoint, v1, symbol);
    drawFlatTopTriangle(v2, midpoint, v1, symbol);
}

Vec3 rotate(Vec3 v, char axis, float rad) 
{
    if (axis == 'x'){
	return Vec3(
		v.x,
		v.y*cos(rad) - v.z*sin(rad),
		v.y*sin(rad) + v.z*cos(rad));
    }
    else if (axis=='y'){
	return Vec3(
	    cos(rad)*v.x + sin(rad)*v.z,
	    v.y,
	    -sin(rad)*v.x + cos(rad)*v.z);
    } else {
	return Vec3(
		cos(rad)*v.x - sin(rad)*v.y,
		sin(rad)*v.x + cos(rad)*v.y,
		v.z);
    }
}

Vec3 getVectorBetweenTwoPoints(Vec3 v, Vec3 w)
{
    return Vec3(
	    w.x - v.x,
	    w.y - v.y,
	    w.z - v.z);
}

Vec3 getCrossProduct(Vec3 v, Vec3 w)
{
    return Vec3(
	v.y * w.z - v.z * w.y,
	-(v.x * w.z - v.z * w.x),
	v.x * w.y- v.y * w.x
    );

}

float getDotProduct(Vec3 v, Vec3 w)
{
    return v.x * w.x + v.y * w.y + v.z * w.z;
}


void drawCube(float radX, float radY, float radZ)
{	
    int symbolIndex = 0;
    Vec3 eyeVector {0,0,1}; // shoots from eye to screen, used for back-face culling

    for (auto triangle : cubeTriangles) 
    {
	Vec3 transformed3DVertices[3]; // create a copy of current triangle's vertices
	for (int i=0; i<3; i++)
	{
	    transformed3DVertices[i] = cubeVertices[triangle[i]];

	    /* Transform each one of the 3 vertices */

	    /* 0. Rotate each vertedx */
	    transformed3DVertices[i] = rotate(transformed3DVertices[i], 'x', radX);
	    transformed3DVertices[i] = rotate(transformed3DVertices[i], 'y', radY);
	    transformed3DVertices[i] = rotate(transformed3DVertices[i], 'z', radZ);

	    /* 1. Push it into the screen */
	    transformed3DVertices[i].z += 10;

	    /* 2. Scale it, but only the x and y, not z. And not by the same factor,
	     *    because a character's height > its width */
	    int scaleFactor {80};
	    transformed3DVertices[i].y *= scaleFactor;
	    transformed3DVertices[i].x *= scaleFactor * 3;
	    
	}
	/* Back-face culling. skip current triangle based on dot product. */
	/*   - first, get the normal vector */
	Vec3 v = getVectorBetweenTwoPoints(transformed3DVertices[0],transformed3DVertices[1]);
	Vec3 w = getVectorBetweenTwoPoints(transformed3DVertices[0],transformed3DVertices[2]);
	Vec3 normalVector = getCrossProduct(v, w);
	/*   - then, get the cross product */
	float crossProduct = getDotProduct(normalVector, eyeVector);
	if (crossProduct >= 0) continue;
    
	/* Project 3D to 2D */
	Vec2 transformed2DVertices[3];
	for (int i=0; i<3; i++) 
	    transformed2DVertices[i] = project3DTo2D(transformed3DVertices[i]);
	/* Draw transformed triangle */
	drawTriangle(
	    transformed2DVertices[0], 
	    transformed2DVertices[1], 
	    transformed2DVertices[2],
	    symbols[symbolIndex]
	);

	symbolIndex++;
    }
}

void loop(float radX, float radY, float radZ){

    while(!isFinished){
	engine.clearScreen();
	drawCube(radX, radY, radZ);

	/* update the rotational radians */
	radX = fmod(radX + 0.2, 2*numbers::pi);
	radY = fmod(radY + 0.2, 2*numbers::pi);
	radZ = fmod(radZ + 0.2, 2*numbers::pi);

	engine.delay(100000, 0);
    }
}
int main()
{
    engine.setCanonicalAndCursor(0);
    
    float radX = 0.0;
    float radY = 0.0;
    float radZ = 0.0; 

    thread worker(loop, ref(radX), ref(radY), ref(radZ));
    char ch;
    while (true){
	ch = getchar();
	if (ch == 'q')
	{
	    isFinished = true;
	    break;
	}
    }
    worker.join();

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    engine.setCanonicalAndCursor(1);
    
}


