#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
#include "SceneGlobal.h"

// Minimum version numbers supported
#define BUNDLE_VERSION_MAJOR_REQUIRED   1 
#define BUNDLE_VERSION_MINOR_REQUIRED   2

#define BUNDLE_TYPE_SCENE               1
#define BUNDLE_TYPE_NODE                2
#define BUNDLE_TYPE_ANIMATIONS          3
#define BUNDLE_TYPE_ANIMATION           4
#define BUNDLE_TYPE_ANIMATION_CHANNEL   5
#define BUNDLE_TYPE_MODEL               10
#define BUNDLE_TYPE_MATERIAL            16
#define BUNDLE_TYPE_EFFECT              18
#define BUNDLE_TYPE_CAMERA              32
#define BUNDLE_TYPE_LIGHT               33
#define BUNDLE_TYPE_MESH                34
#define BUNDLE_TYPE_MESHPART            35
#define BUNDLE_TYPE_MESHSKIN            36
#define BUNDLE_TYPE_FONT                128

// For sanity checking string reads
#define BUNDLE_MAX_STRING_LENGTH        5000

#define BUNDLE_VERSION_MAJOR_FONT_FORMAT  1
#define BUNDLE_VERSION_MINOR_FONT_FORMAT  5

BundleSceneImporter::BundleSceneImporter()
{

}

BundleSceneImporter::~BundleSceneImporter(void)
{

}

bool BundleSceneImporter::LoadSceneFile(const char8* file)
{
	File bundleFile;// = g_pKernel->OpenFileStream(file);
	//if (bundleFile == nullptr) return false;
	if (!bundleFile.Open(file)) return false;

	if (!ReadBundleHeader(&bundleFile)) return false;
	if (!ReadBundleScene(&bundleFile)) return false;
	
	return true;
}

void BundleSceneImporter::FillSceneNodeList(vector<SceneNode*>& sceneNodeList)
{
	for (auto& sceneData : m_nodeDataList)
	{
		SceneNode* sceneNode = BuildSceneNode(sceneData, nullptr);
		sceneNodeList.push_back(sceneNode);
	}
}

SceneNode* BundleSceneImporter::BuildSceneNode(const SceneNodeData& data, SceneNode* parent)
{
	SceneNode* sceneNode = new SceneNode();
	sceneNode->InitNode(data.nodeName.c_str(), parent);
	sceneNode->SetLocalTransform(data.localTrans);

	if (data.modelData.meshDataList.size() > 1)
	{
		int test = 0;
	}

	for (auto& meshData : data.modelData.meshDataList)
	{
		sceneNode->InitModelData(meshData.vertexData, meshData.vertDesc, meshData.indexData);
	}

	for (auto& sceneData : data.childData)
	{
		SceneNode* childNode = BuildSceneNode(sceneData, sceneNode);
		sceneNode->AddChildNode(childNode);
	}

	return sceneNode;
}

bool BundleSceneImporter::LoadModelFile(const char8* file)
{
	return true;
}

bool BundleSceneImporter::ReadBundleHeader(File* bundleFile)
{
	// Read the GPB header info.
	char8 sig[9] = { 0 };
	if (bundleFile->Read(sig, 9) != 9 || memcmp(sig, "\xABGPB\xBB\r\n\x1A\n", 9) != 0) return false;

	// Read version.
	uint8 version[2] = { 0 };
	if (bundleFile->Read(version, 2) != 2) return false;

	// Read ref table.
	uint32 refCount = 0;
	if (!Read(bundleFile, &refCount)) return false;

	// Read all refs.
	m_refList.resize(refCount);
	for (unsigned int i = 0; i < refCount; ++i)
	{
		if ((m_refList[i].id = ReadString(bundleFile)).empty()
			|| !Read(bundleFile, &m_refList[i].type)
			|| !Read(bundleFile, &m_refList[i].offset))
		{
			return false;
		}
	}

	return true;
}

bool BundleSceneImporter::ReadBundleScene(File* bundleFile)
{
	Reference* ref = SeekToFirstType(bundleFile, BUNDLE_TYPE_SCENE);
	if (!ref) return false;

	uint32 childrenCount = 0;
	if (!Read(bundleFile, &childrenCount)) return false;

	if (childrenCount > 0)
	{
		m_nodeDataList.resize(childrenCount);

		Matrix4 mat;
		// Read each child directly into the scene.
		for (uint32 i = 0; i < childrenCount; i++)
		{
			if (!ReadBundleNode(bundleFile, m_nodeDataList[i])) return false;
		}
	}

	// Read active camera.
	string xref = ReadString(bundleFile);

	// Read ambient color.
	float32 red = 0, blue = 0, green = 0;
	if (!Read(bundleFile, &red)) return false;
	if (!Read(bundleFile, &blue)) return false;
	if (!Read(bundleFile, &green)) return false;

	for (uint32 i = 0; i < m_refList.size(); ++i)
	{
		Reference* ref = &m_refList[i];

		if (ref->type == BUNDLE_TYPE_ANIMATIONS)
		{
			// Found a match.
			if (!bundleFile->Seek(ref->offset, EFST_BEGIN))
			{
				return false;
			}

			//readAnimations(scene);
		}
	}

	//resolveJointReferences(scene, NULL);

	return true;
}

bool BundleSceneImporter::ReadBundleNode(File* bundleFile, SceneNodeData& data)
{	
	const char8* nodeName = GetIdFromOffset(bundleFile);
	data.nodeName = nodeName;

	// Read node type.
	uint32 nodeType = 0;
	if (!Read(bundleFile, &nodeType)) return false;

	// Read transform.
	Matrix4 transform;
	if (bundleFile->Read(&transform, sizeof(Matrix4)) != sizeof(Matrix4)) return false;
	
	data.localTrans = transform;

	// Skip the parent ID.
	string parentId = ReadString(bundleFile);
	data.parentName = parentId;

	// Read children.
	uint32 childrenCount = 0;
	if (!Read(bundleFile, &childrenCount)) return false;

	if (childrenCount > 0)
	{
		data.childData.resize(childrenCount);

		// Read each child.
		for (uint32 i = 0; i < childrenCount; i++)
		{
			ReadBundleNode(bundleFile, data.childData[i]);
		}
	}

	// Read camera.
	ReadBundleCamera(bundleFile);

	// Read light.
	ReadBundleLight(bundleFile);

	// Read model.
	if (!ReadBundleModel(bundleFile, data)) return false;

	return true;
}

bool BundleSceneImporter::ReadBundleModel(File* bundleFile, SceneNodeData& data)
{
	string xref = ReadString(bundleFile);
	if (xref.length() > 1 && xref[0] == '#') // TODO: Handle full xrefs
	{
		if (ReadBundleMesh(bundleFile, xref.c_str() + 1, data))
		{
			// Read skin.
			uint8 hasSkin = 0;
			if (!Read(bundleFile, &hasSkin)) return false;

			if (hasSkin)
			{
				ReadBundleMeshSkin(bundleFile);
			}

			// Read material.
			uint32 materialCount = 0;
			if (!Read(bundleFile, &materialCount)) return false;

			if (materialCount > 0)
			{
				for (uint32 i = 0; i < materialCount; ++i)
				{
					std::string materialName = ReadString(bundleFile);
					MeshData& meshData = *data.modelData.meshDataList.rbegin();
					//meshData.materialData = m_materialDataList[materialName];					
				}
			}
		}
	}

	return true;
}


bool BundleSceneImporter::ReadBundleMesh(File* bundleFile, const string& meshId, SceneNodeData& data)
{
	// Save the file position.
	uint32 position = bundleFile->Tell();

	// Seek to the specified mesh.
	Reference* ref = SeekTo(bundleFile, meshId.c_str(), BUNDLE_TYPE_MESH);
	if (ref == NULL) return false;


	// Read vertex format/elements.
	uint32 vertexElementCount = 0;
	if (!Read(bundleFile, &vertexElementCount)) return false;

	if (vertexElementCount < 1) return false;

	vector<Element> vertexElements(vertexElementCount);
	for (uint32 i = 0; i < vertexElementCount; ++i)
	{
		uint32 vUsage = 0, vSize = 0;
		if (!Read(bundleFile, &vUsage)) return false;
		if (!Read(bundleFile, &vSize)) return false;

		vertexElements[i].usage = (Usage)vUsage;
		vertexElements[i].size = vSize;                         //压缩前大小
	}

	MeshData meshData;

	uint32 vertEleOffset = 0;
	for (uint32 i = 0; i < vertexElementCount; i++)
	{
		uint32 semaIndex = 0;
		const char8* usageName = getVertexElementUsageName(vertexElements[i].usage, semaIndex);
		meshData.vertDesc.push_back({ VET_FLOAT32, vertexElements[i].size, usageName, semaIndex, vertEleOffset });
		vertEleOffset += originalVertexElementByteSize(vertexElements[i].usage);
	}



	// Read vertex data.
	uint32 vertexByteCount = 0;       // 每个vertex的大小 和encoder 写入文件的每定点长度一致（压缩后）
	if (!Read(bundleFile, &vertexByteCount)) return false;
	if (vertexByteCount == 0) return false;

	uint32 originalVertexSize = 0;
	uint32 compressedVertexSize = 0;
	for (uint32 i = 0; i < vertexElementCount; ++i)
	{
		originalVertexSize += originalVertexElementByteSize(vertexElements[i].usage);
		compressedVertexSize += compressedVertexElementByteSize(vertexElements[i].usage);
	}

	uint32 vertexCount = vertexByteCount / compressedVertexSize;
	//uint8* vertexData = new uint8[vertexCount * originalVertexSize];
	//uint8* compressedBuffer = new uint8[vertexByteCount];
	meshData.vertexData.Alloc(vertexCount * originalVertexSize);
	MemoryBuffer compressedBuffer(vertexByteCount);

	if (bundleFile->Read(compressedBuffer.GetPointer(), vertexByteCount) != vertexByteCount) return false;

	// uncompress data
	uncompressFixedPointBuffer((uint8*)meshData.vertexData.GetPointer(), (uint8*)compressedBuffer.GetPointer(), vertexCount, &*vertexElements.begin(), vertexElementCount);

// 	uint8* vertData = (uint8*)meshData.vertexData->GetPointer();
// 	for (uint32 i = 0; i < vertexCount; i++)
// 	{
// 		Vector3Df pos = *(Vector3Df*)vertData;
// 		//Vector3Df newPos = mat.TransformCoord(pos);
// 		*(Vector3Df*)vertData = Vector3Df(-pos.x, pos.y, pos.z);
// 
// 		vertData += originalVertexSize;
// 	}


	// Read mesh bounds (bounding box and bounding sphere).
	Vector3Df boundingBoxMin;
	Vector3Df boundingBoxMax;
	if (bundleFile->Read(&boundingBoxMin, sizeof(Vector3Df)) != sizeof(Vector3Df) || bundleFile->Read(&boundingBoxMax, sizeof(Vector3Df)) != sizeof(Vector3Df))
	{
		return false;
	}

	Vector3Df boundingSphereCenter;
	float32 boundingSphereRadius = 0;
	if (bundleFile->Read(&boundingSphereCenter, sizeof(Vector3Df)) != sizeof(Vector3Df) || !Read(bundleFile, &boundingSphereRadius))
	{
		return false;
	}

	data.boundRadius = boundingSphereRadius;

	// Read mesh parts.
	uint32 meshPartCount = 0;
	if (!Read(bundleFile, &meshPartCount)) return false;
	
	//meshData.indexData = g_pKernel->CreateMemBuffer();

	vector<MemoryBuffer>	indexDataBufList(meshPartCount);
	uint32	indexTotalCount = 0;

	for (uint32 i = 0; i < meshPartCount; ++i)
	{
		// Read primitive type, index format and index count.
		uint32 pType = 0, iFormat = 0, iByteCount = 0;
		if (!Read(bundleFile, &pType)) return false;
		if (!Read(bundleFile, &iFormat)) return false;
		if (!Read(bundleFile, &iByteCount)) return false;

		//partData->primitiveType = (Mesh::PrimitiveType)pType;
		//partData->indexFormat = (Mesh::IndexFormat)iFormat;

		uint32 indexSize = 0;
		switch (iFormat)
		{
		case INDEX8: indexSize = 1; break;
		case INDEX16: indexSize = 2; break;
		case INDEX32: indexSize = 4; break;
		default: return false;
		} 

		uint32 indexCount = iByteCount / indexSize;

		indexDataBufList[i].Alloc(indexCount * sizeof(uint32));
		uint32* indexData = (uint32*)indexDataBufList[i].GetPointer();

		for (uint32 j = 0; j < indexCount; j++)
		{
			uint32 index = 0;
			bundleFile->Read(&index, indexSize);
			indexData[j] = index;
		}

		indexTotalCount += indexCount;

		//uint8* indexData = new uint8[iByteCount];
		//if (bundleFile->Read(indexData, iByteCount) != iByteCount)
		//{
			//return false;
		//}
	}


	meshData.indexData.Alloc(indexTotalCount * sizeof(uint32));
	uint32 copyIndexDataOffset = 0;
	for (uint32 i = 0; i < meshPartCount; i++)
	{
		meshData.indexData.Insert(copyIndexDataOffset, indexDataBufList[i]);
		copyIndexDataOffset += indexDataBufList[i].GetLength();
	}

	//meshData.drawType = Render::EDT_TRIANGLELIST;

	// Restore file pointer.
	if (!bundleFile->Seek(position, EFST_BEGIN)) return false;


	data.modelData.meshDataList.push_back(meshData);

	return true;
}

bool BundleSceneImporter::ReadBundleMeshSkin(File* bundleFile)
{	
	// Read bindShape.
	Matrix4 bindShape;
	if (bundleFile->Read(&bindShape, sizeof(Matrix4)) != sizeof(Matrix4)) return false;

	// Read joint count.
	uint32 jointCount = 0;
	if (!Read(bundleFile, &jointCount)) return false;

	if (jointCount == 0) return false;

	// Read joint xref strings for all joints in the list.
	for (uint32 i = 0; i < jointCount; i++)
	{
		string jointName = ReadString(bundleFile);
	}

	// Read bind poses.
	uint32 jointsBindPosesCount = 0;
	if (!Read(bundleFile, &jointsBindPosesCount)) return false;

	if (jointsBindPosesCount > 0)
	{
		Matrix4 m;
		for (uint32 i = 0; i < jointCount; i++)
		{
			if (bundleFile->Read(&m, sizeof(Matrix4)) != sizeof(Matrix4)) return false;
		}
	}
	
	return true;
}


bool BundleSceneImporter::ReadBundleCamera(File* bundleFile)
{
	uint8 cameraType = 0;
	if (!Read(bundleFile, &cameraType)) return false;

	// Check if there isn't a camera to load.
	if (cameraType == 0) return false;

	float32 aspectRatio = 0;
	if (!Read(bundleFile, &aspectRatio)) return false;

	float32 nearPlane = 0;
	if (!Read(bundleFile, &nearPlane)) return false;

	float32 farPlane = 0;
	if (!Read(bundleFile, &farPlane)) return false;

	if (cameraType == 1)
	{
		float32 fieldOfView = 0;
		if (!Read(bundleFile, &fieldOfView)) return false;
	}
	else if (cameraType == 2)
	{
		float32 zoomX = 0;
		if (!Read(bundleFile, &zoomX)) return false;

		float32 zoomY = 0;
		if (!Read(bundleFile, &zoomY)) return false;
	}
	else
	{
		return false;
	}

	return true;
}

bool BundleSceneImporter::ReadBundleLight(File* bundleFile)
{
	uint8 type = 0;
	if (!Read(bundleFile, &type)) return false;

	// Check if there isn't a light to load.
	if (type == 0) return false;

	// Read color.
	float32 red = 0, blue = 0, green = 0;
	if (!Read(bundleFile, &red) || !Read(bundleFile, &blue) || !Read(bundleFile, &green))
	{
		return false;
	}

	if (type == 1)
	{

	}
	else if (type == 2)
	{
		float32 range = 0;
		if (!Read(bundleFile, &range)) return false;
	}
	else if (type == 3)
	{
		float32 range = 0, innerAngle = 0, outerAngle = 0;
		if (!Read(bundleFile, &range)) return false;
		if (!Read(bundleFile, &innerAngle)) return false;
		if (!Read(bundleFile, &outerAngle)) return false;
	}
	else
	{
		return false;
	}

	return true;
}

string BundleSceneImporter::ReadString(File* bundleFile)
{
	uint32 length = 0;
	if (bundleFile->Read(&length, sizeof(uint32)) != sizeof(uint32))
	{
		return "";
	}

	std::string str;
	if (length > 0)
	{
		str.resize(length);
		if (bundleFile->Read(&str[0], length) != length)
		{
			return "";
		}
	}

	return str;
}

bool BundleSceneImporter::Read(File* bundleFile, uint8* value)
{
	if (bundleFile->Read(value, 1) != 1) return false;
	return true;
}
bool BundleSceneImporter::Read(File* bundleFile, uint32* value)
{
	if (bundleFile->Read(value, 4) != 4) return false;
	return true;
}

bool BundleSceneImporter::Read(File* bundleFile, float32* value)
{
	if (bundleFile->Read(value, 4) != 4) return false;
	return true;
}

BundleSceneImporter::Reference* BundleSceneImporter::FindReference(const char8* id)
{
	for (auto& ref : m_refList)
	{
		if (ref.id == id)
		{
			return &ref;
		}
	}

	return NULL;
}

BundleSceneImporter::Reference* BundleSceneImporter::SeekTo(File* bundleFile, const char8* id, uint32 type)
{
	Reference* ref = FindReference(id);
	if (ref == NULL)
	{
		return NULL;
	}

	if (ref->type != type)
	{
		return NULL;
	}

	if (!bundleFile->Seek(ref->offset, EFST_BEGIN))
	{
		return NULL;
	}

	return ref;
}

BundleSceneImporter::Reference* BundleSceneImporter::SeekToFirstType(File* bundleFile, uint32 type)
{
	for (uint32 i = 0; i < m_refList.size(); i++)
	{
		Reference* ref = &m_refList[i];
		if (ref->type == type)
		{
			if (!bundleFile->Seek(ref->offset, EFST_BEGIN))
			{
				return NULL;
			}
			return ref;
		}
	}
	return NULL;
}

const char8* BundleSceneImporter::GetIdFromOffset(File* file)
{
	uint32 pos = file->Tell();
	for (auto& ref : m_refList)
	{
		if (ref.offset == pos)
		{
			return ref.id.c_str();
		}
	}

	return "";
}

//**************************************************************//
//
// 定点数压缩转换对应表
//      1 f = sizeof(float32)
//      1 fixed point = sizeof(short)
//
//      |     TYPE      | ORIGIN    |   FILE(decimal bits)   |
//        POSITION = 1, |   3f      |     4byte * 3
//        NORMAL = 2,       3f            1byte * 2
//        COLOR = 3,        4f            1byte * 4
//        TANGENT = 4,      3f            1byte * 2
//        BINORMAL = 5,     3f            1byte * 2
//        BLENDWEIGHTS = 6, 4f            1byte * 4
//        BLENDINDICES = 7, 4f             2byte
//        TEXCOORD0 = 8,    2f            1byte * 2
//        TEXCOORD1 = 9,    ...          ...
//        TEXCOORD2 = 10,
//        TEXCOORD3 = 11,
//        TEXCOORD4 = 12,
//        TEXCOORD5 = 13,
//        TEXCOORD6 = 14,
//        TEXCOORD7 = 15
//
//**************************************************************//
//

typedef short _FIXED_POINT_BASE_TYPE_;

uint32 BundleSceneImporter::originalVertexElementByteSize(Usage usage)
{
	uint32 count = 0;
	switch (usage)
	{
	case Usage::POSITION:
		count = 3;
		break;
	case Usage::NORMAL:
	case Usage::TANGENT:
	case Usage::BINORMAL:
		count = 3;
		break;
	case Usage::COLOR:
	case Usage::BLENDWEIGHTS:
	case Usage::BLENDINDICES:
		count = 4;
		break;
	default: // VertexFormat::Usage::TEXCOORD ..
		count = 2;
		break;
	}
	return count * sizeof(float32);
}

uint32 BundleSceneImporter::compressedVertexElementByteSize(Usage usage)
{
	uint32 count = 0;
	switch (usage)
	{
	case Usage::POSITION:
		count = 3;
		return count * sizeof(float32);
		break;
	case Usage::NORMAL:
	case Usage::TANGENT:
	case Usage::BINORMAL:
		count = 3;
		break;
	case Usage::COLOR:
		count = 4; //4byte for color

	case Usage::BLENDWEIGHTS:
		count = 4;
	case Usage::BLENDINDICES:
		count = 4;
		break;
	default: // VertexFormat::Usage::TEXCOORD ..
		count = 3;
		break;
	}
	return count * sizeof(unsigned char);
}

const char8* BundleSceneImporter::getVertexElementUsageName(Usage usage, uint32& semaIndex)
{
	semaIndex = 0;

	switch (usage)
	{
	case POSITION: return "POSITION";
	case NORMAL: return "NORMAL";
	case COLOR: return "COLOR";
	case TANGENT: return "TANGENT";
	case BINORMAL: return "BINORMAL";
	case BLENDWEIGHTS: return "WEIGHT";
	case BLENDINDICES: return "INDICES";
	case TEXCOORD0: semaIndex = 0; return "TEXCOORD";
	case TEXCOORD1: semaIndex = 1; return "TEXCOORD";
	case TEXCOORD2: semaIndex = 2; return "TEXCOORD";
	case TEXCOORD3: semaIndex = 3; return "TEXCOORD";
	case TEXCOORD4: semaIndex = 4; return "TEXCOORD";
	case TEXCOORD5: semaIndex = 5; return "TEXCOORD";
	case TEXCOORD6: semaIndex = 6; return "TEXCOORD";
	case TEXCOORD7: semaIndex = 7; return "TEXCOORD";
	}

	return "";
}

inline float32 _fp2fHelper(_FIXED_POINT_BASE_TYPE_ value, int32 decimalBitCount)
{
	//GP_ASSERT(sizeof(_FIXED_POINT_BASE_TYPE_) * 8 >= decimalBitCount);
	//        int intBitCount = sizeof(_FIXED_POINT_BASE_TYPE_) * 8 - 1 - decimalBitCount;

	_FIXED_POINT_BASE_TYPE_ f2fpFactor = 1 << decimalBitCount;
	float32 ret = static_cast<float32>(value) / static_cast<float32>(f2fpFactor);
	return ret;
}

inline void sphereMapVec2ToVec3(float32 x, float32 y, float32* outX, float32* outY, float32* outZ)
{
	Vector2Df decodeNormal2D;
	decodeNormal2D.x = x * 2.0f - 1.0f;
	decodeNormal2D.y = y * 2.0f - 1.0f;

	float32 z = decodeNormal2D.DotProduct(decodeNormal2D);

	if (z < MATH_TOLERANCE) // if n is close to zero, set normal = (0,1,0)
	{
		*outX = 0.0f;
		*outY = 1.0f;
		*outZ = 0.0f;
		return;
	}
	z = z * 2.0f - 1.0f;

	float32 s = sqrt(Math::maximum(1.0f - z * z, 0.0f));

	Vector3Df decodeNormal;
	decodeNormal.x = decodeNormal2D.x * s;
	decodeNormal.y = decodeNormal2D.y * s;
	decodeNormal.z = z;
	decodeNormal.Normalize();

	*outX = decodeNormal.x;
	*outY = decodeNormal.y;
	*outZ = decodeNormal.z;
}

inline float32 readFloatBinary8Bit(uint8* const buf)
{
	return static_cast<float32>(*buf) / 255.0f;
}

inline float32 readFloatBinary8BitSigned(uint8* const buf)
{
	return static_cast<float32>(*buf) / 127.0f;
}

inline float32 readFloatBinary16Bit(uint8* const buf)
{
	return static_cast<float32>(*reinterpret_cast<unsigned short*>(buf)) / static_cast<float32>(((1 << 14) - 3));
}

inline void readVector2Binary12Bit(uint8* buf, float32* outX, float32* outY)
{
	uint32 data1 = *buf++;
	uint32 data2 = *buf++;
	uint32 data3 = *buf++;

	uint32 x = ((data1 & 0xFF) << 4) | ((data2 & 0xF0) >> 4);
	uint32 y = ((data2 & 0xF) << 8) | (data3 & 0xFF);

	*outX = static_cast<float32>(x) / 4095.0f;
	*outY = static_cast<float32>(y) / 4095.0f;

	return;
}

inline float32 readFixedPoint(uint8* const buf, int32 decimalCount)
{
	return _fp2fHelper(*(reinterpret_cast<_FIXED_POINT_BASE_TYPE_ *>(buf)), decimalCount);
}

inline float32 regular(float32 f)
{
	if (f < 0.0f) return 0.0f;
	if (f > 1.0f) return 1.0f;
	return f;
}

void BundleSceneImporter::uncompressFixedPointBuffer(uint8* vertexData, uint8* zippedBuffer, uint32 vertexCount, Element* vertexElements, uint32 vertexElementCount)
{
	float32 *outFloatData = reinterpret_cast<float32*>(vertexData);
	uint8* compressedBuffer = zippedBuffer;

	for (uint32 i = 0; i < vertexCount; ++i)
	{
		// 在encoder代码里，vertexElements的顺序，是符合顶点元素布局顺序的
		for (uint32 j = 0; j < vertexElementCount; ++j)
		{
			switch (vertexElements[j].usage)
			{
			case Usage::POSITION:
			{
				// 3 float32 position (not compressed)
				float32* posData = reinterpret_cast<float32*>(compressedBuffer);
				*outFloatData = *posData;
				*(outFloatData + 1) = *(posData + 1);
				*(outFloatData + 2) = *(posData + 2);

				break;
			}

			case Usage::NORMAL:
			case Usage::TANGENT:
			case Usage::BINORMAL:
			{
				*(outFloatData) = readFloatBinary8Bit(compressedBuffer) * 2.0f - 1.0f;
				*(outFloatData + 1) = readFloatBinary8Bit(compressedBuffer + 1) * 2.0f - 1.0f;
				*(outFloatData + 2) = readFloatBinary8Bit(compressedBuffer + 2) * 2.0f - 1.0f;

				break;
			}
			case Usage::COLOR:
			{
				// 4 byte color
				*outFloatData = readFloatBinary8Bit(compressedBuffer);
				*(outFloatData + 1) = regular(readFloatBinary8Bit(compressedBuffer + 1));
				*(outFloatData + 2) = regular(readFloatBinary8Bit(compressedBuffer + 2));
				*(outFloatData + 3) = regular(readFloatBinary8Bit(compressedBuffer + 3));

				break;
			}
			case Usage::BLENDWEIGHTS:
			{
				*outFloatData = (readFloatBinary8Bit(compressedBuffer));
				*(outFloatData + 1) = (readFloatBinary8Bit(compressedBuffer + 1));
				*(outFloatData + 2) = (readFloatBinary8Bit(compressedBuffer + 2));
				*(outFloatData + 3) = (readFloatBinary8Bit(compressedBuffer + 3));

				break;
			}
			case Usage::BLENDINDICES:
			{
				*outFloatData = (readFloatBinary8Bit(compressedBuffer));
				*(outFloatData + 1) = (readFloatBinary8Bit(compressedBuffer + 1));
				*(outFloatData + 2) = (readFloatBinary8Bit(compressedBuffer + 2));
				*(outFloatData + 3) = (readFloatBinary8Bit(compressedBuffer + 3));

				break;
			}
			default: // VertexFormat::Usage::TEXCOORD ..
			{
				float32 x, y;
				readVector2Binary12Bit(compressedBuffer, &x, &y);
				*outFloatData = x;
				*(outFloatData + 1) = y;

				break;
			}
			}

			compressedBuffer += compressedVertexElementByteSize(vertexElements[j].usage);
			outFloatData += originalVertexElementByteSize(vertexElements[j].usage) / sizeof(float32);
		}
	}
}