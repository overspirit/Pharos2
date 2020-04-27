#pragma once

class MyApp : public IApplication, public IRenderCallback, public IControlViewer
{
public:
	MyApp();
	virtual ~MyApp();

private:
	struct SceneMatrix
	{
		Matrix4 proj;
		Matrix4 view;
	};

	struct Vertex
	{
		//        Vertex(const Vector3Df& pos, const Vector2Df& tex)
		//        {
		//            this->pos = pos;
		//            this->tex = tex;
		//        }
		Vector3Df	pos;
		Color4  	color;
	};

private:
	Renderer*				m_renderer;

	RenderBuffer*           m_vertBuf;
	RenderPipeline*         m_pipeline;
	RenderCommand*          m_renderCommand;
	RenderTexture*          m_texture;

	OctreeScene*            m_scene;
	SceneCamera*            m_camera;

	SceneMatrix             m_sceneMat;
	RenderBuffer*           m_uniformBuf;

	bool            m_bLeftDown;
	bool            m_bRightDown;

public:
	virtual bool Init();
	virtual void Destroy();

	virtual void onWindowChangeSize(int32 width, int32 height);
	virtual bool onMouseEvent(const MouseEvent& event);
	virtual bool onKeyboardEvent(const KeyEvent& event);

	virtual void onControlCreate(const char8* name, int32 v1, float32 v2);
	virtual void onControlValueChange(const char8* name, int32 v1, float32 v2);

	virtual void Update(float32 fElapsed);
	virtual void onRender(float32 elapsed);
};

