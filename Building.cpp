#include "Building.h"
#include "order.h"


class Building;

extern Message message;

Building* Building::create(const std::string & filename)
{
	Building *building = new Building();
	if (building&&building->initWithFile(filename))
	{
		building->autorelease();
		cocos2d::Vec2 born_position(-1000, -1000);
		building->setPosition(born_position);
		return building;
	}
	CC_SAFE_DELETE(building);
	return nullptr;
}

Building* Building::createWithSpriteFrameName(const std::string & filename)
{
	Building *building = new Building();
	if (building&&building->initWithSpriteFrameName(filename))
	{
		building->autorelease();
		return building;
	}
	CC_SAFE_DELETE(building);
	return nullptr;
}


void Building::_isattacked(Moveable* target)
{
	this->setHealth(this->getHealth() - target->getAttack() - this->getDefend());
	this->getblood()->setScaleX(this->getHealth()/this->getMaxHealth());
	if (getHealth() <= 0)
	{
		EraseBuilding();
		cocos2d::log("the unit is destroyed!");
	}
	auto fire_animation = cocos2d::Animation::create();
	for (int i = 1; i <= 4; ++i)
	{
		cocos2d::log("nullptr!!!");
		cocos2d::__String * frameName = cocos2d::__String::createWithFormat("soldierfireright%d.png", i);
		//cocos2d::SpriteFrame * spriteFrame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName->getCString());
		fire_animation->addSpriteFrameWithFile(frameName->getCString());
	}
	fire_animation->setDelayPerUnit(0.15f);
	fire_animation->setRestoreOriginalFrame(true);
	fire_animation->setLoops(1);
	auto action = cocos2d::Animate::create(fire_animation);
	message.getTargetOne()->runAction(action);
}


void Building::EraseBuilding()
{
	auto boom_animation = cocos2d::Animation::create();
	for (int i = 1; i <= 4; ++i)
	{

		cocos2d::__String * frameName = cocos2d::__String::createWithFormat("boom%d.png", i);
		cocos2d::log("framename %s ",frameName->getCString());
		//cocos2d::SpriteFrame * spriteFrame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName->getCString());
		boom_animation->addSpriteFrameWithFile(frameName->getCString());
	}
	boom_animation->setDelayPerUnit(0.15f);
	boom_animation->setRestoreOriginalFrame(true);
	boom_animation->setLoops(1);
	auto action = cocos2d::Animate::create(boom_animation);
	this->runAction(action);

	auto callFunc = cocos2d::CallFuncN::create(CC_CALLBACK_0(Building::boomlisten, this));
	auto sequence = cocos2d::Sequence::create(action, callFunc, NULL);
	this->runAction(sequence);
	
}

void Building::boomlisten()
{
	this->stopAllActions();
	this->setType(0);
	this->setIsMove(true);
	this->setPosition(-1000, -1000);
}


void Building::initBuilding(const cocos2d::Vec2 pos, int type)
{

	//注册监听器
	cocos2d::EventDispatcher * _eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
	buildinglistener = cocos2d::EventListenerTouchOneByOne::create();
	buildinglistener->setSwallowTouches(true);
	buildinglistener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		auto target = static_cast<Sprite*>(event->getCurrentTarget());//获取的当前触摸的目标

		cocos2d::Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		cocos2d::Size s = target->getContentSize();
		cocos2d::Rect rect = cocos2d::Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(locationInNode))//判断触摸点是否在目标的范围内
		{
			if (message.getTargetOne() == nullptr)//target1无目标
			{
				cocos2d::log("it's useless");
			}
			else
			{
				_isattacked(message.getTargetOne());
				cocos2d::log("WARNING!!! I WAS ATTACKED!!!");
				cocos2d::log("The car's blood %d", this->getHealth());
				cocos2d::log("the order is clean");
				message.clean();
			}
			return true;
		}
		else
		{
			return false;
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buildinglistener, this);

	this->setType(type);
	if (type == BUIDMINE_TYPE)
	{
		setHealth(BUIDMINE_HEALTH);
		setMaxHealth(BUIDMINE_MAXHEALTH);
		setDefend(BUIDMINE_DEFEND);
		setBuildTime(BUIDMINE_BUILD_TIME);
		setEffectRange(BUIDMINE_BUILD_TIME);
		setIsMove(true);
		auto frame = cocos2d::SpriteFrame::create("buildingmine.png", cocos2d::Rect(0, 0, 99, 99));
		this->setDisplayFrame(frame);
	}
	else if (type == BUIDPOWER_TYPE)
	{
		setHealth(BUIDPOWER_HEALTH);
		setMaxHealth(BUIDPOWER_MAXHEALTH);
		setDefend(BUIDPOWER_DEFEND);
		setBuildTime(BUIDPOWER_BUILD_TIME);
		//setEffectRange(Power_EffectRange);
		setPrice(BUIDPOWER_PRICE);
		setIsMove(true);
		auto frame = cocos2d::SpriteFrame::create("buildingpower.png", cocos2d::Rect(0, 0, 99, 99));
		this->setDisplayFrame(frame);
	}
	else if (type == BUIDSOLDIER_TYPE)
	{
		setHealth(BUIDSOLDIER_HEALTH);
		setMaxHealth(BUIDSOLDIER_MAXHEALTH);
		setDefend(BUIDSOLDIER_DEFEND);
		setBuildTime(BUIDSOLDIER_BUILD_TIME);
		setPrice(BUIDSOLDIER_PRICE);
		setIsMove(true);
		auto frame = cocos2d::SpriteFrame::create("buildingsoldier.png", cocos2d::Rect(0, 0, 99, 99));
		this->setDisplayFrame(frame);
	}
	else if (type == BUIDCAR_TYPE)
	{
		setHealth(BUIDCAR_HEALTH);
		setMaxHealth(BUIDCAR_MAXHEALTH);
		setDefend(BUIDCAR_DEFEND);
		setBuildTime(BUIDCAR_BUILD_TIME);
		setPrice(BUIDCAR_PRICE);
		setIsMove(true);
		auto frame = cocos2d::SpriteFrame::create("buildingcar.png", cocos2d::Rect(0, 0, 99, 99));
		this->setDisplayFrame(frame);
	}
	setPosition(pos);
	setIsMove(false);
}