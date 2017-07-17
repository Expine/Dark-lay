#include "Planet.h"

#include "system/MyUtils.h"

USING_NS_CC;

/*
 * Constructor
 */
Planet::Planet()
{
	_sprite = NULL;
	_xSprite = NULL;
	_putable = false;
}

/*
 * Destructor
 */
Planet::~Planet()
{
	CC_SAFE_RELEASE_NULL(_sprite);
	CC_SAFE_RELEASE_NULL(_xSprite);
}

/*
 * Initialize
 */
bool Planet::init()
{
    if ( !StageObject::init() )
    {
        return false;
    }

    return true;
}

void Planet::setType(PlanetType type)
{
	_type = type;
	_radius = RADIUS[static_cast<int>(type)];
	_attraction = ATTRACTION[static_cast<int>(type)];
}

bool Planet::setPosition(int x, int y, Stage* stage)
{
	if (x == _x && y == _y)
		return false;

	setX(x);
	setY(y);
	getSprite()->setPosition(_x, _y);

	auto winSize = Director::getInstance()->getWinSize();

	_putable =
		(x >= _radius) &&
		(stage->getWidth() - _radius >= x) &&
		(y >= _radius) &&
		(stage->getHeight() - _radius >= y);

	for (StageObject* obj : stage->getStageObjectByRef())
	{
		if (!_putable)
			break;
		if (obj == this)
			continue;

		Vec2 v = Vec2(x, y);
		_putable &= !(obj->settingCollide(v, (int)_radius));
	}

	_xSprite->setVisible(!_putable);
	return true;
}

void Planet::physics(cocos2d::Vec2& point, cocos2d::Vec2& a)
{
	if (!_putable)
		return;

	Vec2 vec = Vec2(_x, _y) - point;
	a += vec.getNormalized() * (getAttraction() / (vec.x * vec.x + vec.y * vec.y));
}

bool Planet::settingCollide(cocos2d::Vec2 & point, int radius)
{
	float l = (Vec2(_x, _y) - point).getLength();
	return l <= radius + _radius;
}

CollideFlag Planet::collide(cocos2d::Vec2 & start, cocos2d::Vec2 & end, cocos2d::Vec2 & vec)
{
	Vec2 v = end - start;
	Vec2 a = Vec2(_x, _y) - start;
	Vec2 b = Vec2(_x, _y) - end;
	float d = abs(v.cross(a)) / v.getLength();
	vec.set(start + (v * (v.dot(a) / (v.x * v.x + v.y * v.y))));
	if (d <= _radius) {
		if (v.dot(a) * v.dot(b) <= 0) return CollideFlag::finish;
		if (a.x * a.x + a.y * a.y < d * d || b.x * b.x + b.y * b.y < d * d) return CollideFlag::finish;
	}
	return CollideFlag::none;
}


void Planet::draw(cocos2d::Node * target)
{
	//Create sprite
	auto planet = Sprite::create("image/planets.png", Rect(100 * (static_cast<int>(_type) - 1), 0, 100, 100));
	planet->setPosition(_x, _y);
	setSprite(planet);
	target->addChild(planet);

	auto x = Sprite::create("image/x.png");
	x->setPosition(x->getContentSize().width, x->getContentSize().height);
	x->setScale(_radius / 25.0f);
	setXSprite(x);
	_xSprite->setVisible(!_putable);
	planet->addChild(x);
}
