#include "ant.h"

Ant::Ant(float x, float y, float a)
{
	pos = ofPoint(x, y);
	this->a = a;
	pheromoneLevel = 0;
}

void Ant::update(Grid* grid)
{
	while (grid->at(pos)->type != PHEROMONE)
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
			if (point.distance(pos) <= ANT_SEARCH_RADIUS)
			{
				point = ofPoint((_x + grid->w) % grid->w, (_y + grid->h) % grid->h);
				cell = grid->at(point);
				value = 0;
				if (cell->type == CellType::PHEROMONE)
				{
					value = cell->value;
				}
				else if (hunger == 0 && cell->type == CellType::FOOD)
				{
					value = CELL_MAX_VALUE;
				}

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

	if (hunger > 0)
	{
		hunger--;
	}

	ofPoint futurePos = ofPoint(fmod(pos.x + ANT_MOVE_SPEED * cos(a) + grid->w, (float)grid->w), fmod(pos.y + ANT_MOVE_SPEED * sin(a) + grid->h, (float)grid->h));
	cell = grid->at(futurePos);
	if (cell->type == CellType::WALL)
	{
		a += PI;
		pheromoneLevel = ANT_NORMAL_PHEROMONE_LEVEL;
	}
	else if (cell->type == CellType::FOOD)
	{
		a += PI;
		pheromoneLevel = ANT_MAX_PHEROMONE_LEVEL;
		hunger = ANT_FOOD_HUNGER_VALUE;
	}
	else if (cell->type == CellType::PHEROMONE)
	{
		pos = futurePos;

		if (maxValue > 0)
			turnToDiff(maxValueDiff);

		a += pow((rand() / (float)RAND_MAX * 2 - 1), 3) * ANT_RANDOM_TURN_MAX;
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
