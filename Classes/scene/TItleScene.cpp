#include "TItleScene.h"
#include "GameScene.h"
#include "EditScene.h"

#include "stage/Stage.h"

#include "system/MyUtils.h"
#include "wrapper/EventListenerGesture.h"

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

USING_NS_CC;

const static int MAX_ITEMS = 9;

/*
 * Create Scene
 */
Scene* Title::createScene()
{
    auto scene = Scene::create();
    auto layer = Title::create();
    scene->addChild(layer);
    return scene;
}


/*
 * Constructor
 */
Title::Title()
{
	_cursol = 0;
	_stageNumber = 0;
	_stage = NULL;
	_stageSprite = NULL;
}

/*
 * Destructor
 */
Title::~Title()
{
	CC_SAFE_RELEASE(_stage);
}

/*
 * Initialize
 */
bool Title::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    CCLOG("Creating sprite...");

	//Set base data
	auto winSize = Director::getInstance()->getWinSize();

    auto title = Sprite::create("image/title.png");
    title->setPosition(winSize.width / 2, winSize.height / 2);
    this->addChild(title);

	auto stage = Sprite::create();
	stage->setTextureRect(Rect(0, 0, 320, 240));
	stage->setColor(Color3B::WHITE);
	stage->setPosition(title->getContentSize().width / 2, 150);
	title->addChild(stage);

	std::string text;
	for (int i = 0; i < MAX_ITEMS; i++)
	{
		auto stage = Stage::create();
		MyUtils::getInstance()->loadText(text, StringUtils::format("stages/stage%d.json", i));
		stage->parseObject(text);
		stage->stageUpdate();
//		stage->parseRowObject(i)
		_stageList.pushBack(stage);
	}

	auto userDef = UserDefault::getInstance();
	int num = userDef->getIntegerForKey("number", 0);
	log("%d", num);
	//Set path
	auto path = FileUtils::getInstance()->getWritablePath();
	for (int i = 0; i < num; i++)
	{
		ssize_t size = 1024;
		auto stage = Stage::create();
		auto file = path + StringUtils::format("user%d.json", (i + 1));
		auto row_text = FileUtils::getInstance()->getFileData(file, "r", &size);
		text = (char*)row_text;
		delete[] row_text;
		stage->parseObject(text);
		stage->stageUpdate();
		_stageList.pushBack(stage);
	}
	_stageNumber = MAX_ITEMS + num;

	setStage(_stageList.at(0));
	_stageSprite = _stage->getStageSprite();
	_stageSprite->setScaleX(stage->getContentSize().width / _stageSprite->getContentSize().width);
	_stageSprite->setScaleY(stage->getContentSize().height / _stageSprite->getContentSize().height);
	_stageSprite->setPosition(stage->getContentSize().width / 2, stage->getContentSize().height / 2);
    stage->addChild(_stageSprite);

	auto l_arrow = Sprite::create("image/arrows.png", Rect(0, 0, 45, 90));
	l_arrow->setPosition(202, 150);
	title->addChild(l_arrow);

	auto r_arrow = Sprite::create("image/arrows.png", Rect(45, 0, 45, 90));
	r_arrow->setPosition(598, 150);
	title->addChild(r_arrow);

	auto stage_edit = Sprite::create("image/edit.png");
	stage_edit->setPosition(800, 0);
	stage_edit->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	title->addChild(stage_edit);

    CCLOG("Creating Listener...");

    auto stageListener = EventListenerGesture::create();
    stageListener->setSwallowTouches(true);
	stageListener->onTouchBeganChecking = [this](Touch* touch, Event* event)
	{
		if (MyUtils::getInstance()->isTouchInEvent(touch, event))
			return true;
		return false;

	};
	stageListener->onTap = [this] (Vec2 v)
    {
		auto stage = this->getStage();
		Director::getInstance()->pushScene(Game::createScene(stage, CallFunc::create([this] {
			this->resetStageSprite();
		})));
    };
	stageListener->onSwipe = [this, stage](EventListenerGesture::SwipeDirection dir)
	{
		if (dir == EventListenerGesture::SwipeDirection::RIGHT)
			leftArrow(stage);
		if (dir == EventListenerGesture::SwipeDirection::LEFT)
			rightArrow(stage);
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(stageListener, stage);

	auto l_arrow_listener = EventListenerTouchOneByOne::create();
	l_arrow_listener->setSwallowTouches(true);
	l_arrow_listener->onTouchBegan = [this, stage](Touch* touch, Event* event)
	{
		if (MyUtils::getInstance()->isTouchInEvent(touch, event))
		{
			leftArrow(stage);
			return true;
		}
		return false;
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(l_arrow_listener, l_arrow);

	auto r_arrow_listener = EventListenerTouchOneByOne::create();
	r_arrow_listener->setSwallowTouches(true);
	r_arrow_listener->onTouchBegan = [this, stage](Touch* touch, Event* event)
	{
		if (MyUtils::getInstance()->isTouchInEvent(touch, event))
		{
			rightArrow(stage);
			return true;
		}
		return false;
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(r_arrow_listener, r_arrow);

	auto editListener = EventListenerTouchOneByOne::create();
	editListener->setSwallowTouches(true);
	editListener->onTouchBegan = [](Touch* touch, Event* event) {
		if (MyUtils::getInstance()->isTouchInEvent(touch, event))
		{
			Director::getInstance()->pushScene(EditScene::createScene());
			return true;
		}
		return false;
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(editListener, stage_edit);

	SimpleAudioEngine::getInstance()->playBackgroundMusic("bgm/bgm_main.wav", true);

    return true;
}

void Title::resetStageSprite()
{
	_stage->stageUpdate();
	auto stage = _stageSprite->getParent();
	_stageSprite->removeFromParent();
	_stageSprite = _stage->getStageSprite();
	_stageSprite->setScaleX(stage->getContentSize().width / _stageSprite->getContentSize().width);
	_stageSprite->setScaleY(stage->getContentSize().height / _stageSprite->getContentSize().height);
	_stageSprite->setPosition(stage->getContentSize().width / 2, stage->getContentSize().height / 2);
	stage->addChild(_stageSprite);

}

void Title::rightArrow(Sprite* stage)
{
	if (_cursol >= _stageNumber - 1)
		return;
	stage->removeChild(_stageSprite, true);
	setStage(_stageList.at(++_cursol));
	_stageSprite = _stage->getStageSprite();
	_stageSprite->setScaleX(stage->getContentSize().width / _stageSprite->getContentSize().width);
	_stageSprite->setScaleY(stage->getContentSize().height / _stageSprite->getContentSize().height);
	_stageSprite->setPosition(stage->getContentSize().width / 2, stage->getContentSize().height / 2);
	stage->addChild(_stageSprite);
}

void Title::leftArrow(Sprite* stage)
{
	if (_cursol <= 0)
		return;	
	stage->removeChild(_stageSprite, true);
	setStage(_stageList.at(--_cursol));
	_stageSprite = _stage->getStageSprite();
	_stageSprite->setScaleX(stage->getContentSize().width / _stageSprite->getContentSize().width);
	_stageSprite->setScaleY(stage->getContentSize().height / _stageSprite->getContentSize().height);
	_stageSprite->setPosition(stage->getContentSize().width / 2, stage->getContentSize().height / 2);
	stage->addChild(_stageSprite);
}