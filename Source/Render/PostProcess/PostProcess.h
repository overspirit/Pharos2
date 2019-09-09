#pragma once

namespace Pharos
{
	namespace Render
	{
		class PostProcess
		{
		public:
			PostProcess();
			virtual ~PostProcess();

		protected:
			Renderer*					m_renderer;

			RenderTechnique*			m_tech;
			RenderBlock*				m_block;

			vector<RenderTexture*>		m_inputPins;
			vector<RenderTexture*>		m_outputPins;

			RenderFrameBuffer*			m_frameBuf;

		public:
			virtual bool InitWithTech(const char8* techName);

			virtual void SetInputPin(uint32 index, RenderTexture* tex);
			virtual RenderTexture* GetInputPin(uint32 index);
			virtual uint32 GetInputPinNum() { return (uint32)m_inputPins.size(); }

			virtual void SetOutputPin(uint32 index, RenderTexture* tex);
			virtual RenderTexture* GetOutputPin(uint32 index);
			virtual uint32 GetOutputPinsNum() { return (uint32)m_outputPins.size(); }

			virtual void Apply();
		};
	}
}