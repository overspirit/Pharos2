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

	RenderTexture*			m_mainTex;
	RenderTexture*			m_noiseTex;
	RenderTechnique*		m_dissTech;
	RenderProgram*			m_dissShader;

	RenderVariable*			m_dissColor;
	RenderVariable*			m_addColor;
	RenderVariable*			m_dissRange;
	RenderVariable*			m_blendVar;

	Color4*					m_colorData;

	bool			m_pause;

	Noise2D				m_noise;

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

