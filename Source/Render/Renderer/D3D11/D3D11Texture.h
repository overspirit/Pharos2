#pragma once

namespace Pharos
{
	namespace Render
	{
		//总结一下

		//1, 建立Texture时要想传入初始化数据，就要传入每层mip的数据，且pInitData中的SysMemPitch必须指定
		//例子如下
		//D3D11_SUBRESOURCE_DATA data;
		//data.pSysMem = pData;
		//data.SysMemPitch = 4 * width;		//必须指定SysMemPitch才能建立成功
		//data.SysMemSlicePitch = 0;
		//D3D文档中有说明，SysMemPitch在建立Texture2D和Texture3D中使用，另外一个值SysMemSlicePitch
		//在建立Texture3D中使用，具体查看D3D11_SUBRESOURCE_DATA的说明

		//2, D3D11中的format中比D3D9要严格的多。D3D9中A8R8G8B8就是指4个分量都是
		//BYTE型，使用时直接就将分量值映射为0-1.0f的颜色值。而D3D11中的R8G8B8A8则
		//有好几种类型，只有DXGI_FORMAT_R8G8B8A8_UNORM这个类型才会认为分量值
		//是BYTE类型，然后将分量值映射为0-1.0f的浮点颜色值。具体查看关于format中的说明

		//3, usage为D3D11_USAGE_DYNAMIC，miplevels必须为1,不知道为什么。
		//而且这时只能map,且map的参数只能是D3D11_MAP_WRITE_DISCARD,不能使用其他参数。
		//UpdateSubResource无效，D3D文档中说明UpdaeSubResource不能用于多重采样的文理，
		//不知道是不是这个原因。

		//4, UpdateSubResource这个函数可以用在使用D3D11_USAGE_DEFAULT建立的mipLevels为1的纹理上
		// 其他mipLeves数值无效，这时不能map....

		//5, map的效率比UpdateSubResource略高一点，为了实现CopyFromData和CopyRect这两个函数
		//所以在Create纹理时我只使用了mipLevels为1,Usage为D3D11_USAGE_DEFAULT建立。
		//主要考虑纯动态纹理这个东西使用的比较少，大部分动态的纹理都用于Render Target.

		//感觉D3D11的纹理是个巨大的坑...

		class D3D11SampleState;

		class D3D11Texture : public RenderTexture
		{
		public:
			D3D11Texture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
			virtual ~D3D11Texture();

		protected:
			ID3D11Device* m_pDevice;
			ID3D11DeviceContext* m_pContext;

			ID3D11Texture2D*			m_pTexture;
			ID3D11ShaderResourceView*	m_pView;

			D3D11SamplerState*			m_pState;

			uint32			m_width;
			uint32			m_height;
			EPixelFormat	m_fmt;
			uint32			m_eleSize;

		public:
			virtual bool LoadFromFile(const char8* szPath);
			virtual bool LoadFromImage(const Image& pImage);

			//建立一个纹理
			virtual bool Create(int32 width, int32 height, EPixelFormat fmt = EPF_RGBA8_UNORM);
			virtual bool CreateDepthTexture(int32 width, int32 height);
			virtual bool CreateTargetTexture(int32 width, int32 height, EPixelFormat fmt);

			//从内存数据上更新纹理
			virtual bool CopyFromData(const void* pImageData, uint32 nDataSize);
			virtual bool CopyRectFromData(const void* pData, uint32 nDataSize, const Rect2Di& rt);

			//从另一个纹理数据上更新纹理
			virtual bool CopyFromTexture(RenderTexture* srcTex);
			virtual bool CopyRectFromTexture(RenderTexture* srcTex, const Rect2Di& srcRect, const Rect2Di& destRect);

			virtual void SetSampleState(RenderSamplerState* state);

			virtual uint32 GetWidth() const { return m_width; }
			virtual uint32 GetHeight() const { return m_height; }
			virtual EPixelFormat GetFormat() const { return m_fmt; }

			virtual void ApplyToDevice(uint32 slot);

			virtual ID3D11Texture2D* GetTexture() const { return m_pTexture; }
			virtual ID3D11ShaderResourceView* GetTextureView() const { return m_pView; }
		};
	}
}