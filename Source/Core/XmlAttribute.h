#pragma once

namespace Pharos
{
	namespace Core
	{
		class XmlDocument;
		class XmlNode;

		class XmlAttribute : public xml_attribute<char8>
		{
		public:
			XmlAttribute();
			virtual ~XmlAttribute();

			friend class XmlNode;

		public:
			virtual void SetName(const char8* szName);
			virtual const char8* GetName();

			virtual void SetValue(bool bValue);
			virtual void SetValue(const char8* szValue);
			virtual void SetValue(int32 lValue);
			virtual void SetValue(Color4 color);
			virtual void SetValue(float32 fValue);
			virtual void SetValue(const Vector2Df& value);
			virtual void SetValue(const Vector3Df& value);
			virtual void SetValue(const Vector4Df& value);

			virtual bool GetBoolValue();
			virtual const char8* GetStringValue();
			virtual int32 GetIntValue();
			virtual float32 GetFloatValue();
			virtual Color4 GetColorValue();
			virtual Vector2Df GetVector2DValue();
			virtual Vector3Df GetVector3DValue();
			virtual Vector4Df GetVector4DValue();

			virtual XmlNode* GetOwnerNode();
			virtual XmlDocument* GetOwnerDocument();
		};
	}
}