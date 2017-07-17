#include <math.h>

#include "EditScene.h"

#include "stage/Mirror.h"
#include "stage/Planet.h"
#include "stage/Wall.h"
#include "system/MyUtils.h"

USING_NS_CC;

const static int MAX_ITEMS = 7;

cocos2d::Scene * EditScene::createScene()
{
	auto scene = Scene::create();
	auto layer = EditScene::create();
	scene->addChild(layer);
	return scene;
}

EditScene::EditScene()
{
	_back = NULL;
	_stage = NULL;
	_object = NULL;
	_ray = NULL;
	_goal = NULL;
	_stage_sprite = NULL;
	_ray_sprite = NULL;
	_goal_sprite = NULL;
	_editNode = NULL;
	_editListener = NULL;
	_swipeStartPosX = -1;
	_swipeStartPosY = -1;
	_swipePosX = -1;
	_swipePosY = -1;
	_scroll = 0;
	_moveDir = MoveDirection::none;
	_mode = EditMode::NORMAL;
}

EditScene::~EditScene()
{
	CC_SAFE_RELEASE_NULL(_back);
	CC_SAFE_RELEASE_NULL(_stage);
	CC_SAFE_RELEASE_NULL(_object);
}

bool EditScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	setStage(Stage::create());
	getStage()->setWidth(700);
	getStage()->setHeight(450);
	getStage()->setSurroundedWall();

	//Set base data
	auto winSize = Director::getInstance()->getWinSize();

	//Set background
	setBack(Sprite::create("image/background.png"));
	_back->setPosition(winSize.width / 2, winSize.height / 2);
	this->addChild(_back);

	//Set Ray
	_ray = StageRay::create();
	_ray->setX(1);
	_ray->setY(400);
	_ray->setInitVec(Vec2(1, 0));
	_ray->setStage(_stage);
	getStage()->addObjectWithNoDraw(_ray);

	//Set Goal
	_goal = GoalZone::create();
	_goal->setX(650);
	_goal->setY(400);
	_goal->setRadius(50);
	getStage()->addObjectWithNoDraw(_goal);

	auto stage = _stage->getStageSprite();
	_stage->stageUpdate();
	stage->setPosition(_back->getContentSize().width / 2, (_back->getContentSize().height + 100) / 2);
	_back->addChild(stage, 10);
	_stage_sprite = stage;

	//Set sprite
	_ray_sprite = Sprite::create("image/ray.png");
	_ray_sprite->setPosition(_ray->getX(), _ray->getY());
	stage->addChild(_ray_sprite, 200);
	_goal_sprite = _goal->getGoalZone();

	auto columns = Node::create();
	columns->setContentSize(Size(1300, 100));
	columns->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_back->addChild(columns);

	for (int i = 0; i<MAX_ITEMS; i++)
	{
		auto column = Sprite::create("image/column.png");
		column->setPosition(50 + i * 100, 50);
		columns->addChild(column, 20);

		auto planet = Sprite::create("image/items.png", Rect(0 + i * 100, 0, 100, 100));
		planet->setPosition(50 + i * 100, 50);
		planet->setTag(i);
		columns->addChild(planet, 30);

		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(createColumnListener(columns, i), column);
	}

	for (int i = 0; i<8; i++)
	{
		auto column = Sprite::create("image/column.png");
		column->setPosition(50 + i * 100 + MAX_ITEMS * 100, 50);
		columns->addChild(column, 20);

		auto planet = Sprite::create("image/planets.png", Rect(0 + i * 100, 0, 100, 100));
		planet->setPosition(50 + i * 100 + MAX_ITEMS * 100, 50);
		columns->addChild(planet, 30);

		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(createColumnListener(columns, i + MAX_ITEMS), column);
	}

	auto rayListener = EventListenerTouchOneByOne::create();
	rayListener->setSwallowTouches(true);
	rayListener->onTouchBegan = [this](Touch* touch, Event* event)
	{
		if (MyUtils::getInstance()->isTouchInEvent(touch, event) && !getObject())
		{
			setObject(_ray);
			return true;
		}
		return false;
	};
	rayListener->onTouchMoved = [this, winSize](Touch* touch, Event* event)
	{
		int x = touch->getLocation().x - (winSize.width - _stage->getWidth()) / 2;
		int y = touch->getLocation().y - (winSize.height - _stage->getHeight()) / 2;
		x = (x < 0) ? 0 : (x > getStage()->getWidth()) ? getStage()->getWidth() : x;
		y = (y < 0) ? 0 : (y > getStage()->getHeight()) ? getStage()->getHeight() : y;
		_ray_sprite->setPosition(x, y);
		getObject()->setX(x);
		getObject()->setY(y);
		getStage()->stageUpdate();
	};
	rayListener->onTouchEnded = [this](Touch* touch, Event* event)
	{
		setObject(NULL);
	};
	rayListener->onTouchCancelled = [rayListener](Touch* touch, Event* event)
	{
		rayListener->onTouchEnded(touch, event);
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(rayListener, _ray_sprite);

	auto goalListener = EventListenerTouchOneByOne::create();
	goalListener->setSwallowTouches(true);
	goalListener->onTouchBegan = [this](Touch* touch, Event* event)
	{
		if (MyUtils::getInstance()->isTouchInEvent(touch, event) && !getObject())
		{
			setObject(_goal);
			return true;
		}
		return false;
	};
	goalListener->onTouchMoved = [this, winSize](Touch* touch, Event* event)
	{
		int x = touch->getLocation().x - (winSize.width - _stage->getWidth()) / 2;
		int y = touch->getLocation().y - (winSize.height - _stage->getHeight()) / 2;
		x = (x < 0) ? 0 : (x > getStage()->getWidth()) ? getStage()->getWidth() : x;
		y = (y < 0) ? 0 : (y > getStage()->getHeight()) ? getStage()->getHeight() : y;
		_goal_sprite->setPosition(x, y);
		getObject()->setX(x);
		getObject()->setY(y);
		getStage()->stageUpdate();
	};
	goalListener->onTouchEnded = [this](Touch* touch, Event* event)
	{
		setObject(NULL);
	};
	goalListener->onTouchCancelled = [goalListener](Touch* touch, Event* event)
	{
		goalListener->onTouchEnded(touch, event);
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(goalListener, _goal_sprite);

	auto backListener = EventListenerTouchOneByOne::create();
	backListener->setSwallowTouches(true);
	backListener->onTouchBegan = [](Touch* touch, Event* event)
	{
//		Director::getInstance()->popScene();
		return true;
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backListener, _back);

	return true;
}

EventListenerTouchOneByOne * EditScene::createColumnListener(Node * columns, int i)
{
	auto winSize = Director::getInstance()->getWinSize();

	auto columnListener = EventListenerTouchOneByOne::create();
	columnListener->setSwallowTouches(true);
	columnListener->onTouchBegan = [this, i, columns](Touch* touch, Event* event)
	{
		if (MyUtils::getInstance()->isTouchInEvent(touch, event) && _swipePosX == -1)
		{
			EditMode preMode = _mode;
			bool lightup = true;
			if (_mode != EditMode::NORMAL)
			{
				_mode = EditMode::NORMAL;
				if (_editListener)
					this->getEventDispatcher()->removeEventListener(_editListener);
				if (_editNode)
					_editNode->removeFromParent();
				for (int j = 0; j < MAX_ITEMS; j++)
					dynamic_cast<Sprite*>(columns->getChildByTag(j))->setSpriteFrame(SpriteFrame::create("image/items.png", Rect(0 + j * 100, 0, 100, 100)));
//					columns->getChildByTag(j)->runAction(TintTo::create(0.1f, 255, 255, 255));
			}
			_swipePosX = touch->getLocation().x;
			_swipePosY = touch->getLocation().y;
			_swipeStartPosX = _swipePosX;
			_swipeStartPosY = _swipePosY;
			if (i == 2)
			{
				ray(-1);
				_moveDir = MoveDirection::ray;
			}
			else if (i == 3)
			{
				ray(1);
				_moveDir = MoveDirection::ray;
			}
			else if (i == 4)
			{
				if(preMode == EditMode::WALL)
					lightup = false;
				else
				{
					drawLine(true);
					_moveDir = MoveDirection::ray;
				}
			}
			else if (i == 5)
			{
				if (preMode == EditMode::MIRROR)
					lightup = false;
				else
				{
					drawLine(false);
					_moveDir = MoveDirection::ray;
				}
			}
			else if (i == 6)
			{
				if (preMode == EditMode::ERASER)
					lightup = false;
				else
				{
					eraser();
					_moveDir = MoveDirection::ray;
				}
			}
			if (i < MAX_ITEMS && lightup)
			{
//				columns->getChildByTag(i)->runAction(TintTo::create(0.1f, 128, 128, 128));
				dynamic_cast<Sprite*>(columns->getChildByTag(i))->setSpriteFrame(SpriteFrame::create("image/items.png", Rect(0 + i * 100, 100, 100, 100)));
			}
			return true;
		}
		return false;
	};
	columnListener->onTouchMoved = [this, columns, i, winSize](Touch* touch, Event* event)
	{
		if (_moveDir == MoveDirection::none)
		{
			if (abs(_swipePosY - touch->getLocation().y) > abs(_swipePosX - touch->getLocation().x))
			{
				_moveDir = MoveDirection::up;

				if (i >= MAX_ITEMS)
				{
					auto planet = Planet::create();
					planet->setX(touch->getLocation().x - (winSize.width - getStage()->getWidth()) / 2);
					planet->setX(touch->getLocation().y - (winSize.height - getStage()->getHeight()) / 2);
					planet->setType(static_cast<PlanetType>(i - MAX_ITEMS + 1));
					setObject(planet);
					getStage()->addObject(planet);
					getStage()->stageUpdate();
				}
			}
			else
			{
				_moveDir = MoveDirection::side;
			}
		}
		if (_moveDir == MoveDirection::up)
		{
			if (getObject())
			{
				if (dynamic_cast<Planet*>(getObject()) != NULL)
				{
					if( dynamic_cast<Planet*>(getObject())->setPosition(
						touch->getLocation().x - (winSize.width - _stage->getWidth()) / 2, 
						touch->getLocation().y - (winSize.height - _stage->getHeight()) / 2, getStage()))
					getStage()->stageUpdate();
				}
				else
				{
					getObject()->setX(touch->getLocation().x - (winSize.width - _stage->getWidth()) / 2);
					getObject()->setY(touch->getLocation().y - (winSize.height - _stage->getHeight()) / 2);
					getStage()->stageUpdate();
				}
			}
			return;
		}
		else if(_moveDir == MoveDirection::side)
		{
			_scroll -= (touch->getLocation().x - _swipePosX);
			_swipePosX = touch->getLocation().x;
			_swipePosY = touch->getLocation().y;

			_scroll = (_scroll < 0) ? 0 : (_scroll > MAX_ITEMS * 100) ? MAX_ITEMS * 100 : _scroll;

			columns->stopAllActions();
			columns->runAction(MoveTo::create(0.5f, Vec2(-_scroll, 0)));
		}
	};
	columnListener->onTouchEnded = [this, i, columns](Touch* touch, Event* event)
	{
		if (_moveDir == MoveDirection::up)
		{
			if (dynamic_cast<Planet*>(getObject()) != NULL)
			{
				Planet* pl = dynamic_cast<Planet*>(getObject());
				if (!pl->isPutable())
				{
					pl->getSprite()->removeFromParent();
					getStage()->removeObject(pl);
				}
				else
				{
					auto moveListener = createMoveListener(_back, pl, i - MAX_ITEMS + 1);
					this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(moveListener, pl->getSprite());
				}
			}
			setObject(NULL);
		}
		else
		{
			if (abs(_swipeStartPosX - touch->getLocation().x) > 1.0f)
			{}
			else if (i == 0)
				save();
			else if (i == 1)
				discard();
		}
		_swipeStartPosX = -1;
		_swipeStartPosY = -1;
		_swipePosX = -1;
		_swipePosY = -1;
		_moveDir = MoveDirection::none;
		rayStop();
		if (i < MAX_ITEMS)
		{
			if(i < 4)
//				columns->getChildByTag(i)->runAction(TintTo::create(0.1f, 255, 255, 255));
				dynamic_cast<Sprite*>(columns->getChildByTag(i))->setSpriteFrame(SpriteFrame::create("image/items.png", Rect(0 + i * 100, 0, 100, 100)));
		}
	};
	columnListener->onTouchCancelled = [columnListener](Touch* touch, Event* event)
	{
		columnListener->onTouchEnded(touch, event);
	};
	return columnListener;
}

void EditScene::save()
{
	UserDefault *userDef = UserDefault::getInstance();

	int num = userDef->getIntegerForKey("number", 0) + 1;
	userDef->setIntegerForKey("number", num);
	userDef->flush();

	//Set path
	auto path = FileUtils::getInstance()->getWritablePath();
	auto file = path + StringUtils::format("user%d.json", num);

	//Set Data
	std::string text;
	text += "{";
	int i = 1;
	for (StageObject* obj : getStage()->getStageObjectByRef())
	{
		if (i > 1)
			text += ",";
		text += StringUtils::format("\n\t\"obj%d\":{\n", i++);
		if (dynamic_cast<StageRay*>(obj) != NULL)
		{
			StageRay* ray = dynamic_cast<StageRay*>(obj);
			text += "\t\t\"type\" : \"ray\",\n";
			text += StringUtils::format("\t\t\"pos\" : {\n\t\t\t\"x\" : %d,\n\t\t\t\"y\" : %d\n\t\t},\n", obj->getX(), obj->getY());
			text += StringUtils::format("\t\t\"vec\" : {\n\t\t\t\"x\" : %d,\n\t\t\t\"y\" : %d\n\t\t}\n", static_cast<int>(ray->getInitX() * 10000), static_cast<int>(ray->getInitY() * 10000));
		}
		else if (dynamic_cast<GoalZone*>(obj) != NULL)
		{
			GoalZone* goal = dynamic_cast<GoalZone*>(obj);
			text += "\t\t\"type\" : \"goal\",\n";
			text += StringUtils::format("\t\t\"pos\" : {\n\t\t\t\"x\" : %d,\n\t\t\t\"y\" : %d\n\t\t},\n", obj->getX(), obj->getY());
			text += StringUtils::format("\t\t\"radius\" : %d\n", goal->getRadius());
		}
		else if (dynamic_cast<Wall*>(obj) != NULL)
		{
			Wall* wall = dynamic_cast<Wall*>(obj);
			text += "\t\t\"type\" : \"Wall\",\n";
			text += StringUtils::format("\t\t\"pos\" : {\n\t\t\t\"x\" : %d,\n\t\t\t\"y\" : %d\n\t\t},\n", obj->getX(), obj->getY());
			text += StringUtils::format("\t\t\"endpos\" : {\n\t\t\t\"x\" : %d,\n\t\t\t\"y\" : %d\n\t\t}\n", wall->getEndX(), wall->getEndY());
		}
		else if (dynamic_cast<Mirror*>(obj) != NULL)
		{
			Mirror* mirror = dynamic_cast<Mirror*>(obj);
			text += "\t\t\"type\" : \"Mirror\",\n";
			text += StringUtils::format("\t\t\"pos\" : {\n\t\t\t\"x\" : %d,\n\t\t\t\"y\" : %d\n\t\t},\n", obj->getX(), obj->getY());
			text += StringUtils::format("\t\t\"endpos\" : {\n\t\t\t\"x\" : %d,\n\t\t\t\"y\" : %d\n\t\t}\n", mirror->getEndX(), mirror->getEndY());
		}
		text += StringUtils::format("\t}");
	}
	text += "}";


	if (FileUtils::getInstance()->writeStringToFile(text, file))
		CCLOG("Write data to %s.", file.c_str());
	else
		CCLOG("[ERROR] Not Saving ! to %s", file.c_str());

	Director::getInstance()->popScene();
}

void EditScene::discard()
{
	auto check = Sprite::create("image/checkFrame.png");
	check->setPosition(_back->getContentSize().width / 2, _back->getContentSize().height / 2);
	_back->addChild(check, 1000);

	auto ok = Sprite::create("image/checkOK.png");
	ok->setPosition(115, 80);
	check->addChild(ok);

	auto cancel = Sprite::create("image/checkCancel.png");
	cancel->setPosition(283, 78);
	check->addChild(cancel);

	auto penetrationListener = EventListenerTouchOneByOne::create();
	penetrationListener->setSwallowTouches(true);
	penetrationListener->onTouchBegan = [](Touch* touch, Event* event)
	{
		return true;
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(penetrationListener, check);

	auto okListener = EventListenerTouchOneByOne::create();
	okListener->setSwallowTouches(true);
	okListener->onTouchBegan = [](Touch* touch, Event* event)
	{
		if (MyUtils::getInstance()->isTouchInEvent(touch, event))
		{
			Director::getInstance()->popScene();
			return true;
		}
		return false;
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(okListener, ok);

	auto cancelListener = EventListenerTouchOneByOne::create();
	cancelListener->setSwallowTouches(true);
	cancelListener->onTouchBegan = [check](Touch* touch, Event* event)
	{
		if (MyUtils::getInstance()->isTouchInEvent(touch, event))
		{
			check->removeFromParentAndCleanup(true);
			return true;
		}
		return false;
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(cancelListener, cancel);

}

void EditScene::ray(int rotateDir)
{
	_rotate = rotateDir;
	_ray_sprite->stopAllActions();
	_ray_sprite->runAction(RepeatForever::create(Sequence::create(
		CallFunc::create([this] {
			Vec2 v = Vec2(_ray->getInitX(), _ray->getInitY());
			v.rotate(Vec2(0, 0), 0.002f * _rotate);
			_ray->setInitVec(v);
			_rotate *= 1.5f;
			if(_rotate > 0)
				_rotate = (_rotate > 40.0) ? 40.0 : _rotate;
			else
				_rotate = (_rotate < -40.0) ? -40.0 : _rotate;
			_stage->stageUpdate();
		}),
		DelayTime::create(0.1f),
		NULL
	)));
}

void EditScene::rayStop()
{
	_ray_sprite->stopAllActions();
}

void EditScene::drawLine(bool wall)
{
	if (wall)
		_mode = EditMode::WALL;
	else
		_mode = EditMode::MIRROR;
	auto LineNode = Node::create();
	LineNode->setContentSize(Size(800, 500));
	LineNode->setPosition(0, 100);
	LineNode->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_back->addChild(LineNode, 10000);

	auto drawNode = DrawNode::create();
	LineNode->addChild(drawNode);

	auto LineListener = EventListenerTouchOneByOne::create();
	LineListener->setSwallowTouches(true);
	LineListener->onTouchBegan = [this](Touch* touch, Event* event)
	{
		if (MyUtils::getInstance()->isTouchInEvent(touch, event))
		{
			_swipePosX = touch->getLocation().x;
			_swipePosY = touch->getLocation().y;
			_swipeStartPosX = _swipePosX;
			_swipeStartPosY = _swipePosY;
			return true;
		}
		return false;
	};
	LineListener->onTouchMoved = [this, drawNode, wall](Touch* touch, Event* event)
	{
		auto winSize = Director::getInstance()->getWinSize();
		drawNode->clear();
		int x = _swipeStartPosX;
		int y = _swipeStartPosY - 100;
		int ex = touch->getLocation().x;
		int ey = touch->getLocation().y - 100;
		Color4F color;
		if (wall)
			color = Color4F(1.0f, 1.0f, 1.0f, 1.0f);
		else
			color = Color4F(0.0f, 0.0f, 1.0f, 1.0f);
		drawNode->drawSegment(Vec2(x, y), Vec2(ex, ey), 2, color);
	};
	LineListener->onTouchEnded = [this, drawNode, wall](Touch* touch, Event* event)
	{
		auto winSize = Director::getInstance()->getWinSize();

		if (wall)
		{
			auto wall = Wall::create();
			wall->setX(_swipeStartPosX - (winSize.width - _stage->getWidth()) / 2);
			wall->setY(_swipeStartPosY - (winSize.height - _stage->getHeight()) / 2 - 50);
			wall->setEndX(touch->getLocation().x - (winSize.width - _stage->getWidth()) / 2);
			wall->setEndY(touch->getLocation().y - (winSize.height - _stage->getHeight()) / 2 - 50);
			getStage()->addObject(wall);
			getStage()->stageUpdate();

			/*
			auto target = Node::create();
			target->setContentSize(Size(wall->getRight() - wall->getLeft(), wall->getUp() - wall->getBottom()));
			target->setPosition(wall->getLeft(), wall->getBottom());
			target->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
			_stage_sprite->addChild(target);
			setObject(wall);
			addObjectListener(target);
			setObject(NULL);
			*/
		}
		else
		{
			auto mirror = Mirror::create();
			mirror->setX(_swipeStartPosX - (winSize.width - _stage->getWidth()) / 2);
			mirror->setY(_swipeStartPosY - (winSize.height - _stage->getHeight()) / 2 - 50);
			mirror->setEndX(touch->getLocation().x - (winSize.width - _stage->getWidth()) / 2);
			mirror->setEndY(touch->getLocation().y - (winSize.height - _stage->getHeight()) / 2 - 50);
			getStage()->addObject(mirror);
			getStage()->stageUpdate();

			/*
			auto target = Node::create();
			target->setContentSize(Size(mirror->getRight() - mirror->getLeft(), mirror->getUp() - mirror->getBottom()));
			target->setPosition(mirror->getLeft(), mirror->getBottom());
			target->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
			_stage_sprite->addChild(target);
			setObject(mirror);
			addObjectListener(target);
			setObject(NULL);
			*/
		}

		drawNode->clear();
		_swipePosX = -1;
		_swipePosY = -1;
		_swipeStartPosX = -1;
		_swipeStartPosY = -1;
	};
	LineListener->onTouchCancelled = [LineListener](Touch* touch, Event* event)
	{
		LineListener->onTouchEnded(touch, event);
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(LineListener, LineNode);

	_editNode = LineNode;
	_editListener = LineListener;
}

void EditScene::eraser()
{
	_mode = EditMode::ERASER;

	auto eraseNode = Node::create();
	eraseNode->setContentSize(Size(800, 500));
	eraseNode->setPosition(0, 100);
	eraseNode->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_back->addChild(eraseNode, 10000);

	auto eraseListener = EventListenerTouchOneByOne::create();
	eraseListener->setSwallowTouches(true);
	eraseListener->onTouchBegan = [this] (Touch* touch, Event* event){
		if (MyUtils::getInstance()->isTouchInEvent(touch, event))
		{
			auto winSize = Director::getInstance()->getWinSize();
			int x = touch->getLocation().x - (winSize.width - _stage->getWidth()) / 2;
			int y = touch->getLocation().y - (winSize.height - _stage->getHeight()) / 2 - 50;
			auto obj = getStage()->getObjectByCoodinate(x, y);
			if (dynamic_cast<GoalZone*>(obj) != NULL)
				return true;
			if (dynamic_cast<StageRay*>(obj) != NULL)
				return true;
			if (obj)
			{
				getStage()->removeObject(obj);
				getStage()->stageUpdate();
			}
			return true;
		}
		return false;
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eraseListener, eraseNode);

	_editNode = eraseNode;
	_editListener = eraseListener;
}

void EditScene::addObjectListener(Node* target)
{
	auto winSize = Director::getInstance()->getWinSize();

	auto obj = getObject();
	auto moveListener = EventListenerTouchOneByOne::create();
	moveListener->setSwallowTouches(true);
	moveListener->onTouchBegan = [this, obj, winSize](Touch* touch, Event* event)
	{
		if (MyUtils::getInstance()->isTouchInEvent(touch, event) && !getObject())
		{
			setObject(obj);
			getObject()->setX(touch->getLocation().x - (winSize.width - _stage->getWidth()) / 2);
			getObject()->setY(touch->getLocation().y - (winSize.height - _stage->getHeight()) / 2);
			getStage()->stageUpdate();
			return true;
		}
		return false;
	};
	moveListener->onTouchMoved = [this, winSize](Touch* touch, Event* event)
	{
		bool putable = 
			getObject()->getLeft() >= 0 &&
			getObject()->getRight() <= getStage()->getWidth() &&
			getObject()->getBottom() >= 0 &&
			getObject()->getUp() <= getStage()->getHeight();
		if (putable)
		{
			int x = touch->getLocation().x - (winSize.width - _stage->getWidth()) / 2;
			int y = touch->getLocation().y - (winSize.height - _stage->getHeight()) / 2;
			getObject()->setPosition(x, y);
			getStage()->stageUpdate();
		}
	};
	moveListener->onTouchEnded = [this, moveListener, target](Touch* touch, Event* event)
	{
		bool putable =
			getObject()->getLeft() >= 0 &&
			getObject()->getRight() <= getStage()->getWidth() &&
			getObject()->getBottom() >= 0 &&
			getObject()->getUp() <= getStage()->getHeight();
		if (!putable)
		{
			target->removeFromParent();
			getStage()->removeObject(getObject());
		}
		setObject(NULL);

	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(moveListener, target);
}

EventListenerTouchOneByOne * EditScene::createMoveListener(Sprite* back, Planet* pl, int i)
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
			setObject(pl);
			getStage()->stageUpdate();
			return true;
		}
		return false;
	};
	moveListener->onTouchMoved = [this, pl, winSize](Touch* touch, Event* event)
	{
		if (pl->setPosition(touch->getLocation().x - (winSize.width - _stage->getWidth()) / 2, touch->getLocation().y - (winSize.height - _stage->getHeight()) / 2, _stage))
			getStage()->stageUpdate();
	};
	moveListener->onTouchEnded = [this, pl, moveListener, back](Touch* touch, Event* event)
	{
		if (!pl->isPutable())
		{
			pl->getSprite()->removeFromParent();
			getStage()->removeObject(pl);
		}
		setObject(NULL);
	};

	return moveListener;
}
