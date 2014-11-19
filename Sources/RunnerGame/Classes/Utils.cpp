#include "Utils.h"


float Utils::randomValueBetween(float low, float high){
	return (((float)rand() / RAND_MAX) * (high - low)) + low;
}
int Utils::randomValueBetween(int low, int high){
	return low + (rand() % high);
}

Vec2 Utils::collisedDirection(Rect a, Rect b)
{
	if (a.getMinY()<b.getMaxY() && a.getMinY()>b.getMinY())
	{
		//left or right
		if (a.getMidX() <= b.getMinX())
			return ccp(-1, 0);
		if (a.getMidX() >= b.getMaxX())
			return ccp(1, 0);
	}else
	{
		//top or bottom
		if (a.getMidY() >= b.getMaxY())
			return ccp(0, 1);
		if (a.getMidY() <= b.getMaxY())
			return ccp(0, -1);
	}
	return ccp(0, 0);
}
