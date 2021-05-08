#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

class glWindow
{
public:
	glWindow();
	glWindow( unsigned int width , unsigned int height );
	~glWindow();

	void Initialize();

	bool ShouldClose() { return glfwWindowShouldClose( mainWindow ); }

	void SwapBuffers() { glfwSwapBuffers( mainWindow ); }

	GLfloat GetBufferWidth() { return bufferWidth; }
	GLfloat GetBufferHeight() { return bufferHeight; }
	GLfloat GetDeltaX();
	GLfloat GetDeltaY();
	
	int* GetKeys() { return keys; }


private:
	GLFWwindow* mainWindow;

	unsigned int bufferWidth;
	unsigned int bufferHeight;

	float lastx , lasty;
	float deltax , deltay;
	bool firstMove;

	int keys[ 1024 ] = { 0 };

	void CreateCallbacks();

	static void HandleInput( GLFWwindow* window , int key , int scancode , int action , int mods );
	static void HandeCursor( GLFWwindow* window , double xPos , double yPos );
};

