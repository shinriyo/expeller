#include "GameScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;

CCScene* Game::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    Game *layer = Game::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    HudLayer *hud = new HudLayer();
    hud->init();
    scene->addChild(hud);
    layer->_hud = hud;
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Game::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("pickup.caf");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("hit.caf");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("move.caf");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("TileMap.caf");
    
    _tileMap = new CCTMXTiledMap();

    // Area and Stage
    int stage = 1;
    int area = 1;

    CCString* tileMapStr = CCString::createWithFormat("TileMap_%d_%d.tmx", stage, area);
    CCLOG("%s", tileMapStr->getCString());
    _tileMap->initWithTMXFile(tileMapStr->getCString());
    _background = _tileMap->layerNamed("Background");
    _foreground = _tileMap->layerNamed("Foreground");

    _meta = _tileMap->layerNamed("Meta");
    _meta->setVisible(false);
    
    this->addChild(_tileMap);
    
    CCTMXObjectGroup *objectGroup = _tileMap->objectGroupNamed("Objects");
    
    if(objectGroup == NULL){
        CCLog("tile map has no objects object layer");
        return false;
    }
    
    CCDictionary *spawnPoint = objectGroup->objectNamed("SpawnPoint");
    
    int x = ((CCString)*spawnPoint->valueForKey("x")).intValue();
    int y = ((CCString)*spawnPoint->valueForKey("y")).intValue();
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Player.plist");
    _player = new CCSprite();
    _player->initWithSpriteFrameName("Player_right_1.png");
    _player->setPosition(ccp(x,y));
    
    this->setupPlayerAnimations();
    
    this->addChild(_player);
    this->setViewPointCenter(_player->getPosition());
    
    this->setTouchEnabled(true);
    
    return true;
}

void Game::setupPlayerAnimations()
{
    CCAnimation *pAnimationFront = CCAnimation::create();
    CCAnimation *pAnimationBack  = CCAnimation::create();
    CCAnimation *pAnimationLeft  = CCAnimation::create();
    CCAnimation *pAnimationRight = CCAnimation::create();

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

    pAnimationFront->setDelayPerUnit(0.5f);
    pAnimationBack->setDelayPerUnit(0.5f);
    pAnimationLeft->setDelayPerUnit(0.5f);
    pAnimationRight->setDelayPerUnit(0.5f);
    _animationCache = CCAnimationCache::sharedAnimationCache();
    _animationCache->addAnimation( pAnimationFront, "FRONT" );
    _animationCache->addAnimation( pAnimationBack,  "BACK" );
    _animationCache->addAnimation( pAnimationLeft,  "LEFT" );
    _animationCache->addAnimation( pAnimationRight, "RIGHT" );
}

void Game::setViewPointCenter(CCPoint position)
{
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    int x = MAX(position.x, winSize.width/2);
    int y = MAX(position.y, winSize.height/2);
    x = MIN(x, (_tileMap->getMapSize().width * this->_tileMap->getTileSize().width) - winSize.width / 2);
    y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height/2);
    CCPoint actualPosition = ccp(x, y);
    
    CCPoint centerOfView = ccp(winSize.width/2, winSize.height/2);
    CCPoint viewPoint = ccpSub(centerOfView, actualPosition);
    this->setPosition(viewPoint);
}

#pragma mark - handle touches

void Game::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool Game::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
    return true;
}

void Game::setPlayerPosition(CCPoint position)
{
        _hud->setStageLabel(1, 1);
    CCPoint point = ccpAdd(position, CCPoint(1, 1));
    CCPoint tileCoord2 = this->tileCoordForPosition(point);
    int tileGid2 = _meta->tileGIDAt(tileCoord2);

    CCPoint tileCoord = this->tileCoordForPosition(position);
    int tileGid = _meta->tileGIDAt(tileCoord);
    
    if (tileGid) {
        CCDictionary *properties = _tileMap->propertiesForGID(tileGid);
        if (properties) {
            // obstacle
            CCString *collision = new CCString();
            *collision = *properties->valueForKey("Collidable");
            
            if (collision && (collision->compare("True") == 0)) {
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("hit.caf");
                return;
            }
            
            // item get
            CCString *collectible = new CCString();
            
            *collectible = *properties->valueForKey("Collectable");
            if (collectible && (collectible->compare("True") == 0)) {
                _meta->removeTileAt(tileCoord);
                _foreground->removeTileAt(tileCoord);
                _numCollected++;
                _hud->numCollectedChanged(_numCollected);
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pickup.caf");
            }
        }
    }
    _player->setPosition(position);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("move.caf");
}

void Game::ccTouchEnded(CCTouch *touch, CCEvent *event)
{
    CCPoint touchLocation = touch->getLocationInView();
    touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
    touchLocation = this->convertToNodeSpace(touchLocation);
    
    CCPoint playerPos = _player->getPosition();
    CCPoint diff = ccpSub(touchLocation, playerPos);
    _player->stopAllActions();
    
    if ( abs(diff.x) > abs(diff.y) ) {
        if (diff.x > 0) {
            // right
            runPlayerAcrion("RIGHT");
            //_player->setFlipX(false);
            playerPos.x += _tileMap->getTileSize().width;
        } else {
            // left
            runPlayerAcrion("LEFT");
            //_player->setFlipX(true);
            playerPos.x -= _tileMap->getTileSize().width;
        }
    } else {
        if (diff.y > 0) {
            // up
            // BACK
            runPlayerAcrion("BACK");
            playerPos.y += _tileMap->getTileSize().height;
        } else {
            // down
            // FRONT
            runPlayerAcrion("FRONT");
            playerPos.y -= _tileMap->getTileSize().height;
        }
    }
    
    // safety check on the bounds of the map
    if (playerPos.x <= (_tileMap->getMapSize().width * _tileMap->getTileSize().width) &&
        playerPos.y <= (_tileMap->getMapSize().height * _tileMap->getTileSize().height) &&
        playerPos.y >= 0 &&
        playerPos.x >= 0 )
    {
        this->setPlayerPosition(playerPos);
    }
    
    this->setViewPointCenter(_player->getPosition());
}

void Game::runPlayerAcrion(const char* name)
{
    CCAnimation *pAnimation = _animationCache->animationByName(name);
    CCRepeatForever *pAction = CCRepeatForever::create( CCAnimate::create(pAnimation) );
    _player->runAction(pAction);
}

CCPoint Game::tileCoordForPosition(CCPoint position)
{
    int x = position.x / _tileMap->getTileSize().width;
    int y = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - position.y) / _tileMap->getTileSize().height;
    return ccp(x, y);
}
