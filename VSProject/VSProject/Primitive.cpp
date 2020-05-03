#include "Primitive.h"

void Box::Start()
{
	rigidbody.inverseMass = 1.0f / (float)scale.Magnitude();
	CalculateInertiaTensor();
	rigidbody.Start(startingVelocity);
}

void Sphere::Start()
{
	rigidbody.inverseMass = 1.0f / (radius * 3.0f);
	CalculateInertiaTensor();
	rigidbody.Start(startingVelocity);
}

void Primitive::Update()
{	
	if (freeze || isStatic) return;

	//if (!rigidbody.isAwake && !colliding && !rigidbody.isKinematic) rigidbody.SetAwake(true);

	if (rigidbody.PhysicsUpdate())
	{
		translation += rigidbody.velocity * Global::deltaTime;
		Mathe::AddScaledVector(orientation, rigidbody.angularVelocity, Global::deltaTime, false);

		updateTransform = true;

		rigidbody.EndPhysicsUpdate(colliding);
	}

	if (updateTransform) UpdateTransform();
}

void Primitive::DrawForVertices(std::vector<Vertex> vertices)
{
	glBegin(drawType);

	Vector3 position;
	for (uint16_t v = 0; v < vertices.size(); v++)
	{
		position = vertices[v].position;
		Mathe::Transform(position, transform);

		glNormal3f((GLfloat)vertices[v].normal.x, (GLfloat)vertices[v].normal.y, (GLfloat)vertices[v].normal.z);

		if (colliding && debugCollision)
		{
			glColor3f(1.0f, 0.0f, 0.0f);
		}
		else
		{
			glColor3f((GLfloat)vertices[v].colour.r, (GLfloat)vertices[v].colour.g, (GLfloat)vertices[v].colour.b);
		}
		glVertex3f((GLfloat)position.x, (GLfloat)position.y, (GLfloat)position.z);
	}
	glEnd();
}

void Primitive::DrawForTris(std::vector<Tri> tris)
{
	glBegin(GL_TRIANGLES);

	//Materials::BindTexture((int)rigidbody.material);

	Vector3 position;
	for (uint16_t v = 0; v < tris.size(); v++)
	{
		for (uint16_t p = 0; p < 3; p++)
		{
			position = tris[v].positions[p];
			Mathe::Transform(position, transform);

			//glTexCoord2f(tris[v].uvs[p * 2], tris[v].uvs[(p * 2) + 1]);
			//glMateriali(GL_FRONT, GL_DIFFUSE, Materials::textures[0].textureID);
			glNormal3f((GLfloat)tris[v].normal.x, (GLfloat)tris[v].normal.y, (GLfloat)tris[v].normal.z);
			if (colliding && debugCollision)
			{
				glColor3f(1.0f, 0.0f, 0.0f);
			}
			else
			{
				glColor3f((GLfloat)tris[v].colour.r, (GLfloat)tris[v].colour.g, (GLfloat)tris[v].colour.b);
			}
			glVertex3f((GLfloat)position.x, (GLfloat)position.y, (GLfloat)position.z);
		}
	}
	glEnd();

	if (drawBoundingVolume) boundingVolume.Draw();
}

void Primitive::GetOrientation(Quaternion* _orientation) const
{
	*_orientation = orientation;
}

Matrix3 Primitive::GetOrientation(Matrix4 _matrix)
{
	float matVals[9] =
	{
		_matrix.matrix[0], _matrix.matrix[1], _matrix.matrix[2],
		_matrix.matrix[4], _matrix.matrix[5], _matrix.matrix[6],
		_matrix.matrix[8], _matrix.matrix[9], _matrix.matrix[10]
	};

	return Matrix3(matVals);
}

void Primitive::SetOrientation(const Quaternion& _orientation)
{
	orientation = _orientation;
	orientation.Normalise();
}

void Primitive::SetOrientation(const float r, const float i, const float j, const float k)
{
	orientation.r = r;
	orientation.i = i;
	orientation.j = j;
	orientation.k = k;
	orientation.Normalise();
}

void Box::CalculateInertiaTensor()
{
	float matVals[9] = { 0.0f };
	matVals[0] = (1.0f / (12.0f * rigidbody.inverseMass)) * ((scale.y * scale.y) + (scale.z * scale.z));
	matVals[4] = (1.0f / (12.0f * rigidbody.inverseMass)) * ((scale.x * scale.x) + (scale.z * scale.z));
	matVals[8] = (1.0f / (12.0f * rigidbody.inverseMass)) * ((scale.x * scale.x) + (scale.y * scale.y));

	rigidbody.inverseInertiaTensor = Matrix3(matVals);
	rigidbody.inverseInertiaTensor.Inverse();
}

void Box::UpdateTransform()
{
	if (freeze) return;
	if (translation.y < -25.0) freeze = true;

	transform.Identity();

	//Update the transform matrix4x4 with the new transform vectors.
	Mathe::Translate(transform, translation.x, translation.y, translation.z);
	Mathe::Rotate(transform, orientation);

	collisionVolume.axisMat = transform;
	Mathe::TransformInverseInertiaTensor(rigidbody.inverseInertiaTensorWorld, rigidbody.inverseInertiaTensor, GetOrientation(transform));

	Mathe::Scale(transform, scale.x, scale.y, scale.z);

	boundingVolume.GenerateForBox(transform); //Sets min and max vertices
	collisionVolume.Update(translation, scale);

	updateTransform = false;
}

void Sphere::CalculateInertiaTensor()
{
	float matVals[9] = { 0.0f };
	matVals[0] = (2.0f / (5.0f * rigidbody.inverseMass)) * (radius * radius);
	matVals[4] = (2.0f / (5.0f * rigidbody.inverseMass)) * (radius * radius);
	matVals[8] = (2.0f / (5.0f * rigidbody.inverseMass)) * (radius * radius);

	rigidbody.inverseInertiaTensor = Matrix3(matVals);
	rigidbody.inverseInertiaTensor.Inverse();
}

void Sphere::UpdateTransform()
{
	if (freeze) return;
	if (translation.y < -25.0) freeze = true;

	transform.Identity();

	Mathe::Translate(transform, translation.x, translation.y, translation.z);
	Mathe::Rotate(transform, orientation);

	collisionVolume.axisMat = transform;
	Mathe::TransformInverseInertiaTensor(rigidbody.inverseInertiaTensorWorld, rigidbody.inverseInertiaTensor, GetOrientation(transform));

	boundingVolume.UpdateMinMax(translation - Vector3(radius, radius, radius), translation + Vector3(radius, radius, radius));
	collisionVolume.Update(translation);

	updateTransform = false;
}

void Plane::UpdateTransform()
{
	transform.Identity();

	Mathe::Translate(transform, translation.x, translation.y, translation.z);
	Mathe::Rotate(transform, orientation);

	Vector3 min = scale * -1.0f;
	Vector3 max = scale;
	Mathe::Transform(min, transform);
	Mathe::Transform(max, transform);

	for (uint16_t i = 0; i < 3; i++)
		if (min[i] > max[i])
		{
			float temp = min[i];
			min[i] = max[i];
			max[i] = temp;
		}

	collisionVolume.axisMat = transform;
	Mathe::TransformInverseInertiaTensor(rigidbody.inverseInertiaTensorWorld, rigidbody.inverseInertiaTensor, GetOrientation(transform));

	Mathe::Scale(transform, scale.x, scale.y, scale.z);
	
	//boundingVolume.UpdateMinMax(Vector3(-scale.x, translation.y, -scale.z), Vector3(scale.x, translation.y, scale.z));
	boundingVolume.UpdateMinMax(min, max);
	collisionVolume.Update(translation, normal);

	updateTransform = false;
}
