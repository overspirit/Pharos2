#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
#include "SceneGlobal.h"

extern ID3D11Device*			g_device;
extern ID3D11DeviceContext*		g_context;

char8	g_vs[] =
"cbuffer cbPerScene : register(b0)\n"
"{\n"
"	row_major matrix g_world : packoffset(c0);\n"
"	row_major matrix g_view : packoffset(c4);\n"
"	row_major matrix g_proj : packoffset(c8);\n"
"};\n"
"void Sprite3DNormalVS(float4 pos : POSITION, float3 normal : NORMAL, out float4 oColor : COLOR, out float4 oPos : SV_POSITION)\n"
"{\n"
"	oPos = mul(pos, mul(g_world, mul(g_view, g_proj)));\n"
"	normal = normalize(normal);\n"
"	float3 lightDir = normalize(float3(1.0, 1.0, 1.0));\n"
"	oColor = clamp(dot(-lightDir, normal), 0, 1.0) * float4(1.0, 1.0, 1.0, 1.0) + float4(0.3, 0.3, 0.3, 0.3);\n"
"}\n";

char8	g_ps[] =
"float4 Sprite3DColorPS(float4 color : COLOR) : SV_TARGET\n"
"{\n"
"	return color;\n"
"}\n";

D3D11ShaderProgram* SceneNode::s_shaderProgram = nullptr;

SceneNode::SceneNode()
{
	m_hidden = false;

	m_boundingRadius = 0;

	m_renderLayout = nullptr;
}

SceneNode::~SceneNode()
{

}

void SceneNode::InitNode(const char8* name, SceneNode* parent)
{
	m_name = name;

	m_parent = parent;
}

void SceneNode::InitModelData(const MemoryBuffer& vertData, const vector<VertLayoutDesc>& vertDesc, const MemoryBuffer& indexData)
{
	if (s_shaderProgram == nullptr)
	{
		s_shaderProgram = new D3D11ShaderProgram(g_device, g_context);
		s_shaderProgram->CompileVertexShader(g_vs, "Sprite3DNormalVS");
		s_shaderProgram->CompilePixelShader(g_ps, "Sprite3DColorPS");
	}

	m_renderLayout = new D3D11RenderLayout(g_device, g_context);

	m_renderLayout->CreateVertexBuffer(vertData.GetLength(), (MemoryBuffer*)&vertData);
	m_renderLayout->SetInputLayoutDesc(&vertDesc[0], vertDesc.size());
	m_renderLayout->CreateIndexBuffer(indexData.GetLength() / sizeof(uint32), (MemoryBuffer*)&indexData);

	ID3D10Blob* blob = s_shaderProgram->GetVertexBlob();
	m_renderLayout->CreateD3D11InputLayout(blob);
}

void SceneNode::SetLocalTransform(const Matrix4& mat)
{
	m_localTransform = mat;
}

void SceneNode::SetLocalPosition(const Vector3Df& pos)
{
	m_localTransform.SetTranslation(pos);
}

const Matrix4& SceneNode::GetLocalTransform()
{
	return m_localTransform;
}

const Matrix4& SceneNode::GetGlobalTransform()
{
	return m_globalTransform;
}

const Vector3Df& SceneNode::GetLocalPosition()
{
	m_localPos = m_localTransform.GetTranslation();

	return m_localPos;
}

const Vector3Df& SceneNode::GetGlobalPosition()
{
	m_globalPos = m_globalTransform.GetTranslation();

	return m_globalPos;
}

void SceneNode::SetBoundingRadius(float32 fRadius)
{
	m_boundingRadius = fRadius;
}

void SceneNode::AddChildNode(SceneNode* node)
{
	m_childList.push_back(node);
	node->m_parent = this;
}

void SceneNode::RemoveChildNode(SceneNode* node)
{
	for (auto iter = m_childList.begin(); iter != m_childList.end(); iter++)
	{
		if (*iter == node)
		{
			m_childList.erase(iter);
			break;
		}
	}
}

void SceneNode::PlayAnim(const char8* animName)
{

}

void SceneNode::Update(float32 fElapsed)
{
// 	if (m_parent != nullptr)
// 	{
// 		m_globalTransform = m_localTransform * m_parent->GetGlobalTransform();
// 	}
// 	else
// 	{
// 		m_globalTransform = m_localTransform;
// 	}

	g_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (m_renderLayout != nullptr && s_shaderProgram != nullptr)
	{
		m_renderLayout->ApplyToDevice();
		s_shaderProgram->ApplyToDevice();

		uint32 indexNum = m_renderLayout->GetIndexNum();
		g_context->DrawIndexed(indexNum, 0, 0);
	}

	for (uint32 i = 0; i < m_childList.size(); i++)
	{
		m_childList[i]->Update(fElapsed);
	}
}
