#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
#include "SceneGlobal.h"

SceneImporter::SceneImporter(void)
{

}

SceneImporter::~SceneImporter(void)
{

}

bool SceneImporter::ImportScene(OctreeScene* scene)
{
	for (SceneNodeData& nodeData : m_nodeDataList)
	{
		SceneNode* node = CreateNode(nodeData);
		if (node != nullptr)
		{
			scene->AddSceneNode(node);
		}
	}

	return true;
}

SceneNode* SceneImporter::CreateNode(const SceneNodeData& data)
{
	SceneNode* parentNode = sSceneMgr->GetSceneNode(data.parentName.c_str());

	SceneNode* sceneNode = sSceneMgr->CreateSceneNode(data.nodeName.c_str(), parentNode);
	
	sceneNode->SetLocalTransform(data.localTrans);
	sceneNode->SetBoundingRadius(data.boundRadius);

	for (auto& childData : data.childData)
	{
		SceneNode* child = this->CreateNode(childData);
		if (child != nullptr) sceneNode->AddChildNode(child);
	}

	Model* model = this->CreateModel(data.modelData);
	sceneNode->AddModel(model);

	return sceneNode;
}

Model* SceneImporter::CreateModel(const ModelData& modelData)
{
	Model* model = new Model();

 	for (const MeshData& meshData : modelData.meshDataList)
 	{
		Mesh* mesh = this->CreateMesh(meshData);
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

Mesh* SceneImporter::CreateMesh(const MeshData& meshData)
{
	Mesh* mesh = new Mesh();

	mesh->SetMeshData((MemoryBuffer*)&meshData.vertexData, meshData.vertDesc, (MemoryBuffer*)&meshData.indexData);
	mesh->SetDrawType(meshData.drawType);

	Material* material = this->CreateMaterial(meshData.materialData);
	if (material != nullptr) mesh->SetAttachMaterial(material);
	
	return mesh;
}

Material* SceneImporter::CreateMaterial(const MaterialData& materialData)
{
	Renderer* renderer = sRenderMgr->GetCurrentRenderer();

	Material* material = new Material();
	
	material->SetRenderTechnique(materialData.techName.c_str());

	for (auto texIter : materialData.samplerDataList)
	{
		string texName = texIter.first;		
		const SamplerData& sampleData = texIter.second;
		string texPath = sampleData.texPath;
		RenderTexture* tex = renderer->LoadTexture(texPath.c_str());
		material->SetParameterValue(texName.c_str(), tex);
	}

	for (auto varIter : materialData.varList)
	{
		string varName = varIter.first;
		Core::PropType type = varIter.second.type;

		switch (type)
		{
			case Core::EPT_STRING:
			{

			}
			break;
			case Core::EPT_NUMBER:
			{
				material->SetParameterValue(varName.c_str(), varIter.second.fValue);
			}
			break;
			case Core::EPT_VECTOR2:
			{
				material->SetParameterValue(varName.c_str(), varIter.second.vt2Value);
			}
			break;
			case Core::EPT_VECTOR3:
			{
				material->SetParameterValue(varName.c_str(), varIter.second.vt3Value);
			}
			break;
			case Core::EPT_VECTOR4:
			{
				material->SetParameterValue(varName.c_str(), varIter.second.vt4Value);
			}
			break;
			case Core::EPT_MATRIX:
			{
				material->SetParameterValue(varName.c_str(), varIter.second.matValue);
			}
			break;
		}
	}

	return material;
}