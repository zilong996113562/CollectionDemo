//
//  MainScene.h
//  MonkGame
//
//  Created by 陈子龙 on 15-5-24.
//
//

#ifndef __MonkGame__MainScene__
#define __MonkGame__MainScene__

#include "cocos2d.h"
USING_NS_CC;

class MainSceneLayer : public cocos2d::Layer
{
public:

    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    CREATE_FUNC(MainSceneLayer);
};

#endif /* defined(__MonkGame__MainScene__) */
