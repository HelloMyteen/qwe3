#pragma once

#include <iostream>
#include "game_unit.h"
#include "BuildingData.h"
#include "Moveable.h"

class Moveable;

class Building:public GameUnit
{
public:
	Building() = default;

	/*建筑死亡后销毁*/
	void EraseBuilding();

	/*初始化建筑*/
	void initBuilding(const cocos2d::Vec2 pos, int type);

	//创建
	static Building* create(const std::string & filename);

	static Building* createWithSpriteFrameName(const std::string & filename);

	void _isattacked(Moveable* target);

	bool _canattack(Moveable* target);

	void boomlisten();
	
private:

	cocos2d::EventListenerTouchOneByOne* buildinglistener;

};
