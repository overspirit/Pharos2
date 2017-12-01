#pragma once

namespace Pharos
{
	namespace Core
	{
		class PlatformWindows //: public Utils::Singleton<PlatformWindows>
		{
		public:
			PlatformWindows(void);
			virtual ~PlatformWindows(void);

		private:
			HWND			m_hWnd;
			
			Timer				m_timer;
			uint32				m_fps;
			uint32				m_renderCount;
			

			D3D_DRIVER_TYPE		m_driverType;
			D3D_FEATURE_LEVEL	m_featureLevel;

			DXGI_SWAP_CHAIN_DESC		m_swapChainDesc;

			ID3D11Device*			m_device;
			ID3D11DeviceContext*	m_context;

			IDXGIAdapter*	m_dxgiAdapter;
			IDXGIFactory*	m_dxgiFactory;
			IDXGISwapChain* m_swapChain;

			//D3D11FrameBufferPtr		m_defFrameBuf;
			string					m_adapterName;
			uint32					m_adapterMemSize;
						
			//D3D11RenderLayoutPtr	m_bindLayout;
			//D3D11ShaderProgramPtr	m_bindShader;
			
		private: 			
			static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		public:
			virtual bool Init();			
			virtual void Destroy();
			
			bool InitDevice();

			virtual int32 Run();

			//virtual void onKeyboardEvent(const KeyEvent& keyEvent);
			//virtual void onMouseEvent(const MouseEvent& mouseEvent);

			virtual void onWindowCreate();
			virtual void onWindowChangeSize(int32 width, int32 height);
			virtual void onWindowDestroy();
		};
	}
}
