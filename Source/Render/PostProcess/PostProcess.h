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
			vector<RenderTexture*>		m_inputPins;
			vector<RenderTexture*>		m_outputPins;

		public:
			virtual bool Init();

			virtual void SetInputPin(uint32 index, RenderTexture* tex);
			virtual RenderTexture* GetOutputPin(uint32 index);
			virtual uint32 GetOutputPinsNum() { return (uint32)m_outputPins.size(); }

			virtual void Apply() = 0;
		};
	}
}