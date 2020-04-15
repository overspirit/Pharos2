#pragma once

class MyApp : public IApplication, public IRenderCallback, public IControlViewer
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

	OctreeScene*			m_scene;
	SceneCamera*			m_camera;
	Model*					m_model;
	bool			m_bLeftDown;
	bool			m_bRightDown;

	float32			m_elapsed;

public:
	virtual bool Init();
	virtual void Destroy();

	virtual void onWindowChangeSize(int32 width, int32 height);
	virtual bool onMouseEvent(const MouseEvent& event);
	virtual bool onKeyboardEvent(const KeyEvent& event);

	virtual void Update(float32 fElapsed);
	virtual void Render(float32 fElapsed);
	virtual void onRender(float32 elapsed);
};

