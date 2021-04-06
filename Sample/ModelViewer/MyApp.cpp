#include "PreCompile.h"
#include "Global.h"

#define MOVE_SPEED 10.0f

MyApp::MyApp()
{
	m_scene = nullptr;
	m_camera = nullptr;
	m_model = nullptr;
    m_material = nullptr;
    
	m_bLeftDown = false;
	m_bRightDown = false;
    m_bMidDown = false;

	m_model = nullptr;

	m_elapsed = 0;

	m_loopPlay = false;
	m_playing = false;

	m_currAnimIndex = 0;

	m_playBtn = nullptr;
	m_pauseBtn = nullptr;
	m_currFrameLabel = nullptr;
	m_totalFrameLabel = nullptr;
	m_frameSlider = nullptr;
}

MyApp::~MyApp()
{
}

IMPL_CREATE_APPLICATION(MyApp)

bool MyApp::Init()
{
	Size2Di wndSize = sKernel->GetWindowSize();

	Render::RenderParam param;
	param.width = wndSize.width;
	param.height = wndSize.height;
	param.sampleType = Render::EMT_None;
	param.sync = false;
	param.fullScreen = false;
	param.gammaEnabled = false;
	param.hdrEnabled = false;
	param.backColor = 0xFF8F8F8F;
	sRenderMgr->StartUp(param);

	//sRenderMgr->RegisterRenderCallback(this);

	//Scene
	//////////////////////////////////////////////////////////////////////////
	m_scene = sSceneMgr->CreateScene();
	sSceneMgr->SetCurrScene(m_scene);
	m_scene->SetSceneGridShow(true);

	m_camera = m_scene->GetSceneCamera();
	m_camera->BuildViewMatrix(Vector3Df(0, 2.0f, -5.0f), Vector3Df(0, 1.0f, 0));
	m_camera->BuildProjMatrix((float32)PI / 4, wndSize.width, wndSize.height, 1.0f, 1000.0f);
	//////////////////////////////////////////////////////////////////////////

	//UI
	//////////////////////////////////////////////////////////////////////////
	sDesktopMgr->SetDesktopSize(wndSize.width, wndSize.height);
	sDesktopMgr->LoadUILayoutFile("Interface/ViewerPanel/Panel.xml");

	m_playBtn = (Button*)sDesktopMgr->GetControl("PlayAnim");
	m_pauseBtn = (Button*)sDesktopMgr->GetControl("PauseAnim");
	m_currFrameLabel = (FontString*)sDesktopMgr->GetControl("FrameBeginLabel");
	m_totalFrameLabel = (FontString*)sDesktopMgr->GetControl("FrameEndLabel");
	m_frameSlider = (Slider*)sDesktopMgr->GetControl("AnimFrame");

	m_playBtn->SetHidden(false);
	m_pauseBtn->SetHidden(true);

	sDesktopMgr->RegisterControlViewer("PlayAnim", this, (EVENT_CALLBACK)&MyApp::onPlayAnimationClick);
	sDesktopMgr->RegisterControlViewer("PauseAnim", this, (EVENT_CALLBACK)&MyApp::onPauseAnimationClick);
	sDesktopMgr->RegisterControlViewer("PrevAnim", this, (EVENT_CALLBACK)&MyApp::onPrevAnimationClick);
	sDesktopMgr->RegisterControlViewer("NextAnim", this, (EVENT_CALLBACK)&MyApp::onNextAnimationClick);
	sDesktopMgr->RegisterControlViewer("LoopPlay", this, (EVENT_CALLBACK)&MyApp::onLoopPlayClick);
	sDesktopMgr->RegisterControlViewer("OpenFile", this, (EVENT_CALLBACK)&MyApp::onOpenFileClick);
	sDesktopMgr->RegisterControlViewer("AnimFrame", this, (EVENT_CALLBACK)&MyApp::onFrameSliderValueChange);
    sDesktopMgr->RegisterControlViewer("AmbientRatio", this, (EVENT_CALLBACK)&MyApp::onAmbientRatioValueChange);
    sDesktopMgr->RegisterControlViewer("DiffuseRatio", this, (EVENT_CALLBACK)&MyApp::onDiffuseRatioValueChange);
    sDesktopMgr->RegisterControlViewer("SpecularRatio", this, (EVENT_CALLBACK)&MyApp::onSpecularRatioValueChange);
    sDesktopMgr->RegisterControlViewer("SpecularPow", this, (EVENT_CALLBACK)&MyApp::onSpecularPowValueChange);
	//////////////////////////////////////////////////////////////////////////
	return true;
}

void MyApp::Destroy()
{
	SAFE_DELETE(m_scene);
}

void MyApp::onWindowChangeSize(int32 width, int32 height)
{

}

bool MyApp::onMouseEvent(const MouseEvent& event)
{
	if (event.button == MOUSE_LEFT)
	{
		m_bLeftDown = (event.state != STATE_UP) ? true : false;
	}
	else if (event.button == MOUSE_RIGHT)
	{
		m_bRightDown = (event.state != STATE_UP) ? true : false;
	}
    else if (event.button == MOUSE_MID)
    {
        m_bMidDown = (event.state != STATE_UP) ? true : false;
    }
	else
	{
		m_camera->Stretch(event.wheel / 120.0f);
	}

	if (m_bLeftDown)
	{
		m_camera->Slither(Vector2Df((float32)event.ox, (float32)event.oy));
	}
	else if (m_bRightDown)
	{
		m_camera->Round(Vector2Df((float32)event.ox, (float32)event.oy));
	}
    else if (m_bMidDown)
    {
        m_camera->Drag(Vector2Df((float32)event.ox, (float32)event.oy));
    }

	return false;
}

bool MyApp::onKeyboardEvent(const KeyEvent& event)
{
	switch (event.key)
	{
	case KEY_A:
	{
		m_camera->Move(Vector3Df(-1.0f * m_elapsed * MOVE_SPEED, 0, 0));
	}
	break;
	case KEY_D:
	{
		m_camera->Move(Vector3Df(1.0f * m_elapsed * MOVE_SPEED, 0, 0));
	}
	break;
	case KEY_W:
	{
		m_camera->Move(Vector3Df(0, 0, 1.0f * m_elapsed * MOVE_SPEED));
	}
	break;
	case KEY_S:
	{
		m_camera->Move(Vector3Df(0, 0, -1.0f * m_elapsed * MOVE_SPEED));
	}
	break;
	case KEY_Q:
	{
		m_camera->Move(Vector3Df(0, 1.0f * m_elapsed * MOVE_SPEED, 0));
	}
	break;
	case KEY_E:
	{
		m_camera->Move(Vector3Df(0, -1.0f * m_elapsed * MOVE_SPEED, 0));
	}
	break;
	}

	return true;
}

bool MyApp::onPlayAnimationClick(UIObject* obj, const EventArgs& eventArgs)
{
	if (m_model == nullptr) return false;

	m_model->PlayAnimation();

	m_playBtn->SetHidden(true);
	m_pauseBtn->SetHidden(false);

	m_playing = true;

	return true;
}

bool MyApp::onPauseAnimationClick(UIObject* obj, const EventArgs& eventArgs)
{
	if (m_model == nullptr) return false;

	m_model->PauseAnimation();

	m_playBtn->SetHidden(false);
	m_pauseBtn->SetHidden(true);

	m_playing = false;

	return true;
}

bool MyApp::onPrevAnimationClick(UIObject* obj, const EventArgs& eventArgs)
{
	if (m_model == nullptr) return false;
	if (m_animNameList.size() <= 0) return false;

	m_currAnimIndex--;
	m_currAnimIndex = m_currAnimIndex < 0 ? (uint32)(m_animNameList.size() - 1) : m_currAnimIndex;

	m_model->SetCurrentAnimation(m_animNameList[m_currAnimIndex].c_str());

	SetSliderFromAnimation();

	return true;
}

bool MyApp::onNextAnimationClick(UIObject* obj, const EventArgs& eventArgs)
{
	if (m_model == nullptr) return false;
	if (m_animNameList.size() <= 0) return false;

	m_currAnimIndex++;
	m_currAnimIndex = m_currAnimIndex >= m_animNameList.size() ? 0 : m_currAnimIndex;

	m_model->SetCurrentAnimation(m_animNameList[m_currAnimIndex].c_str());

	SetSliderFromAnimation();

	return true;
}

bool MyApp::onLoopPlayClick(UIObject* obj, const EventArgs& eventArgs)
{
	m_loopPlay = eventArgs.args[0].ba;

	return true;
}

XmlNode* AppendVectorNode(XmlNode* parent, const char8* name, const Vector3Df& v)
{
    XmlNode* vectorNode = parent->AppendChild(name);
    
    XmlAttribute* vectorAttr = vectorNode->AppendAttribute("v");
    
    char buf[255];
    sprintf(buf, "%.6f %.6f %.6f", v.x, v.y, v.z);
    vectorAttr->SetValue(buf);
    
    return vectorNode;
}

bool MyApp::onOpenFileClick(UIObject* obj, const EventArgs& eventArgs)
{
#if defined(_LINUX_PLATFORM_)
    // string openFilePath = OpenFileDialog(sKernel->GetHomeDirectoryPath());
    // if (openFilePath.empty()) return false;
    
	string openFilePath;

	gtk_init(NULL, NULL);

    int file_return;
    GtkWidget *file =gtk_file_chooser_dialog_new("SelectFile",NULL,GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,GTK_STOCK_OK,GTK_RESPONSE_ACCEPT,NULL);
    if(gtk_dialog_run(GTK_DIALOG(file))==GTK_RESPONSE_ACCEPT)
	{
		openFilePath = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file));
		//printf(filename);
	}
		

	//g_signal_connect(G_OBJECT(file),"destroy",G_CALLBACK(gtk_main_quit),NULL);

	gtk_widget_destroy(file);
	
	while(gtk_events_pending())
	{
		gtk_main_iteration();
	}
#endif

    /*
	OPENFILENAME ofn;
	TCHAR szFileName[MAX_PATH] = _T("");
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = sPlatform->GetWindowsHandle();;    //父窗口句柄
	ofn.lpstrFilter = _T("All Files (*.*)\0*.*\0");   //打开的文件类型，这里以xml和所有文件为例
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_FILEMUSTEXIST;
	ofn.lpstrDefExt = _T("*");        //默认的打开的文件类型
	ofn.lpstrInitialDir = _T(".\\");     //默认的打开的文件路径，这里以当前目录为例

	if (!GetOpenFileName(&ofn)) return false;

	char8 buf[MAX_PATH];
	Utils::unicode_to_utf8((char16*)szFileName, -1, buf, MAX_PATH);

	SceneImporter* sceneImporter = sSceneMgr->CreateSceneImporter(buf);*/
    
    // Utils::Path path(openFilePath.c_str());
    // if(strcmp(path.GetFileExtension(), ".fbx") == 0)
    // {
    //     Converter converter;
    //     converter.OpenFbx(openFilePath.c_str());
        
    //     string fullPath = path.GetFullPath();
    //     string fileName = path.GetFileName();
        
    //     openFilePath = fullPath + fileName + ".sceneml";
    //     converter.SetName(fileName.c_str());
    //     converter.Save(openFilePath.c_str());
        
    //     //printf(openFilePath.c_str());
    // }
    
    
    SceneImporter* sceneImporter = sSceneMgr->CreateSceneImporter(openFilePath.c_str());//"Model/Ifrit/Ifrit.sceneml");
    sceneImporter->ImportScene(m_scene);
    SAFE_DELETE(sceneImporter);
    
    SceneNode* sceneNode = m_scene->GetSceneNode((uint32)0);
    m_model = sceneNode->GetModel(0);
    m_material = m_model->GetSubModelMaterial(0, 0);
    
    for (uint32 i = 0; i < m_model->GetAnimationNum(); i++)
    {
        const char8* animName = m_model->GetAnimationName(i);
        m_animNameList.push_back(animName);
    }
    
    SetSliderFromAnimation();

	return true;
}

bool MyApp::onFrameSliderValueChange(UIObject* obj, const EventArgs& eventArgs)
{
	if (!m_playing)
	{
		if (m_model == nullptr) return false;

		m_model->SetCurrentAnimationFrame((uint32)eventArgs.args[0].fa);
	}

	return true;
}

bool MyApp::onAmbientRatioValueChange(UIObject* obj, const EventArgs& eventArgs)
{
    printf("onAmbientRatioValueChange value:%f\n", eventArgs.args[0].fa);
    
    m_material->SetAbmbinetRatioParamValue(eventArgs.args[0].fa);
    return true;
}

bool MyApp::onDiffuseRatioValueChange(UIObject* obj, const EventArgs& eventArgs)
{
    printf("onDiffuseRatioValueChange value:%f\n", eventArgs.args[0].fa);
    
    m_material->SetDiffuseRatioParamValue(eventArgs.args[0].fa);
    
    return true;
}

bool MyApp::onSpecularRatioValueChange(UIObject* obj, const EventArgs& eventArgs)
{
    printf("onSpecularRatioValueChange value:%f\n", eventArgs.args[0].fa);
    
    m_material->SetSpecularRatioParamValue(eventArgs.args[0].fa);
    
    return true;
}

bool MyApp::onSpecularPowValueChange(UIObject* obj, const EventArgs& eventArgs)
{
    printf("onSpecularPowValueChange value:%f\n", eventArgs.args[0].fa);
    
    m_material->SetSpecularPowParamValue(eventArgs.args[0].fa);
    
    return true;
}

void MyApp::SetSliderFromAnimation()
{
	uint32 frameNum = m_model->GetCurrentAnimationFrameNum();

	m_frameSlider->SetValueRange(0, (float32)frameNum - 1, 1);
	m_frameSlider->SetValue(0);

	m_currFrameLabel->SetText("0");

	char8 buf[32];
	sprintf(buf, "%d", frameNum - 1);
	m_totalFrameLabel->SetText(buf);
}

void MyApp::onPreSceneUpdate(float32 fElapsed)
{
	m_elapsed = fElapsed;

	if (m_model == nullptr) return;

	uint32 frame = m_model->GetCurrentAnimationFrame();

	if (m_playing)
	{
		m_frameSlider->SetValue((float32)frame);

		uint32 animFrameNum = m_model->GetCurrentAnimationFrameNum();
		uint32 currFrame = m_model->GetCurrentAnimationFrame();

		if (currFrame >= animFrameNum - 1)
		{
			if (m_loopPlay)
			{
				m_model->SetCurrentAnimationFrame(0);
				m_model->PlayAnimation();
			}
			else
			{
				m_playBtn->SetHidden(false);
				m_pauseBtn->SetHidden(true);

				m_playing = false;
			}
		}
	}

	char8 buf[32];
	sprintf(buf, "%d", frame);
	m_currFrameLabel->SetText(buf);
}
