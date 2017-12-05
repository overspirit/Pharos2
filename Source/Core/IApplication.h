#pragma once

namespace Pharos
{
	class IApplication
	{
	public:
		virtual ~IApplication(){}

	public:
		virtual bool Init() = 0;
		virtual void Destroy() = 0;

		virtual void onViewCreate() = 0;
		virtual void onViewDestroy() = 0;
		virtual void onViewChangeSize(int32 width, int32 height) = 0;

		virtual bool onMouseEvent(const MouseEvent& event) = 0;
		virtual bool onKeyboardEvent(const KeyEvent& evnet) = 0;

		virtual void onSensorAccelerometerEvent(const Vector3Df& acc, float64 timestamp) = 0;
		virtual void onSensorMagenticFieldEvent(const Vector3Df& magnetic, float64 timestamp) = 0;
		virtual void onSensorGyroscopeEvnet(const Vector3Df& gryo, float64 timestamp) = 0;

		virtual void Update(float32 fElapsed) = 0;
		virtual void Render(float32 fElapsed) = 0;
	};
}