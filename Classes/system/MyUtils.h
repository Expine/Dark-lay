#ifndef SYSTEM_MYUTILS_H_
#define SYSTEM_MYUTILS_H_

#include "cocos2d.h"

const int STORY_MAX_NUMBER = 6;

const int CUT_MASK_RIGHT = 1;
const int CUT_MASK_LEFT = 2;
const int CUT_MASK_UP = 4;
const int CUT_MASK_DOWN = 8;

class MyUtils
{
private:
	static MyUtils* _instance;
protected:
	MyUtils();
	~MyUtils();
public:
	static MyUtils* getInstance();
	static void removeInstance();
	bool isTouchInEvent(cocos2d::Touch* touch, cocos2d::Event* event);
	bool isTouchInEvent(cocos2d::Touch* touch, cocos2d::Event* event, int down_x, int down_y, int append_x, int append_y);
	bool isTouchInEvent(cocos2d::Vec2 point, cocos2d::Event* event);
	bool isTouchInEvent(cocos2d::Vec2 point, cocos2d::Node* node);
	bool isTouchInEvent(cocos2d::Vec2 point, cocos2d::Node* node, int append_x, int append_y);
	bool isTouchInEvent(cocos2d::Vec2 point, cocos2d::Node* node, int down_x, int down_y, int append_x, int append_y);
	void loadText(std::string &text, const std::string &file);
	void splitFile(std::vector<std::string> &result,const std::string& input);
	void splitString(std::vector<std::string> &result,const std::string& input, char delim);
	bool startWith(const std::string &ori, const std::string &com);
	int getOffset(const std::string &src, int pos);
};

#endif /* SYSTEM_MYUTILS_H_ */
