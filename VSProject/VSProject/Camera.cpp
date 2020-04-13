#include "Camera.h"


/*
Initializes the variables and creates the key map according to the
given keyboard configuration. On azerty keyboards, 'z', 'q', 's', 'd'
and on qwerty keyboards, 'w', 'a', 's', 'd', allow the user to move
in the 3D world with the left hand while controling the mouse with
the right hand. Indeed, these letters are on the left-hand side of
the keyboard and offer a configuration similar to the arrow pad's one.
*/

Camera::Camera(float p_X, float p_Y, float p_Z, float p_psi, float p_theta, float p_rotation_speed, float p_translation_speed, int p_window_width, int p_window_height) :
	X(p_X),
	Y(p_Y),
	Z(p_Z),
	mouse_x(p_window_width / 2),
	mouse_y(p_window_height / 2),
	windowWidth(p_window_width),
	windowHeight(p_window_height),
	psi(p_psi),
	theta(p_theta),
	rotation_speed(p_rotation_speed),
	translation_speed(p_translation_speed),
	time(0) {
	for (int i = 0; i < 255; i++) { keys[i] = false; }
	//key_map[DIR::FORWARD] = static_cast<int>('w');
	//key_map[DIR::BACKWARD] = static_cast<int>('s');
	//key_map[DIR::LEFT] = static_cast<int>('a');
	//key_map[DIR::RIGHT] = static_cast<int>('d');
}

Quaternion Camera::GetLookRot()
{
	Vector3 rot = GetSight() - GetWorldPos();
	//rot += GetWorldPos();
	return Mathe::VectorToQuaternion(rot, false);
}
/*
Computes the new angle values given the mouse direction. The sight's
only limits are when looking up or down: it is impossible for the user to
look at his feets or straight up in the air (these edge cases would need
a special treatment in the gluLookAt function).
*/
void Camera::rotation(int x, int y) {
	theta -= static_cast<float>(x - mouse_x)* rotation_speed;
	psi += static_cast<float>(y - mouse_y)* rotation_speed;
	if (psi <= 0.1f)            psi = 0.1f;
	else if (psi >= 0.95f * M_PI) psi = 0.95f * M_PI;
	mouse_x = x;
	mouse_y = y;
}

/*
Computes the new sphere center given the speed and direction of the camera.
The direction depends on the current angles values, the keys being pushed,
and the elapsed time since the last function call.
*/
void Camera::translation() {
	float t = static_cast<float>(glutGet(GLUT_ELAPSED_TIME) - time);
	time = glutGet(GLUT_ELAPSED_TIME);
	if (keys[static_cast<int>('w')]) {
		X += sin(theta) * sin(psi) * translation_speed * t;
		Y += cos(psi) * translation_speed * t;
		Z += cos(theta) * sin(psi) * translation_speed * t;
	}
	if (keys[static_cast<int>('s')]) {
		X -= sin(theta) * sin(psi) * translation_speed * t;
		Y -= cos(psi) * translation_speed * t;
		Z -= cos(theta) * sin(psi) * translation_speed * t;
	}
	if (keys[static_cast<int>('a')]) {
		X -= sin(theta - M_PI / 2) * sin(psi) * translation_speed * t;
		Z -= cos(theta - M_PI / 2) * sin(psi) * translation_speed * t;
	}
	if (keys[static_cast<int>('d')]) {
		X -= sin(theta + M_PI / 2) * sin(psi) * translation_speed * t;
		Z -= cos(theta + M_PI / 2) * sin(psi) * translation_speed * t;
	}
}

//void Camera::GetInput(int key, int xx, int yy)
//{
//	float fraction = 1.0f;
//
//	std::cout << "moving camera" << std::endl;
//
//	switch (key) 
//	{
//	case GLUT_KEY_LEFT:
//		angle -= 0.01f;
//		viewDirection.x = sin(angle);
//		viewDirection.z = -cos(angle);
//		break;
//	case GLUT_KEY_RIGHT:
//		angle += 0.01f;
//		viewDirection.x = sin(angle);
//		viewDirection.z = -cos(angle);
//		break;
//	case GLUT_KEY_UP:
//		position.x += viewDirection.x * fraction;
//		position.z += viewDirection.z * fraction;
//		break;
//	case GLUT_KEY_DOWN:
//		position.x -= viewDirection.x * fraction;
//		position.z -= viewDirection.z * fraction;
//		break;
//	}
//}
//
//void Camera::KeyPress(char key, int xx, int yy)
//{
//	std::cout << "Key press" << std::endl;
//
//	switch (key)
//	{
//	case 'w':
//		deltaMove += 0.5f;
//		break;
//	case 'a':
//		deltaMove -= 0.5f;
//		break;
//	case 's':
//		deltaAngle -= 0.01f;
//		break;
//	case 'd':
//		deltaAngle += 0.01f;
//		break;
//	}
//}
//
//void Camera::KeyRelease(char key, int x, int y)
//{
//	std::cout << "Key release" << std::endl;
//
//	switch (key)
//	{
//	case 'w':
//		deltaMove = 0;
//		break;
//	case 'a':
//		deltaMove = 0;
//		break;
//	case 's':
//		deltaAngle = 0;
//		break;
//	case 'd':
//		deltaAngle = 0;
//		break;
//	}
//}
//
//void Camera::CheckDeltas()
//{
//	if (deltaMove)
//	{
//		position.x += deltaMove * viewDirection.x * 0.1f;
//		position.z += deltaMove * viewDirection.z * 0.1f;
//	}
//	if (deltaAngle)
//	{
//		angle += deltaAngle;
//		viewDirection.x = sin(angle);
//		viewDirection.z = -cos(angle);
//	}
//}
//
//void Camera::LookAt()
//{
//	gluLookAt(position.x, 1.0, position.z,
//		position.x + viewDirection.x, 1.0, position.z + viewDirection.z,
//		0.0, 1.0, 0.0);
//}
