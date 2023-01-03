#include "Pile.h"

Pile::Pile()
{
}


Pile::~Pile()
{
}

cocos2d::Sprite* Pile::getSprite()
{
	return mSprite;
}
void Pile::setSprite(cocos2d::Sprite* sprite)
{
	mSprite=sprite;
}
