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

/* defining the 3D cube, which consists of triangles & vertices */
struct Vec3 {
    float x, y, z;
    Vec3(float &xi, float &yi, float &zi)
	: x(xi), y(yi), z(zi) {}
};

struct Vec2 {
    float x, y;
    Vec2(float &xi, float &yi)
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
}

vector<vector<int>> tringles {
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
}

void drawCube()
{

}

void drawTriangle(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2)
{

}





int main()
{
    Engine engine;
    engine.setCanonicalAndCursor(0);
    
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
    
    engine.clearScreen();

    engine.setCanonicalAndCursor(1);
    return 1;
}
