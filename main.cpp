#include "consoleEngine3D.h"
#include <string>
#include <print>
using namespace std;
// make&&clear&&./main.out 

int main()
{
    Engine engine;
    
    engine.clearScreen();
    engine.setCanonicalAndCursor(0);
    engine.print("hi", 10, 40);
    engine.setCanonicalAndCursor(1);
    
    print("\n");
    return 1;
}
