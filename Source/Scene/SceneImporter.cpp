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

	Model* model = this->CreateModel(m_modelDataList[data.modelId]);
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

	Material* material = this->CreateMaterial(m_materialDataList[meshData.materialName]);
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

bool SceneImporter::SaveTo(const char8* file)
{
	XmlDocument doc;
	if (!doc.CreateRootNode("Scene")) return false;

	XmlNode* rootNode = doc.GetRootNode();	
	XmlNode* materialChunkNode = rootNode->AppendChild("materials_chunk");
	XmlNode* modelChunkNode = rootNode->AppendChild("models_chunk");
	XmlNode* nodeChunkNode = rootNode->AppendChild("nodes_chunk");

	for (auto iter : m_materialDataList)
	{
		SaveMaterialData(iter.second, materialChunkNode);
	}	

	for (auto iter : m_modelDataList)
	{
		SaveModelData(iter.second, modelChunkNode);
	}

	for (SceneNodeData& nodeData : m_nodeDataList)
	{
		SaveSceneNodeData(nodeData, nodeChunkNode, modelChunkNode);
	}

	doc.Save(file);

	return true;
}

void SceneImporter::SaveSceneNodeData(SceneNodeData& nodeData, XmlNode* parentNode, XmlNode* modelRootNode)
{
	XmlNode* nodeNode = parentNode->AppendChild("node");

	XmlAttribute* nodeNameAttr = nodeNode->AppendAttribute("name");
	nodeNameAttr->SetValue(nodeData.nodeName.c_str());

	Quaternion quat = nodeData.localTrans.GetQuaternion();
	Vector3Df scale = nodeData.localTrans.GetScale();
	Vector3Df trans = nodeData.localTrans.GetTranslation();


// 	Matrix4 test;
// 	for (uint32 i = 0; i < 16; i++)
// 	{
// 		float32 r = Math::random(0.0f, 5.0f);
// 
// 		test.m[i] = r;
// 	}

// 	Quaternion quat = test.GetQuaternion();
// 	Vector3Df scale = test.GetScale();
// 	Vector3Df pos = test.GetTranslation();

// 	Matrix4 scaleMat;
// 	scaleMat.SetScale(scale);
// 	//Matrix4 scaleMat2;
// 	//scaleMat.GetInverse(scaleMat2);
// 
// 	Matrix4 test2 = quat.GetMatrix();	
// 	test2.SetTranslation(trans);
// 	test2 *= scaleMat;
// 
// 	if (nodeData.localTrans != test2)
// 	{
// 		int test = 0;
// 	}

	if (scale.IsEmpty()) scale = Vector3Df(1.0f, 1.0f, 1.0f);

	XmlAttribute* nodeRotaAttr = nodeNode->AppendAttribute("rotation");
	nodeRotaAttr->SetValue(*(Vector4Df*)&quat);

	XmlAttribute* nodeScaleAttr = nodeNode->AppendAttribute("scale");
	nodeScaleAttr->SetValue(scale);

	XmlAttribute* nodeTransAttr = nodeNode->AppendAttribute("position");
	nodeTransAttr->SetValue(trans);

	XmlAttribute* nodeRadiusAttr = nodeNode->AppendAttribute("bounding_radius");
	nodeRadiusAttr->SetValue(nodeData.boundRadius);

	XmlNode* modelNode = nodeNode->AppendChild("model");
	XmlAttribute* idAttr = modelNode->AppendAttribute("id");
	idAttr->SetValue((int32)nodeData.modelId);

	for (SceneNodeData& childData : nodeData.childData)
	{
		SaveSceneNodeData(childData, nodeNode, modelRootNode);
	}
}

void SceneImporter::SaveMaterialData(MaterialData& materialData, XmlNode* materialRootNode)
{
	XmlNode* materialNode = materialRootNode->AppendChild("material");

	//没有读取这个变量
	//materialData.techDefines;

	XmlAttribute* nameAttr = materialNode->AppendAttribute("name");
	nameAttr->SetValue(materialData.materialName.c_str());

	XmlAttribute* techniqueAttr = materialNode->AppendAttribute("technique");
	techniqueAttr->SetValue(materialData.techName.c_str());

	for (auto iter : materialData.varList)
	{
		XmlNode* varNode = materialNode->AppendChild("variable");
		XmlAttribute* varNameAttr = varNode->AppendAttribute("name");
		varNameAttr->SetValue(iter.first.c_str());

		XmlAttribute* varValueAttr = varNode->AppendAttribute("value");

		switch (iter.second.type)
		{	
			case EPT_STRING: varValueAttr->SetValue(iter.second.strValue.c_str()); break;
			case EPT_NUMBER: varValueAttr->SetValue(iter.second.fValue); break;
			case EPT_VECTOR2: varValueAttr->SetValue(iter.second.vt2Value); break;
			case EPT_VECTOR3: varValueAttr->SetValue(iter.second.vt3Value); break;
			case EPT_VECTOR4: varValueAttr->SetValue(iter.second.vt4Value); break;
			case EPT_MATRIX: varValueAttr->SetValue(iter.second.matValue); break;
		}		
	}

	for (auto iter : materialData.samplerDataList)
	{
		XmlNode* sampleNode = materialNode->AppendChild("sampler");

		XmlAttribute* sampleNameAttr = sampleNode->AppendAttribute("name");
		sampleNameAttr->SetValue(iter.second.samplerName.c_str());

		//没读取纹理过滤状态...
		//iter.second.stateDesc;

		XmlNode* samplePathNode = sampleNode->AppendChild("texture");
		XmlAttribute* samplePathAttr = samplePathNode->AppendAttribute("path");
		samplePathAttr->SetValue(iter.second.texPath.c_str());
	}	

	for (auto iter : materialData.stateList)
	{
		XmlNode* stateNode = materialNode->AppendChild("state");

		XmlAttribute* stateNameAttr = stateNode->AppendAttribute("name");
		stateNameAttr->SetValue(iter.first.c_str());

		XmlAttribute* stateValueAttr = stateNode->AppendAttribute("value");
		stateValueAttr->SetValue(iter.second.c_str());
	}
}

uint32 SceneImporter::SaveModelData(ModelData& modelData, XmlNode* modelRootNode)
{
	uint32 childNum = modelRootNode->GetChildNum();

	XmlNode* modelNode = modelRootNode->AppendChild("model");

	XmlAttribute* modelIdAttr = modelNode->AppendAttribute("id");
	modelIdAttr->SetValue((int32)childNum);

	XmlNode* meshChunkNode = modelNode->AppendChild("meshes_chunk");

	for (MeshData& meshData : modelData.meshDataList)
	{		
		XmlNode* meshNode = meshChunkNode->AppendChild("mesh");
		XmlAttribute* meshMtlNameAttr = meshNode->AppendAttribute("mtl_id");
		meshMtlNameAttr->SetValue(meshData.materialName.c_str());

		uint8* vertData = (uint8*)meshData.vertexData.GetPointer();

		uint8* tempVertData = vertData;
		uint32 vertDataOffset = 0;

		XmlNode* vertChunkNode = meshNode->AppendChild("vertices_chunk");

		while (vertDataOffset < meshData.vertexData.GetLength())
		{
			tempVertData = vertData + vertDataOffset;

			XmlNode* vertNode = vertChunkNode->AppendChild("vertex");

			for (VertLayoutDesc& desc : meshData.vertDesc)
			{
				if (desc.elementSemantic == "POSITION")
				{
					Vector3Df* pos = (Vector3Df*)(tempVertData + desc.elementOffset);

					XmlAttribute* posAttr = vertNode->AppendAttribute("v");
					posAttr->SetValue(*pos);

					vertDataOffset += sizeof(Vector3Df);
				}
				else if (desc.elementSemantic == "NORMAL")
				{
					Vector3Df* normal = (Vector3Df*)(tempVertData + desc.elementOffset);

					XmlNode* normalNode = vertNode->AppendChild("normal");
					XmlAttribute* normalAttr = normalNode->AppendAttribute("v");
					normalAttr->SetValue(*normal);

					vertDataOffset += sizeof(Vector3Df);
				}
				else if (desc.elementSemantic == "COLOR")
				{
					Vector4Df* color = (Vector4Df*)(tempVertData + desc.elementOffset);

					XmlNode* colorNode = vertNode->AppendChild("color");
					XmlAttribute* colorAttr = colorNode->AppendAttribute("v");
					colorAttr->SetValue(*color);

					vertDataOffset += sizeof(Vector4Df);
				}
				else if (desc.elementSemantic == "TEXCOORD")
				{
					Vector2Df* uv = (Vector2Df*)(tempVertData + desc.elementOffset);

					XmlNode* uvNode = vertNode->AppendChild("tex_coord");
					XmlAttribute* uvAttr = uvNode->AppendAttribute("v");
					uvAttr->SetValue(*uv);

					vertDataOffset += sizeof(Vector2Df);
				}
				else
				{
					assert(false);
				}
			}
		}
		
		uint32* indexData = (uint32*)meshData.indexData.GetPointer();
		uint32 triNum = meshData.indexData.GetLength() / sizeof(uint32) / 3;
		if (triNum > 0)
		{
			XmlNode* triChunkNode = meshNode->AppendChild("triangles_chunk");

			char8 temp[255];

			for (uint32 i = 0; i < triNum; i++)
			{
				XmlNode* triNode = triChunkNode->AppendChild("triangle");
				XmlAttribute* triAttr = triNode->AppendAttribute("index");

				sprintf(temp, "%d %d %d", indexData[i * 3], indexData[i * 3 + 1], indexData[i * 3 + 2]);
				triAttr->SetValue(temp);
			}
		}		
	}

	//暂时没有骨骼数据
// 	for (BoneInfo& bondInfo : modelData.boneInfoList)
// 	{
// 
// 	}
		
	//暂时没有动画数据
// 	for (SkelAnimation& skelAnim : modelData.skelAnimList)
// 	{
// 
// 	}

	return childNum;
}