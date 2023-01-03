#ifndef __IN_GAME_H__
#define __IN_GAME_H__

#include "cocos2d.h"
#include "Pile.h"
#include "Define.h"
#include <vector>
#include "ui\CocosGUI.h"
#include <string>
#include "SoundManager.h"
#include "MenuGame.h"
#include <sstream>
using namespace cocos2d;
class InGame : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	void update(float);
	void createPile();
	void gameover();

	void pauseGame(cocos2d::Ref* pSender, ui::Widget::TouchEventType type);

	bool onContactBegin(cocos2d::PhysicsContact& contact);
	void updateSelect(float del);

	// implement the "static create()" method manually
	CREATE_FUNC(InGame);

private:
	std::vector<Pile*> mPile;
	cocos2d::Sprite* mBird;
};

#endif // __INGAME_SCENE_H__
