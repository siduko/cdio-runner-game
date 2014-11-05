#include "DataController.h"


DataController::DataController()
{
	string path = FileUtils::getInstance()->fullPathForFilename("Levels/GameData.plist");
	_gameData = FileUtils::getInstance()->getValueMapFromFile(path);
	_chapters = _gameData["Chapters"].asValueVector();
}


DataController::~DataController()
{
}

DataController* DataController::getInstance()
{
	if (!_instance)
		_instance = new DataController();
	return _instance;
}

ValueMap DataController::getChapterByIndex(int index)
{
	if (index >= 0 && index < _chapters.size())
		return _chapters.at(index).asValueMap();
	return ValueMap();
}

cocos2d::ValueVector DataController::getLevelsInChapterByIndex(int index)
{
	ValueMap chapter = getChapterByIndex(index);
	return chapter["Levels"].asValueVector();
}

int DataController::getChapterScoreByIndex(int index)
{
	int score = 0;
	ValueVector levels = getLevelsInChapterByIndex(index);
	for (auto l : levels)
	{
		ValueMap level = l.asValueMap();
		score += level["Score"].asInt();
	}
	return score;
}

int DataController::getChapterStarByIndex(int index)
{
	int star = 0;
	ValueVector levels = getLevelsInChapterByIndex(index);
	for (auto l : levels)
	{
		ValueMap level = l.asValueMap();
		star += level["Star"].asInt();
	}
	return star;
}

int DataController::getChapterStarMaxByIndex(int index)
{
	int starMax = 0;
	ValueVector levels = getLevelsInChapterByIndex(index);
	for (auto l : levels)
	{
		ValueMap level = l.asValueMap();
		starMax += level["StarMax"].asInt();
	}
	return starMax;
}

cocos2d::ValueMap DataController::getLevelByChapterIndex(int chapterIndex, int levelIndex)
{
	ValueVector levels = getLevelsInChapterByIndex(chapterIndex);

	if (levelIndex >= 0 && levelIndex < levels.size())
		return levels.at(levelIndex).asValueMap();
	return ValueMap();
}

void DataController::saveGameData()
{
	string path = FileUtils::getInstance()->fullPathForFilename("Levels/GameData.plist");
	FileUtils::getInstance()->writeToFile(_gameData, path);
}

DataController* DataController::_instance;
