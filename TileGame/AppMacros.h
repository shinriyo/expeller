//
//  AppMacros.h
//  TileGame
//
//  Created by 臣輔 杉田 on 2/9/14.
//
//

#ifndef TileGame_AppMacros_h
#define TileGame_AppMacros_h

#include "cocos2d.h"

typedef struct tagResource
{
    cocos2d::CCSize size;
    char directory[100];
} Resource;

static Resource smallResource = { cocos2d::CCSizeMake(384, 512), "S" };
static Resource mediumResource = { cocos2d::CCSizeMake(768, 1024), "M" };
static Resource largeResource = { cocos2d::CCSizeMake(1536, 2048), "L" };

static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(768, 1024);

#define SCORE_FONT_SIZE (cocos2d::CCEGLView::sharedOpenGLView()->getDesignResolutionSize().width / mediumResource.size.width * 48)

#endif
