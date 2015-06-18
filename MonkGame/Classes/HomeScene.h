//
//  HomeScene.h
//  MonkGame
//
//  Created by 陈子龙 on 15-5-25.
//
//

#ifndef __MonkGame__HomeScene__
#define __MonkGame__HomeScene__

#include "cocos2d.h"
USING_NS_CC;

class HomeSceneLayer : public Layer
{
public:
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void menuStartCallback(cocos2d::Ref* pSender);//”开始“按钮回调
    
    void menucommentCallback(cocos2d::Ref* pSender);//”点评“按钮回调
    
    void menuFeedbackCallback(cocos2d::Ref* pSender);//“反馈”按钮回调
    
    void menuShareCallback(cocos2d::Ref* pSender);//”分享“按钮回调
    
    void onKeyReleased(EventKeyboard::KeyCode keyCode,Event * pEvent);
    
    CREATE_FUNC(HomeSceneLayer);
};


#endif /* defined(__MonkGame__HomeScene__) */
