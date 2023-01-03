#include "SoundManager.h"
#include "SimpleAudioEngine.h"
#include "AudioEngine.h"
using namespace CocosDenshion;

SoundManager* SoundManager::instance = 0;
SoundManager * SoundManager::getInstance()
{
	if (instance == 0)
	{
		instance = new SoundManager();

	}
	return instance;
}

SoundManager::SoundManager()
{
	m_Vl_Music = 100;
	m_Vl_Sound = 100;
	audioBackground = SimpleAudioEngine::getInstance();
	audioButton = SimpleAudioEngine::getInstance();
}

SoundManager::~SoundManager()
{
}
void SoundManager::playBackgroundMusic()
{
	
	audioBackground->playBackgroundMusic("Sound/background.mp3", true);
};

void SoundManager::playSoundEffect(int n)
{
	switch (n)
	{
	case Sound::button:
		audioButton->playEffect("Sound/button_click.mp3",false);
		break;
	case Sound::die:
		audioButton->playEffect("Sound/sfx_die.wav", false);
		break;
	case Sound::pass:
		audioButton->playEffect("Sound/sfx_point.wav", false);
		break;
	case Sound::wing:
		audioButton->playEffect("Sound/sfx_wing.wav", false, 1.0f, 0.0f, 1.0f);
		break;
	default:
		break;
	}
}
bool SoundManager::getState()
{
	return m_state;
};
void SoundManager::showSetting(Node* scene)
{
	m_state = false;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	static ui::Slider* slVolumeMusic;
	static ui::Slider* slVolumeSound;

	settingBackground = Sprite::create("Image/background_Setting.png");
	settingBackground->setPosition(visibleSize / 2);
	scene->addChild(settingBackground);

	slVolumeMusic = ui::Slider::create();
	slVolumeMusic->setAnchorPoint(Vec2(0, 0.5));
	slVolumeMusic->setScale(1.7);
	slVolumeMusic->loadBarTexture( ("Image/slider_bar_bg.png"));
	slVolumeMusic->loadSlidBallTextures("Image/slider_ball_normal.png", "Images/lider_ball_pressed.png", "Images/lider_ball_disable.png");
	slVolumeMusic->loadProgressBarTexture("Image/slider_bar_pressed.png");
	slVolumeMusic->setPosition(Vec2(settingBackground->getContentSize().width/4,settingBackground->getPosition().y-settingBackground->getContentSize().height/8));
	slVolumeMusic->setPercent(m_Vl_Music);
	slVolumeMusic->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			audioBackground->setBackgroundMusicVolume(slVolumeMusic->getPercent());
			SoundManager::getInstance()->playSoundEffect(Sound::button);

			break;
		case ui::Widget::TouchEventType::ENDED:
			break;
		};
	});

	settingBackground->addChild(slVolumeMusic);


	slVolumeSound = ui::Slider::create();
	slVolumeSound->setAnchorPoint(Vec2(0, 0.5));
	slVolumeSound->setScale(1.7);
	slVolumeSound->loadBarTexture("Image/slider_bar_bg.png");
	slVolumeSound->loadSlidBallTextures("Image/slider_ball_normal.png", "Image/slider_ball_pressed.png", "Images/lider_ball_disable.png");
	slVolumeSound->loadProgressBarTexture("Image/slider_bar_pressed.png");
	slVolumeSound->setPosition(slVolumeMusic->getPosition()-Vec2(0,100));
	slVolumeSound->setPercent(m_Vl_Sound);
	slVolumeSound->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			audioButton->setEffectsVolume(slVolumeSound->getPercent());
			SoundManager::getInstance()->playSoundEffect(Sound::button);
			break;
		case ui::Widget::TouchEventType::ENDED:
			break;
		};
	});
	settingBackground->addChild(slVolumeSound);


	
	auto label_Music = Label::createWithSystemFont("Music", "arial", 20);
	label_Music->setColor(Color3B(0, 0, 0));
	label_Music->setPosition(slVolumeMusic->getPosition() - Vec2(label_Music->getContentSize().width / 2 + 25, 0));
	settingBackground->addChild(label_Music);

	auto label_Sound = Label::createWithSystemFont("Sound", "arial", 20);
	label_Sound->setColor(Color3B(0, 0, 0));
	label_Sound->setPosition(slVolumeSound->getPosition() - Vec2(label_Sound->getContentSize().width / 2 + 25, 0));
	settingBackground->addChild(label_Sound);


	auto btn_apply = ui::Button::create("Image/applySetting.PNG");
	btn_apply->setScale(0.5);
	btn_apply->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			SoundManager::getInstance()->playSoundEffect(Sound::button);

			m_Vl_Music = slVolumeMusic->getPercent();
			m_Vl_Sound = slVolumeSound->getPercent();
			audioButton->setEffectsVolume(m_Vl_Sound);
			audioBackground->setBackgroundMusicVolume(m_Vl_Music);
			scene->removeAllChildren();
			if (Director::getInstance()->isPaused())
				Director::getInstance()->resume();
			m_state = true;
			break;

		};
	});
	settingBackground->addChild(btn_apply, 5);
	btn_apply->setPosition(label_Sound->getPosition() + Vec2(btn_apply->getContentSize().width*0.5, -btn_apply->getContentSize().height * 3 / 4));


	auto btn_cancel = ui::Button::create("Image/cancelSetting.PNG");
	btn_cancel->setScale(0.5f);
	btn_cancel->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			SoundManager::getInstance()->playSoundEffect(Sound::button);

			audioButton->setEffectsVolume(m_Vl_Sound);
			audioBackground->setBackgroundMusicVolume(m_Vl_Music);
			scene->removeAllChildren();
			m_state = true;
			if (Director::getInstance()->isPaused())
				Director::getInstance()->resume();
			break;
		};
	});
	settingBackground->addChild(btn_cancel, 5);
	btn_cancel->setPosition(btn_apply->getPosition() + Vec2(btn_apply->getContentSize().width*0.5, 0));
};