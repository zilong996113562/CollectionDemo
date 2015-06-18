//
//  GameScene.h
//  MonkGame
//
//  Created by 陈子龙 on 15-5-19.
//
//

#ifndef __MonkGame__GameScene__
#define __MonkGame__GameScene__

#include "cocos2d.h"
USING_NS_CC; //添加cocos2d的命名空间。

class GameSceneLayer : public Layer { //创建GameSceneLayer类，继承于Layer。GameSceneLayer属于图层类
    
public:
    
    GameSceneLayer();
    ~GameSceneLayer();
    
    static Scene *scene(); //静态函数，创建Scene场景，返回场景对象
    
    virtual bool init();//init虚函数，进行初始化操作，返回布尔类型
    
    void onKeyReleased(EventKeyboard::KeyCode keyCode,Event * pEvent);
    
private:
    
    Vector<Sprite *> MonkRectArr;   //记录小和尚
                                    /*创建Vector类型的MonkRectArr和HanmmerArr成员变量，并指定Vector容器内存放的是Sprite指针类型
                                   ”Vector构造函数参数“是容器的“初始化变量” */
    Vector<Sprite *> HammerArr;    //存放场景中所有的锤子
    
    void guideGame();
    
    void fireControl();
    
    void fire(float dt);
    
    int m_fireItemCount;//记录当前发射第几拨
    
    int m_fireCount;//记录当前这一拨中，第几次发射
    
    Rect m_fingerRect; //当前手指点击的区域
    
    int Heart_Max_Tag;
    
    int Heart_Min_Tag;
    
    int m_score;
    
    void cloudAction();
    
    void check(float dt);//检测碰撞
    
    void hammerDismiss(Sprite *hammer);//碰撞后移除锤子
    
    void monkAnimation(Sprite *monk);
    
    void showAddScore(Vec2 point);
    
    void refreshScore();//刷新分数
    
    /*
     scheduleUpdate()调度器
    来自cocos2d-x官方文档：该调度器是使用Node的刷新事件update方法，该方法在每帧绘制之前都会被调用一次
    停止这个调度器，可以使用unschedulerUpdate()方法。
     */
   // void update(float dt) override;//Node默认是没有启用update事件的,重载update方法，来执行自己的逻辑代码。
    void pauseMenuCall(Ref* pSender); //暂停游戏
    
    void continueGame(Ref* pSender);  //继续游戏
    
    void RestartGame(Ref* pSender);//重新开始游戏菜单回调
    
    void ResurrectGame(Ref* pSender);
    
    void HomeSceneGame(Ref* pSender);//回到首页场景
    
    CREATE_FUNC(GameSceneLayer);//当前类创建的对象，将会由自动释放。
    
    
};



#endif /* defined(__MonkGame__GameScene__) */
