#include "system/MyUtils.h"

USING_NS_CC;

//The instance.
MyUtils* MyUtils::_instance = NULL;

/*
 * Constructor
 */
MyUtils::MyUtils()
{
}

/*
 * Destructor
 */
MyUtils::~MyUtils()
{
	removeInstance();
}

/*
 * get this instance.(Singleton)
 */
MyUtils* MyUtils::getInstance()
{
	//if NULL, create instance
	if(!_instance)
		_instance = new (std::nothrow) MyUtils();

	return _instance;
}

/*
 * remove this instance.(Singleton)
 */
void MyUtils::removeInstance()
{
	if(_instance)
	{
		delete _instance;
		_instance = NULL;
	}
}

/*
 * Checking event range contains touch point by touch and event data.
 */
bool MyUtils::isTouchInEvent(Touch* touch, Event* event)
{
	return isTouchInEvent(touch->getLocation(), event->getCurrentTarget());
};

/*
 * Checking event range contains touch point by touch and event data. 
 * down_x or y is extension of left or lower. 
 * append_x or y is extension of right or upper.
 */
bool MyUtils::isTouchInEvent(Touch* touch, Event* event, int down_x, int down_y, int append_x, int append_y)
{
	return isTouchInEvent(touch->getLocation(), event->getCurrentTarget(), down_x, down_y, append_x, append_y);
};

/*
 * Checking event range contains point by point and event data. 
 */
bool MyUtils::isTouchInEvent(Vec2 point, Event* event)
{
	return isTouchInEvent(point, event->getCurrentTarget());
};

/*
 * Checking node range contains point by point and node data. 
 */
bool MyUtils::isTouchInEvent(Vec2 point, Node* t)
{
	return isTouchInEvent(point, t, 0, 0, 0, 0);
};

/*
 * Checking node range contains point by point and node data. 
 * down_x or y is extension of left or lower. 
 * append_x or y is extension of right or upper.
 */
bool MyUtils::isTouchInEvent(Vec2 point, Node* t, int down_x, int down_y, int append_x, int append_y)
{
	//NULL check
	if(!t)
	{
		CCLOG("[ERROR] isInTocuh node is NULL");
		return false;
	}

	Rect targetBox;
	//if ignore anchorpoint, not multiple anchorpoint.
	if(t->isIgnoreAnchorPointForPosition())
		targetBox = Rect(t->getParent()->convertToWorldSpace(t->getPosition()).x - t->getContentSize().width - down_x,
						t->getParent()->convertToWorldSpace(t->getPosition()).y - t->getContentSize().height - down_y,
						t->getContentSize().width + append_x, t->getContentSize().height + append_y);
	else
		targetBox = Rect(t->getParent()->convertToWorldSpace(t->getPosition()).x - t->getContentSize().width * t->getAnchorPoint().x - down_x,
						t->getParent()->convertToWorldSpace(t->getPosition()).y - t->getContentSize().height * t->getAnchorPoint().y - down_y,
						t->getContentSize().width + append_x, t->getContentSize().height + append_y);
	return targetBox.containsPoint(point);
};

/*
 * Load text data by file.
 */
void MyUtils::loadText(std::string &text, const std::string &file)
{
	text = FileUtils::getInstance()->getStringFromFile(file);
}

/*
 * Split input data by delim and store result.
 */
void MyUtils::splitString(std::vector<std::string> &result ,const std::string& input, char delim)
{
	std::string item;
	for(char ch : input){
		if(ch==delim){
			result.push_back(item);
			item.clear();
		} else {
			item += ch;
		}
	}
	result.push_back(item);
}

/*
 * Sprite input file data by '\n' and store result.
 * Only for UTF-8 file data. BOM is supported. 
 * Clear result in this function.
 */
void MyUtils::splitFile(std::vector<std::string> &result ,const std::string& input)
{
	CCLOG("[Log]Split file is %s", input.c_str());

	//load file text
	std::string fileText;
	loadText(fileText, input);

	//clear result data
	result.clear();
	std::string item;

	//for BOM
	bool first = true;
	bool bom = false;
	int bom_count = 0;

	for(char ch : fileText)
	{
		//BOM check
		if(first)
		{
			unsigned char unch = (unsigned char)ch;
			if(unch == 0xEF)
				bom = true;
			first = false;
		}
		if(bom && bom_count < 3)
		{
			bom_count++;
			continue;
		}

		//'\r' is skip
		if(ch=='\r')
			continue;

		///'\n' is delimiter
		if(ch=='\n')
		{
			result.push_back(item);
			item.clear();
		}
		else
			item += ch;
	}
	result.push_back(item);
}

/*
 * Checking ori is start with com
 */
bool MyUtils::startWith(const std::string &ori, const std::string &com)
{
	return ori.substr(0, com.size()) == com;
}

/*
 * Get offset of char of src[pos] 
 * Only for UTF-8.
 * Not checking illigal value.
 */
int MyUtils::getOffset(const std::string &src, int pos)
{
	char ch = src[pos];
	int byte = 0;

	// checking number of 1 on head.
	for(int i = 0; i < 6; i++)
	{
		if(((ch >> ( 7 - i )) & 0x01) == 0)
		{
			byte = i;
			break;
		}
	}

	if(byte != 0)
		--byte;

	return byte + 1;	
}