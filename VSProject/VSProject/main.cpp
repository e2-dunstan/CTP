#include <Windows.h>

#include "Engine.h"

Engine* engine = new Engine();


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

	std::cout << "Initialising GLUT..." << std::endl;

	// Initialize GLUT
	glutInit(&argc, argv);
	// Set up some memory buffers for our display
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	// Set the window size
	glutInitWindowSize(1280, 720);

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

	engine->Update();

	glutMainLoop();
	return 0;
}