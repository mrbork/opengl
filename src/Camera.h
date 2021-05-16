#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();
	Camera( glm::vec3 startPosition , glm::vec3 startUp , float startYaw , float startPitch , float moveSpeed , float turnSpeed );

	glm::mat4 CalculateViewMatrix();

	void KeyControl( bool* keys , float deltaTime);
	void MouseControl( float deltaX , float deltaY , bool rightMouse);

	float GetViewAnglesX() { return yaw; }
	float GetViewAnglesY() { return pitch; }

private:
	glm::vec3 position;
	glm::vec3 up;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;

	float move;
	float turn;

	void Update();

};

