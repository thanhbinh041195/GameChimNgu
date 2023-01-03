#pragma once
#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Define.h"
using namespace cocos2d;
class SoundManager
{
public:
	static SoundManager* getInstance();

	void playBackgroundMusic();
	void playSoundEffect(int);
	void showSetting(cocos2d::Node* Scene);
	bool getState();
private:
	static SoundManager* instance;

	CocosDenshion::SimpleAudioEngine* audioBackground;
	CocosDenshion::SimpleAudioEngine* audioButton;

	float m_Vl_Music;
	float m_Vl_Sound;
	bool m_state;
	SoundManager();
	~SoundManager();

	cocos2d::Sprite* settingBackground;
};