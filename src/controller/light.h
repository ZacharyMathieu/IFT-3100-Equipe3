#include "ofMain.h"

constexpr int LIGHT_COUNT = 4;
constexpr float LIGHT_ROTATION_SPEED = 0.02;

float randf(float min, float max)
{
	return (rand() / (float)RAND_MAX) * (max - min) + min;
}

ofFloatColor randColor()
{
	return ofFloatColor(randf(0, 1), randf(0, 1), randf(0, 1), 1);
}


class Light {
public:
	ofPoint center;
	float radius;
	int index;
	ofFloatColor color;
	int count = 0;
	float offset = 0;
	ofPoint pos = ofPoint(0);

	Light(ofPoint&, float, int, ofFloatColor* color = nullptr);
	void update();
};
