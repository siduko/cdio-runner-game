#include "Utils.h"


float Utils::randomValueBetween(float low, float high){
	return (((float)rand() / RAND_MAX) * (high - low)) + low;
}
int Utils::randomValueBetween(int low, int high){
	return low + (rand() % high);
}