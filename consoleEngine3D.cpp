#include "consoleEngine3D.h"
#include <stdio.h>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <print>
#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ
#include <chrono> // for delay
#include <thread> // for delay


/* terminal methods */
void Engine::setCanonicalAndCursor(const int &cmd)
{
    // toggle canonical mode
    tcgetattr(STDIN_FILENO, &tio);
    cmd == 1 ? 
	tio.c_iflag &= (~ICANON & ~ECHO):
	tio.c_iflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &tio);

    // toggle cursor (show or not show)
    cmd == 1 ? 
	std::print("\033[?25h"):
	std::print("\033[?25l");
}
float Engine::getTerminalHeight()
{
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return (float)size.ws_row;
}
float Engine::getTerminalWidth()
{
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return (float)size.ws_col;
}

void Engine::delay(const int &time, const int &is_thread)
{
    if (is_thread == 1)
	std::this_thread::sleep_for(std::chrono::milliseconds(time));
    else
	usleep(time);
}
/* rendering methods */
void Engine::print(const std::string &input, const int &x, const int &y)
{
    std::print("\033[{};{}H", y, x); // reversed cuz the syntax is {ROW};{COL}
    std::print("{}", input);
}
void Engine::clearScreen()
{
    std::print("\033[2J"); // clear screen
    std::print("\033[1;1H"); // move cursor to (1,1)
}
