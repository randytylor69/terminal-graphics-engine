#include "consoleEngine3D.h"
#include <string>
#include <print>
#include <thread>
#include <vector>
#include <unistd.h>
#include <cmath>
#include <algorithm>
using namespace std;
// make&&clear&&./main.out 

#define TERM_COLS 76
#define TERM_ROWS 40


Engine engine;
string symbols = "..++@@**--==";
bool isFinished = false;


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
	Vec3 v = engine.getVectorBetweenTwoVertices(transformed3DVertices[0],transformed3DVertices[1]);
	Vec3 w = engine.getVectorBetweenTwoVertices(transformed3DVertices[0],transformed3DVertices[2]);
	Vec3 normalVector = engine.getCrossProduct(v, w);
	/*   - then, get the cross product */
	float crossProduct = engine.getDotProduct(normalVector, eyeVector);
	if (crossProduct >= 0) continue;
    
	/* Project 3D to 2D */
	Vec2 transformed2DVertices[3];
	for (int i=0; i<3; i++) 
	    transformed2DVertices[i] = project3DTo2D(transformed3DVertices[i]);
	/* Draw transformed triangle */
	engine.drawTriangle(
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

	usleep(100000);
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

    engine.clearScreen();
    printf("Program Finished.\n");
    engine.setCanonicalAndCursor(1);
    return 0;
}


