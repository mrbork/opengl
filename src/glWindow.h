#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

class glWindow
{
public:
	glWindow();
	glWindow( unsigned int width , unsigned int height );

	void Initialize();

	bool ShouldClose() { return glfwWindowShouldClose( mainWindow ); }

	void SwapBuffers() { glfwSwapBuffers( mainWindow ); }

	void Clear() { glfwTerminate(); }

private:
	GLFWwindow* mainWindow;

	unsigned int bufferWidth;
	unsigned int bufferHeight;
};

