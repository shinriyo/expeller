//
//  EndingScene.h
//  TileGame
//
//  Created by 17age forever on 2/8/14.
//
//

#ifndef __TileGame__EndingScene__
#define __TileGame__EndingScene__

#include "cocos2d.h"

class Ending : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(Ending);
};

#endif /* defined(__TileGame__EndingScene__) */
