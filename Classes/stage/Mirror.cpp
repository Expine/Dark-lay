#include "Mirror.h"

#include "system/MyUtils.h"

USING_NS_CC;

/*
 * Constructor
 */
Mirror::Mirror()
{
	_endX = 0;
	_endY = 0;
	_drawNode = NULL;
}

/*
 * Destructor
 */
Mirror::~Mirror()
{
	if (_drawNode)
		_drawNode->removeFromParent();
	_drawNode = NULL;
}

/*
 * Initialize
 */
bool Mirror::init()
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
void Mirror::draw(Node* target)
{
	auto node = DrawNode::create();
	node->drawSegment(Vec2(_x, _y), Vec2(_endX, _endY), 2, Color4F(0.0f, 0.0f, 1.0f, 1.0f));
	target->addChild(node);

	_drawNode = node;
}

bool Mirror::settingCollide(cocos2d::Vec2 & point, int radius)
{
	float _r = radius;
	Vec2 v = Vec2(_endX, _endY) - Vec2(_x, _y).getNormalized();
	Vec2 a = point - Vec2(_x, _y);
	Vec2 b = point - Vec2(_endX, _endY);
	float d = abs(v.cross(a));
	if (d > _r) return false;
	Vec2 _v = v * v.dot(a);
	if (0 <= v.dot(_v) && v.dot(_v) <= v.dot(Vec2(_endX, _endY) - Vec2(_x, _y))) return true;
	return a.x * a.x + a.y * a.y < radius * _r || b.x * b.x + b.y * b.y < radius * radius;
}

/*
* Collide
*/
CollideFlag Mirror::collide(Vec2& start, Vec2& end, Vec2& vec)
{
	Vec2 v = Vec2(_x - start.x, _y - start.y);
	Vec2 v1 = Vec2(end.x - start.x, end.y - start.y);
	Vec2 v2 = Vec2(_endX - _x, _endY - _y);
	float t1 = v.cross(v2) / v1.cross(v2);
	float t2 = v.cross(v1) / v1.cross(v2);

	if (0 <= t1 && t1 <= 1 && 0 <= t2 && t2 <= 1)
	{
		vec.x = _x + t2 * v2.x;
		vec.y = _y + t2 * v2.y;
		return CollideFlag::mirror;
	}

	return CollideFlag::none;
}

void Mirror::getReflectPos(Vec2 & result, Vec2 pos1, Vec2 pos2)
{
	Vec2 dir = pos2 - pos1;
	Vec2 vec1 = (Vec2(_endX, _endY) - Vec2(_x, _y)).getNormalized();
	Vec2 vec2 = ((vec1 * vec1.dot(dir)) - dir) * 2 + dir;
	result.set(pos1 + vec2);
}

void Mirror::getReflectVelocity(Vec2 & result, Vec2 v)
{
	Vec2 vec = (Vec2(_endX, _endY) - Vec2(_x, _y)).getNormalized();
	result.set((vec * vec.dot(v) - v) * 2 + v);
}

