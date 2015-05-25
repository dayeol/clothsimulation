#pragma once
#include "main.h"
#include <vector>


using namespace std;

class Object
{
public:
	static unsigned int totalNumberOfVertices;
	Object(void);
	~Object(void);
	
	void traverse();
	virtual void draw()=0;

	//handle hierarchy
	bool addSibling(Object*);
	bool addChild(Object*);
	int getOffset() { return offset; };
	int id;

	void setm(mat4 _m) { m = _m; }
private:
	static int numberOfObjects;
	GLint offset;		//points array의 시작 index
	mat4	m;			//object의 current transform matrix
	Object	*sibling;
	Object	*child;

};