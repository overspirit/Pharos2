#include "PreCompile.h"
#include "Pharos.h"

XmlAttribute::XmlAttribute()
{
}

XmlAttribute::~XmlAttribute()
{

}

void XmlAttribute::SetName(const char8* szName)
{
	uint32 len = (uint32)Utils::strlen_utf8(szName);
	char8* pName = document()->allocate_string(nullptr, len + 1);
	strcpy(pName, szName);
	pName[len] = 0;
	name(pName, len);
}

const char8* XmlAttribute::GetName()
{
	return name();
}

void XmlAttribute::SetValue(bool bValue)
{
	if (bValue) value("true", 4);
	else value("false", 5);
}

void XmlAttribute::SetValue(const char8* szValue)
{
	uint32 len = (uint32)Utils::strlen_utf8(szValue);
	char8* pValue = document()->allocate_string(nullptr, len + 1);
	strcpy(pValue, szValue);
	pValue[len] = 0;
	value(pValue, len);
}

void XmlAttribute::SetValue(int32 lValue)
{
	char* pValue = document()->allocate_string(nullptr, 16);
	sprintf(pValue, "%d", lValue);
	uint32 ret = (uint32)Utils::strlen_utf8(pValue);
	value(pValue, ret);
}

void XmlAttribute::SetValue(Color4 color)
{
	char* pValue = document()->allocate_string(nullptr, 16);
	sprintf(pValue, "0x%X", color);
	uint32 ret = (uint32)Utils::strlen_utf8(pValue);
	value(pValue, ret);
}

void XmlAttribute::SetValue(float32 fValue)
{
	char* pValue = document()->allocate_string(nullptr, 16);
	sprintf(pValue, "%f", fValue);
	uint32 ret = (uint32)Utils::strlen_utf8(pValue);
	value(pValue, ret);
}

void XmlAttribute::SetValue(const Vector2Df& vt)
{
	char* pValue = document()->allocate_string(nullptr, 32);
	sprintf(pValue, "%f %f", vt.x, vt.y);
	uint32 ret = (uint32)Utils::strlen_utf8(pValue);
	value(pValue, ret);
}

void XmlAttribute::SetValue(const Vector3Df& vt)
{
	char* pValue = document()->allocate_string(nullptr, 48);
	sprintf(pValue, "%f %f %f", vt.x, vt.y, vt.z);
	uint32 ret = (uint32)Utils::strlen_utf8(pValue);
	value(pValue, ret);
}

void XmlAttribute::SetValue(const Vector4Df& vt)
{
	char* pValue = document()->allocate_string(nullptr, 64);
	sprintf(pValue, "%f %f %f %f", vt.x, vt.y, vt.z, vt.w);
	uint32 ret = (uint32)Utils::strlen_utf8(pValue);
	value(pValue, ret);
}

void XmlAttribute::SetValue(const Matrix4& mat)
{
	char* pValue = document()->allocate_string(nullptr, 256);
	sprintf(pValue, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f", 
		mat.m[0], mat.m[1], mat.m[2], mat.m[3],
		mat.m[4], mat.m[5], mat.m[6], mat.m[7],
		mat.m[8], mat.m[9], mat.m[10], mat.m[11],
		mat.m[12], mat.m[13], mat.m[14], mat.m[15]);
	uint32 ret = (uint32)Utils::strlen_utf8(pValue);
	value(pValue, ret);
}

bool XmlAttribute::GetBoolValue()
{
	const char8* bValue = value();
	switch (*(uint32*)bValue)
	{
	case 0x65757274: return true;	//字符串true
	case 0x736c6166: return false;	//字符串false
	}

	return false;
}

const char8* XmlAttribute::GetStringValue()
{
	return value();
}

int32 XmlAttribute::GetIntValue()
{
	return (int32)strtol(value(), nullptr, 10);
}

float32 XmlAttribute::GetFloatValue()
{
	return (float32)strtod(value(), nullptr);
}

Color4 XmlAttribute::GetColorValue()
{
	return (Color4)strtoul(value(), nullptr, 16);
}

Vector2Df XmlAttribute::GetVector2DValue()
{
	char8* data = value();

	Vector2Df vt;
	vt.x = strtof(data, &data);
	vt.y = strtof(data, &data);
	return vt;
}

Vector3Df XmlAttribute::GetVector3DValue()
{
	char8* data = value();

	Vector3Df vt;
	vt.x = strtof(data, &data);
	vt.y = strtof(data, &data);
	vt.z = strtof(data, &data);
	return vt;
}

Vector4Df XmlAttribute::GetVector4DValue()
{
	char8* data = value();

	Vector4Df vt;
	vt.x = strtof(data, &data);
	vt.y = strtof(data, &data);
	vt.z = strtof(data, &data);
	vt.w = strtof(data, &data);
	return vt;
}

Matrix4 XmlAttribute::GetMatrix4Value()
{
	char8* data = value();

	Matrix4 mat;
	mat.m[0] = strtof(data, &data);
	mat.m[1] = strtof(data, &data);
	mat.m[2] = strtof(data, &data);
	mat.m[3] = strtof(data, &data);
	mat.m[4] = strtof(data, &data);
	mat.m[5] = strtof(data, &data);
	mat.m[6] = strtof(data, &data);
	mat.m[7] = strtof(data, &data);
	mat.m[8] = strtof(data, &data);
	mat.m[9] = strtof(data, &data);
	mat.m[10] = strtof(data, &data);
	mat.m[11] = strtof(data, &data);
	mat.m[12] = strtof(data, &data);
	mat.m[13] = strtof(data, &data);
	mat.m[14] = strtof(data, &data);
	mat.m[15] = strtof(data, &data);
	return mat;
}

XmlNode* XmlAttribute::GetOwnerNode()
{
	return (XmlNode*)parent();
}

XmlDocument* XmlAttribute::GetOwnerDocument()
{
	return (XmlDocument*)document();
}
