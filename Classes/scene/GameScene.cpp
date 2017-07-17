#include "GameScene.h"

#include "scene/TitleScene.h"
#include "system/MyUtils.h"
#include "stage/Object.h"
#include "stage/Ray.h"
#include "wrapper/EventListenerGesture.h"

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

USING_NS_CC;

/*
 * Create Scene
 */
Scene* Game::createScene(Stage* stage, CallFunc* cf)
{
    auto scene = Scene::create();
    auto layer = Game::create();
	layer->setStage(stage);
	layer->drawStage();
	layer->setCf(cf);
	scene->addChild(layer);
    return scene;
}

/*
 * Constructor
 */
Game::Game()
{
	_cursol = 0;
	_stage = NULL;
	_planet = NULL;
	_cf = NULL;
	CC_SAFE_RELEASE_NULL(_cf);
}

/*
 * Destructor
 */
Game::~Game()
{
	CC_SAFE_RELEASE(_cf);
	CC_SAFE_RELEASE(_stage);
	CC_SAFE_RELEASE(_planet);
}

/*
 * Initialize
 */
bool Game::init()
{
	if (!Layer::init())
	{
		return false;
	}

    return true;
}

void Game::drawStage()
{
//	CCLOG("Creating sprite...");

	//Set base data
	auto winSize = Director::getInstance()->getWinSize();

	auto back = Sprite::create("image/background.png");
	back->setPosition(winSize.width / 2, winSize.height / 2);
	this->addChild(back);

	_stage->stageUpdate();

	auto stage = _stage->getStageSprite();
	stage->setPosition(back->getContentSize().width / 2, (back->getContentSize().height + 100) / 2);
	back->addChild(stage, 10);

	for (int i = 0; i<8; i++)
	{
		auto column = Sprite::create("image/column.png");
		column->setPosition(50 + i * 100, 50);
		back->addChild(column, 20);

		auto planet = Sprite::create("image/planets.png", Rect(0 + i * 100, 0, 100, 100));
		planet->setPosition(50 + i * 100, 50);
		back->addChild(planet, 30);

		auto planetListener = EventListenerTouchOneByOne::create();
		planetListener->setSwallowTouches(true);
		planetListener->onTouchBegan = [this, winSize, back, planet, i](Touch* touch, Event* event)
		{
			if (MyUtils::getInstance()->isTouchInEvent(touch, event))
			{
				auto pl = Planet::create();
				pl->setX(planet->getPosition().x - (winSize.width - _stage->getWidth()) / 2);
				pl->setY(planet->getPosition().y - (winSize.height - _stage->getHeight()) / 2);
				pl->setType(static_cast<PlanetType>(i+1));
				setPlanet(pl);
				getStage()->addObject(pl);
				getStage()->stageUpdate();
				return true;
			}
			return false;
		};
		planetListener->onTouchMoved = [this, winSize, back, planet, i](Touch* touch, Event* event)
		{
			if( getPlanet()->setPosition(touch->getLocation().x  - (winSize.width - _stage->getWidth())/2, touch->getLocation().y - (winSize.height - _stage->getHeight()) / 2 , _stage) )
				getStage()->stageUpdate();
		};
		planetListener->onTouchEnded = [this, winSize, i, back](Touch* touch, Event* event)
		{
			if (!getPlanet()->isPutable())
			{
				getPlanet()->getSprite()->removeFromParent();
				getStage()->removeObject(getPlanet());
			}
			else
			{
				auto moveListener = createMoveListener(back, getPlanet(), i);
				this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(moveListener, getPlanet()->getSprite());
			}
			setPlanet(NULL);

			if (getStage()->getGoaled())
			{
				for (StageObject* obj : _stage->getObjects())
					if (dynamic_cast<StageRay*>(obj) != NULL)
						(dynamic_cast<StageRay*>(obj))->animationWithGoal();

				auto cancelNode = Node::create();
				cancelNode->setContentSize(Size(800, 600));
				this->addChild(cancelNode, 1000);

				auto cancelListener = EventListenerTouchOneByOne::create();
				cancelListener->setSwallowTouches(true);
				cancelListener->onTouchBegan = [this](Touch* touch, Event* event)
				{
//					CCLOG("CancelListener");
					getStage()->removeAllAddObject();
					getStage()->stageUpdate();
					Director::getInstance()->popScene();
					this->getCf()->execute();
					return true;
				};
				this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(cancelListener, cancelNode);

				getStage()->getGoalZone()->explosion();
				SimpleAudioEngine::getInstance()->playBackgroundMusic("bgm/bgm_clear.wav", false);
			}
		};
		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(planetListener, column);
	}

	if (_stage)
		for (StageObject* obj : _stage->getAddStageObjectByRef())
			if (dynamic_cast<Planet*>(obj) != NULL)
				this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(createMoveListener(back, dynamic_cast<Planet*>(obj), static_cast<int>((dynamic_cast<Planet*>(obj))->getType()) + 1), (dynamic_cast<Planet*>(obj))->getSprite());

	auto backListener = EventListenerGesture::create();
	backListener->setSwallowTouches(true);
	backListener->onSwipe = [this] (EventListenerGesture::SwipeDirection direction) {
		if (direction == EventListenerGesture::SwipeDirection::DOWN)
		{
			Director::getInstance()->popScene();
			this->getCf()->execute();
		}
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backListener, back);
}

EventListenerTouchOneByOne * Game::createMoveListener(Sprite* back, Planet* pl, int i)
{
	auto winSize = Director::getInstance()->getWinSize();

	auto moveListener = EventListenerTouchOneByOne::create();
	moveListener->setSwallowTouches(true);
	moveListener->onTouchBegan = [this, pl, winSize, i](Touch* touch, Event* event)
	{
		if (MyUtils::getInstance()->isTouchInEvent(touch, event))
		{
			pl->setX(touch->getLocation().x - (winSize.width - _stage->getWidth()) / 2);
			pl->setY(touch->getLocation().y - (winSize.height - _stage->getHeight()) / 2);
			setPlanet(pl);
			getStage()->stageUpdate();
			return true;
		}
		return false;
	};
	moveListener->onTouchMoved = [this, winSize](Touch* touch, Event* event)
	{
		if (getPlanet()->setPosition(touch->getLocation().x - (winSize.width - _stage->getWidth()) / 2, touch->getLocation().y - (winSize.height - _stage->getHeight()) / 2, _stage))
			getStage()->stageUpdate();
	};
	moveListener->onTouchEnded = [this, moveListener, back](Touch* touch, Event* event)
	{
		if (!getPlanet()->isPutable())
		{
			getPlanet()->getSprite()->removeFromParent();
			getStage()->removeObject(getPlanet());
		}
		setPlanet(NULL);

		if (getStage()->getGoaled())
		{
			for (StageObject* obj : _stage->getObjects())
				if (dynamic_cast<StageRay*>(obj) != NULL)
					(dynamic_cast<StageRay*>(obj))->animationWithGoal();

			auto cancelNode = Node::create();
			cancelNode->setContentSize(Size(800, 600));
			this->addChild(cancelNode, 1000);

			auto cancelListener = EventListenerTouchOneByOne::create();
			cancelListener->setSwallowTouches(true);
			cancelListener->onTouchBegan = [this](Touch* touch, Event* event)
			{
				getStage()->removeAllAddObject();
				getStage()->stageUpdate();
				Director::getInstance()->popScene();
				this->getCf()->execute();
				return true;
			};
			this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(cancelListener, cancelNode);

			getStage()->getGoalZone()->explosion();
			SimpleAudioEngine::getInstance()->playBackgroundMusic("bgm/bgm_clear.wav", false);
		}
	};

	return moveListener;
}
