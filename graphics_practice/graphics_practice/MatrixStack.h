#pragma once
#include "Angel.h"

/*
SOURCE FROM
www.cs.unm.edu/~angel/BOOK/INTERACTIVE_COMPUTER_GRAPHICS/SIXTH_EDITION/CODE/CHAPTER08/WINDOWS_VERSIONS/example2.cpp
*/

class MatrixStack {
	int    _index;
	int    _size;
	mat4*  _matrices;

public:
	MatrixStack(int numMatrices = 32) :_index(0), _size(numMatrices)
	{
		_matrices = new mat4[numMatrices];
	}

	~MatrixStack()
	{
		delete[]_matrices;
	}

	void push(const mat4& m) {
		//assert( _index + 1 < _size );
		_matrices[_index++] = m;
	}

	mat4& pop(void) {
		//assert( _index - 1 >= 0 );
		_index--;
		return _matrices[_index];
	}
};
