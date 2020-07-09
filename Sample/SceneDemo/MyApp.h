#pragma once

class MyApp : public IApplication, public IRenderCallback, public IControlViewer
{
public:
	MyApp();
	virtual ~MyApp();

private:
	struct SceneMatrix
	{
		Matrix4 view;
		Matrix4 proj;	
		Color4f	color;	
	};

private:
	OctreeScene*            m_scene;
	SceneCamera*            m_camera;
	Model*					m_model;
	
	SceneMatrix             m_sceneMat;

	MapNode*		m_mapNode;
	
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

