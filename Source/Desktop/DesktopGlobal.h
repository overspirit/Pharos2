#pragma once

#include "UIDefine.h"
#include "IControlViewer.h"

#include "FontTextureManager.h"

#include "RenderFont.h"
#include "RenderImage.h"
#include "RenderSprite.h"

#include "UIObjects/UIObject.h"
#include "UIObjects/Texture.h"
#include "UIObjects/FontString.h"
#include "UIObjects/Frame.h"
#include "UIObjects/WorldFrame.h"
#include "UIObjects/Button.h"
#include "UIObjects/CheckButton.h"
#include "UIObjects/Slider.h"
#include "UIObjects/Progress.h"

#include "DesktopMgr.h"

#define sDesktopMgr DesktopMgr::Inst()

#define sFontTexMgr FontTextureManager::Inst()

#define sRenderSpirite RenderSprite::Inst()

using namespace Desktop;

inline Utils::Factory<UIObject>& GetUIFactoryInst()
{
	static Utils::Factory<UIObject> factory;
	return factory;
}




