#include "ant.h"

Ant::Ant(float x, float y, float a)
{
    pos = ofPoint(x, y);
    this->a = a;
}

void Ant::update(Grid *grid)
{
    float maxValue = 0;
    float maxValueDistance = SEARCH_RADIUS;
    ofPoint target = pos;
    pheromoneLevel = MAX_PHEROMONE_LEVEL;
    for (int _y = pos.y - SEARCH_RADIUS + grid->h; _y < pos.y + SEARCH_RADIUS + grid->h; _y++)
    {
        for (int _x = pos.x - SEARCH_RADIUS + grid->w; _x < pos.x + SEARCH_RADIUS + grid->w; _x++)
        {
            ofPoint point = ofPoint(_x % grid->w, _y % grid->h);
            Cell *cell = grid->at(point);
            float dist = point.distance(pos);
            if (cell->type == CellType::PHEROMONE)
            {
                if (dist <= SEARCH_RADIUS)
                {
                    float value = cell->value;
                    if (value > maxValue || (value == maxValue && dist < maxValueDistance))
                    {
                        maxValue = value;
                        maxValueDistance = dist;
                        target = point;
                    }
                }
            }
            else
            {
                pheromoneLevel -= MAX_PHEROMONE_LEVEL / (dist * dist);
            }
        }
    }

    ofPoint futurePos = ofPoint(fmod(pos.x + MOVE_SPEED * cos(a) + grid->w, (float)grid->w), fmod(pos.y + MOVE_SPEED * sin(a) + grid->h, (float)grid->h));
    if (grid->at(futurePos)->type == CellType::PHEROMONE)
    {
        pos = futurePos;
    }
    else
    {
        a += PI;
    }

    turnTorwards(target);

    if (pheromoneLevel < 0)
        pheromoneLevel = 0;
}

void Ant::turnTorwards(ofPoint target)
{
    float targetAngle = atan2(target.y - pos.y, target.x - pos.x);
    targetAngle = fmod(targetAngle + TWO_PI, TWO_PI);
    float diff = targetAngle - a;

    if (diff >= 0)
    {
        a += min(TURN_SPEED, diff);
    }
    else
    {
        a += max(-TURN_SPEED, diff);
    }

    a = fmod(a + TWO_PI, TWO_PI);
}
