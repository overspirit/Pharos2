#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"

D3D11RenderLayout::D3D11RenderLayout(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	m_indexBuf = nullptr;
	m_vertBuf = nullptr;
	m_layout = nullptr;

	ZeroMemory(&m_indexBufDesc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&m_vertBufDesc, sizeof(D3D11_BUFFER_DESC));
}

D3D11RenderLayout::~D3D11RenderLayout()
{
	SAFE_RELEASE(m_indexBuf);
	SAFE_RELEASE(m_vertBuf);
	SAFE_RELEASE(m_layout);
}

bool D3D11RenderLayout::CreateVertexBuffer(uint32 sizeInBytes, MemoryBuffer* data)
{
	if (m_vertBuf != nullptr) SAFE_RELEASE(m_vertBuf);

	m_vertBufDesc.ByteWidth = sizeInBytes;
	m_vertBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_vertBufDesc.MiscFlags = 0;
	m_vertBufDesc.StructureByteStride = 0;

	HRESULT hr = E_FAIL;

	if (data == nullptr)
	{
		m_vertBufDesc.Usage = D3D11_USAGE_DYNAMIC;
		m_vertBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		hr = m_pDevice->CreateBuffer(&m_vertBufDesc, nullptr, &m_vertBuf);

		m_vertBufType = EB_Dynamic;
	}
	else
	{
		m_vertBufDesc.Usage = D3D11_USAGE_DEFAULT;
		m_vertBufDesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA initialData;
		initialData.pSysMem = data->GetPointer();
		initialData.SysMemPitch = 0;
		initialData.SysMemSlicePitch = 0;
		hr = m_pDevice->CreateBuffer(&m_vertBufDesc, &initialData, &m_vertBuf);

		m_vertBufType = EB_Static;
	}

	if (FAILED(hr)) return false;

	m_vertBufSize = sizeInBytes;

	return true;
}

bool D3D11RenderLayout::CreateIndexBuffer(uint32 indexNum, MemoryBuffer* data)
{
	if (m_indexBuf != nullptr) SAFE_RELEASE(m_indexBuf);

	m_indexBufDesc.ByteWidth = indexNum * sizeof(uint32);
	m_indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_indexBufDesc.MiscFlags = 0;
	m_indexBufDesc.StructureByteStride = 0;

	HRESULT hr = E_FAIL;

	if (data == nullptr)
	{
		m_indexBufDesc.Usage = D3D11_USAGE_DYNAMIC;
		m_indexBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		hr = m_pDevice->CreateBuffer(&m_indexBufDesc, nullptr, &m_indexBuf);

		m_indexBufType = EB_Dynamic;
	}
	else
	{
		m_indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
		m_indexBufDesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA initialData;
		initialData.pSysMem = data->GetPointer();
		initialData.SysMemPitch = 0;
		initialData.SysMemSlicePitch = 0;
		hr = m_pDevice->CreateBuffer(&m_indexBufDesc, &initialData, &m_indexBuf);

		m_indexBufType = EB_Static;
	}

	if (FAILED(hr)) return false;

	m_indexNum = indexNum;

	return true;
}

void D3D11RenderLayout::CopyVertexBuffer(MemoryBuffer* data, uint32 dataSize, uint32 nOffsetInBytes, bool bNoOverwrite)
{
	if (m_vertBufType != EB_Dynamic) return;

	D3D11_MAP type = D3D11_MAP_WRITE_DISCARD;
	if (bNoOverwrite) type = D3D11_MAP_WRITE_NO_OVERWRITE;

	//D3D11文档中写明，如果MapType使用了D3D11_MAP_WRITE_DISCARD或者D3D11_MAP_WRITE_NO_OVERWRITE, MapFlag必须为0
	D3D11_MAPPED_SUBRESOURCE mapData;
	HRESULT hr = m_pContext->Map(m_vertBuf, 0, type, 0, &mapData);

	if (SUCCEEDED(hr))
	{
		uint8* dest = (uint8*)mapData.pData;
		const void* src = data->GetPointer();
		uint32 copySize = Math::minimum(m_vertBufSize - nOffsetInBytes, Math::minimum(data->GetLength(), dataSize));
		memcpy(dest + nOffsetInBytes, src, copySize);
	}

	m_pContext->Unmap(m_vertBuf, 0);
}

void D3D11RenderLayout::CopyIndexBuffer(MemoryBuffer* data, uint32 dataSize, uint32 nOffsetInBytes, bool bNoOverwrite)
{
	if (m_indexBufType != EB_Dynamic) return;

	D3D11_MAP type = D3D11_MAP_WRITE_DISCARD;
	if (bNoOverwrite) type = D3D11_MAP_WRITE_NO_OVERWRITE;

	D3D11_MAPPED_SUBRESOURCE mapData;
	HRESULT hr = m_pContext->Map(m_indexBuf, 0, type, 0, &mapData);

	if (SUCCEEDED(hr))
	{
		uint32* dest = (uint32*)mapData.pData;
		const void* src = data->GetPointer();
		uint32 copySize = Math::minimum(m_vertBufSize - nOffsetInBytes, Math::minimum(data->GetLength(), dataSize));
		memcpy(dest + nOffsetInBytes, src, copySize);
	}

	m_pContext->Unmap(m_indexBuf, 0);
}

DXGI_FORMAT D3D11RenderLayout::GetD3D11Format(VertElementType elementType, uint32 elementNum)
{
	switch (elementType)
	{
	case Pharos::Render::VET_FLOAT32:
	{
		switch (elementNum)
		{
		case 1: return DXGI_FORMAT_R32_FLOAT;
		case 2: return DXGI_FORMAT_R32G32_FLOAT;
		case 3: return DXGI_FORMAT_R32G32B32_FLOAT;
		case 4: return DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
	}
	break;
	case Pharos::Render::VET_FLOAT16:
	{
		switch (elementNum)
		{
		case 1: return DXGI_FORMAT_R16_FLOAT;
		case 2: return DXGI_FORMAT_R16G16_FLOAT;
		case 4: return DXGI_FORMAT_R16G16B16A16_FLOAT;
		}
	}
	break;
	case Pharos::Render::VET_SINT32:
	{
		switch (elementNum)
		{
		case 1: return DXGI_FORMAT_R32_SINT;
		case 2: return DXGI_FORMAT_R32G32_SINT;
		case 3: return DXGI_FORMAT_R32G32B32_SINT;
		case 4: return DXGI_FORMAT_R32G32B32A32_SINT;
		}
	}
	break;
	case Pharos::Render::VET_SINT16:
	{
		switch (elementNum)
		{
		case 1: return DXGI_FORMAT_R16_SINT;
		case 2: return DXGI_FORMAT_R16G16_SINT;
		case 4: return DXGI_FORMAT_R16G16B16A16_SINT;
		}
	}
	break;
	case Pharos::Render::VET_SINT8:
	{
		switch (elementNum)
		{
		case 1: return DXGI_FORMAT_R8_SINT;
		case 2: return DXGI_FORMAT_R8G8_SINT;
		case 4: return DXGI_FORMAT_R8G8B8A8_SINT;
		}
	}
	break;
	case Pharos::Render::VET_UINT32:
	{
		switch (elementNum)
		{
		case 1: return DXGI_FORMAT_R32_UINT;
		case 2: return DXGI_FORMAT_R32G32_UINT;
		case 3: return DXGI_FORMAT_R32G32B32_UINT;
		case 4: return DXGI_FORMAT_R32G32B32A32_UINT;
		}
	}
	break;
	case Pharos::Render::VET_UINT16:
	{
		switch (elementNum)
		{
		case 1: return DXGI_FORMAT_R16_UINT;
		case 2: return DXGI_FORMAT_R16G16_UINT;
		case 4: return DXGI_FORMAT_R16G16B16A16_UINT;
		}
	}
	break;
	case Pharos::Render::VET_UINT8:
	{
		switch (elementNum)
		{
		case 1: return DXGI_FORMAT_R8_UINT;
		case 2: return DXGI_FORMAT_R8G8_UINT;
		case 4: return DXGI_FORMAT_R8G8B8A8_UINT;
		}
	}
	break;
	case Pharos::Render::VET_SNORM16:
	{
		switch (elementNum)
		{
		case 1: return DXGI_FORMAT_R16_SNORM;
		case 2: return DXGI_FORMAT_R16G16_SNORM;
		case 4: return DXGI_FORMAT_R16G16B16A16_SNORM;
		}
	}
	break;
	case Pharos::Render::VET_SNORM8:
	{
		switch (elementNum)
		{
		case 1: return DXGI_FORMAT_R8_SNORM;
		case 2: return DXGI_FORMAT_R8G8_SNORM;
		case 4: return DXGI_FORMAT_R8G8B8A8_SNORM;
		}
	}
	break;
	case Pharos::Render::VET_UNORM16:
	{
		switch (elementNum)
		{
		case 1: return DXGI_FORMAT_R16_UNORM;
		case 2: return DXGI_FORMAT_R16G16_UNORM;
		case 4: return DXGI_FORMAT_R16G16B16A16_UNORM;
		}
	}
	break;
	case Pharos::Render::VET_UNORM8:
	{
		switch (elementNum)
		{
		case 1: return DXGI_FORMAT_R8_UNORM;
		case 2: return DXGI_FORMAT_R8G8_UNORM;
		case 4: return DXGI_FORMAT_R8G8B8A8_UNORM;
		}
	}
	break;
	}
	return DXGI_FORMAT_FORCE_UINT;
}

uint32 D3D11RenderLayout::GetVertElementSize(VertElementType elementType, uint32 elementNum)
{
	uint32 elementBitNum = 0;

	switch (elementType)
	{
	case Pharos::Render::VET_FLOAT32:
	case Pharos::Render::VET_UINT32:
	case Pharos::Render::VET_SINT32:
		elementBitNum = 4;
		break;
	case Pharos::Render::VET_FLOAT16:
	case Pharos::Render::VET_SINT16:
	case Pharos::Render::VET_UINT16:
	case Pharos::Render::VET_SNORM16:
	case Pharos::Render::VET_UNORM16:
		elementBitNum = 2;
		break;
	case Pharos::Render::VET_UINT8:
	case Pharos::Render::VET_SINT8:
	case Pharos::Render::VET_SNORM8:
	case Pharos::Render::VET_UNORM8:
		elementBitNum = 1;
		break;
	}

	return elementBitNum * elementNum;
}

bool D3D11RenderLayout::SetInputLayoutDesc(const VertLayoutDesc* desc, uint32 descNum)
{
	if (m_layout != nullptr) SAFE_RELEASE(m_layout);

	m_layoutDesc.resize(descNum);
	
	for (uint32 i = 0; i < m_layoutDesc.size(); i++)
	{
		m_layoutDesc[i].elementType = desc[i].elementType;
		m_layoutDesc[i].elementNum = desc[i].elementNum;
		m_layoutDesc[i].elementSemantic = desc[i].elementSemantic;
		m_layoutDesc[i].semanticIndex = desc[i].semanticIndex;
		m_layoutDesc[i].elementOffset = desc[i].elementOffset;

		m_vertStride += GetVertElementSize(desc[i].elementType, desc[i].elementNum);
	}

	return true;
}

bool D3D11RenderLayout::CreateD3D11InputLayout(ID3D10Blob* vs_blob)
{
	if (vs_blob == nullptr) return false;
	if (m_layout != nullptr) return true;

	uint32 descNum = (uint32)m_layoutDesc.size();
	vector<D3D11_INPUT_ELEMENT_DESC> descList(descNum);
	for (uint32 i = 0; i < descNum; i++)
	{
		descList[i].SemanticName = m_layoutDesc[i].elementSemantic.c_str();
		descList[i].SemanticIndex = m_layoutDesc[i].semanticIndex;
		descList[i].Format = GetD3D11Format(m_layoutDesc[i].elementType, m_layoutDesc[i].elementNum);
		descList[i].InputSlot = 0;
		descList[i].AlignedByteOffset = m_layoutDesc[i].elementOffset;
		descList[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		descList[i].InstanceDataStepRate = 0;
	}

	HRESULT hr = m_pDevice->CreateInputLayout(&*descList.begin(), (UINT)descList.size(), vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), &m_layout);
	return SUCCEEDED(hr);
}

void D3D11RenderLayout::ApplyToDevice()
{
//#pragma message("remove this code!!!")
	//m_pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	//set input layout
	m_pContext->IASetInputLayout(m_layout);

	//???set vertex buffer
	uint32 vertOffset = 0;	
	m_pContext->IASetVertexBuffers(0, 1, &m_vertBuf, &m_vertStride, &vertOffset);

	//set index buffer
	if (m_indexBuf != nullptr)
		m_pContext->IASetIndexBuffer(m_indexBuf, DXGI_FORMAT_R32_UINT, 0);
}