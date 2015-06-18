//
//  MainScene.cpp
//  MonkGame
//
//  Created by 陈子龙 on 15-5-24.
//
//

#include "MainScene.h"
#include "gameconfig.h"
#include "HomeScene.h"
#include "simpleAudioEngine.h"
#define CLOUD_01_VEC2  Vec2(180,470)
#define CLOUD_02_VEC2  Vec2(448,438)

Scene * MainSceneLayer::createScene(){

    auto scene=Scene::create(); //创建场景对象
    
    auto lay=MainSceneLayer::create(); //创建层对象
    
    scene->addChild(lay);
    
    return scene;//返回一个场景对象
    
}

bool MainSceneLayer::init(){

    if (!Layer::init()) {
        
        return false;
    }
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("UIResource_ch.plist");//加载精灵图集，单例，只要一次加载全局使用
    
    auto sky=Sprite::createWithSpriteFrameName("sky.png");
    
    sky->setAnchorPoint(Vec2(1.0,1.0)); //设置锚点到左上角
    
    sky->setPosition(Vec2(WINSIZE_WIDTH,WINSIZE_HEIGHT));
    
    this->addChild(sky);
    
    auto cloud_01=Sprite::createWithSpriteFrameName("cloud_01.png");
    cloud_01->setPosition(CLOUD_01_VEC2);
    this->addChild(cloud_01);
    
    auto cloud_02=Sprite::createWithSpriteFrameName("cloud_02.png");
    cloud_02->setPosition(CLOUD_02_VEC2);
    this->addChild(cloud_02);
    
    auto cloud_03=Sprite::createWithSpriteFrameName("cloud_03.png");
    cloud_03->setPosition(Vec2(cloud_03->getContentSize().width/2,400));
    this->addChild(cloud_03);
    
    auto cloud_04=Sprite::createWithSpriteFrameName("cloud_04.png");
    cloud_04->setPosition(Vec2(WINSIZE_WIDTH-50,380));
    this->addChild(cloud_04);
    
    auto wall=Sprite::createWithSpriteFrameName("wall.png");
    
    wall->setAnchorPoint(Vec2(0.5,0));
    
    wall->setPosition(Vec2(WINSIZE_WIDTH/2,0));
    
    this->addChild(wall);
    
    auto monk_head=Sprite::createWithSpriteFrameName("monk_head.png");
    
    monk_head->setPosition(Vec2(320,655));
    
    this->addChild(monk_head);
    
    auto lab=Label::createWithBMFont("fonts.fnt","LOADING...");;
    
    lab->setPosition(WINSIZE_WIDTH/2,472);
    
    lab->setScale(0.9);
    
    this->addChild(lab);//添加加载文字到图层
    
    auto defend_monk_word=Sprite::create("defend_monk_word.png");
    
    defend_monk_word->setPosition(Vec2(320,WINSIZE_HEIGHT));
    
    this->addChild(defend_monk_word);
    
     CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("in.mp3");
    
    auto move = MoveTo::create(1, Vec2(320, 575));
    auto move_ease_in = EaseElasticOut::create(move->clone());
    auto delay = DelayTime::create(1.0f);
    auto call=CallFunc::create([=](){
    
        Director::getInstance()->replaceScene(TransitionCrossFade::create(2.0f,HomeSceneLayer::createScene()));//跳转场景
        
    });
    
    auto seq = Sequence::create(move_ease_in, delay->clone(),call,nullptr);
    defend_monk_word->runAction(seq);
    
    
    auto slogan=Sprite::createWithSpriteFrameName("slogan.png");//口号
    
    slogan->setPosition(Vec2(320,236));
    
    this->addChild(slogan);
    
    
    return true;
}