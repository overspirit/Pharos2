#pragma once

/* DataType */
#define GL_BYTE                                          0x1400
#define GL_UNSIGNED_BYTE                                 0x1401
#define GL_SHORT                                         0x1402
#define GL_UNSIGNED_SHORT                                0x1403
#define GL_INT                                           0x1404
#define GL_UNSIGNED_INT                                  0x1405
#define GL_FLOAT                                         0x1406
#define GL_FIXED                                         0x140C

/* BeginMode */
#define GL_POINTS                                        0x0000
#define GL_LINES                                         0x0001
#define GL_LINE_LOOP                                     0x0002
#define GL_LINE_STRIP                                    0x0003
#define GL_TRIANGLES                                     0x0004
#define GL_TRIANGLE_STRIP                                0x0005
#define GL_TRIANGLE_FAN                                  0x0006

#define MATH_TOLERANCE 2e-37f

namespace Pharos
{
	namespace Scene
	{
		class BundleSceneImporter : public SceneImporter
		{
		public:
			BundleSceneImporter(void);
			virtual ~BundleSceneImporter(void);

		private:
			struct Reference
			{
				string id;
				uint32 type;
				uint32 offset;
			};
			
			enum Usage
			{
				POSITION = 1,
				NORMAL = 2,
				COLOR = 3,
				TANGENT = 4,
				BINORMAL = 5,
				BLENDWEIGHTS = 6,
				BLENDINDICES = 7,
				TEXCOORD0 = 8,
				TEXCOORD1 = 9,
				TEXCOORD2 = 10,
				TEXCOORD3 = 11,
				TEXCOORD4 = 12,
				TEXCOORD5 = 13,
				TEXCOORD6 = 14,
				TEXCOORD7 = 15
			};

			struct Element
			{
				Usage usage;
				uint32 size;
			};

			/**
			* Defines supported index formats.
			*/
			enum IndexFormat
			{
				INDEX8 = GL_UNSIGNED_BYTE,
				INDEX16 = GL_UNSIGNED_SHORT,
				INDEX32 = GL_UNSIGNED_INT
			};

			/**
			* Defines supported primitive types.
			*/
			enum PrimitiveType
			{
				TRIANGLES = GL_TRIANGLES,
				TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
				LINES = GL_LINES,
				LINE_STRIP = GL_LINE_STRIP,
				POINTS = GL_POINTS
			};

			struct MaterialParameter
			{
				string name;
				string id;
				map<string, TypeValue>  paramList;
				vector<MaterialParameter> childList;
			};

		private:
			vector<Reference>		m_refList;		
			
			MaterialParameter		m_materialParam;

			string		m_bundlePath;

			
		private:
			string ReadString(File* bundleFile);

			bool Read(File* bundleFile, uint8* value);
			bool Read(File* bundleFile, uint32* value);
			bool Read(File* bundleFile, float32* value);

			Reference* FindReference(const char8* id);
			Reference* SeekTo(File* bundleFile, const char8* id, uint32 type);
			Reference* SeekToFirstType(File* bundleFile, uint32 type);

			bool ReadBundleHeader(File* bundleFile);
			bool ReadBundleScene(File* bundleFile);
			bool ReadBundleNode(File* bundleFile, SceneNodeData& data);
			bool ReadBundleCamera(File* bundleFile);
			bool ReadBundleLight(File* bundleFile);
			bool ReadBundleModel(File* bundleFile, SceneNodeData& data);
			bool ReadBundleMesh(File* bundleFile, const string& meshId, SceneNodeData& data);
			bool ReadBundleMeshSkin(File* bundleFile);
			bool ReadBundleMaterial(Properties* materialProp, MaterialParameter& param);

			uint32 originalVertexElementByteSize(Usage usage);
			uint32 compressedVertexElementByteSize(Usage usage);
			const char8* getVertexElementUsageName(Usage usage, uint32& semaIndex);

			const char8* GetIdFromOffset(File* file);

			void uncompressFixedPointBuffer(uint8* vertexData, uint8* zippedBuffer, uint32 vertexCount, Element* vertexElements, uint32 vertexElementCount);

		public:
			virtual bool LoadSceneFile(const char8* file);
			virtual bool LoadModelFile(const char8* file);
		};
	}
}