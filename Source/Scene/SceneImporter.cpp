#include "PreCompile.h"
#include "Pharos.h"

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

Model* SceneImporter::GenerateModel(const char* name)
{
    return this->CreateModel(m_modelDataList[name]);
}

SceneNode* SceneImporter::CreateNode(const SceneNodeData& data)
{
	SceneNode* parentNode = sSceneMgr->GetSceneNode(data.parentName.c_str());

	SceneNode* sceneNode = sSceneMgr->CreateSceneNode(data.nodeName.c_str(), parentNode);
    if (sceneNode == nullptr) return NULL;
    
	sceneNode->SetLocalTransform(data.localTrans);
	sceneNode->SetBoundingRadius(data.boundRadius);

	for (auto& childData : data.childData)
	{
		SceneNode* child = this->CreateNode(childData);
		if (child != nullptr) sceneNode->AddChildNode(child);
	}

	for (auto& modelId : data.modelIdList)
	{
		Model* model = GenerateModel(modelId.c_str());
		sceneNode->AddModel(model);
	}

	return sceneNode;
}

Model* SceneImporter::CreateModel(const ModelData& modelData)
{
    Model* model = sRenderHelper->GenerateModel(modelData.name.c_str());
	
 	for (const ModelData::MeshInfo& meshInfo : modelData.meshList)
 	{
		MeshData& meshData = m_meshDataList[meshInfo.id];
		Mesh* mesh = this->CreateMesh(meshData);
		if (mesh != nullptr)
		{
			uint32 subModelIndex = model->AddSubModelMesh(mesh);

			for (const string& materialName : meshInfo.materialList)
			{
				Material* material = this->CreateMaterial(m_materialDataList[materialName]);
				if (material != nullptr) model->AddSubModelMaterial(subModelIndex, material);
			}
		}
 	}

	for (const string& skelName : modelData.skeletonList)
	{
		SkeletonData& skelData = m_skeletonDataList[skelName];

		for (const BoneInfo& boneInfo : skelData.boneInfoList)
		{
			model->SetBoneInfo(boneInfo.name.c_str(), boneInfo.id, boneInfo.parentId, boneInfo.bindPose);
		}
	}

	for (const string& animName : modelData.animList)
	{
		SkelAnimation& modelSkelAnim = model->AddSkelAnimation(animName.c_str());
		modelSkelAnim = m_animDataList[animName];
	}

	if (modelData.animList.size() > 0)
	{
		model->SetCurrentAnimation(modelData.animList[0].c_str());
	}

	return model;
}

Mesh* SceneImporter::CreateMesh(const MeshData& meshData)
{
    Mesh* mesh = sRenderHelper->GenerateMesh(meshData.meshName.c_str());

    mesh->SetMeshVertexDesc(meshData.vertDesc);
	mesh->SetMeshVertexData((MemoryBuffer*)&meshData.vertexData, meshData.vertCount);
	mesh->SetMeshIndexData((MemoryBuffer*)&meshData.indexData, meshData.faceCount);
	mesh->SetDrawType(meshData.drawType);
	
	return mesh;
}

Material* SceneImporter::CreateMaterial(const MaterialData& materialData)
{
	Renderer* renderer = sRenderMgr->GetCurrentRenderer();

	Material* material = sRenderHelper->GenerateMaterial(materialData.techName.c_str(), materialData.materialName.c_str());
    if(material == NULL) return NULL;
    
    material->SetLightDirectionParamValue(materialData.lightDir);
    material->SetLightColorParamValue(materialData.lightColor);
    
    material->SetMaterialColorParamValue(materialData.mateialColor);
    material->SetAbmbinetColorParamValue(materialData.ambientColor);
    material->SetDiffuseColorParamValue(materialData.diffuseColor);
    material->SetSpecularColorParamValue(materialData.specularColor);
    material->SetAbmbinetRatioParamValue(materialData.ambientAlbedo);
    material->SetDiffuseRatioParamValue(materialData.diffuseAlbedo);
    material->SetSpecularRatioParamValue(materialData.specularAlbedo);
    material->SetSpecularPowParamValue(materialData.specularPow);
    
    material->SetTransparentEnabled(materialData.transparent);
    material->SetForceDepthWrite(materialData.forceDepth);
    material->SetCullBackFace(materialData.cullBack);
    material->SetClockwiseFrontFace(materialData.clockWise);

	return material;
}

PropType SceneImporter::GetStringPropType(const char8* str)
{
	uint32 commaCount = 0;
	char8* valuePtr = const_cast<char8*>(str);
	while ((valuePtr = strchr(valuePtr, '\x20')))
	{
		valuePtr++;
		commaCount++;
	}

	switch (commaCount)
	{
		case 0: return isStringNumeric(str) ? EPT_NUMBER : EPT_STRING;
		case 1: return EPT_VECTOR2;
		case 2: return EPT_VECTOR3;
		case 3: return EPT_VECTOR4;
		case 15: return EPT_MATRIX;
		default: return EPT_STRING;
	}

	return EPT_NONE;
}

bool SceneImporter::isStringNumeric(const char8* str)
{
	if (*str == '-')
		str++;

	if (!isdigit(*str)) return false;

	str++;

	uint32 decimalCount = 0;
	while (*str)
	{
		if (!isdigit(*str))
		{
			if (*str == '.' && decimalCount == 0)
			{
				decimalCount++;
			}
			else
			{
				return false;
			}
		}
		str++;
	}

	return true;
}

Vector2Df SceneImporter::ParseVector2(const char8* str)
{
	Vector2Df out;

	if (str != nullptr)
	{
		char8* data = (char8*)str;
		out.x = strtof(data, &data);
		out.y = strtof(data, &data);
	}

	return out;
}

Vector3Df SceneImporter::ParseVector3(const char8* str)
{
	Vector3Df out;

	if (str != nullptr)
	{
		char8* data = (char8*)str;
		out.x = strtof(data, &data);
		out.y = strtof(data, &data);
		out.z = strtof(data, &data);
	}

	return out;
}

Vector4Df SceneImporter::ParseVector4(const char8* str)
{
	Vector4Df out;

	if (str != nullptr)
	{
		char8* data = (char8*)str;
		out.x = strtof(data, &data);
		out.y = strtof(data, &data);
		out.z = strtof(data, &data);
		out.w = strtof(data, &data);
	}

	return out;
}

Matrix4 SceneImporter::ParseMatrix4(const char8* str)
{
	Matrix4 out;

	if (str != nullptr)
	{
		char8* data = (char8*)str;

		for (int32 i = 0; i < 16; i++)
		{
			out.m[i] = strtof(data, &data);
		}
	}

	return out;
}

bool SceneImporter::SaveTo(const char8* file)
{
	XmlDocument* doc = (XmlDocument*)sResMgr->CreateResource(ERT_XML, file);
	if (doc == NULL) return false;

	XmlNode* rootNode = doc->AppendRootNode("Scene");
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

	doc->Save();

	return true;
}

void SceneImporter::SaveSceneNodeData(SceneNodeData& nodeData, XmlNode* parentNode, XmlNode* modelRootNode)
{
	XmlNode* nodeNode = parentNode->AppendChild("node");

	XmlAttribute* nodeNameAttr = nodeNode->AppendAttribute("name");
	nodeNameAttr->SetValue(nodeData.nodeName.c_str());

	XmlAttribute* nodeRotaAttr = nodeNode->AppendAttribute("transform");
	nodeRotaAttr->SetValue(nodeData.localTrans);

	XmlAttribute* nodeRadiusAttr = nodeNode->AppendAttribute("bounding_radius");
	nodeRadiusAttr->SetValue(nodeData.boundRadius);

	for(const string& modelId : nodeData.modelIdList)
	{
		XmlNode* modelNode = nodeNode->AppendChild("item");

		XmlAttribute* typeAttr = modelNode->AppendAttribute("type");
		typeAttr->SetValue("model");

		XmlAttribute* idAttr = modelNode->AppendAttribute("id");
		idAttr->SetValue(modelId.c_str());
	}

	for (SceneNodeData& childData : nodeData.childData)
	{
		SaveSceneNodeData(childData, nodeNode, modelRootNode);
	}
}

void SceneImporter::SaveMaterialData(MaterialData& materialData, XmlNode* materialRootNode)
{
	XmlNode* materialNode = materialRootNode->AppendChild("material");

}

uint32 SceneImporter::SaveModelData(ModelData& modelData, XmlNode* modelRootNode)
{
	uint32 childNum = modelRootNode->GetChildNum();
	/*
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
*/
	return childNum;
}
