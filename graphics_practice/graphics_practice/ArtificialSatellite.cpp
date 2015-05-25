#include "ArtificialSatellite.h"
#include "Control.h"
#include "main.h"

ArtificialSatellite::ArtificialSatellite(float _distance, float rotationSpeed)
{
	totalNumberOfVertices += 11;

	distance = _distance;
	revSpeed = rotationSpeed;
	revAngle = 0;

	int offset = getOffset();

	int idx = 0;
	points[offset + idx++] = point4(-2.0, 1.0, 0.0, 1.0);
	points[offset + idx++] = point4(-2.0, 0.0, 0.0, 1.0);
	points[offset + idx++] = point4(-1.0, 0.0, 0.0, 1.0);
	points[offset + idx++] = point4(-1.0, 1.0, 0.0, 1.0);

	points[offset + idx++] = point4(1.0, 0, 0.0, 1.0);
	points[offset + idx++] = point4(1.0, 1.0, 0, 1.0);
	points[offset + idx++] = point4(2.0, 1.0, 0, 1.0);
	points[offset + idx++] = point4(2.0, 0, 0, 1.0);

	points[offset + idx++] = point4(0, 1.0, 0, 1.0);
	points[offset + idx++] = point4(-0.6, 0, 0, 1.0);
	points[offset + idx++] = point4(0.6, 0, 0, 1.0);

	for (int i = 0; i < 8; i++)
	{
		colors[offset + i] = color4(0.7, 0.7, 0.7, 1.0);
	}

	for (int i = 8; i < 11; i++)
	{
		colors[offset + i] = color4(0.5, 0.2, 0.6, 1.0);
	}

}


ArtificialSatellite::~ArtificialSatellite()
{
}

void ArtificialSatellite::updateAngle()
{
	revAngle += controller.doAnimation ? revSpeed * controller.animationSpeed : 0;

	if (revAngle >= 360.0f)
		revAngle -= 360.0f;
}

void ArtificialSatellite::draw()
{
	updateAngle();

	mvstack.push(model_view);

	mat4 instance = RotateZ(revAngle) * Translate(0, -distance, 0) * Scale(0.005,0.005,0);

	model_view = model_view * instance;

	glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);

	int offset = getOffset();
	glDrawArrays(GL_TRIANGLE_FAN, offset, 4);
	glDrawArrays(GL_TRIANGLE_FAN, offset+4, 4);
	glDrawArrays(GL_TRIANGLE_FAN, offset+8, 3);

	model_view = mvstack.pop();
}