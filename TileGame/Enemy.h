//
//  Enemy.h
//  TileGame
//
//  Created by 臣輔 杉田 on 2/13/14.
//
//

#ifndef __TileGame__Enemy__
#define __TileGame__Enemy__

#include "cocos2d.h"

using namespace cocos2d;

class Enemy : public CCSprite
{
public:
    CCAnimationCache* setupAnimations(const char* enemyName);
//    void randomWalk(CCTMXTiledMap *tileMap);
    void randomWalk(CCTMXLayer *meta, CCTMXTiledMap *tileMap);
    Enemy();
private:
    void finishAnimation();
    bool _isMoveable = true;
    void runAnimation(const char* name);
    float const SPEED = 1.0f;
    CCPoint tileCoordForPosition(CCTMXTiledMap *tileMap, CCPoint position);
};

#endif /* defined(__TileGame__Enemy__) */
