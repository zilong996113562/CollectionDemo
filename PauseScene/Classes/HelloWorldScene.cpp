#include "HelloWorldScene.h"
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto bg=Sprite::create("HelloWorld.png");
    
    bg->setPosition(Vec2(visibleSize/2));
    
    bg->setScale(2);
    
    addChild(bg);
    
    
 auto white_circle=Sprite::create("white_circle.png");
    
    white_circle->runAction(FadeOut::create(0));//立即淡出隐藏白点
    
    addChild(white_circle);

    auto listen=EventListenerTouchOneByOne::create();
    
    listen->onTouchBegan=[=](Touch * t,Event * e){
        
        auto fadein=FadeIn::create(0);
        
        white_circle->setPosition(t->getLocation()); //获取触摸坐标点并设置白点精灵坐标
        
        auto fadeOut=FadeOut::create(0.5);       //让白点淡出

        
        white_circle->runAction(Sequence::create(fadein,fadeOut,NULL));//执行淡出动作完成后，执行callback
        
        
        
        return false;
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listen,this);
    
    auto menu=Menu::create(
        MenuItemImage::create("CloseNormal.png",
                             "CloseSelected.png",
                             [=](Ref* pSender){
                                 
                                 /*
                                  Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(this,true);
                                  等价于_eventDispatcher->pauseEventListenersForTarget(this, true);
                                  */
                                 _eventDispatcher->pauseEventListenersForTarget(this, true); //暂停事件监听
                              
                                 auto colorlay=LayerColor::create(Color4B(0, 0, 0, 128)); //创建一个半透明的颜色层
                                 this->addChild(colorlay);
                                 
                
                                 
                                 auto closeItem = MenuItemFont::create("close", [=](Ref* sender){
                                     
                                     colorlay->removeFromParent();//从父节点清除clorlay图层,clorlay中的内容（菜单）会随之一起销毁
                                     
                                     _eventDispatcher->resumeEventListenersForTarget(this, true);//设置重新接收事件监听
                 
                                 });
                                 
                                 closeItem->setPosition(Vec2(100,100));
                                 
                                 auto menu2=Menu::create(closeItem,NULL);
                                 
                                 colorlay->addChild(menu2); //添加menu2到颜色图层colorlay
                        
                                 
                             }),NULL);
    
    
                             addChild(menu);
    
    return true;
}


