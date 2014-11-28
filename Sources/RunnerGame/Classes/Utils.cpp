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
	}
	if (a.getMinY()>=b.getMaxY() || a.getMinY()<=b.getMinY())
	{
		//top or bottom
		if (a.getMidY() >= b.getMaxY())
			return ccp(0, 1);
		if (a.getMidY() <= b.getMaxY())
			return ccp(0, -1);
	}
	return ccp(0, 0);
}

std::string Utils::count2Timer(int count)
{
	int mm = count / 60;
	int ss = count % 60;
	string s = (mm<10?'0'+to_string(mm):to_string(mm)) + ":" + (ss<10 ? '0' + to_string(ss) : to_string(ss));
	return s;
}
