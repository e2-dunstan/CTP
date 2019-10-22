#include "Engine.h"


Engine::Engine()
{
}

void Engine::Init()
{
	obj->Create(Object::Type::CUBE, Vector3(1, 1, 1), Vector3(0, 0, -10), Vector3(30, 30, 30));
}

void Engine::Update()
{
	if (objectsInitialised) return;

	//obj->Create(CUBE, Vector3(1, 1, 1), Vector3(0, 0, -10), Vector3(30, 30, 30));

	objectsInitialised = true;
}

void Engine::Render()
{
	//glLoadIdentity();

	//Floor
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.2);
	glColor3f(0.8f, 0.8f, 0.8f);
	glBegin(GL_QUADS);
		glVertex3f(-100.0f, -10.0f, -100.0f);
		glVertex3f(-100.0f, -10.0f, 100.0f);
		glVertex3f(100.0f, -10.0f, 100.0f);
		glVertex3f(100.0f, -10.0f, -100.0f);
	glEnd();

	obj->Draw();
}

void Engine::Animate()
{
	obj->Animate();
}
