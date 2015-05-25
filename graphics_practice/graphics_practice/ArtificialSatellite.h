#pragma once
#include "Object.h"
class ArtificialSatellite : public Object
{
public:
	ArtificialSatellite( float distance, float rotationSpeed );
	~ArtificialSatellite();
	virtual void draw();
	void updateAngle();
	
private:
	float distance;
	float revSpeed;

	float revAngle;
};

