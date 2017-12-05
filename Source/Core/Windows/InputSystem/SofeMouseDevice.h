#pragma once

namespace Pharos
{
	namespace Core
	{
		class CSofeMouseDevice : public CMouseDevice
		{
		public:
			CSofeMouseDevice(void);
			virtual ~CSofeMouseDevice(void);

		public:
			virtual bool InitDevice(IDirectInput8* pDirectInput, HWND hWnd, const GUID& guid = GUID_NULL);

			virtual bool UpdateDevice();
		};
	}
}
