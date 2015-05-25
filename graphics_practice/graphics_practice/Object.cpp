#include "Object.h"
#include <fstream>
#include <string>
#include <sstream>

unsigned int Object::totalNumberOfVertices = 0;
int Object::numberOfObjects = 0;

Object::Object(void)
{
	id = numberOfObjects++; 
	offset = totalNumberOfVertices;
	m = mat4(1.0f); //Identity
	sibling = NULL;
	child = NULL;
}

Object::~Object(void)
{
}

void Object::traverse()
{
	mvstack.push(model_view);
	model_view = model_view * m;
	draw();

	if(child!=NULL)
	{
		child->traverse();
	}
	model_view = mvstack.pop();
	
	if(sibling!=NULL)
	{
		sibling->traverse();
	}
}

bool Object::addChild(Object* newChild)
{
	if (newChild == NULL)
		return false;

	//if it already has a child, add it as a sibling of the child.
	if (child)
	{
		child->addSibling(newChild);
	}
	else
	{
		child = newChild;
	}
	return true;
}

bool Object::addSibling(Object* newSibling)
{
	if (newSibling == NULL)
		return false;
	//chain�� �� ���� �ƴϰ� �� �տ� �߰��Ѵ�. traverse �� �ʿ䰡 ����.
	Object* currentSibling = sibling;
	sibling = newSibling;
	newSibling->sibling = currentSibling;
	return true;
}
