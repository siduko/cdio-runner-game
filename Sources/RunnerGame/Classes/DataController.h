#ifndef DataController_h__
#define DataController_h__
#include "cocos2d.h"
USING_NS_CC;
using namespace std;


class DataController
{
private:
	static DataController* _instance;
	ValueMap _gameData;
	ValueMap _gameSettings;
	ValueVector _chapters;
public:
	DataController();
	~DataController();
	static DataController* getInstance();
	ValueMap getChapterByIndex(int index);
	void setChapterByIndex(int index,ValueMap value);
	inline ValueVector getChapters(){
		return _chapters;
	}
	ValueVector getLevelsInChapterByIndex(int index);
	int getChapterScoreByIndex(int index);
	int getChapterStarByIndex(int index);
	int getChapterStarMaxByIndex(int index);
	ValueMap getLevelByChapterIndex(int chapterIndex, int levelIndex);
	void setLevelByChapterIndex(int chapterIndex, int levelIndex, ValueMap value);
	ValueMap getGameSettings();
	void saveGameData();
};
#endif // DataController_h__