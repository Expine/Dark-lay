#include <random>

#include "Ray.h"
#include "Mirror.h"

#include "scene/GameScene.h"
#include "system/MyUtils.h"

USING_NS_CC;

const float StageRay::dt = 1.0f;

/*
 * Constructor
 */
StageRay::StageRay()
{
	_goalFlag = false;
	_initX = 0;
	_initY = 0;
	_stage = NULL;
	_drawNode = NULL;
	_points.reserve(MAX_ITEM + 10);
}

/*
 * Destructor
 */
StageRay::~StageRay()
{
	CC_SAFE_RELEASE_NULL(_drawNode);
}

/*
 * Initialize
 */
bool StageRay::init()
{
    if ( !StageObject::init() )
    {
        return false;
    }

    return true;
}

/*
 * Draw
 */
void StageRay::draw(Node* target)
{
	auto node = DrawNode::create();
	setDrawNode(node);
	redrawNode();
	target->addChild(node);
}

/*
 * Update
 */
void StageRay::update()
{
	_goalFlag = false;
	_points.clear();
	_points.push_back(Vec2(_x, _y));

	auto a = Vec2(0, 0);
	auto v = Vec2(_initX, _initY);

	bool finishFlag = false;
	for(int i=0; i < MAX_ITEM && !finishFlag; i++)
	{
		v = v + (a * dt);

		for (StageObject* obj : _stage->getObjects())
		{
			obj->physics(_points[i], a);
		}
		_points.push_back(_points[i] + (v * dt));

		Vec2 vec = Vec2(0, 0); //�Փ˓_
		for (StageObject* obj : _stage->getObjects())
		{
			Mirror* m;
			Vec2 temp;
			switch (obj->collide(_points[i], _points[i+1], vec))
			{
			case CollideFlag::none:
				break;
			case CollideFlag::mirror:
				m = (Mirror*)obj;
				m->getReflectVelocity(v, v);
				m->getReflectPos(temp, vec, _points.back());
				_points.pop_back();
				_points.push_back(vec);
				_points.push_back(temp);
				i++;
				break;
			case CollideFlag::goal:
				_goalFlag = true;
			case CollideFlag::finish:
				finishFlag = true;
				_points.pop_back();
				_points.push_back(vec);
				break;
			}
		}

	}

	if (_drawNode)
		redrawNode();

	_stage->setGoaled(_goalFlag);
}

void StageRay::redrawNode()
{
	_drawNode->clear();
	Vec2 prePoint = Vec2(_x, _y);
	for (unsigned int i = 0; i<_points.size(); i++)
	{
		_drawNode->drawSegment(prePoint, Vec2(_points[i].x, _points[i].y), 1, Color4F(1.0f, 1.0f, 1.0f, 0.5f));
		prePoint.set(_points[i].x, _points[i].y);
	}
}

void StageRay::setInitVec(Vec2 v)
{
	v.normalize();
	_initX = v.x;
	_initY = v.y;
}

void StageRay::animationWithGoal()
{
	_drawNode->runAction(
		RepeatForever::create(
			Sequence::create(
				CallFunc::create([this] {
					_drawNode->clear();
					Vec2 prePoint = Vec2(_x, _y);
					Color4F color = Color4F(rand_0_1(), rand_0_1(), rand_0_1(), 0.5f);
					for (unsigned int i = 0; i<_points.size(); i++)
					{
						_drawNode->drawSegment(prePoint, Vec2(_points[i].x, _points[i].y), 1, color);
						prePoint.set(_points[i].x, _points[i].y);
					}
				}),
				DelayTime::create(0.1f),
				NULL
			)
		)
	);
}

