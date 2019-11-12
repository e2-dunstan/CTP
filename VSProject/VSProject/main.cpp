#include <Windows.h>

#include "Engine.h"
#include "Camera.h"


// More cubes
// Shaders
// Read book on separating axis theorem, arbitrary aligned cubes
// Dont do axis align for now


float rotationSpeed = 0.005f;
float translationSpeed = 0.01f;	
int windowWidth = 1280;
int windowHeight = 720;

GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
GLfloat light_ambient[] = { 0.8, 0.8, 0.8, 1.0 };
GLfloat light_specular[] = { 0.5, 0.5, 0.9, 1.0 };
GLfloat light_position[] = { 0.0, 10.0, 0.0, 1.0 };

bool mouseHeld = false;

Engine* engine = new Engine();
Camera* camera = new Camera(Camera::QWERTY, 0, 10, 0, 3.14159265f / 2.0f, 0, rotationSpeed, translationSpeed, windowWidth, windowHeight);

//in miliseconds
unsigned int timeSinceStart = 0;
unsigned int oldTimeSinceStart = 0;
int deltaTime = 0;


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
	if (mouseHeld)
	{
		camera->rotation(x, y);
	}
}
void MouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN && mouseHeld == false)
		{
			camera->setMouse(x, y);
			mouseHeld = true;
		}
		else if (state == GLUT_UP && mouseHeld == true)
		{
			mouseHeld = false;
		}
	}
}

void Init()
{
	std::cout << "Calling Init functions... " << std::endl;

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80, (double)windowWidth / (double)windowHeight, 1, 1000);

	//glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glMatrixMode(GL_MODELVIEW);

	engine->Init();

	std::cout << "Done!" << std::endl;
}

void changeViewPort(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80, (double)w / (double)h, 1, 50);
	glMatrixMode(GL_MODELVIEW);
}

void render()
{
	timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = timeSinceStart - oldTimeSinceStart;
	oldTimeSinceStart = timeSinceStart;

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

	//Be careful with this
	engine->Update(deltaTime);
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

	// Create the window
	glutCreateWindow("Rigidbody Dynamics Engine with an Integrated Path Tracer ");

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

	//this is not called every frame
	engine->Update(deltaTime);

	glutMainLoop();
	return 0;
}
