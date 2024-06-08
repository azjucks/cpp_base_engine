#include <transform.h>
using namespace Physics;

#include <sstream>

#include <gameobject.h>
using namespace Engine;

Transform::Transform(GameObject* go)
	: Component(go)
{

}

void Transform::updateTransform()
{
	modelMatrix = getModelUpdated();
}

mat4 Transform::getModelUpdated()
{
	mat4 mat = Maths::identity();

	if (parent != nullptr && parent != this)
		mat = mat * parent->getModelUpdated();

	mat = mat
		* Maths::translate(position)
		* Maths::rotateX(rotation.x)
		* Maths::rotateY(rotation.y)
		* Maths::rotateZ(rotation.z)
		* Maths::scale(scale);

	return mat;
}

std::string Transform::getSaveFormat()
{
	std::string s = "transform "
		+ std::to_string(position.x) + " "
		+ std::to_string(position.y) + " "
		+ std::to_string(position.z) + " "
		+ std::to_string(rotation.x) + " "
		+ std::to_string(rotation.y) + " "
		+ std::to_string(rotation.z) + " "
		+ std::to_string(scale.x) + " "
		+ std::to_string(scale.y) + " "
		+ std::to_string(scale.z) + " ";

	if (parent != nullptr)
	{
		s += std::to_string(true) + " " + parent->gameObject->name;
	}
	else
	{
		s += std::to_string(false) + " ";
	}
	
	s += "\n";

	return s;
}

#include <graph.h>

void Transform::loadFromSaveFormat(const std::string& line)
{
	std::istringstream iss(line);

	std::string transform;
	iss >> transform;

	iss >> position.x;
	iss >> position.y;
	iss >> position.z;
	iss >> rotation.x;
	iss >> rotation.y;
	iss >> rotation.z;
	iss >> scale.x;
	iss >> scale.y;
	iss >> scale.z;

	bool hasParent = false;
	iss >> hasParent;
	if (hasParent)
	{
		std::string parentName;
		iss >> parentName;
		GameObject* parentGo = DataStructure::Graph::instance()->getCurrentScene()->findGameObjectWithName(parentName);
		if (parentGo != nullptr)
		{
			parent = parentGo->transform.get();
		}
	}
}