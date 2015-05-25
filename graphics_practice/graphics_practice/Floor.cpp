#include "Floor.h"
#include "Control.h"
#include "Camera.h"

Floor::Floor(float _x, float _y)
{
	x = _x;
	y = _y;
	size = 0;

	for (int i = -x; i <= x; i++)
	{
		for (int j = -y; j <= y; j++)
		{
			vertices.push_back(vec4(i * 0.1 - 0.05, j * 0.1 - 0.05, 0, 1));
			vertices.push_back(vec4(i * 0.1 - 0.05, j * 0.1 + 0.05, 0, 1));
			vertices.push_back(vec4(i * 0.1 + 0.05, j * 0.1 + 0.05, 0, 1));
			vertices.push_back(vec4(i * 0.1 + 0.05, j * 0.1 - 0.05, 0, 1));

			colors.push_back(vec4(1, 1, 1, 1));
			colors.push_back(vec4(1, 1, 1, 1));
			colors.push_back(vec4(1, 1, 1, 1));
			colors.push_back(vec4(1, 1, 1, 1));
			size++;
		}
	}
}

Floor::Floor()
{
}

Floor::~Floor()
{
}

void Floor::draw(bool isSelect)
{
	
	mvstack.push(model_view);
	glUniformMatrix4fv(isSelect ? sModelView : ModelView, 1, GL_TRUE, model_view);

	for (int i = 0; i < size; i++)
		glDrawArrays(GL_LINE_LOOP, i * 4, 4);
	//cout << model_view << endl;
	
	model_view = mvstack.pop();
}
