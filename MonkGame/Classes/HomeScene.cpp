//
//  HomeScene.cpp
//  MonkGame
//
//  Created by 陈子龙 on 15-5-25.
//
//

#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "../android/jni/JniHelper.h"

#include "jni.h"

// 引入相关的头文件
#include "Cocos2dx/Common/CCUMSocialSDK.h"
#include "Cocos2dx/ShareButton/UMShareButton.h"
// 使用友盟命令空间
USING_NS_UM_SOCIAL;

#endif//以上代码是安卓引用文件


#include "simpleAudioEngine.h"
#include "HomeScene.h"
#include "gameconfig.h"
#include "GameScene.h"
#define CLOUD_01_VEC2  Vec2(180,470)
#define CLOUD_02_VEC2  Vec2(448,438)

Scene* HomeSceneLayer::createScene(){
    
    auto scene=Scene::create(); //创建场景对象
    
    auto lay=HomeSceneLayer::create(); //创建层对象
    
    scene->addChild(lay);
    
    return scene;//返回一个场景对象

}

bool HomeSceneLayer::init(){
    
    if (!Layer::init()) {
        
        return false;
    }
    //对手机返回键的监听
    auto listener = EventListenerKeyboard::create();
    //和回调函数绑定
    listener->onKeyReleased = CC_CALLBACK_2(HomeSceneLayer::onKeyReleased,this);
    //添加到事件分发器中
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
    
    
    //SpriteFrameCache::getInstance()->addSpriteFramesWithFile("UIResource_ch.plist");
    
    auto sky=Sprite::createWithSpriteFrameName("sky.png");
    
    sky->setAnchorPoint(Vec2(1.0,1.0));
    
    sky->setPosition(Vec2(WINSIZE_WIDTH,WINSIZE_HEIGHT));
    
    this->addChild(sky);
    
    auto cloud_01=Sprite::createWithSpriteFrameName("cloud_01.png");
    cloud_01->setPosition(CLOUD_01_VEC2);
    this->addChild(cloud_01);
    
    auto moveBy_01=MoveBy::create(15,Vec2(WINSIZE_WIDTH-250,0));
    auto rev_01=moveBy_01->reverse();
    auto seq_01=Sequence::create(moveBy_01,rev_01, NULL);
    cloud_01->runAction(RepeatForever::create(seq_01));
    
    auto cloud_02=Sprite::createWithSpriteFrameName("cloud_02.png");
    cloud_02->setPosition(CLOUD_02_VEC2);
    this->addChild(cloud_02);
    
    auto moveBy_02=MoveBy::create(13,Vec2(-WINSIZE_WIDTH+250,0));
    auto rev_02=moveBy_02->reverse();
    auto seq_02=Sequence::create(moveBy_02,rev_02, NULL);
    cloud_02->runAction(RepeatForever::create(seq_02));
    
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
    
    auto Defend_monk=Sprite::createWithSpriteFrameName("Defend_monk.png");
    
    Defend_monk->setPosition(Vec2(320,670));
    
    this->addChild(Defend_monk);
    
    auto best_score=Sprite::createWithSpriteFrameName("best_score.png");
    
    best_score->setPosition(Vec2(320,520));
    
    this->addChild(best_score);
    
    auto labFont=Label::createWithBMFont("fonts.fnt","0");
    
    if (Value(UserDefault::getInstance()->getStringForKey("string").c_str()).asInt()!=0) {
        
        labFont->setString(UserDefault::getInstance()->getStringForKey("string").c_str());
        
    }
    
    labFont->setAnchorPoint(Vec2(0,0));//左对齐
    
    labFont->setPosition(Vec2(304,502));
    
    
    this->addChild(labFont);
    
    float baseX=106;  //设定初始值，即第一个和尚的X,Y轴坐标。
    float baseY=235;
    for (int i=0;i<5;i++)
    {                                                 //循环5次，创建五个精灵对象
        
        __String  *fileName=__String::createWithFormat("monk_0%d.png",i+1); //fileName存储变换的路径字符串
        
        auto monkSpr=Sprite::createWithSpriteFrameName(fileName->getCString()); //获得fileName对象存储的路径
        
        monkSpr->setPosition(Vec2(baseX,baseY));
        
        this->addChild(monkSpr);
        
        baseX+=107; //从第二个和尚开始加X坐标值加120
        
    }
    
    auto StartItem = MenuItemSprite::create(
                                           Sprite::createWithSpriteFrameName("start_01.png"),
                                           Sprite::createWithSpriteFrameName("start_02.png"),
                                           CC_CALLBACK_1(HomeSceneLayer::menuStartCallback, this));
    
    StartItem->setPosition(Vec2(320,410));
    
    
    auto CommentItem = MenuItemSprite::create(
                                             Sprite::createWithSpriteFrameName("comment_01.png"),
                                             Sprite::createWithSpriteFrameName("comment_02.png"),
                                           CC_CALLBACK_1(HomeSceneLayer::menucommentCallback, this));
    
    CommentItem->setPosition(Vec2(132,110));
    
    auto FeedbackItem = MenuItemSprite::create(
                                            Sprite::createWithSpriteFrameName("feedback_01.png"),
                                            Sprite::createWithSpriteFrameName("feedback_02.png"),
                                           CC_CALLBACK_1(HomeSceneLayer::menuFeedbackCallback, this));
    
    FeedbackItem->setPosition(Vec2(320,110));
    
    auto ShareItem = MenuItemSprite::create(
                                           Sprite::createWithSpriteFrameName("share_01.png"),
                                           Sprite::createWithSpriteFrameName("share_02.png"),
                                           CC_CALLBACK_1(HomeSceneLayer::menuShareCallback, this));
    
    ShareItem->setPosition(Vec2(508,110));
    
    
    
    
    auto menu = Menu::create(StartItem,CommentItem,FeedbackItem,ShareItem,NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
    


    return true;
}

void HomeSceneLayer::menuStartCallback(Ref* pSender){
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.mp3");

    Director::getInstance()->replaceScene(GameSceneLayer::scene());

}

void HomeSceneLayer::menucommentCallback(cocos2d::Ref* pSender){

    Application::getInstance()->openURL("http://keng.mm88bb.com/");

}

void HomeSceneLayer::menuFeedbackCallback(cocos2d::Ref* pSender){ //反馈按钮回调定义
    
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    JniMethodInfo minfo;
    
    bool isHave=JniHelper::getStaticMethodInfo(minfo,"org/cocos2dx/cpp/AppActivity","showToast","()V");

    if(isHave)
    {
    	CCLog("jni:此函数存在");
    	minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID);
    }
    
    #endif//以上代码是安卓引用文件

}

void HomeSceneLayer::menuShareCallback(cocos2d::Ref* pSender){

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    // 获取一个CCUMSocialSDK实例
    CCUMSocialSDK *sdk = CCUMSocialSDK::create("5568516967e58e897a0011b9");
    // 设置友盟appkey,如果create中设置了不用调用该函数
    // sdk->setAppKey("4eaee02c527015373b000003");
    // **********************   设置平台信息  ***************************
    sdk->setQQAppIdAndAppKey("1104678982", "7L3gWJ8PaTha6JMb");
    sdk->setWeiXinAppInfo("wx44a50d1bd0fe1bb8","b5e2dd02b8417337c6ac324d90a9e276");
    // sdk->setYiXinAppKey("设置易信和易信朋友圈的app id");
    // sdk->setLaiwangAppInfo("设置来往和来往动态的app id",
    //              "设置来往和来往动态的app key", "我的应用名");
    // sdk->setFacebookAppId("你的facebook appid");
    //     // 打开或者关闭log
    sdk->setLogEnable(true) ;
    // **********************   END ***************************
    // 设置用户点击一条图文分享时用户跳转到的目标页面, 一般为app主页或者下载页面
    sdk->setTargetUrl("http://keng.mm88bb.com/");
    // 设置友盟分享面板上显示的平台
    vector<int>* platforms = new vector<int>();
    platforms->push_back(SINA);
    // platforms->push_back(RENREN) ;不需要添加人人网的分享
    platforms->push_back(DOUBAN) ;
    platforms->push_back(QZONE) ;
    // platforms->push_back(QQ) ;//也不需要QQ的
    platforms->push_back(WEIXIN_CIRCLE) ; //添加朋友圈
    // 设置平台, 在调用分享、授权相关的函数前必须设置SDK支持的平台
    sdk->setPlatforms(platforms) ;

    // 打开分享面板, 注册分享回调, 参数1为分享面板上的平台, 参数2为要分享的文字内容，
    // 参数3为要分享的图片路径(android和IOS的图片地址格式不一致，因此分平台设置), 参数4为分享回调.
    
    auto ret=UserDefault::getInstance()->getStringForKey("string");
    
    __String  *str2=__String::createWithFormat("#保卫小和尚#本人最高得分%d分。做事要掌握好节奏，否则会死",Value(ret.c_str()).asInt());
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    sdk->openShare("#保卫小和尚#本人最高得分120分，获得过", "assets/ic_lunch_share.png", NULL);
    //设置微信分享内容
    sdk->setPlatformShareContent(WEIXIN_CIRCLE, "weixin-share",
                                 "assets/ic_lunch_share.png", str2->getCString(),
                                 "http://blog.csdn.net/bboyfeiyu");

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    sdk->openShare("要分享的文字内容","share.png",NULL);
#endif
    
#endif//以上代码是安卓引用文件
    
}

void HomeSceneLayer::onKeyReleased(EventKeyboard::KeyCode keyCode,Event * pEvent)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //里面代码是给安卓平台编译的，安卓平台按返回键提示退出
    JniMethodInfo info;
    bool isHave = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "exitGame", "()V");
    
    if(isHave)
    {
        CCLOG("the function is exist");
        info.env->CallStaticVoidMethod(info.classID, info.methodID);
        
    }
    else
    {
        CCLOG("no exist");
    }
    
#endif
}
