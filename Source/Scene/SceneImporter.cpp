#include "PreCompile.h"
//#include "RenderGlobal.h"
#include "SceneGlobal.h"

SceneImporter::SceneImporter(void)
{

}

SceneImporter::~SceneImporter(void)
{

}

bool SceneImporter::ImportScene(ISceneGraphPtr scene)
{
	for (SceneNodeData& nodeData : m_nodeDataList)
	{
		ISceneNodePtr node = CreateNode(nodeData);
		if (node != nullptr)
		{
			scene->AddSceneNode(node);
		}
	}

	return true;
}

ISceneNodePtr SceneImporter::CreateNode(const SceneNodeData& data)
{
	ISceneNodePtr parentNode = sSceneMgr->GetSceneNode(data.parentName.c_str());

	ISceneNodePtr sceneNode = sSceneMgr->CreateSceneNode(data.nodeName.c_str(), parentNode);
	
	sceneNode->SetLocalTransform(data.localTrans);
	sceneNode->SetBoundingRadius(data.boundRadius);

	for (auto& childData : data.childData)
	{
		ISceneNodePtr child = this->CreateNode(childData);
		if (child != nullptr) sceneNode->AddChildNode(child);
	}

	IModelPtr model = this->CreateModel(data.modelData);
	sceneNode->AddModel(model);

	return sceneNode;
}

IModelPtr SceneImporter::CreateModel(const ModelData& modelData)
{
	IModelPtr model = sRenderMgr->GenerateModelObject();

 	for (const MeshData& meshData : modelData.meshDataList)
 	{
		IMeshPtr mesh = this->CreateMesh(meshData);
		if (mesh != nullptr)
		{
			model->AddMesh(mesh);
		}
 	}
// 
// 	m_boneInfoList.resize(modelData.boneInfoList.size());
// 	for (const BoneInfo& boneInfo : modelData.boneInfoList)
// 	{
// 		m_boneInfoList[boneInfo.id] = boneInfo;
// 	}
// 
// 	for (const SkelAnimation& skelAnim : modelData.skelAnimList)
// 	{
// 		m_animList[skelAnim.name] = skelAnim;
// 	}
// 
// 	if (m_animList.size() > 0)
// 	{
// 		m_currAnim = &m_animList.begin()->second;
// 
// 		uint32 boneNum = (uint32)m_currAnim->frameList[0].tranList.size();
// 		m_animBoneTrans.resize(boneNum);
// 	}
// 
// 	return true;

	return model;
}

IMeshPtr SceneImporter::CreateMesh(const MeshData& meshData)
{
	IMeshPtr mesh = sRenderMgr->GenerateMeshObject();

	mesh->SetMeshData(meshData.vertexData, meshData.vertDesc, meshData.indexData);
	mesh->SetDrawType(meshData.drawType);

	IMaterialPtr material = this->CreateMaterial(meshData.materialData);
	if (material != nullptr) mesh->SetAttachMaterial(material);
	
	return mesh;
}

IMaterialPtr SceneImporter::CreateMaterial(const MaterialData& materialData)
{
	IRendererPtr renderer = sRenderMgr->GetCurrentRenderer();

	IMaterialPtr material = sRenderMgr->GenerateMaterialObject();
	
	material->SetRenderTechnique(materialData.techName.c_str());

	for (auto texIter : materialData.samplerDataList)
	{
		string texName = texIter.first;		
		const SamplerData& sampleData = texIter.second;
		string texPath = sampleData.texPath;
		IRenderTexturePtr tex = renderer->LoadTexture(texPath.c_str());
		material->SetParameterValue(texName.c_str(), tex);
	}

	for (auto varIter : materialData.varList)
	{
		string varName = varIter.first;
		Engine::PropType type = varIter.second.type;

		switch (type)
		{
			case Engine::EPT_STRING:
			{

			}
			break;
			case Engine::EPT_NUMBER:
			{
				material->SetParameterValue(varName.c_str(), varIter.second.fValue);
			}
			break;
			case Engine::EPT_VECTOR2:
			{
				material->SetParameterValue(varName.c_str(), varIter.second.vt2Value);
			}
			break;
			case Engine::EPT_VECTOR3:
			{
				material->SetParameterValue(varName.c_str(), varIter.second.vt3Value);
			}
			break;
			case Engine::EPT_VECTOR4:
			{
				material->SetParameterValue(varName.c_str(), varIter.second.vt4Value);
			}
			break;
			case Engine::EPT_MATRIX:
			{
				material->SetParameterValue(varName.c_str(), varIter.second.matValue);
			}
			break;
		}
	}

	return material;
}