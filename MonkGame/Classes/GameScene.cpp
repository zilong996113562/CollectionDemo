
#include "GameScene.h"
#include "gameconfig.h"
#include "HomeScene.h"
#include "SimpleAudioEngine.h"
#define GAMEOVER_COLOR_LAYER_TAG 6000
#define GAMESCENE_HEART_MAX_TAG 5003
#define GAMESCENE_HEART_MIN_TAG 5001
#define SCORE_LAB_TAG 7000
#define CLOUD_01_VEC2  Vec2(180,470)
#define CLOUD_02_VEC2  Vec2(448,438)
#define CLOUD_01_TAG 1001
#define CLOUD_02_TAG 1002
#define HAMMER_FIRE_TIME_INTERVAL 3,2,1.5,0.8

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

// 引入相关的头文件
#include "Cocos2dx/Common/CCUMSocialSDK.h"
// 使用友盟命令空间
USING_NS_UM_SOCIAL;

#endif//以上代码是安卓引用文件

GameSceneLayer::GameSceneLayer()
{
    m_fireCount=0;
    m_fireItemCount=0;
    m_fingerRect=Rect(0.0f,0.0f,0.0f,0.0f);
    m_score=0;
    Heart_Max_Tag=GAMESCENE_HEART_MAX_TAG;
    Heart_Min_Tag=GAMESCENE_HEART_MIN_TAG;
}

GameSceneLayer::~GameSceneLayer()
{
    
    MonkRectArr.clear();//清空容器所有对象
    
    HammerArr.clear();//清空锤子容器
    
}

Scene * GameSceneLayer::scene()
{ //实现GameSceneLayer类中的sene()函数

    auto scene=Scene::create(); //创建场景对象
    
    auto lay=GameSceneLayer::create(); //创建层对象
    
    scene->addChild(lay);

    return scene;//返回一个场景对象
}


bool GameSceneLayer::init()
{

    if (!Layer::init())
    {   //调用父类的init函数进行判断
        
        return false;
        
    }
    
    auto listener = EventListenerKeyboard::create();
    //和回调函数绑定
    listener->onKeyReleased = CC_CALLBACK_2(GameSceneLayer::onKeyReleased,this);
    //添加到事件分发器中
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
    
    auto bgSpr=Sprite::createWithSpriteFrameName("sky.png");
    
    bgSpr->setAnchorPoint(Vec2(1.0,1.0));
    
    bgSpr->setPosition(Vec2(WINSIZE_WIDTH,WINSIZE_HEIGHT));
    
    this->addChild(bgSpr);
    
    auto cloud_01=Sprite::createWithSpriteFrameName("cloud_01.png");
    cloud_01->setPosition(CLOUD_01_VEC2);
    cloud_01->setTag(CLOUD_01_TAG);
    this->addChild(cloud_01);
    
    auto moveBy_01=MoveBy::create(15,Vec2(WINSIZE_WIDTH-250,0));
    auto rev_01=moveBy_01->reverse();
    auto seq_01=Sequence::create(moveBy_01,rev_01, NULL);
    cloud_01->runAction(RepeatForever::create(seq_01));
    
    auto cloud_02=Sprite::createWithSpriteFrameName("cloud_02.png");
    cloud_02->setPosition(CLOUD_02_VEC2);
     cloud_01->setTag(CLOUD_02_TAG);
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
    
    float baseX=106;  //设定初始值，即第一个和尚的X,Y轴坐标。
    for (int i=0;i<5;i++)
    {                                                 //循环5次，创建五个精灵对象
        
        __String  *fileName=__String::createWithFormat("monk_0%d.png",i+1); //fileName存储变换的路径字符串
        
        auto monkSpr=Sprite::createWithSpriteFrameName(fileName->getCString()); //获得fileName对象存储的路径
        
        monkSpr->setPosition(Vec2(baseX,235));
        
        this->addChild(monkSpr);
        
        baseX+=107; //从第二个和尚开始加X坐标值加120
        
        MonkRectArr.pushBack(monkSpr); //按顺序添加每个和尚到vec
        
    }
    
    baseX=80;//利用上面和尚的baseX重新赋值
    int heartTag=5000;
    for (int i=0; i<3; i++)
    {
        
        auto spr=Sprite::createWithSpriteFrameName("heart.png");
        spr->setPosition(Vec2(baseX,100));
        spr->setTag(++heartTag);//spr的Tag值是5001-5003
        this->addChild(spr);
        baseX+=60;
        
    }
    
    auto lab=LabelAtlas::create("","digital.png",39,54,48);
    
    lab->setTag(SCORE_LAB_TAG);
    
    lab->setAnchorPoint(Vec2(1,0));//右对齐
    
    lab->setPosition(Vec2(WINSIZE_WIDTH-50,70));
    
    this->addChild(lab); //把lab分数计数添加到层
    
    auto item= MenuItemSprite::create(   //暂停按钮
                                     Sprite::createWithSpriteFrameName("pause.png"),
                                     Sprite::createWithSpriteFrameName("pause.png"),
                                     CC_CALLBACK_1(GameSceneLayer::pauseMenuCall, this));
    
    item->setPosition(Vec2(320,100));
    
    auto menu = Menu::create(item, NULL);
    menu->setPosition(Vec2::ZERO);
    addChild(menu);
    
//-------------------init上面部分是初始化精灵添加到层------------------------------------------------------------
    
    guideGame();//显示向导精灵
    
    fireControl();//调用发射控制台，开始调度器
    
    auto listen=EventListenerTouchOneByOne::create(); //触摸监听
    
    

    listen->onTouchBegan=[&](Touch *t,Event *e){
        
        
        auto white_circle=Sprite::createWithSpriteFrameName("white_circle.png");
        
        white_circle->setPosition(t->getLocation()); //获取触摸坐标点并设置白点精灵坐标
    
        addChild(white_circle);
        
        m_fingerRect=white_circle->getBoundingBox();//把手指点击的区域保存到m_fingerRect变量中
        
        auto fadeOut=FadeOut::create(0.2);//让白点淡出
        
        auto callback=CallFunc::create([=](){    //使用lambda表达式简化函数调用
            
            white_circle->removeFromParent();    //删除白点对象
            
            m_fingerRect=Rect(0.0f,0.0f,0.0f,0.0f); //手指点击区域设置为初始值
            
        });
        
        white_circle->runAction(Sequence::create(fadeOut,callback,NULL));//执行淡出动作完成后，执行callback
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listen,this);
    
    this->schedule(schedule_selector(GameSceneLayer::check));//指定每帧执行自定义的函数
    
    refreshScore();//刷新分数
    
    return true;
    
}

void GameSceneLayer::guideGame()
{
    
    auto guide=Sprite::createWithSpriteFrameName("guide.png");
    
    guide->setPosition(Vec2(358,507));
    
    this->addChild(guide);
    
    auto fade=FadeOut::create(2.0);
    
    auto call=CallFunc::create([=](){
    
        guide->removeFromParent();
    
    });
    
    auto dongCall=CallFunc::create([](){
        
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("dong.mp3");
        
    });
    
    auto delay=DelayTime::create(0.5); //延迟
    
    guide->runAction(Sequence::create(delay,dongCall,fade,call,NULL)); //淡出完毕后清理精灵对象

}

void GameSceneLayer::fireControl()
{
    
    float time[]={HAMMER_FIRE_TIME_INTERVAL};//发射时间间隔
    /*第一个参数selector即为你要添加的事件函数
     第二个参数interval为事件触发时间间隔
     第三个参数repeat为触发一次事件后还会触发的次数，默认值为kRepeatForever，表示无限触发次数
     第四个参数delay表示第一次触发之前的延时
     第三和第四个参数可以省略
     */
    schedule(schedule_selector(GameSceneLayer::fire), time[m_fireItemCount], kRepeatForever, 0);
    
}

void GameSceneLayer::fire(float dt)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("in.mp3");
    
    m_fireCount++;//每调用函数一次，进行垒加，保存发射总次数
   // log("%d",m_fireCount);//调试用
    
    if (m_fireCount%7==0) { //发射7颗锤子，执行if语句
        
        //log("以上是第%d拨",m_fireItemCount);//调试用
        m_fireItemCount++;//当这拨发射7次，即进行下一拨
        
        if(m_fireItemCount>3)m_fireItemCount=0;
        
        unschedule(schedule_selector(GameSceneLayer::fire));//把原来的定时器关掉
        
        fireControl();//重新调用控制发射
        
    }
    
    int r=arc4random()%5; //随机一个0-5的数字赋给r，这个函数不需要定义随机种子
    
    auto monk=MonkRectArr.at(r);//返回Vector容器中指定位置的元素（即r索引的和尚）
    
    auto point=monk->getPosition(); //获取随机的和尚坐标点。
    
    auto hammer=Sprite::createWithSpriteFrameName("hammer.png"); //创建锤子精灵对象
    
    hammer->setPosition(Vec2(point.x,point.y+monk->getContentSize().height));
    
    this->addChild(hammer);
    
    HammerArr.pushBack(hammer);//每把当前创建的锤子放到容器里面
    
    auto jump=JumpTo::create(1.3,Vec2(point.x,point.y+monk->getContentSize().height/2),WINSIZE_HEIGHT-360,1); //跳跃函数。使锤子自由落体到和尚位置,指定时间，落点，高度，跳跃次数

//--------------------------以下是锤子跳跃抵达落点，即砸到和尚------------------------------------------
    auto jumpEnd=CallFunc::create([=]()
    {  //当跳跃结束后执行
        
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("zhong.mp3");
        
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("die.mp3");
        
        if (Heart_Max_Tag>=Heart_Min_Tag)
        {
    
            this->getChildByTag(Heart_Max_Tag)->setVisible(false); //隐藏红心
            
            if (Heart_Max_Tag==Heart_Min_Tag){  //Heart_Max_Tag等于最小值红心隐藏完毕
                
                unschedule(schedule_selector(GameSceneLayer::fire));
                auto colorlay=LayerColor::create(Color4B(0, 0, 0, 128)); //创建一个半透明的颜色层
                colorlay->setTag(GAMEOVER_COLOR_LAYER_TAG);
                this->addChild(colorlay);
                
                auto game_over=Sprite::createWithSpriteFrameName("game_over.png");
                game_over->setPosition(Vec2(320, 692));
                colorlay->addChild(game_over);
                
                auto border=Sprite::createWithSpriteFrameName("border.png");
                border->setPosition(320,456);
                colorlay->addChild(border);
                
                auto lab=LabelAtlas::create("","digital.png",39,54,48);
                
                //lab->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
                
                
                lab->setTag(SCORE_LAB_TAG);
                
                lab->setPosition(Vec2(450,470));
                
                lab->setAnchorPoint(Vec2(0.6,0));
                
                colorlay->addChild(lab); //把lab分数计数添加到层
                
                auto labFont=Label::createWithBMFont("fonts.fnt", "", TextHAlignment::CENTER);
                
                labFont->setPosition(325,325);
                
                labFont->setScale(0.8);
                
                colorlay->addChild(labFont);
                
//---------------------------以下进行最高分比较与存储-------------------------------------------
                
                __String  *str=__String::createWithFormat("%d",m_score);//根据变量m_score创建一个字符串
                
                lab->setString(str->getCString());  //lab显示字符串内容
                
                auto MaxScore=LabelAtlas::create("","digital.png",39,54,48);
                
                MaxScore->setPosition(Vec2(450,350));
                
                MaxScore->setAnchorPoint(Vec2(0.6,0));
                
                colorlay->addChild(MaxScore);
                
           //-------------------以下是最高分进行存储---------------------------------
                auto ret=UserDefault::getInstance()->getStringForKey("string"); //读取数据
                
                  __String  *str2=__String::createWithFormat("%d",m_score);//根据变量m_score创建一个字符串
                
                if( Value(str->getCString()).asInt()>=Value(ret.c_str()).asInt()){   //字符串转换成数值再进行比较
                    
                    UserDefault::getInstance()->setStringForKey("string",str->getCString());//设置用户数据
                    
                    UserDefault::getInstance()->flush();//保存到XML文件
                    
                     MaxScore->setString(str2->getCString());  //lab显示字符串内容
                }else{
                
                   // log("ret：：%s",ret.c_str());
                    MaxScore->setString(ret.c_str());  //lab显示字符串内容
                
                }
                
//----------------------------以下是根据不同分数给予不同奖牌------------------
                if(Value(str->getCString()).asInt()>=5000){
                    auto diamond=Sprite::createWithSpriteFrameName("diamond.png");
                    diamond->setPosition(217, 418);
                    colorlay->addChild(diamond);
                    
                    labFont->setString("钻石牌");
                
                }else if(Value(str->getCString()).asInt()>=2000){
                
                    auto gold=Sprite::createWithSpriteFrameName("gold.png");
                    gold->setPosition(217, 418);
                    colorlay->addChild(gold);
                    
                    labFont->setString(__String::createWithFormat("距离钻石牌还差%d分",(5000-Value(str->getCString()).asInt()))->getCString());
                    
                }else if(Value(str->getCString()).asInt()>=800){
                    
                    auto silver=Sprite::createWithSpriteFrameName("silver.png");
                    silver->setPosition(217, 418);
                    colorlay->addChild(silver);
                    
                    labFont->setString(__String::createWithFormat("距离大金牌还差%d分",(2000-Value(str->getCString()).asInt()))->getCString());
                    
                }else if(Value(str->getCString()).asInt()>=400){
                    
                    auto bronze=Sprite::createWithSpriteFrameName("bronze.png");
                    bronze->setPosition(217, 418);
                    colorlay->addChild(bronze);
                    
                    labFont->setString(__String::createWithFormat("距离小银牌还差%d分",(800-Value(str->getCString()).asInt()))->getCString());
                    
                }else{
                
                    auto iron=Sprite::createWithSpriteFrameName("iron.png");
                    iron->setPosition(217, 418);
                    colorlay->addChild(iron);
                    
                    labFont->setString(__String::createWithFormat("距离破铜牌还差%d分",(400-Value(str->getCString()).asInt()))->getCString());
                
                }
                
                //----------------------------以下是游戏结束界面中的两个菜单按钮-----------------------------
                auto again= MenuItemSprite::create(  //重新开始菜单
                                                    Sprite::createWithSpriteFrameName("again_01.png"),
                                                    Sprite::createWithSpriteFrameName("again_02.png"),
                                                    CC_CALLBACK_1(GameSceneLayer::RestartGame, this));
                
                again->setPosition(Vec2(188,231));//y轴向下移

                auto relive= MenuItemSprite::create( //复活菜单
                                                   Sprite::createWithSpriteFrameName("relive_01.png"),
                                                   Sprite::createWithSpriteFrameName("relive_02.png"),
                                                 CC_CALLBACK_1(GameSceneLayer::ResurrectGame, this));
                
                relive->setPosition(Vec2(456,231));//y轴向下移
                
                //以下是屏蔽下层触摸操作步骤
                auto shieldTopTouch=MenuItem::create();     //1.创建一个菜单项
                
                shieldTopTouch->setContentSize(Size(WINSIZE_WIDTH,WINSIZE_HEIGHT)); //2.设置菜单项成屏幕大小
                
                shieldTopTouch->setPosition(Vec2(WINSIZE_WIDTH/2,WINSIZE_HEIGHT/2));//3.放在屏幕中间位置
                
                auto reliveMenu = Menu::create(shieldTopTouch,relive,again,NULL);//4.先绘制屏蔽菜单项，然后再两个按钮的菜单，才不会挡着按钮
                
                reliveMenu->setPosition(Vec2::ZERO);
                
                colorlay->addChild(reliveMenu, 1);
                
            }
            
            Heart_Max_Tag--;
            
        }
        
        /*
 //------------------砸中小和尚,伴随着场景震动-------以下是场景震动代码----------------
         */
        auto move=MoveBy::create(0.05,Point(10,10));
        auto reve=move->reverse();
        auto seq=Sequence::create(move,reve,move,reve,move,reve, NULL);
        this->runAction(seq);
        
    });
    
    auto rotate=RotateBy::create(2,720);//2秒钟内旋转两圈
    
    auto callback=CallFunc::create([=](){    //使用lambda表达式简化函数调用,锤子砸中和尚后执行
        
       // hammer->removeFromParent();//从父节点中删除当前子节点
        
        monkAnimation(monk);//执行和尚晕动画
        
        for (auto hammerSpr:HammerArr)
        {
            
            hammerSpr->stopAllActions();//锤子砸中后，停止容器中，所有锤子的动作
            
            auto rote=RotateBy::create(0.8, 360);//让锤子重新0.8秒内旋转2圈
            
            auto out=FadeOut::create(0.8);//掉落下来以0.8秒淡出
            
            auto call=CallFunc::create([=](){
                
                hammerSpr->removeFromParent();//执行从父类中删除子类
            
            });
            
            auto spa=Spawn::create(rote,out,NULL);
            
            hammerSpr->runAction(Sequence::create(spa,call,NULL));
            
        }
        
        HammerArr.clear();//移除存放在“容器”的锤子对象
        
        m_fireCount=0;
        m_fireItemCount=0;
        
        unschedule(schedule_selector(GameSceneLayer::fire));//把原来的定时器关掉
        
        fireControl();//重新调用控制发射
        
    });
    
    /*
        spa动作是，边跳跃边旋转，当跳跃完成后执行jumpEnd动作
     */
    
    auto timer=DelayTime::create(0.6);
    auto delayCall=CallFunc::create([=](){
        
        bool lock=false;
        hammer->setUserData(&lock);
    
    });
    
    auto seq=Sequence::create(timer,delayCall,NULL);

    auto spa=Spawn::create(Sequence::create(jump,callback,jumpEnd,NULL) ,rotate,seq,NULL);
    
   
    
    hammer->runAction(spa);
    
}

void GameSceneLayer::check(float dt)
{   //实现碰撞检测函数
 
    if(!m_fingerRect.equals(Rect(0.0f,0.0f,0.0f,0.0f))){    //判断只有触摸点不为初始值才执行碰撞检测代码
        
        for (auto hammer:HammerArr)
        { //3.v增强型for循环，遍历容器元素
            
            if(!hammer->getUserData()) continue; //不是上升到最高点退出循环，以免上升的时候被拦截。
            
            if (m_fingerRect.intersectsRect(hammer->getBoundingBox())){ //检测手指点击的区域与锤子的区域是否交集
                
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("deng.mp3");
                
                hammerDismiss(hammer); //如果交集调用hammerDismiss函数并传锤子对象给它
                
                showAddScore(hammer->getPosition());//加分
                
                m_fingerRect=Rect(0.0f,0.0f,0.0f,0.0f); //每次碰撞检测后，设置成默认值，替换存留着最后一个手指点击的区域
                
                auto move=MoveBy::create(0.05,Point(0,5));
                auto reve=move->reverse();
                auto seq=Sequence::create(move,reve,move,reve, NULL);
                this->runAction(seq);
                
                break;//退出for循环
                
            }
        }
    }
}

void GameSceneLayer::hammerDismiss(Sprite *hammer)
{ //手指点击区域和锤子区域发生碰撞后，回调的函数
    
    hammer->stopAllActions();   //停止
    /*
        随机一个0到1数，等于0取冒号后面正数，不等于零取后面的负数。
     */
    auto jump=JumpBy::create(0.5,Vec2(arc4random()%2==0 ? 80 :-80, 50), 100, 1);
    
    auto rota=RotateBy::create(0.5,180);//0.5秒旋转1圈
    
    auto Out=FadeOut::create(0.5); //0.5秒时间淡出。对象还处在，只是隐藏了
    
    auto call=CallFunc::create([=](){
        
        hammer->removeFromParent();//移除->淡出隐藏在场景中的对象
        
        HammerArr.eraseObject(hammer);//清理容器中的锤子
        
    });
    
    
    /*
        执行完所有动态动作后，执行call动作，进行清理锤子对象。
     */
    auto seq=Sequence::create(Spawn::create(jump,rota,Out,NULL),call,NULL);
    
    hammer->runAction(seq);
    
}

void GameSceneLayer::monkAnimation(Sprite *monk)
{ //砸中和尚执行的动画
    
    auto animation=Animation::create();
    
    for(int i=1;i<=3;i++){
        
        animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("bag_0%d.png",i)));
        
        animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("dizzy_0%d.png",i)));
        
        }
    
    animation->setDelayPerUnit(3.0f/15.0f);//3秒15帧
    
    animation->setRestoreOriginalFrame(true); //让精灵恢复最初状态
    
    AnimationCache::getInstance()->addAnimation(animation,"dizziness");//缓存所有动画和动画帧，取个名称
    
    auto animate=Animate::create(AnimationCache::getInstance()->getAnimation("dizziness"));//创建动画-》动作
    
    auto bag_01=Sprite::createWithSpriteFrameName("bag_01.png");//创建精灵对象
    
    bag_01->setPosition(monk->getPosition().x,monk->getPosition().y+monk->getContentSize().height/2);
    
    addChild(bag_01);
    
    auto rep=Repeat::create(animate,2.0); //创建一个重复动作
    
    auto callClear=CallFunc::create([=](){

            bag_01->removeFromParent();
        
        });
    
    bag_01->runAction(Sequence::create(rep,callClear,NULL));
    
}


void GameSceneLayer::RestartGame(Ref* pSender)
{ //重新开始游戏
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.mp3");
    
    this->removeChildByTag(GAMEOVER_COLOR_LAYER_TAG);
    
    for (int i=5001; i<=5003; i++)  //让红心恢复显示到最大，即三颗红心都复原
    {
        
        this->getChildByTag(i)->setVisible(true);
    }

    Heart_Max_Tag=GAMESCENE_HEART_MAX_TAG;//重新赋予最大Tag值给Heart_Max_Tag,让红心能够从最右边一个个得消除
    
    m_fireCount=0;
    
    m_fireItemCount=0;
    
    m_score=0;
    
    refreshScore();
    
    HammerArr.clear();//清空容器中所有元素
    
    guideGame(); //显示指引精灵
    
    fireControl();//调用控制台，重新开始游戏

}


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void shareCallback(int platform, int stCode, string& errorMsg)
{
    if ( stCode == 100 )
    {
        CCLog("#### HelloWorld 开始分享");
        
        
    }
    else if ( stCode == 200 )
    {
        CCLog("#### HelloWorld 分享成功");
        
        //this->resume();

    }
    else
    {
        CCLog("#### HelloWorld 分享出错");
    }
    
    CCLog("platform num is : %d.", platform);
}
#endif//以上代码是安卓引用文件

void GameSceneLayer::ResurrectGame(Ref* pSender){   //复活游戏菜单回调
    
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
    sdk->setPlatformShareContent(WEIXIN_CIRCLE, "weixin-share",
                                 "assets/ic_lunch_share.png",str2->getCString(),
                                 "http://blog.csdn.net/bboyfeiyu");
    
    sdk->openShare("#本人最高得分120分，获得过", "assets/ic_lunch_share.png", share_selector(shareCallback));
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    sdk->openShare("要分享的文字内容","share.png", share_selector(shareCallback));
#endif
    
#endif//以上代码是安卓引用文件
    
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.mp3");
    
    this->removeChildByTag(GAMEOVER_COLOR_LAYER_TAG);
    
    
    
    this->getChildByTag(Heart_Min_Tag)->setVisible(true);////让红心恢复一个，Tag值最小那个，即显示最左边那个红心
    
    Heart_Max_Tag=GAMESCENE_HEART_MIN_TAG;  //重新赋予最左边那个Tag值给Heart_Max_Tag,让复活的那颗红心能够消除
    
    m_fireCount=0;
    
    m_fireItemCount=0;
    
    HammerArr.clear();//清空容器中所有元素
    
    fireControl();//调用控制台，重新开始游戏
    
}

void GameSceneLayer::showAddScore(Vec2 point)
{  //实现加分函数
    
    auto spr=Sprite::createWithSpriteFrameName("ten.png");
    
    spr->setPosition(point);
    
    this->addChild(spr);

    auto move=MoveBy::create(0.5,Vec2(0,50));//y轴向上移一点

    auto scale=ScaleTo::create(0.2,1.1);//0.2秒缩放1.1倍
  
    auto fade=FadeOut::create(0.3);
 
    auto seq=Sequence::create(DelayTime::create(0.2),fade, NULL);
    
    auto spa=Spawn::create(move,scale,seq,NULL);

    auto call=CallFunc::create([=](){   //加分
        
        spr->removeFromParent();
    
        m_score+=10;
        
        refreshScore();
        
    });
    
    spr->runAction(Sequence::create(spa,call,NULL));
    
}

void GameSceneLayer::refreshScore()
{
    
    auto lab=(LabelAtlas *)this->getChildByTag(SCORE_LAB_TAG);
    
     __String  *str=__String::createWithFormat("%d",m_score);//根据变量m_score创建一个字符串
    
    lab->setString(str->getCString());  //lab显示字符串内容
    
}

void GameSceneLayer::pauseMenuCall(Ref* pSender)//暂停按钮的回调
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.mp3");
    
    unschedule(schedule_selector(GameSceneLayer::fire));
    auto colorlay=LayerColor::create(Color4B(0, 0, 0, 128)); //创建一个半透明的颜色层
    colorlay->setTag(GAMEOVER_COLOR_LAYER_TAG);
    this->addChild(colorlay);
    
    auto HomeItem= MenuItemSprite::create(   //首页按钮
                                     Sprite::createWithSpriteFrameName("home_01.png"),
                                     Sprite::createWithSpriteFrameName("home_02.png"),
                                     CC_CALLBACK_1(GameSceneLayer::HomeSceneGame, this));
    
    HomeItem->setPosition(Vec2(320,540));
    
    auto item= MenuItemSprite::create(   //恢复按钮
                                     Sprite::createWithSpriteFrameName("continue.png"),
                                     Sprite::createWithSpriteFrameName("continue.png"),
                                     CC_CALLBACK_1(GameSceneLayer::continueGame, this));
    
    item->setPosition(Vec2(320,100));
    
    auto shieldTopTouch=MenuItem::create();     //1.创建一个菜单项
    
    shieldTopTouch->setContentSize(Size(WINSIZE_WIDTH,WINSIZE_HEIGHT)); //2.设置菜单项成屏幕大小
    
    shieldTopTouch->setPosition(Vec2(WINSIZE_WIDTH/2,WINSIZE_HEIGHT/2));//3.放在屏幕中间位置
    
    auto menu = Menu::create(shieldTopTouch,HomeItem,item, NULL);
    
    menu->setPosition(Vec2::ZERO);
    
    colorlay->addChild(menu);
    
    for (auto hammer:HammerArr )
    {
       
        hammer->pause();
        unschedule(schedule_selector(GameSceneLayer::fire));
        
    }
    
}

void GameSceneLayer::continueGame(Ref* pSender){ //恢复按钮回调
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.mp3");
    
    this->removeChildByTag(GAMEOVER_COLOR_LAYER_TAG);
    
    float time[]={HAMMER_FIRE_TIME_INTERVAL};//发射时间间隔

    for (auto hammer:HammerArr )
    {

        hammer->resume();
        schedule(schedule_selector(GameSceneLayer::fire),time[m_fireItemCount], kRepeatForever, 0);
        
    }
    
    /**
     防止一开始游戏就按暂停，再开始后，容器里面因为还没有元素，for循环里面的语句没有执行到
     **/
    schedule(schedule_selector(GameSceneLayer::fire),time[m_fireItemCount], kRepeatForever, 0);
    
}

void GameSceneLayer::HomeSceneGame(Ref* pSender){//首页场景回调
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.mp3");
    
    Director::getInstance()->replaceScene(HomeSceneLayer::createScene());

}

void GameSceneLayer::onKeyReleased(EventKeyboard::KeyCode keyCode,Event * pEvent) //系统返回键回调
{
    Director::getInstance()->replaceScene(HomeSceneLayer::createScene());
}
