#ifndef __STAGE_H__
#define __STAGE_H__

#include "cocos2d.h"

#include "stage/GoalZone.h"
#include "stage/Object.h"

class Stage : public cocos2d::Ref
{
private:
	cocos2d::Vector<StageObject*> _addObjects;
protected:
	Stage();
	virtual ~Stage();
    virtual bool init();    
public:
	CREATE_FUNC(Stage);
	CC_SYNTHESIZE(cocos2d::Vector<StageObject*>, _objects, Objects);
	CC_SYNTHESIZE(int, _width, Width);
	CC_SYNTHESIZE(int, _height, Height);
	CC_SYNTHESIZE(bool, _goaled, Goaled);
	CC_SYNTHESIZE(GoalZone*, _goalZone, GoalZone);
	CC_SYNTHESIZE_RETAIN(cocos2d::Node*, _target, Target);
	void parseObject(std::string& fileName);
	void parseRowObject(int no);
	void setSurroundedWall();
	void addObject(StageObject* obj);
	void addObjectWithNoDraw(StageObject* obj) { _objects.pushBack(obj); };
	inline void removeObject(StageObject* obj) { _objects.eraseObject(obj); _addObjects.eraseObject(obj); };
	inline void removeAllAddObject() { for (StageObject* obj : _addObjects) _objects.eraseObject(obj); };
	cocos2d::Sprite* getStageSprite();
	void stageUpdate();
	const cocos2d::Vector<StageObject*>& getStageObjectByRef();
	inline const cocos2d::Vector<StageObject*>& getAddStageObjectByRef() { return _addObjects; };
	StageObject* getObjectByCoodinate(int x, int y);
};

#endif // __STAGE_H__
