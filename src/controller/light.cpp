#include "Light.h"

Light::Light(ofPoint& center, float radius, int index, ofFloatColor* color)
{
	this->center = center;
	this->radius = radius;
	this->index = index;
	if (color == nullptr)
	{
		this->color = randColor();
	}
	else
	{
		this->color = *color;
	}
	this->offset = index * (TWO_PI / LIGHT_COUNT);
}

void Light::update()
{
	count++;
	if (count * LIGHT_ROTATION_SPEED > TWO_PI)
	{
		count = 0;
	}

	pos = center + ofPoint(radius * cos(offset + count * LIGHT_ROTATION_SPEED), 0, sin(offset + count * LIGHT_ROTATION_SPEED));
}
