#pragma once

class MyApp : public IApplication, public IRenderCallback
{
public:
	MyApp();
	virtual ~MyApp();

private:
	struct Vertex
	{
		Vertex(const Vector3Df& pos, const Vector2Df& tex)
		{
			this->pos = pos;
			this->tex = tex;
		}
		Vector3Df	pos;
		Vector2Df	tex;
	};

private:
	Renderer*				m_renderer;
	RenderFrameBuffer*		m_defFrameBuf;

	RenderLayout*			m_copyLayout;
	RenderProgram*			m_copyShader;
	RenderTechnique*		m_copyTech;

	SceneCamera*			m_camera;
	bool			m_bLeftDown;
	bool			m_bRightDown;

public:
	virtual bool Init();
	virtual void Destroy();

	virtual void onViewCreate();
	virtual void onViewDestroy();
	virtual void onViewChangeSize(int32 width, int32 height);

	virtual bool onMouseEvent(const MouseEvent& event);
	virtual bool onKeyboardEvent(const KeyEvent& evnet);

	virtual void onSensorAccelerometerEvent(const Vector3Df& acc, float64 timestamp);
	virtual void onSensorMagenticFieldEvent(const Vector3Df& magnetic, float64 timestamp);
	virtual void onSensorGyroscopeEvnet(const Vector3Df& gryo, float64 timestamp);

	virtual void Update(float32 fElapsed);
	virtual void Render(float32 fElapsed);
	virtual void onRender(float32 elapsed);
};

