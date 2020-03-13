#include "Primitive.h"

void Primitive::Start()
{
	rigidbody.inverseMass = 1.0f / (scale.SumComponents() * 3.0);
	CalculateInertiaTensor();
	rigidbody.Start();
}

void Primitive::Update()
{	
	previousPosition = translation;

	if (rigidbody.PhysicsUpdate())
	{
		translation += rigidbody.velocity * Global::deltaTime;
		Mathe::AddScaledVector(orientation, rigidbody.angularVelocity, Global::deltaTime, false);
		//UpdateTransform();
		updateTransform = true;

		rigidbody.EndPhysicsUpdate(colliding);
	}
	if (updateTransform) UpdateTransform();
}

void Primitive::Draw()
{
	glBegin(GetDrawType(type));

	//Only locally transform the vertices. This makes it so that each draw call
	//the vertices do not have to be cleared and redefined if their transforms have
	//changed. Performance boost. (or is it?)
	//std::vector<Vertex> verts = vertices;
	Vector3 position;
	for (unsigned v = 0; v < vertices.size(); v++)
	{
		position = vertices[v].position;
		Mathe::Transform(position, transform);

		glNormal3f(vertices[v].normal.x, vertices[v].normal.y, vertices[v].normal.z);

		//For future shader use.
		//GLfloat colour[] = { objects[i].vertices[v].colour.r, objects[i].vertices[v].colour.g, objects[i].vertices[v].colour.b,  objects[i].vertices[v].colour.a};
		//glMaterialfv(GL_FRONT, GL_DIFFUSE, colour);
		if (colliding && debugCollision)
		{
			glColor3f(1, 0, 0);
		}
		else
		{
			glColor3f(vertices[v].colour.r, vertices[v].colour.g, vertices[v].colour.b);
		}
		glVertex3f(position.x, position.y, position.z);
	}

	glEnd();

	//boundingVolume.Draw();
}

void Primitive::CalculateInertiaTensor()
{
	double matVals[9] = { 0.0 };
	switch (type)
	{
	case Type::BOX:
	{
		matVals[0] = (1.0 / (12.0 * rigidbody.inverseMass)) * ((scale.y * scale.y) + (scale.z * scale.z));
		matVals[4] = (1.0 / (12.0 * rigidbody.inverseMass)) * ((scale.x * scale.x) + (scale.z * scale.z));
		matVals[8] = (1.0 / (12.0 * rigidbody.inverseMass)) * ((scale.x * scale.x) + (scale.y * scale.y));
		break;
	}
	case Type::SPHERE:
	{
		matVals[0] = (2.0 / (5.0 * rigidbody.inverseMass)) * ((double)radius * (double)radius);
		matVals[4] = (2.0 / (5.0 * rigidbody.inverseMass)) * ((double)radius * (double)radius);
		matVals[8] = (2.0 / (5.0 * rigidbody.inverseMass)) * ((double)radius * (double)radius);
		break;
	}
	case Type::CAPSULE: //ASSUMED SAME AS CYLINDER FOR NOW
	case Type::CYLINDER:
	{
		matVals[0] = ((1.0 / (12.0 * rigidbody.inverseMass)) * ((double)collisionVolume.length * (double)collisionVolume.length))
			+ ((1.0 / (4.0 * rigidbody.inverseMass)) * ((double)radius * (double)radius));
		matVals[4] = ((1.0 / (12.0 * rigidbody.inverseMass)) * ((double)collisionVolume.length * (double)collisionVolume.length))
			+ ((1.0 / (4.0 * rigidbody.inverseMass)) * ((double)radius * (double)radius));
		matVals[8] = (1.0 / (2.0 * rigidbody.inverseMass)) * ((double)radius * (double)radius);
		break;
	}
	case Type::COMPLEX:
	case Type::PLANE:
	default:
		return;
	}

	rigidbody.inverseInertiaTensor = Matrix3(matVals);
	rigidbody.inverseInertiaTensor.Inverse();
}

void Primitive::Tween(float speed, const Vector3& direction, float approxDistance)
{
	if (/*!initialised || */!rigidbody.isKinematic) return;

	if (!tweenMaxSet)
	{
		tweenMax = tweenOrigin + (direction * approxDistance);
		tweenMaxSet = true;
	}

	translation += (direction * (float)(Global::deltaTime * speed)) * (moveTowards ? 1.0f : -1.0f);

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
	if (freeze) return;
	if (translation.y < -25.0)
	{
		std::cout << "WARNING: object fallen below ground plane." << std::endl;
		freeze = true;
	}

	transform.Identity();

	//Update the transform matrix4x4 with the new transform vectors.
	Mathe::Translate(transform, translation.x, translation.y, translation.z);
	Mathe::Rotate(transform, orientation);

	collisionVolume.axisMat = transform;
	Mathe::TransformInverseInertiaTensor(rigidbody.inverseInertiaTensorWorld, rigidbody.inverseInertiaTensor, GetOrientation(transform));

	Mathe::Scale(transform, scale.x, scale.y, scale.z);

	if (type == Type::CAPSULE || type == Type::CYLINDER)
	{
		upDirMat.Identity();
		//Mathe::Rotate(upDirMat, rotation.x, rotation.y, rotation.z);
		Mathe::Rotate(upDirMat, orientation);
		upDir = Vector3(0, 1, 0);
		Mathe::Transform(upDir, upDirMat);
		upDir = upDir.Normalise();
	}

	boundingVolume.Generate(vertices, transform); //Gets min and max vertices
	collisionVolume.Update(translation, radius, scale, orientation);

	updateTransform = false;
}

void Primitive::GetOrientation(Quaternion* _orientation) const
{
	*_orientation = orientation;
}

Quaternion Primitive::GetOrientation() const
{
	return orientation;
}

Matrix3 Primitive::GetOrientation(Matrix4 _matrix)
{
	return GetOrientation(_matrix.matrix);
}

Matrix3 Primitive::GetOrientation(double _matrix[16])
{
	double matVals[9] =
	{
		_matrix[0], _matrix[1], _matrix[2],
		_matrix[4], _matrix[5], _matrix[6],
		_matrix[8], _matrix[9], _matrix[10]
	};

	return Matrix3(matVals);
}

void Primitive::SetOrientation(const Quaternion& _orientation)
{
	orientation = _orientation;
	orientation.Normalise();
}

void Primitive::SetOrientation(const double r, const double i, const double j, const double k)
{
	orientation.r = r;
	orientation.i = i;
	orientation.j = j;
	orientation.k = k;
	orientation.Normalise();
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
