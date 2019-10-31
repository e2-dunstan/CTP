#include "Engine.h"


Engine::Engine()
{
}

void Engine::Init()
{
	obj->Create(RigidBody::Type::CUBE, Vector3(1.5, 1.5, 1.5), Vector3(5, 3, 5), Vector3(0, 45, 45));
	obj->Create(RigidBody::Type::CUBE, Vector3(3, 1, 1), Vector3(0, 1, 10), Vector3(0, 30, 0));
	obj->Create(RigidBody::Type::CUBE, Vector3(1, 1, 1), Vector3(10, 1, 0), Vector3(0, 0, 0));
}

void Engine::Update()
{
	obj->Update();
}

void Engine::Render()
{
	//glLoadIdentity();

	//Floor
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.2);
	glColor3f(0.8f, 0.8f, 0.8f);
	glBegin(GL_QUADS);
		glVertex3f(-64.0f, 0.0f, -64.0f);
		glVertex3f(-64.0f, 0.0f, 64.0f);
		glVertex3f(64.0f, 0.0f, 64.0f);
		glVertex3f(64.0f, 0.0f, -64.0f);
	glEnd();

	obj->Draw();
}

void Engine::Animate()
{
	obj->Animate();
}
