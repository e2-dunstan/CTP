#include <Windows.h>

#include "Engine.h"
#include "InputManager.h"

float rotationSpeed = 0.005f;
float translationSpeed = 0.01f;	
int windowWidth = 1280;
int windowHeight = 720;

bool mouseHeld = false;

Engine* engine = new Engine();
InputManager* inputManager = new InputManager();
Camera* camera = new Camera(Camera::QWERTY, 0, 10, 0, 1, 1, rotationSpeed, translationSpeed, windowWidth, windowHeight);


void PressKey(unsigned char key, int xx, int yy)
{
	camera->setKeyboard(key, true);
}
void ReleaseKey(unsigned char key, int x, int y)
{
	camera->setKeyboard(key, false);
}
void MouseMove(int x, int y)
{
	if (inputManager->mouseHeld)
	{
		camera->rotation(x, y);
	}
}
void MouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN && inputManager->mouseHeld == false)
		{
			camera->setMouse(x, y);
			inputManager->mouseHeld = true;
		}
		else if (state == GLUT_UP && inputManager->mouseHeld == true)
		{
			inputManager->mouseHeld = false;
		}
	}
}

void Init()
{
	std::cout << "Calling Init functions... ";

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	engine->Init();

	std::cout << "Done!" << std::endl;
}

void changeViewPort(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (double)w / (double)h, 1, 50);
	glMatrixMode(GL_MODELVIEW);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	//camera->LookAt();
	camera->translation();
	gluLookAt(camera->getX(), camera->getY(), camera->getZ(),
		camera->getSightX(), camera->getSightY(), camera->getSightZ(),
		0, 1, 0);

	engine->Render();

	glutSwapBuffers();
}

void timer(int)
{
	glutPostRedisplay();

	engine->Animate();

	//60 fps
	glutTimerFunc(1000 / 60, timer, 0);
}



int main(int argc, char* argv[]) {

	std::cout << "Initialising GLUT... ";

	// Initialize GLUT
	glutInit(&argc, argv);
	// Set up some memory buffers for our display
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	// Set the window size
	glutInitWindowSize(windowWidth, windowHeight);

	// Create the window with the title "Hello,GL"
	glutCreateWindow("Rigidbody Dynamics Engine with an Integrated Path Tracer ");
	// Bind the two functions (above) to respond when necessary
	glutReshapeFunc(changeViewPort);
	glutDisplayFunc(render);

	glutTimerFunc(0, timer, 0);

	// Very important!  This initializes the entry points in the OpenGL driver so we can 
	// call all the functions in the API.
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}

	std::cout << "Successful!" << std::endl;

	Init();

	glutKeyboardFunc(PressKey);
	glutKeyboardUpFunc(ReleaseKey);

	//press and release
	//glutPassiveMotionFunc(MouseMove);
	glutMotionFunc(MouseMove);
	glutMouseFunc(MouseButton);

	engine->Update();

	glutMainLoop();
	return 0;
}
