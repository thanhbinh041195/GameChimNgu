#pragma once
#include "cocos2d.h"
class Pile
{
public:
	Pile();
	~Pile();
	cocos2d::Sprite* getSprite();
	void setSprite(cocos2d::Sprite*);
private:
	cocos2d::Sprite* mSprite;
};

