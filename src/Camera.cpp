#include "Camera.h"

Camera::Camera()
{
	position = glm::vec3( 0.0f , 0.0f , 0.0f );
	worldUp = glm::vec3( 0.0f , 1.0f , 0.0f );
	yaw = 0.0f;
	pitch = 0.0f;
	front = glm::vec3( 0.f , 0.f , -1.f );

	move = 1.0f;
	turn = 0.5f;

	right = glm::vec3( 0.0f , 0.0f , 0.0f );
	up = glm::vec3( 0.0f , 0.0f , 0.0f );
}

Camera::Camera( glm::vec3 startPosition , glm::vec3 startUp , float startYaw , float startPitch , float moveSpeed , float turnSpeed )
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3( 0.f , 0.f , -1.f );

	move = moveSpeed;
	turn = turnSpeed;

	Update();
}

glm::mat4 Camera::CalculateViewMatrix()
{
	return glm::lookAt( position , position + front , up );
}

void Camera::KeyControl( bool* keys , float deltaTime )
{

	if ( keys[ GLFW_KEY_LEFT_SHIFT ] )
		move = 4;
	else
		move = 1;

	float velocity = move * deltaTime;

	if ( keys[ GLFW_KEY_W ] )
		position += front * velocity;
	if ( keys[ GLFW_KEY_S ] )
		position -= front * velocity;
	if ( keys[ GLFW_KEY_A ] )
		position -= right * velocity;
	if ( keys[ GLFW_KEY_D ] )
		position += right * velocity;
	if ( keys[ GLFW_KEY_SPACE ] )
		position += up * velocity;
	if ( keys[ GLFW_KEY_LEFT_CONTROL ] )
		position -= up * velocity;

}

void Camera::MouseControl( float deltaX , float deltaY )
{
	deltaX *= turn;
	deltaY *= turn;

	yaw += deltaX;
	pitch += deltaY;

	pitch = glm::clamp( pitch , -89.f , 89.f );

	Update();

}

void Camera::Update()
{
	front.x = cos( glm::radians( pitch ) ) * cos( glm::radians( yaw ) );
	front.y = sin( glm::radians( pitch ) );
	front.z = cos( glm::radians( pitch ) ) * sin( glm::radians( yaw ) );
	front = glm::normalize( front );

	right = glm::normalize( glm::cross( front , worldUp ) );
	up = glm::normalize( glm::cross( right , front ) );
}