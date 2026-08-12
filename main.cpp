#include "consoleEngine3D.h"
#include <string>
#include <print>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;
// make&&clear&&./main.out 

#define TERM_COLS 76
#define TERM_ROWS 40


Engine engine;

/* defining the 3D cube, which consists of triangles & vertices */
struct Vec3 {
    float x, y, z;
    Vec3(const float &xi, const float &yi, const float &zi)
	: x(xi), y(yi), z(zi) {}
};

struct Vec2 {
    float x, y;
    Vec2(const float &xi, const float &yi)
	: x(xi), y(yi) {}
};

vector<Vec3> vertices {
    Vec3(-1, -1, -1), // 0
    Vec3(-1,  1, -1), // 1
    Vec3( 1,  1, -1), // 2
    Vec3( 1, -1, -1), // 3
    Vec3( 1,  1,  1), // 4
    Vec3( 1, -1,  1), // 5
    Vec3(-1, -1,  1), // 6
    Vec3(-1,  1,  1), // 7
};

vector<vector<int>> tringles = {
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

void drawCube()
{

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
	char* pSymbol = &symbol;
	string s(pSymbol, 1); // conversion function requires a ptr param
	engine.print(s, i, y);
    }
}


void drawFlatBottomTriangle(Vec2 top, Vec2 bottom1, Vec2 bottom2)
{
    Vec2 pStart = top;
    Vec2 pEnd = top;
    while (pStart.y != bottom1.y) {

	drawScanLine(pStart.y, pStart.x, pEnd.x, '*');

	/* modify pStart */
	pStart.y += 1;
	pStart.x += (bottom1.x - top.x) / (bottom1.y - top.y);

	/* modify pEnd */
	pEnd.y += 1;
	pEnd.x += (bottom2.x - top.x) / (bottom2.y - top.y);
    }
    drawScanLine(pStart.y, pStart.x, pEnd.x, '*');
}

void drawFlatTopTriangle(Vec2 bottom, Vec2 top1, Vec2 top2)
{
    Vec2 pStart = bottom;
    Vec2 pEnd = bottom;
    while (pStart.y != top1.y) {

	drawScanLine(pStart.y, pStart.x, pEnd.x, '*');

	/* modify pStart */
	pStart.y -= 1;
	pStart.x -= (bottom.x - top1.x) / (bottom.y - top1.y);

	/* modify pEnd */
	pEnd.y -= 1;
	pEnd.x -= (bottom.x - top2.x) / (bottom.y - top2.y);
    }
}

void drawTriangle(Vec2 v0, Vec2 v1, Vec2 v2)
{
    /* compute the middle point using right angled triangles */
    Vec2 midpoint = {0, 0};
    midpoint.y = v1.y;

    midpoint.x = v0.x + (v2.x - v0.x) / (v2.y - v0.y) * (v1.y - v0.y);

    /* split the triangle into 2 and use both functions to draw it */
    drawFlatBottomTriangle(v0, midpoint, v1);
    drawFlatTopTriangle(v2, midpoint, v1);
}


int main()
{
    engine.setCanonicalAndCursor(0);
    engine.clearScreen();
    Vec2 v0 = {5, 5};
    Vec2 v1 = {15, 15};
    Vec2 v2 = {10, 30};

    drawTriangle(v0, v1, v2);
    print("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

    engine.setCanonicalAndCursor(1);
    return 1;
}


	//    array<array<char, TERM_COLS>, TERM_ROWS> screen {};
	//    fill(&screen[0][0], &screen[0][0] + sizeof(screen), ' ');
	//    int screenBuffer[TERM_ROWS * TERM_COLS] = {};
	//
	//    array<char, 4> foo= {'q', 'w', 'e', 'r'};
	//    int count = 0;
	//    while (true) {
	// if (count == 5) break;
	// // update
	// engine.clearScreen();
	// screen[0][0] = foo[count];
	// for (const auto &row: screen)
	// {
	//     for (const auto &col : row) print("{}", col);
	//     print("\n");
	// }
	// count++;
	// engine.delay(1000000, 0);
	//    }
