#include "Stage.h"

#include "GoalZone.h"
#include "Mirror.h"
#include "Ray.h"
#include "Wall.h"

#include "system/MyUtils.h"

USING_NS_CC;

/*
 * Constructor
 */
Stage::Stage()
{
	_width = 0;
	_height = 0;
	_target = NULL;
}

/*
 * Destructor
 */
Stage::~Stage()
{
	_objects.clear();
	_addObjects.clear();
	CC_SAFE_RELEASE_NULL(_target);
}

/*
 * Initialize
 */
bool Stage::init()
{
    return true;
}

void Stage::parseObject(std::string& text)
{
	int depth = 0;
	std::map<int, std::string> elements;
	std::string temp;
	std::string temp2;
	std::string name;
	std::string type;
	Vec2 pos;
	Vec2 endpos;
	Vec2 vec;
	int radius;
	for (char ch : text) {
		if (ch == '{')
			depth += 1;
		else if (ch == '}')
		{
			temp2 = elements[depth];
			if (temp2 == "type")
				type = temp;
			else if (temp2 == "x")
			{
				temp2 = elements[depth - 1];
				if (temp2 == "pos")
					pos.x = atoi(temp.c_str());
				else if (temp2 == "endpos")
					endpos.x = atoi(temp.c_str());
				else if (temp2 == "vec")
					vec.x = atoi(temp.c_str());
			}
			else if (temp2 == "y")
			{
				temp2 = elements[depth - 1];
				if (temp2 == "pos")
					pos.y = atoi(temp.c_str());
				else if (temp2 == "endpos")
					endpos.y = atoi(temp.c_str());
				else if (temp2 == "vec")
					vec.y = atoi(temp.c_str());
			}
			else if (temp2 == "radius")
				radius = atoi(temp.c_str());

			temp.clear();
			temp2.clear();

			elements[depth].clear();
			depth -= 1;

			if (depth == 1)
			{
//				log("%s = (%f, %f)(%f, %f) | (%f, %f)", type.c_str(), pos.x, pos.y, endpos.x, endpos.y, vec.x, vec.y);
				if (type == "ray")
				{
					auto ray = StageRay::create();
					ray->setX(pos.x);
					ray->setY(pos.y);
					ray->setInitVec(vec);
					ray->setStage(this);
					_objects.pushBack(ray);
				}
				else if (type == "Wall")
				{
					auto wall = Wall::create();
					wall->setX(pos.x);
					wall->setY(pos.y);
					wall->setEndX(endpos.x);
					wall->setEndY(endpos.y);
					_objects.pushBack(wall);
				}
				else if (type == "Mirror")
				{
					auto mirror = Mirror::create();
					mirror->setX(pos.x);
					mirror->setY(pos.y);
					mirror->setEndX(endpos.x);
					mirror->setEndY(endpos.y);
					_objects.pushBack(mirror);
				}
				else if (type == "goal")
				{
					auto goal = GoalZone::create();
					goal->setX(pos.x);
					goal->setY(pos.y);
					goal->setRadius(radius);
					_goalZone = goal;
					_objects.pushBack(goal);

				}
			}
		}
		else if (ch == '\n')
			continue;
		else if (ch == '\r')
			continue;
		else if (ch == '\t')
			continue;
		else if (ch == ' ')
			continue;
		else if (ch == ':')
		{
			elements[depth] = temp;
			temp.clear();
		}
		else if (ch == ',')
		{
			temp2 = elements[depth];
			if (temp2 == "type")
				type = temp;
			else if (temp2 == "x")
			{
				temp2 = elements[depth - 1];
				if (temp2 == "pos")
					pos.x = atoi(temp.c_str());
				else if (temp2 == "endpos")
					endpos.x = atoi(temp.c_str());
				else if (temp2 == "vec")
					vec.x = atoi(temp.c_str());
			}
			else if (temp2 == "y")
			{
				temp2 = elements[depth - 1];
				if (temp2 == "pos")
					pos.y = atoi(temp.c_str());
				else if (temp2 == "endpos")
					endpos.y = atoi(temp.c_str());
				else if (temp2 == "vec")
					vec.y = atoi(temp.c_str());
			}
			else if (temp2 == "radius")
				radius = atoi(temp.c_str());

			temp.clear();
			temp2.clear();
		}
		else if (ch == '"')
			continue;
		else
			temp += ch;
	}

	for (StageObject* obj : _objects)
	{
		_width = (_width < obj->getX()) ? obj->getX() : _width;
		_height = (_height < obj->getY()) ? obj->getY() : _height;
	}


	/*
	for (StageObject* obj : _objects)
	{
		obj->setY(_height - obj->getY());

		if (dynamic_cast<StageRay*>(obj) != NULL)
		{
			StageRay* ray = dynamic_cast<StageRay*>(obj);
			ray->setInitVec(Vec2(ray->getInitX(), ray->getInitY() * -1));
		}
		if (dynamic_cast<Wall*>(obj) != NULL)
		{
			Wall* wall = dynamic_cast<Wall*>(obj);
			wall->setEndY(_height - wall->getEndY());
		}
		if (dynamic_cast<Mirror*>(obj) != NULL)
		{
			Mirror* mirror = dynamic_cast<Mirror*>(obj);
			mirror->setEndY(_height - mirror->getEndY());
		}
	}
	*/
}

void Stage::parseRowObject(int no)
{
	if (no == 0)
	{
		auto goal = GoalZone::create();
		goal->setX(250);
		goal->setY(150);
		goal->setRadius(40);
		_goalZone = goal;
		_objects.pushBack(goal);

		auto ray = StageRay::create();
		ray->setX(1);
		ray->setY(270);
		ray->setInitVec(Vec2(1, 0));
		ray->setStage(this);
		_objects.pushBack(ray);

		_width = 400;
		_height = 300;
	}
	else if (no == 1)
	{
		auto goal = GoalZone::create();
		goal->setX(175);
		goal->setY(300);
		goal->setRadius(25);
		_goalZone = goal;
		_objects.pushBack(goal);

		auto ray = StageRay::create();
		ray->setX(1);
		ray->setY(300);
		ray->setInitVec(Vec2(2, 1));
		ray->setStage(this);
		_objects.pushBack(ray);

		auto wall = Wall::create();
		wall->setX(100);
		wall->setY(400);
		wall->setEndX(100);
		wall->setEndY(300);
		_objects.pushBack(wall);

		auto mirror = Mirror::create();
		mirror->setX(30);
		mirror->setY(370);
		mirror->setEndX(50);
		mirror->setEndY(300);
		_objects.pushBack(mirror);

		_width = 400;
		_height = 400;
	}

	setSurroundedWall();
}

void Stage::setSurroundedWall()
{
	int x[4] = { 0, _width, _width, 0 };
	int y[4] = { _height, _height, 0, 0 };
	int ex[4] = { _width, _width, 0, 0 };
	int ey[4] = { _height, 0, 0, _height };
	for(int i=0; i<4; i++)
	{
		auto wall = Wall::create();
		wall->setX(x[i]);
		wall->setY(y[i]);
		wall->setEndX(ex[i]);
		wall->setEndY(ey[i]);
		_objects.pushBack(wall);
	}
}

void Stage::addObject(StageObject* obj)
{
	_objects.pushBack(obj);
	_addObjects.pushBack(obj);
	obj->draw(_target);
}

Sprite* Stage::getStageSprite()
{
	auto node = Sprite::create();
	node->setContentSize(Size(_width, _height));
	setTarget(node);

	auto back = Sprite::create("image/background.png");
	back->setPosition(_width / 2, _height / 2);
	back->setScaleX(_width / 800.0f);
	back->setScaleY(_height / 600.0f);
	node->addChild(back);

	for(StageObject* obj : _objects)
	{
		obj->draw(node);
	}

	return node;
}

void Stage::stageUpdate()
{
	for (StageObject* obj : _objects)
	{
		obj->update();
	}
}

const Vector<StageObject*>& Stage::getStageObjectByRef()
{
	return _objects;
}

StageObject * Stage::getObjectByCoodinate(int x, int y)
{
	StageObject* result = nullptr;
	for (StageObject* obj : _objects)
	{
		if (x >= obj->getLeft() && x <= obj->getRight() && y >= obj->getBottom() && y <= obj->getUp())
			result = obj;
	}

	return result;
}
