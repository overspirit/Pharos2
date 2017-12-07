#pragma once

namespace Pharos
{
	namespace Core
	{
		enum PropType
		{
			EPT_NONE,
			EPT_STRING,
			EPT_NUMBER,
			EPT_VECTOR2,
			EPT_VECTOR3,
			EPT_VECTOR4,
			EPT_MATRIX

		};
		class Properties : public ResBase//< IProperties >, public enable_shared_from_this< Properties >
		{
		public:
			Properties();	
			virtual ~Properties();

		private:
			struct Property
			{
				Property(const char8* name, const char8* value)
				{
					this->name = name;
					this->value = value;
				}

				std::string name;
				std::string value;				
			};

		private:
			std::string _namespace;
			std::string _id;
			std::string _parentID;
			std::vector<Property> _properties;
			//std::list<Property>::iterator _propertiesItr;

			std::vector< Properties* > _namespaces;
			std::vector< Properties* >::const_iterator _namespacesItr;

			std::vector<Property> _variables;
			bool				_visited;
			Properties*			_parent;

		private:
			void readProperties(File* file);
			void skipWhiteSpace(File* file);
			char8* trimWhiteSpace(char8* str);
			void mergeWith(Properties* overrides);
			void resolveInheritance(const char8* id = NULL);

			Properties* clone();

			static bool isStringNumeric(const char8* str);
			static bool isVariable(const char8* str, char8* outName, size_t outSize);
			static char8 readChar(File* file);
			static char8* readLine(File* file, char8* str, int32 num);

			static bool parseVector2(const char8* str, Vector2Df* out);
			static bool parseVector3(const char8* str, Vector3Df* out);
			static bool parseVector4(const char8* str, Vector4Df* out);
			static bool parseAxisAngle(const char8* str, Quaternion* out);
			static bool parseColor(const char8* str, Color4* out);

			void rewind();

		public:
			virtual bool Open(const char8* path);			
			virtual bool Init(File* file, const char8* name, const char8* id, const char8* parentID, Properties* parent);
			
			virtual bool Exists(const char8* name) const;

			virtual uint32 GetPropertyCount() const;
			virtual const char8* GetPropertyName(uint32 index) const;
			virtual const char8* GetPropertyValue(uint32 index) const;
			virtual const char8* GetPropertyValue(const char8* name) const;
			virtual Properties* GetNextNamespace();
			virtual Properties* GetNamespace(const char8* id, bool searchNames = false) const;

			virtual const char8* GetName() const;
			virtual const char8* GetId() const;
			virtual PropType GetType(const char8* name = NULL) const;

			virtual const char8* GetString(const char8* name = NULL, const char8* defaultValue = NULL) const;
			virtual bool SetString(const char8* name, const char8* value);
			virtual bool GetBool(const char8* name = NULL, bool defaultValue = false) const;
			virtual int32 GetInt(const char8* name = NULL) const;
			virtual float32 GetFloat(const char8* name = NULL) const;
			virtual long GetLong(const char8* name = NULL) const;
			virtual bool GetMatrix(const char8* name, Matrix4* out) const;
			virtual bool GetVector2(const char8* name, Vector2Df* out) const;
			virtual bool GetVector3(const char8* name, Vector3Df* out) const;
			virtual bool GetVector4(const char8* name, Vector4Df* out) const;
			virtual bool GetQuaternionFromAxisAngle(const char8* name, Quaternion* out) const;
			virtual bool GetColor(const char8* name, Color4* out) const;
			virtual const char8* GetVariable(const char8* name, const char8* defaultValue = NULL) const;
			virtual void SetVariable(const char8* name, const char8* value);
		};
	}
}