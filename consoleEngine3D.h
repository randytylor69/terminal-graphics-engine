#ifndef CONSOLE_ENGINE_3D_H_
#define CONSOLE_ENGINE_3D_H_

#include <termios.h>
#include <string>
class Engine
{
    private:
	termios tio;
    public:
	/* terminal methods */
	void setCanonicalAndCursor(const int &cmd);
	float getTerminalHeight();
	float getTerminalWidth();
	void delay(const int &time, const int &is_thread); // timeout for x seconds
	/* rendering methods */
	void print(const std::string &input, const int &x, const int &y);
	void clearScreen();

};


#endif
