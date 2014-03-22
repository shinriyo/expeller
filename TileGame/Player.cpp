//
//  Player.cpp
//  TileGame
//
//  Created by 臣輔 杉田 on 2/13/14.
//
//

#include "Player.h"
#include "SimpleAudioEngine.h"

Player::Player()
{
}

CCAnimationCache* Player::setupAnimations()
{
    CCAnimation *pAnimationFront = CCAnimation::create();
    CCAnimation *pAnimationBack  = CCAnimation::create();
    CCAnimation *pAnimationLeft  = CCAnimation::create();
    CCAnimation *pAnimationRight = CCAnimation::create();
    CCAnimation *pAnimationAttack = CCAnimation::create();
    
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSpriteFrame *frame;
    
    // 手前 FRONT
    frame = cache->spriteFrameByName("Player_front_1.png");
    pAnimationFront->addSpriteFrame(frame);
    frame = cache->spriteFrameByName("Player_front_2.png");
    pAnimationFront->addSpriteFrame(frame);
    
    // 上 BACK
    frame = cache->spriteFrameByName("Player_back_1.png");
    pAnimationBack->addSpriteFrame(frame);
    frame = cache->spriteFrameByName("Player_back_2.png");
    pAnimationBack->addSpriteFrame(frame);
    
    // 右
    frame = cache->spriteFrameByName("Player_right_1.png");
    pAnimationRight->addSpriteFrame(frame);
    frame = cache->spriteFrameByName("Player_right_2.png");
    pAnimationRight->addSpriteFrame(frame);
    
    // 左
    frame = cache->spriteFrameByName("Player_left_1.png");
    pAnimationLeft->addSpriteFrame(frame);
    frame = cache->spriteFrameByName("Player_left_2.png");
    pAnimationLeft->addSpriteFrame(frame);
    
    // 攻撃
    frame = cache->spriteFrameByName("Player_attack_1.png");
    pAnimationAttack->addSpriteFrame(frame);
    frame = cache->spriteFrameByName("Player_attack_2.png");
    pAnimationAttack->addSpriteFrame(frame);
    
    pAnimationFront->setDelayPerUnit(0.2f);
    pAnimationBack->setDelayPerUnit(0.2f);
    pAnimationLeft->setDelayPerUnit(0.2f);
    pAnimationRight->setDelayPerUnit(0.2f);
    pAnimationAttack->setDelayPerUnit(0.2f);

    CCAnimationCache *animationCache = CCAnimationCache::sharedAnimationCache();
    animationCache->addAnimation( pAnimationFront, "P_FRONT" );
    animationCache->addAnimation( pAnimationBack,  "P_BACK" );
    animationCache->addAnimation( pAnimationLeft,  "P_LEFT" );
    animationCache->addAnimation( pAnimationRight, "P_RIGHT" );
    animationCache->addAnimation( pAnimationAttack, "P_ATTACK" );

    return animationCache;
}

void Player::runAnimation(const char* name)
{
    CCAnimationCache *animationCache = CCAnimationCache::sharedAnimationCache();
    CCAnimation *pAnimation = animationCache->animationByName(name);
    CCRepeatForever *pAction = CCRepeatForever::create( CCAnimate::create(pAnimation) );
    this->runAction(pAction);
}

