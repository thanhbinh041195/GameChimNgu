#include "InGame.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
Size visibleSize;
Label* lb_Point;
Sprite* bg_Settting;
Node* nodeSetting;
ui::Button* btnPause;
Scene* InGame::createScene()
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	auto layer = InGame::create();
	scene->addChild(layer);

	return scene;
}

bool InGame::init()
{
	if (!Scene::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	if (Director::getInstance()->isPaused())
	{
		Director::getInstance()->resume();
	}
	nodeSetting = Node::create();
	//nodeSetting->setPosition(visibleSize/2);
	this->addChild(nodeSetting, 10);

	btnPause = ui::Button::create("Image/pause.png");
	btnPause->setScale(0.5);
	btnPause->addTouchEventListener(CC_CALLBACK_2(InGame::pauseGame, this));
	btnPause->setPosition(visibleSize - btnPause->getContentSize()*btnPause->getScale() / 2);
	this->addChild(btnPause, 1);

	auto background = Sprite::create("Image/download.PNG");
	background->setContentSize(visibleSize);
	background->setPosition(visibleSize / 2);
	this->addChild(background);

	auto edgeDie = Node::create();
	edgeDie->setPosition(visibleSize / 2);
	auto phy = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 10.0f);
	phy->setCollisionBitmask(Mask::wallMask);
	phy->setContactTestBitmask(true);
	edgeDie->setPhysicsBody(phy);
	this->addChild(edgeDie);

	createPile();



	mBird = Sprite::create("Image/bird.png");

	auto physic = PhysicsBody::createCircle(mBird->getContentSize().width / 2);
	mBird->setPhysicsBody(physic);
	physic->setCollisionBitmask(Mask::birdMask);
	physic->setContactTestBitmask(true);
	physic->setDynamic(false);
	mBird->setPosition((float)visibleSize.width / 2.75, visibleSize.height / 2);
	this->addChild(mBird);



	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = [=](Touch* touch, Event* event){
		
		mBird->stopAllActions();

		mBird->runAction(Sequence::create(RotateTo::create(0.01, -45), MoveBy::create(0.2, Vec2(0, 30)), RotateTo::create(0.01, 0), DelayTime::create(0.2f), RotateTo::create(0.01, 45), MoveTo::create((mBird->getPosition().y / (visibleSize.height / 4))*0.5, Vec2(mBird->getPosition().x, 0)), nullptr));
		//this->schedule(schedule_selector(InGame::updateSelect), 0.4f);
		SoundManager::getInstance()->playSoundEffect(Sound::wing);
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);



	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(InGame::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	auto label = Label::createWithTTF("Point:", "fonts/KissyHugs.ttf", 20);
	label->setPosition(visibleSize / 2 + Size(0, 250));
	lb_Point = Label::createWithTTF("0", "fonts/KissyHugs.ttf", 20);
	lb_Point->setAnchorPoint(Vec2(0, 0.5));
	lb_Point->setPosition(label->getPosition() + Vec2(50, 0));
	this->addChild(lb_Point);
	this->addChild(label);


	this->scheduleUpdate();
	return true;
}

void InGame::update(float del)
{
	for (int i = 0; i < mPile.size(); i++)
	{
		mPile.at(i)->getSprite()->setPosition(mPile.at(i)->getSprite()->getPosition() + Vec2(-1.5, 0));
		if (i % 2 == PileType::PileTop && mPile.at(i)->getSprite()->getPosition().x <= 0)
		{
			float del = cocos2d::RandomHelper::random_real(-0.6, 0.6);
			mPile.at(i)->getSprite()->setPosition(Vec2(visibleSize.width, visibleSize.height / 2) + Vec2(0, mPile.at(i)->getSprite()->getContentSize().height*mPile.at(i)->getSprite()->getScaleY() / 2 + 45) + Vec2(0, mPile.at(i)->getSprite()->getContentSize().height*del));
			mPile.at(i + 1)->getSprite()->setPosition(Vec2(visibleSize.width, visibleSize.height / 2) - Vec2(0, mPile.at(i)->getSprite()->getContentSize().height*mPile.at(i)->getSprite()->getScaleY() / 2 + 45) + Vec2(0, mPile.at(i)->getSprite()->getContentSize().height*del));
		}
	}

}

void InGame::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void InGame::createPile()
{
	float delRand = 0;
	for (int i = 0; i < 6; i++)
	{

		if (i % 2 == 0)
			delRand = cocos2d::RandomHelper::random_real(-0.6, 0.6);
		auto pile = new Pile();
		if (i % 2 == PileType::PileTop)  // top
		{

			pile->setSprite(Sprite::create("Image/up_bar.png"));
			pile->getSprite()->setScaleY(2);
			pile->getSprite()->setPosition(Vec2(visibleSize.width, visibleSize.height / 2) + Vec2(0, pile->getSprite()->getContentSize().height*pile->getSprite()->getScaleY() / 2 + 45) + Vec2(i / 2 * visibleSize.width / 3, 0) + Vec2(0, pile->getSprite()->getContentSize().height*delRand));

			auto node = Node::create();
			auto phy = PhysicsBody::createBox(Size(5, visibleSize.height * 2));
			phy->setDynamic(false);
			phy->setCollisionBitmask(Mask::pass_Wall);
			phy->setContactTestBitmask(true);
			node->setPhysicsBody(phy);
			pile->getSprite()->addChild(node);
			node->setPosition(pile->getSprite()->getContentSize().width / 2, 0);
		}
		else // Down
		{
			pile->setSprite(Sprite::create("Image/down_bar.png"));
			pile->getSprite()->setScaleY(2);
			pile->getSprite()->setPosition(Vec2(visibleSize.width, visibleSize.height / 2) - Vec2(0, pile->getSprite()->getContentSize().height*pile->getSprite()->getScaleY() / 2 + 45) + Vec2(i / 2 * visibleSize.width / 3, 0) + Vec2(0, pile->getSprite()->getContentSize().height*delRand));
		}

		auto phys = PhysicsBody::createBox(pile->getSprite()->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
		phys->setCollisionBitmask(Mask::wallMask);
		phys->setContactTestBitmask(true);
		phys->setDynamic(false);
		pile->getSprite()->setPhysicsBody(phys);

		mPile.push_back(pile);

		this->addChild(pile->getSprite());
	}
}
bool InGame::onContactBegin(cocos2d::PhysicsContact& contact)
{
	PhysicsBody* a = contact.getShapeA()->getBody();
	PhysicsBody* b = contact.getShapeB()->getBody();
	if ((a->getCollisionBitmask() == Mask::birdMask && b->getCollisionBitmask() == Mask::wallMask) || (a->getCollisionBitmask() == Mask::wallMask && b->getCollisionBitmask() == Mask::birdMask))
	{
		SoundManager::getInstance()->playSoundEffect(Sound::die);
		Director::getInstance()->pause();
		auto bg = Sprite::create("Image/background.png");
		bg->setOpacity(80);
		bg->setPosition(visibleSize / 2);
		bg->setContentSize(visibleSize);
		this->addChild(bg);
		gameover();
	}
	if ((a->getCollisionBitmask() == Mask::birdMask && b->getCollisionBitmask() == Mask::pass_Wall) || (a->getCollisionBitmask() == Mask::pass_Wall && b->getCollisionBitmask() == Mask::birdMask))
	{
		SoundManager::getInstance()->playSoundEffect(Sound::pass);
		std::ostringstream point;
		point << atoi(lb_Point->getString().c_str()) + 1;
		lb_Point->setString(point.str());
	}
	return true;
}
void InGame::pauseGame(Ref* pSender, ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case ui::Widget::TouchEventType::ENDED:
	{
		SoundManager::getInstance()->playSoundEffect(Sound::button);
		Director::getInstance()->pause();

		bg_Settting = Sprite::create("Image/background_pause.png");
		bg_Settting->setPosition(visibleSize / 2);
		bg_Settting->setScale(0.8);
		bg_Settting->setScaleY(0.8);
		addChild(bg_Settting);

		auto btnContinues = ui::Button::create("Image/remuse.png");
		auto btnSetting = ui::Button::create("Image/options.png");
		auto btnHome = ui::Button::create("Image/menu.png");

		btnContinues->setPosition(bg_Settting->getContentSize() / 2 + Size(0, bg_Settting->getContentSize().height / 8));
		btnContinues->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::ENDED:
			{
				SoundManager::getInstance()->playSoundEffect(Sound::button);
				Director::getInstance()->resume();
				//this->resumeSchedulerAndActions();
				removeChild(bg_Settting);
				break;
			}
			default:
				break;
			}
		});

		btnSetting->setPosition(btnContinues->getPosition() - Vec2(0, btnContinues->getContentSize().height));
		btnSetting->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::ENDED:
			{
				SoundManager::getInstance()->playSoundEffect(Sound::button);
				//bg_Settting->setVisible(false);
				this->removeChild(bg_Settting);
				SoundManager::getInstance()->showSetting(nodeSetting);
				break;
			}
			default:
				break;
			}
		});


		btnHome->setPosition(btnSetting->getPosition() - Vec2(0, btnSetting->getContentSize().height));
		btnHome->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::ENDED:
			{
				SoundManager::getInstance()->playSoundEffect(Sound::button);
				Director::getInstance()->replaceScene(MenuGame::createScene());
				break;
			}
			default:
				break;
			}
		});

		bg_Settting->addChild(btnContinues);
		bg_Settting->addChild(btnHome);
		bg_Settting->addChild(btnSetting);
		break;
	}
	default:
		break;
	}
}
//void InGame::updateSelect(float del)
//{
//	CCLOG("dddddddddd");
//	if (SoundManager::getInstance()->getState() == true)
//	{
//		bg_Settting->setVisible(true);
//		this->unschedule(schedule_selector(InGame::updateSelect));
//	}
//}
void InGame::gameover()
{
	btnPause->setEnabled(false);

	auto bg = Sprite::create("Image/background.png");
	bg->setPosition(visibleSize / 2);
	bg->setContentSize(visibleSize);
	bg->setOpacity(70);
	this->addChild(bg);

	auto lable = Label::createWithTTF("Game Over", "fonts/KissyHugs.ttf", 30);
	bg->addChild(lable);
	lable->setPosition(bg->getContentSize() / 2 + Size(0, bg->getContentSize().height / 4));
	auto lable1 = Label::createWithTTF(lb_Point->getString().c_str(), "fonts/KissyHugs.ttf", 30);
	lable1->setPosition(lable->getPosition() + Vec2(0, -50));
	bg->addChild(lable1);

	auto btnHome = ui::Button::create("Image/Home.png");
	btnHome->setScale(0.3);
	btnHome->setPosition(bg->getPosition() - Vec2(btnHome->getContentSize().width*btnHome->getScale(), 0));
	bg->addChild(btnHome);
	btnHome->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::ENDED:
			SoundManager::getInstance()->playSoundEffect(Sound::button);
			Director::getInstance()->replaceScene(MenuGame::createScene());
			break;
		default:
			break;
		}
	});



	auto btnReturn = ui::Button::create("Image/return.png");
	btnReturn->setScale(0.3);
	btnReturn->setPosition(bg->getPosition() + Vec2(btnReturn->getContentSize().width*btnReturn->getScale(), 0));
	bg->addChild(btnReturn);
	btnReturn->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::ENDED:
			SoundManager::getInstance()->playSoundEffect(Sound::button);
			Director::getInstance()->resume();
			Director::getInstance()->replaceScene(InGame::createScene());
			break;
		default:
			break;
		}
	});

}