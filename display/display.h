#ifndef DISPLAY_INCLUDED_H
#define DISPLAY_INCLUDED_H


#include <GLFW/glfw3.h>
#include <string>

class Display
{
	bool isFullScreen;
public:
	Display(int width, int height, const std::string& title);

	void Clear(float r, float g, float b, float a);
	void SwapBuffers();
	bool IsFullscreen(  );

	virtual ~Display();
//protected:
//private:
	void operator=(const Display& display) {}
	Display(const Display& display) {}

	GLFWwindow* m_window;
	int error;
};

#endif
