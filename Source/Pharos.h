#pragma once

#include "TypesDef.h"

//Utils
////////////////////////////////////////////////////////////////////////
#include "Utils/Util.h"
#include "Utils/Factory.h"
#include "Utils/Path.h"
#include "Utils/Singleton.h"
#include "Utils/Thread.h"
////////////////////////////////////////////////////////////////////////

//Math
////////////////////////////////////////////////////////////////////////
#include "Math/Common.h"
#include "Math/Size2D.h"
#include "Math/Vector2D.h"
#include "Math/Vector3D.h"
#include "Math/Vector4D.h"
#include "Math/Point2D.h"
#include "Math/Line2D.h"
#include "Math/Line3D.h"
#include "Math/Rect2D.h"
#include "Math/Plane3D.h"
#include "Math/Triangle3D.h"
#include "Math/AABBox3D.h"
#include "Math/Frustum.h"
#include "Math/Matrix4.h"
#include "Math/Quaternion.h"
#include "Math/DualQuaternion.h"
#include "Math/Color.h"
#include "Math/Geometry.h"
////////////////////////////////////////////////////////////////////////

//Core
////////////////////////////////////////////////////////////////////////
#include "Core/CoreGlobal.h"
////////////////////////////////////////////////////////////////////////

//Render
////////////////////////////////////////////////////////////////////////
// #include "Render/RenderGlobal.h"
////////////////////////////////////////////////////////////////////////

//Desktop
////////////////////////////////////////////////////////////////////////
// #include "Desktop/DesktopGlobal.h"
////////////////////////////////////////////////////////////////////////

//Scene
////////////////////////////////////////////////////////////////////////
// #include "Scene/SceneGlobal.h"
////////////////////////////////////////////////////////////////////////

using namespace Pharos;

IApplication* NewApplication();
#define IMPL_CREATE_APPLICATION(myApp) IApplication* NewApplication(){return new myApp();}
// 
// extern Engine::IKernel* g_pKernel;
// extern Render::IRenderMgr* g_pRenderMgr;
// extern Scene::ISceneMgr* g_pSceneMgr;
// extern Desktop::IDesktopMgr* g_pDesktopMgr;
