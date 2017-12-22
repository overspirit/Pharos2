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

		class Properties
		{
		public:
			Properties();
			Properties(File* file);
			Properties(const Properties& copy);
			Properties(File* file, const char8* name, const char8* id, const char8* parentID, Properties* parent);
			~Properties();

		private:
			struct Property
			{
				std::string name;
				std::string value;
				Property(const char8* name, const char8* value) : name(name), value(value) { }
			};

		private:
			std::string			_namespace;
			std::string			_id;
			std::string			_parentID;

			std::vector<Property>				_properties;
			std::vector<Property>::iterator		_propertiesItr;

			std::vector<Properties*>					_namespaces;
			std::vector<Properties*>::const_iterator	_namespacesItr;

			std::vector<Property>*		_variables;

			bool			_visited;

			Properties*		_parent;

		private:
			void readProperties(File* file);
			char8* readLine(File* file, char8* str, int32 num);
			void skipWhiteSpace(File* file);
			char8* trimWhiteSpace(char8* str);
			void mergeWith(Properties* overrides);
			void resolveInheritance(const char8* id = NULL);

			static char8 readChar(File* file);
			static bool isStringNumeric(const char8* str);
			static bool isVariable(const char8* str, char8* outName, size_t outSize);

			Properties* clone();

			void rewind();

			static bool parseVector2(const char8* str, Vector2Df* out);
			static bool parseVector3(const char8* str, Vector3Df* out);
			static bool parseVector4(const char8* str, Vector4Df* out);
			static bool parseAxisAngle(const char8* str, Quaternion* out);
			static bool parseColor(const char8* str, Color4* out);

		public:
			bool Open(const char8* path);
						
			bool Exists(const char8* name) const;

			virtual uint32 GetPropertyCount() const;
			virtual const char8* GetPropertyName(uint32 index) const;
			virtual const char8* GetPropertyValue(uint32 index) const;
			virtual const char8* GetPropertyValue(const char8* name) const;
			virtual Properties* GetNextNamespace();
			virtual Properties* GetNamespace(const char8* id, bool searchNames = false) const;

			virtual const char8* GetName() const;
			const char8* GetId() const;
			PropType GetType(const char8* name = NULL) const;

			const char8* GetString(const char8* name = NULL, const char8* defaultValue = NULL) const;
			bool SetString(const char8* name, const char8* value);
			bool GetBool(const char8* name = NULL, bool defaultValue = false) const;
			int32 GetInt(const char8* name = NULL) const;
			float32 GetFloat(const char8* name = NULL) const;
			long GetLong(const char8* name = NULL) const;
			bool GetMatrix(const char8* name, Matrix4* out) const;
			bool GetVector2(const char8* name, Vector2Df* out) const;
			bool GetVector3(const char8* name, Vector3Df* out) const;
			bool GetVector4(const char8* name, Vector4Df* out) const;
			bool GetQuaternionFromAxisAngle(const char8* name, Quaternion* out) const;
			bool GetColor(const char8* name, Color4* out) const;
			const char8* GetVariable(const char8* name, const char8* defaultValue = NULL) const;
			void SetVariable(const char8* name, const char8* value);
		};
	}
}