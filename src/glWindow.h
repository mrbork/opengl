#include "pch.h"

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
	GLfloat GetFov() { return fov; }
	
	bool* GetKeys() { return keys; }
	bool* GetButtons() { return buttons; }

	GLFWwindow* GetWindow() { return mainWindow; }


private:
	GLFWwindow* mainWindow;

	unsigned int bufferWidth;
	unsigned int bufferHeight;

	float lastx , lasty;
	float deltax , deltay;
	bool firstMove;

	float fov;

	bool keys[ 1024 ] = { 0 };
	bool buttons[ 8 ] = { 0 };

	void CreateCallbacks();

	static void HandleInput( GLFWwindow* window , int key , int scancode , int action , int mods );
	static void HandeCursor( GLFWwindow* window , double xPos , double yPos );
	static void HandleScroll( GLFWwindow* window , double xoffset , double yoffset );
	static void HandleButton( GLFWwindow* window , int button , int action , int mods );
};

