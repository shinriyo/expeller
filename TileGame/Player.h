//
//  Player.h
//  TileGame
//
//  Created by 臣輔 杉田 on 2/13/14.
//
//

#ifndef __TileGame__Player__
#define __TileGame__Player__

#include "cocos2d.h"

using namespace cocos2d;

class Player : public CCSprite
{
public:
    CCAnimationCache* setupAnimations();
};

#endif /* defined(__TileGame__Player__) */
