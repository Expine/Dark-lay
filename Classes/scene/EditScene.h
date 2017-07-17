#ifndef __EDIT_SCENE_H__
#define __EDIT_SCENE_H__

#include "cocos2d.h"

#include "stage/Stage.h"
#include "stage/Object.h"
#include "stage/Ray.h"
#include "stage/Planet.h"
#include "stage/GoalZone.h"

enum class MoveDirection {
	side, up, none, ray
};

enum class EditMode {
	NORMAL, WALL, MIRROR, ERASER
};

class EditScene : public cocos2d::Layer
{
private:
	cocos2d::EventListenerTouchOneByOne* createColumnListener(cocos2d::Node* columns, int i);
	void save();
	void discard();
	void ray(int rotateDir);
	void rayStop();
	void drawLine(bool wall);
	void eraser();
	void addObjectListener(cocos2d::Node* target);
	cocos2d::EventListenerTouchOneByOne* createMoveListener(cocos2d::Sprite* back, Planet* pl, int i);
protected:
	EditScene();
	virtual ~EditScene();
	virtual bool init();
public:
	static cocos2d::Scene* createScene();
	CREATE_FUNC(EditScene);
	CC_SYNTHESIZE_RETAIN(cocos2d::Sprite*, _back, Back);
	CC_SYNTHESIZE_RETAIN(Stage*, _stage, Stage);
	CC_SYNTHESIZE_RETAIN(StageObject*, _object, Object);

	int _swipeStartPosX;
	int _swipeStartPosY;
	int _swipePosX;
	int _swipePosY;
	int _scroll;
	float _rotate;
	EditMode _mode;
	MoveDirection _moveDir;
	StageRay* _ray;
	GoalZone* _goal;
	cocos2d::Sprite* _stage_sprite;
	cocos2d::Sprite* _ray_sprite;
	cocos2d::Sprite* _goal_sprite;
	cocos2d::Node* _editNode;
	cocos2d::EventListenerTouchOneByOne* _editListener;
};

#endif // __EDIT_SCENE_H__
