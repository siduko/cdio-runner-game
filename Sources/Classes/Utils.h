#ifndef Utils_h__
#define Utils_h__
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

class Utils
{
public:
	static float randomValueBetween(float low, float high);
	static int randomValueBetween(int low, int high);
	template<typename T>
	static string to_string(const T& t) {
		ostringstream os;
		os << t;
		return os.str();
	}
};
#endif // Utils_h__