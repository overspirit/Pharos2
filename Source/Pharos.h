#pragma once

#include "TypesDef.h"

//Utils
////////////////////////////////////////////////////////////////////////
//#include "Utils/Util.h"
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

//Engine
////////////////////////////////////////////////////////////////////////
// #include "Engine/EventDefine.h"
// #include "Engine/ITimer.h"
// #include "Engine/IResource.h"
// #include "Engine/IFile.h"
// #include "Engine/IFont.h"
// #include "Engine/IImage.h"
// #include "Engine/IXmlAttribute.h"
// #include "Engine/IXmlNode.h"
// #include "Engine/IXmlDocument.h"
// #include "Engine/IProperties.h"
// #include "Engine/IApplication.h"
//#include "Engine/IMemBuffer.h"
// #include "Engine/IKernel.h"
////////////////////////////////////////////////////////////////////////

//Render
////////////////////////////////////////////////////////////////////////
// #include "Render/RenderDefine.h"
// #include "Render/IRenderSampleState.h"
// #include "Render/IRenderBlendState.h"
// #include "Render/IRenderDepthStencilState.h"
// #include "Render/IRenderRasterizerState.h"
// #include "Render/IRenderTexture.h"
// #include "Render/IRenderShaderData.h"
// #include "Render/IRenderProgram.h"
// #include "Render/IRenderFrameBuffer.h"
// #include "Render/IRenderLayout.h"
// #include "Render/IRenderer.h"
// #include "Render/IRenderVariable.h"
// #include "Render/IRenderPass.h"
// #include "Render/IRenderTechnique.h"
// #include "Render/IRenderBlock.h"
// #include "Render/IRenderImage.h"
// #include "Render/IRenderFont.h"
// #include "Render/IRenderCallback.h"
// #include "Render/IMaterial.h"
// #include "Render/IMesh.h"
// #include "Render/IModel.h"
// #include "Render/IRenderMgr.h"
////////////////////////////////////////////////////////////////////////

//Desktop
////////////////////////////////////////////////////////////////////////
// #include "Desktop/IControlViewer.h"
// #include "Desktop/IDesktopMgr.h"
////////////////////////////////////////////////////////////////////////

//Scene
////////////////////////////////////////////////////////////////////////
// #include "Scene/ICamera.h"
// #include "Scene/ISceneNode.h"
// #include "Scene/ISceneGraph.h"
// #include "Scene/ISceneImporter.h"
// #include "Scene/ISceneMgr.h"
////////////////////////////////////////////////////////////////////////

using namespace Pharos;

// IApplicationPtr NewApplication();
// #define IMPL_CREATE_APPLICATION(myApp) IApplicationPtr NewApplication(){return MakeSharedPtr<myApp>();}
// 
// extern Engine::IKernel* g_pKernel;
// extern Render::IRenderMgr* g_pRenderMgr;
// extern Scene::ISceneMgr* g_pSceneMgr;
// extern Desktop::IDesktopMgr* g_pDesktopMgr;
