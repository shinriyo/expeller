//
//  Enemy.cpp
//  TileGame
//
//  Created by 臣輔 杉田 on 2/13/14.
//
//

#include "Enemy.h"

CCAnimationCache* Enemy::setupAnimations(const char* enemyName)
{
    CCAnimation *pAnimationFront = CCAnimation::create();
    CCAnimation *pAnimationBack  = CCAnimation::create();
    CCAnimation *pAnimationLeft  = CCAnimation::create();
    CCAnimation *pAnimationRight = CCAnimation::create();
    CCAnimation *pAnimationAttack = CCAnimation::create();
    
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSpriteFrame *frame;
    
    // 手前 FRONT
    CCString* tmpStr = CCString::createWithFormat("%s_front_1.png", enemyName);
    frame = cache->spriteFrameByName(tmpStr->getCString());
    pAnimationFront->addSpriteFrame(frame);
    tmpStr = CCString::createWithFormat("%s_front_2.png", enemyName);
    frame = cache->spriteFrameByName(tmpStr->getCString());
    pAnimationFront->addSpriteFrame(frame);
    
    // 上 BACK
    tmpStr = CCString::createWithFormat("%s_back_1.png", enemyName);
    frame = cache->spriteFrameByName(tmpStr->getCString());
    pAnimationBack->addSpriteFrame(frame);
    tmpStr = CCString::createWithFormat("%s_back_2.png", enemyName);
    frame = cache->spriteFrameByName(tmpStr->getCString());
    pAnimationBack->addSpriteFrame(frame);
    
    // 右
    tmpStr = CCString::createWithFormat("%s_right_1.png", enemyName);
    frame = cache->spriteFrameByName(tmpStr->getCString());
    pAnimationRight->addSpriteFrame(frame);
    tmpStr = CCString::createWithFormat("%s_right_2.png", enemyName);
    frame = cache->spriteFrameByName(tmpStr->getCString());
    pAnimationRight->addSpriteFrame(frame);
    
    // 左
    tmpStr = CCString::createWithFormat("%s_left_1.png", enemyName);
    frame = cache->spriteFrameByName(tmpStr->getCString());
    pAnimationLeft->addSpriteFrame(frame);
    tmpStr = CCString::createWithFormat("%s_left_2.png", enemyName);
    frame = cache->spriteFrameByName(tmpStr->getCString());
    pAnimationLeft->addSpriteFrame(frame);
    
    // 攻撃
    tmpStr = CCString::createWithFormat("%s_attack_1.png", enemyName);
    frame = cache->spriteFrameByName(tmpStr->getCString());
    pAnimationAttack->addSpriteFrame(frame);
    tmpStr = CCString::createWithFormat("%s_attack_2.png", enemyName);
    frame = cache->spriteFrameByName(tmpStr->getCString());
    pAnimationAttack->addSpriteFrame(frame);
    
    pAnimationFront->setDelayPerUnit(0.5f);
    pAnimationBack->setDelayPerUnit(0.5f);
    pAnimationLeft->setDelayPerUnit(0.5f);
    pAnimationRight->setDelayPerUnit(0.5f);
    pAnimationAttack->setDelayPerUnit(0.5f);
    
    CCAnimationCache *animationCache = CCAnimationCache::sharedAnimationCache();
    animationCache = CCAnimationCache::sharedAnimationCache();
    animationCache->addAnimation( pAnimationFront, "FRONT" );
    animationCache->addAnimation( pAnimationBack,  "BACK" );
    animationCache->addAnimation( pAnimationLeft,  "LEFT" );
    animationCache->addAnimation( pAnimationRight, "RIGHT" );
    animationCache->addAnimation( pAnimationAttack, "ATTACK" );
    
    return animationCache;
}