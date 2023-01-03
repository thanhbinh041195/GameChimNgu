#include "MenuGame.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

ui::Button* btnPlay;
ui::Button* btnSetting;
ui::Button* btnExit;

Scene* MenuGame::createScene()
{
	auto scene = Scene::create();
	auto layer = MenuGame::create();
	scene->addChild(layer);
	return scene;
}

bool MenuGame::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	SoundManager::getInstance()->playBackgroundMusic();
	auto lable = Label::createWithTTF("Flappy bird", "fonts/KissyHugs.ttf", 70);
	lable->setPosition(visibleSize / 2 + Size(0, visibleSize.height / 3));


	auto background = Sprite::create("Image/download.PNG");
	background->setContentSize(visibleSize);
	background->setPosition(visibleSize / 2);
	this->addChild(background);

	btnPlay = ui::Button::create("Image/button-play.png");
	btnPlay->setPosition(visibleSize / 2 + Size(0, visibleSize.height / 10));
	btnPlay->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::ENDED:
			Director::getInstance()->replaceScene(InGame::createScene());
			SoundManager::getInstance()->playSoundEffect(Sound::button);
			break;
		default:
			break;
		}
	});

	btnExit = ui::Button::create("Image/button-Exit.png");
	btnExit->addTouchEventListener(CC_CALLBACK_1(MenuGame::menuCloseCallback, this));

	btnSetting = ui::Button::create("Image/button-Setting.png");
	btnSetting->setPosition(btnPlay->getPosition() - Vec2(0, btnPlay->getContentSize().height + 10));
	auto nodeSetting = Node::create();
	this->addChild(nodeSetting,1);
	btnSetting->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::ENDED:
		{
			SoundManager::getInstance()->playSoundEffect(Sound::button);
			btnPlay->setEnabled(false);
			btnSetting->setEnabled(false);
			btnExit->setEnabled(false);
			SoundManager::getInstance()->showSetting(nodeSetting);
			this->scheduleUpdate();
			break;
		}
		default:
			break;
		}
	});

	
	btnExit->setPosition(btnSetting->getPosition() - Vec2(0, btnSetting->getContentSize().height + 10));

	this->addChild(btnPlay);
	this->addChild(btnSetting);
	this->addChild(btnExit);
	this->addChild(lable);
	return true;
}

void MenuGame::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
void MenuGame::update(float del)
{
	if (SoundManager::getInstance()->getState() == true)
	{
		btnPlay->setEnabled(true);
		btnSetting->setEnabled(true);
		btnExit->setEnabled(true);
		this->unscheduleUpdate();
	}
}