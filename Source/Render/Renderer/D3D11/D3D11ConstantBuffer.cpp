﻿#include "PreCompile.h"
#include "Pharos.h"

D3D11ConstantBuffer::D3D11ConstantBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	m_pBuffer = nullptr;
}

D3D11ConstantBuffer::~D3D11ConstantBuffer()
{
	SAFE_RELEASE(m_pBuffer);
}

void D3D11ConstantBuffer::CopyData(const MemoryBuffer& data, uint32 offset)
{
	uint32 dataSize = data.GetLength();
	uint32 totalDataSize = m_data.GetLength();

	if (totalDataSize < offset + dataSize)
	{
		m_data.ChangeSize(offset + dataSize);
	}

	m_data.Insert(offset, data);
}

void D3D11ConstantBuffer::CopyData(const void* data, uint32 len, uint32 offset)
{
	uint32 dataSize = len;
	uint32 totalDataSize = m_data.GetLength();

	if (totalDataSize < offset + dataSize)
	{
		m_data.ChangeSize(offset + dataSize);
	}

	m_data.Insert(offset, data, len);
}

bool D3D11ConstantBuffer::CreateBuffer(uint32 bufSize)
{
	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.ByteWidth = bufSize;
	HRESULT hr = m_pDevice->CreateBuffer(&desc, nullptr, &m_pBuffer);

	return SUCCEEDED(hr);
}

void D3D11ConstantBuffer::ApplyDevice(uint32 slot)
{
	uint32 dataSize = m_data.GetLength();
	if (dataSize == 0)
	{
		//assert(false);
		return;
	}

	//验证MemoryBuffer是否和ConstantBuffer的大小是否匹配
	//如果MemeoryBuffer比ConstantBuffer的数据大，需要重新建立ConstantBuffer
	if (m_pBuffer != nullptr)
	{
		D3D11_BUFFER_DESC desc;
		m_pBuffer->GetDesc(&desc);
		uint32 bufSize = desc.ByteWidth;

		if (dataSize > bufSize)
		{
			SAFE_RELEASE(m_pBuffer);

			if (!CreateBuffer(dataSize))
			{
				assert(false);
				return;
			}
		}
	}
	else
	{
		//D3D11要求ConstantBuffer的大小必须是16的整数倍...
		if (!CreateBuffer(dataSize < 16u ? 16u : dataSize))
		{
			assert(false);
			return;
		}
	}
	
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	m_pContext->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	uint8* dest = (uint8*)mappedResource.pData;

	memcpy(dest, m_data.GetPointer(), dataSize);

	m_pContext->Unmap(m_pBuffer, 0);

	m_pContext->VSSetConstantBuffers(slot, 1, &m_pBuffer);
	m_pContext->PSSetConstantBuffers(slot, 1, &m_pBuffer);
	m_pContext->GSSetConstantBuffers(slot, 1, &m_pBuffer);
	m_pContext->HSSetConstantBuffers(slot, 1, &m_pBuffer);
	m_pContext->DSSetConstantBuffers(slot, 1, &m_pBuffer);
	m_pContext->CSSetConstantBuffers(slot, 1, &m_pBuffer);
}