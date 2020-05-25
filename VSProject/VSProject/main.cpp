#pragma once
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <thread>

#include "Engine.h"
#include "Camera.h"
#include "ConsoleControls.h"

namespace
{
	//Camera variables.
	const float rotationSpeed = 0.005f;
	const float translationSpeed = 0.01f;
	const int windowWidth = 1280;
	const int windowHeight = 720;

	GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat light_ambient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	//GLfloat light_specular[] = { 0.5, 0.5, 0.9, 1.0 };
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };

	bool mouseHeld = false;

	std::unique_ptr<Engine> engine = std::make_unique<Engine>();
	std::unique_ptr<Camera> camera = std::make_unique<Camera>(0, 4, 0, Mathe::ToRadians(80), 0, rotationSpeed, translationSpeed, windowWidth, windowHeight);
	std::unique_ptr<ConsoleControls> consoleControls = std::make_unique<ConsoleControls>(/*engine.get()*/);

	//in miliseconds
	unsigned int timeSinceStart = 0;
	unsigned int oldTimeSinceStart = 0;

	bool beginUpdate = false;
	//double deltaTimeDebug = 0;

	int numPhysicsUpdatesPerSecond = 0;

	const float playbackSpeed = 1.0f;

	const bool sfmlWindow = false;
	const bool openglWindow = true;

	bool pathTracerActive = false;
}

void PressKey(unsigned char key, int xx, int yy)
{
	camera->setKeyboard(key, true);
}
void ReleaseKey(unsigned char key, int x, int y)
{
	camera->setKeyboard(key, false);

	consoleControls->OnKeyRelease(key, engine.get());

	if (key == 'p') Global::shouldUpdate = !Global::shouldUpdate;
	if (key == 'c') Global::writeDataToFile = true;
	if (key == 'r')
	{
		pathTracerActive = !pathTracerActive;
		//if (pathTracerActive) engine->UpdateTrisForRayCamera();
	}
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
	//Testing
	/*
	else if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_UP)
		{
			ScreenToWorldSpace(x, y, windowWidth, windowHeight, camera->GetWorldPos(), camera->GetLookRot());
		}
	}*/
}

void Init()
{
	std::cout << "Calling Init functions... ";

	//Delta time.
	timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	Global::deltaTime = (timeSinceStart - oldTimeSinceStart) / 1000.0f;
	oldTimeSinceStart = timeSinceStart;

	//Initialise OpenGL

	glClearColor(Global::skyColour.x, Global::skyColour.y, Global::skyColour.z, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80, (double)windowWidth / (double)windowHeight, 1, 1000);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	//Lighting will be fully implemented later.
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

	//Init physics engine.
	engine->Init();

	std::cout << "Done!" << std::endl << std::endl;

	consoleControls->Init();
}

void changeViewPort(int w, int h)
{
	Global::deltaTime = 0;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80, (double)w / (double)h, 1, 1000);
	glMatrixMode(GL_MODELVIEW);

	Global::deltaTime = 0;
}

void render()
{	
	//Clear the buffers.
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
	//Delta time.
	timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	Global::deltaTime = (float)(timeSinceStart - oldTimeSinceStart) * playbackSpeed / 1000.0f;
	oldTimeSinceStart = timeSinceStart;

	if (!beginUpdate && Global::shouldUpdate && timeSinceStart > 3000) //wait 3 seconds before updating
		beginUpdate = true;

	if (pathTracerActive && engine->rayCamera->ShouldDrawNextFrame())
	{
		engine->UpdateTrisForRayCamera();
		engine->rayCamera->CastRays(camera->GetWorldPos(), windowWidth, windowHeight);
	}

	glutPostRedisplay();

	if (beginUpdate && Global::shouldUpdate && Global::deltaTime < 1.0f)
	{
		engine->Update();
	}
	//else std::cout << engine->primitiveManager->GetPrimitives().size() << std::endl;

	glutTimerFunc(0, timer, 0);
}


void SFMLMainLoop()
{
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML");
	std::cout << "Successful!" << std::endl;
	
	unsigned char* pixels = new unsigned char[windowWidth * windowHeight * 3];
	std::basic_ifstream<unsigned char> file("PixelBuffer.bin");
	file.read(pixels, windowWidth * windowHeight * 3);
	file.close();
	unsigned int pixelIndex = 0;

	sf::Image img;
	img.create(windowWidth, windowHeight);
	for (unsigned int w = 0; w < windowWidth; w++)
	{
		for (unsigned int h = 0; h < windowHeight; h++)
		{
			sf::Uint8 r = pixels[pixelIndex++];
			sf::Uint8 g = pixels[pixelIndex++];
			sf::Uint8 b = pixels[pixelIndex++];
			img.setPixel(w, h, sf::Color(r, g, b, 255));
		}
	}

	img.saveToFile("PixelBufferSFML");

	window.close();
	/*
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		//window.draw(img);
		window.display();
	}*/
}

int main(int argc, char* argv[]) 
{
	if (sfmlWindow)
	{
		std::cout << "Initialising SFML... ";

		SFMLMainLoop();
		//sf::CircleShape shape(100.f);
		//shape.setFillColor(sf::Color::Green);
	}
	if (openglWindow)
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		std::cout << "Initialising GLUT... ";

		// Initialize GLUT
		glutInit(&argc, argv);
		// Set up some memory buffers for our display
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
		// Set the window size
		glutInitWindowSize(windowWidth, windowHeight);

		glutCreateWindow("Rigidbody Dynamics Engine with an Integrated Path Tracer ");

		glutReshapeFunc(changeViewPort);
		glutDisplayFunc(render);

		glutTimerFunc(0, timer, 0);
		//glutTimerFunc(0, physicsUpdateCounter, 0);

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

		//https://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/loading.php
	}

	_CrtDumpMemoryLeaks();
	return 0;
}
