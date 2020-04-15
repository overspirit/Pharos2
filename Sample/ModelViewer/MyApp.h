#pragma once

//IControlViewer放在第一位，防止出现IControlViewer中类成员函数指针注册时出现的编译警告
class MyApp : public IControlViewer, public IRenderCallback, public IApplication
{
public:
	MyApp();
	virtual ~MyApp();

private:
	OctreeScene*			m_scene;
	SceneCamera*			m_camera;
	Model*					m_model;
	bool			m_bLeftDown;
	bool			m_bRightDown;

	float32			m_elapsed;

	bool		m_loopPlay;
	bool		m_playing;

	vector<string>		m_animNameList;
	int32				m_currAnimIndex;

	Button*				m_playBtn;
	Button*				m_pauseBtn;
	FontString*			m_currFrameLabel;
	FontString*			m_totalFrameLabel;
	Slider*				m_frameSlider;

private:
	virtual bool onPlayAnimationClick(UIObject* obj, const EventArgs& eventArgs);
	virtual bool onPauseAnimationClick(UIObject* obj, const EventArgs& eventArgs);
	virtual bool onPrevAnimationClick(UIObject* obj, const EventArgs& eventArgs);
	virtual bool onNextAnimationClick(UIObject* obj, const EventArgs& eventArgs);
	virtual bool onLoopPlayClick(UIObject* obj, const EventArgs& eventArgs);
	virtual bool onOpenFileClick(UIObject* obj, const EventArgs& eventArgs);
	virtual bool onFrameSliderValueChange(UIObject* obj, const EventArgs& eventArgs);

	virtual void SetSliderFromAnimation();

public:
	virtual bool Init();
	virtual void Destroy();

	virtual void onWindowChangeSize(int32 width, int32 height);
	virtual bool onMouseEvent(const MouseEvent& event);
	virtual bool onKeyboardEvent(const KeyEvent& event);

	virtual void Update(float32 fElapsed);
	virtual void onRender(float32 elapsed);
};

