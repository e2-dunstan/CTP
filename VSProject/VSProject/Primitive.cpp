#include "Primitive.h"

void Primitive::Update()
{	
	rigidbody.UpdatePhysics();
	if (rigidbody.velocity != Vector3())
	{
		rigidbody.velocity.DebugOutput();
		translation += rigidbody.velocity;
		updateTransform = true;
	}

	//Update transforms if they have changed.
	if (updateTransform) UpdateTransform();

	//rigidbody.CalculateVelocity(translation);
}

void Primitive::Draw()
{
	glBegin(GetDrawType(type));

	//Only locally transform the vertices. This makes it so that each draw call
	//the vertices do not have to be cleared and redefined if their transforms have
	//changed. Performance boost.
	std::vector<Vertex> verts = vertices;
	for (int v = 0; v < verts.size(); v++)
	{
		Mathe::Transform(verts[v].position, transform);

		glNormal3f(verts[v].normal.x, verts[v].normal.y, verts[v].normal.z);

		//For future shader use.
		//GLfloat colour[] = { objects[i].vertices[v].colour.r, objects[i].vertices[v].colour.g, objects[i].vertices[v].colour.b,  objects[i].vertices[v].colour.a};
		//glMaterialfv(GL_FRONT, GL_DIFFUSE, colour);
		if (colliding)
		{
			glColor3f(1, 0, 0);
		}
		else
		{
			glColor3f(verts[v].colour.r, verts[v].colour.g, verts[v].colour.b);
		}
		glVertex3f(verts[v].position.x, verts[v].position.y, verts[v].position.z);
	}

	glEnd();

	//boundingVolume.Draw();

	//Debug display for bounding and collision volumes.
	//Bounding volumes have been put on hold until spatial data structures have been implemented.
	//if (drawBoundingVolumes) primitives[i]->boundingVolume->Draw();
	//if (drawCollisionVolumes) primitives[i]->collisionVolume.Draw();
}

void Primitive::CalculateInertiaTensor()
{
	double matVals[16] = { 0 };
	switch (type)
	{
	case Type::BOX:
	{
		matVals[0] = (1 / (12 * rigidbody.inverseMass)) * ((scale.y * scale.y) + (scale.z * scale.z));
		matVals[5] = (1 / (12 * rigidbody.inverseMass)) * ((scale.x * scale.x) + (scale.z * scale.z));
		matVals[10] = (1 / (12 * rigidbody.inverseMass)) * ((scale.x * scale.x) + (scale.y * scale.y));
		break;
	}
	case Type::SPHERE:
	{
		matVals[0] = (2 / (5 * rigidbody.inverseMass)) * (radius * radius);
		matVals[5] = (2 / (5 * rigidbody.inverseMass)) * (radius * radius);
		matVals[10] = (2 / (5 * rigidbody.inverseMass)) * (radius * radius);
		break;
	}
	case Type::CAPSULE: //ASSUMED SAME AS CYLINDER FOR NOW
	case Type::CYLINDER:
	{
		matVals[0] = ((1 / (12 * rigidbody.inverseMass)) * (collisionVolume.length * collisionVolume.length))
			+ ((1 / (4 * rigidbody.inverseMass)) * (radius * radius));
		matVals[5] = ((1 / (12 * rigidbody.inverseMass)) * (collisionVolume.length * collisionVolume.length))
			+ ((1 / (4 * rigidbody.inverseMass)) * (radius * radius));
		matVals[10] = (1 / (2 * rigidbody.inverseMass)) * (radius * radius);
		break;
	}
	case Type::COMPLEX:
	case Type::PLANE:
	default:
		return;
	}

	rigidbody.inverseInertiaTensor = Matrix(matVals);
	rigidbody.inverseInertiaTensor.Inverse3x3();

}

void Primitive::Tween(float speed, const Vector3& direction, float approxDistance)
{
	if (/*!initialised || */!rigidbody.isKinematic) return;

	if (!tweenMaxSet)
	{
		tweenMax = tweenOrigin + (direction * approxDistance);
		tweenMaxSet = true;
	}

	translation += (direction * (Global::deltaTime * speed)) * (moveTowards ? 1 : -1);

	//Reverse movement
	if ((moveTowards && translation.Distance(tweenMax) < 0.2)
		|| (!moveTowards && translation.Distance(tweenOrigin) < 0.2))
	{
		moveTowards = !moveTowards;
	}

	updateTransform = true;
}

void Primitive::SetTweenOrigin()
{
	tweenOrigin = translation;
}

void Primitive::UpdateTransform()
{
	transform.Identity();

	//Update the transform matrix4x4 with the new transform vectors.
	Mathe::Translate(transform, translation.x, translation.y, translation.z);
	Mathe::Rotate(transform, rotation.x, rotation.y, rotation.z);
	collisionVolume.axisMat = transform;
	Mathe::Scale(transform, scale.x, scale.y, scale.z);

	if (type == Type::CAPSULE || type == Type::CYLINDER)
	{
		upDirMat.Identity();
		Mathe::Rotate(upDirMat, rotation.x, rotation.y, rotation.z);
		upDir = Vector3(0, 1, 0);
		Mathe::Transform(upDir, upDirMat);
		upDir = upDir.Normalise();
	}

	boundingVolume.Generate(vertices, transform); //Gets min and max vertices
	collisionVolume.Update(translation, radius, scale / 2, rotation);

	updateTransform = false;
}

GLenum Primitive::GetDrawType(Type objectType)
{
	switch (objectType)
	{
	case Type::CYLINDER:
	{
		return GL_TRIANGLE_STRIP;
	}
	case Type::CAPSULE:
	case Type::SPHERE:
	{
		return GL_TRIANGLES;
	}
	case Type::BOX:
	default:
	{
		return GL_QUADS;
	}
	}
}
