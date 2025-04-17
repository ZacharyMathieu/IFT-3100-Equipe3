#include "ant.h"

Ant::Ant(float x, float y, float a)
{
	pos = ofPoint(x, y);
	this->a = a;
	pheromoneLevel = 0;
}

void Ant::update(Grid* grid)
{
	while (grid->at(pos)->type == WALL)
	{
		pos.x += 1;
		pos.y += 0.3;
	}

	ofPoint point;
	Cell* cell;
	float value;
	float diff;
	float maxValue = 0;
	float maxValueDiff = 0;
	int _x, _y;

	if (pheromoneLevel < ANT_NORMAL_PHEROMONE_LEVEL)
		pheromoneLevel += ANT_PHEROMONE_LEVEL_DECAY_VALUE;
	else if (pheromoneLevel > ANT_NORMAL_PHEROMONE_LEVEL)
		pheromoneLevel -= ANT_PHEROMONE_LEVEL_DECAY_VALUE;

	for (_y = pos.y - ANT_SEARCH_RADIUS; _y < pos.y + ANT_SEARCH_RADIUS; _y++)
	{
		for (_x = pos.x - ANT_SEARCH_RADIUS; _x < pos.x + ANT_SEARCH_RADIUS; _x++)
		{
			point = ofPoint((_x + grid->w) % grid->w, (_y + grid->h) % grid->h);
			cell = grid->at(point);
			if (cell->type == CellType::PHEROMONE)
			{
				if (point.distance(pos) <= ANT_SEARCH_RADIUS)
				{
					value = cell->value;
					if (value >= maxValue)
					{
						diff = angleDiff(angleTo(point));
						if (value > maxValue || diff < maxValueDiff) {
							maxValue = value;
							maxValueDiff = diff;
						}
					}
				}
			}
		}
	}

	ofPoint futurePos = ofPoint(fmod(pos.x + ANT_MOVE_SPEED * cos(a) + grid->w, (float)grid->w), fmod(pos.y + ANT_MOVE_SPEED * sin(a) + grid->h, (float)grid->h));
	if (grid->at(futurePos)->type != CellType::WALL)
	{
		pos = futurePos;

		if (maxValue > 0)
			turnToDiff(maxValueDiff);

		a += pow((rand() / (float)RAND_MAX * 2 - 1), 3) * ANT_RANDOM_TURN_MAX;
	}
	else
	{
		a += PI;
		pheromoneLevel = ANT_MAX_PHEROMONE_LEVEL;
	}
}

void Ant::turnToDiff(float diff)
{
	if (diff >= 0)
	{
		a += min(ANT_TURN_SPEED, diff);
	}
	else
	{
		a += max(-ANT_TURN_SPEED, diff);
	}

	if (a > TWO_PI) a -= TWO_PI;
}

float Ant::angleTo(ofPoint point) {
	return atan2(point.y - pos.y, point.x - pos.x);
}

float Ant::angleDiff(float angle) {
	//float a1 = angle - a;
	//float a2 = a1 - TWO_PI;
	if (angle < a) {
		return angle - a;
	}
	return angle - a - TWO_PI;
}
