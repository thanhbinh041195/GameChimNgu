#ifndef __MENU_GAME_H__
#define __MENU_GAME_H__

#include "cocos2d.h"
#include "ui\CocosGUI.h"
#include "InGame.h"
#include "SoundManager.h"
class MenuGame : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	void update(float);
	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	CREATE_FUNC(MenuGame);

private:
};

#endif // __MENU_SCENE_H__
