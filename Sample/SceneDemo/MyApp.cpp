#include "PreCompile.h"
#include "Global.h"

#define MOVE_SPEED 10.0f

MyApp::MyApp()
{
	m_font = nullptr;
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
	//param.gammaEnabled = false;
	sRenderMgr->StartUp(param);

	sRenderMgr->RegisterRenderCallback(this);

	m_renderer = sRenderMgr->GetCurrentRenderer();
	//m_copyTex = m_renderer->LoadTexture("Model/Earth/AtmosphericScattering.bmp");

	//m_copyTech = sRenderMgr->GenerateRenderTechnique("Copy");
	//m_copyShader = m_copyTech->GetPass(0)->GetShaderProgram();


	m_font = sResMgr->GenerateFont("Font/Fontin.ttf");

	//m_fontIndex = sRenderSpirite->GetFontInfoIndex(m_font);
	
// 	File charListFile;
// 	charListFile.Open("Font/charlist.txt");
// 
// 	uint16 flag = 0;
// 	charListFile.Read(&flag, sizeof(flag));
// 
// 	uint32 charCount = charListFile.GetSize() / sizeof(char16) - 1;
// 	char16* charListBuffer = new char16[charCount];
// 	charListFile.Read(charListBuffer, charCount * sizeof(charCount));
// 
// 	for (int i = 0; i < charCount; i++)
// 	{
// 		font->GetDisCharInfo(charListBuffer[i]);
// 	}

	return true;
}

void MyApp::Destroy()
{
	//SAFE_DELETE(m_font);

	SAFE_DELETE(m_copyTex);

	SAFE_DELETE(m_copyTech);
}

void MyApp::onWindowChangeSize(int32 width, int32 height)
{

}

bool MyApp::onMouseEvent(const MouseEvent& event)
{
	return false;
}

bool MyApp::onKeyboardEvent(const KeyEvent& event)
{
	return false;
}

void MyApp::onControlCreate(const char8* name, int32 v1, float32 v2)
{

}

void MyApp::onControlValueChange(const char8* name, int32 v1, float32 v2)
{
}

void MyApp::Update(float32 fElapsed)
{

}

void MyApp::onRender(float32 elapsed)
{
	m_renderer->BindProgram(m_copyShader);

	sRenderMgr->DrawFullScreenQuad(m_copyTex);

// 	const char8 text[] = u8"已加载“C:\\Windows\\System32\\comdlg32.dll”。无法查找或打开 PDB 文件。";
// 	SpriteBrush brush;
// 	brush.scale = 1.0f;	//缩放系数...
// 	brush.color = 0xFFFFFFFF;
// 	brush.outlineColor = 0xFFFF0000;
// 	brush.outlineScale = 0.0f;
// 	brush.shadowColor = 0x00000000;
// 	brush.shadowOffsetX = 0;
// 	brush.shadowOffsetY = 0;
	//sRenderSpirite->RenderText(m_fontIndex, text, sizeof(text), brush, 100, 100);
}
