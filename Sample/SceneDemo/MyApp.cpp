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


bool is_str_utf8(const char* str)
{
	unsigned int nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节
	unsigned char chr = *str;
	bool bAllAscii = true;
	for (unsigned int i = 0; str[i] != '\0'; ++i) {
		chr = *(str + i);
		//判断是否ASCII编码,如果不是,说明有可能是UTF8,ASCII用7位编码,最高位标记为0,0xxxxxxx
		if (nBytes == 0 && (chr & 0x80) != 0) {
			bAllAscii = false;
		}
		if (nBytes == 0) {
			//如果不是ASCII码,应该是多字节符,计算字节数
			if (chr >= 0x80) {
				if (chr >= 0xFC && chr <= 0xFD) {
					nBytes = 6;
				}
				else if (chr >= 0xF8) {
					nBytes = 5;
				}
				else if (chr >= 0xF0) {
					nBytes = 4;
				}
				else if (chr >= 0xE0) {
					nBytes = 3;
				}
				else if (chr >= 0xC0) {
					nBytes = 2;
				}
				else {
					return false;
				}
				nBytes--;
			}
		}
		else {
			//多字节符的非首字节,应为 10xxxxxx
			if ((chr & 0xC0) != 0x80) {
				return false;
			}
			//减到为零为止
			nBytes--;
		}
	}
	//违返UTF8编码规则
	if (nBytes != 0) {
		return false;
	}
	if (bAllAscii) { //如果全部都是ASCII, 也是UTF8
		return true;
	}
	return true;
}


int GetAllgpxFilepathFromfolder(char*  Path)
{
	char szFind[MAX_PATH];
	WIN32_FIND_DATAA FindFileData;
	strcpy(szFind, Path);
	strcat(szFind, "\\*.*");
	HANDLE hFind = FindFirstFileA(szFind, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)
		return -1;

	do
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (strcmp(FindFileData.cFileName, ".") != 0 && strcmp(FindFileData.cFileName, "..") != 0)
			{
				//发现子目录，递归之
				char szFile[MAX_PATH] = { 0 };
				strcpy(szFile, Path);
				strcat(szFile, "\\");
				strcat(szFile, FindFileData.cFileName);
				GetAllgpxFilepathFromfolder(szFile);
			}
		}
		else
		{
			string filePath = string(Path) + "/" + FindFileData.cFileName;
			Utils::Path upath(filePath.c_str());
			string ext = upath.GetFileExtension();
			if (ext != ".cpp" && ext != ".h" && ext != ".mm") continue;

			//找到文件，处理之
			std::cout << Path << "\\" << FindFileData.cFileName << std::endl;

			File file;
			file.Open(filePath.c_str());

			uint32 fileSize = file.GetSize();

			char* buffer = new char[fileSize + 1];
			buffer[fileSize + 1] = { 0 };

			file.Read(buffer, fileSize);

			if ((uint8)buffer[0] == 0xef) continue;

			char title[3] = { 0xef, 0xbb, 0xbf };

			file.Seek(0, EFST_BEGIN);

			file.Write(title, 3);

			file.Write(buffer, fileSize);					
		}
	} while (FindNextFileA(hFind, &FindFileData));

	FindClose(hFind);

	return 0;
}

bool MyApp::Init()
{
	GetAllgpxFilepathFromfolder("D:/WorkSpace/Pharos2/Source");
	GetAllgpxFilepathFromfolder("D:/WorkSpace/Pharos2/Sample");
	GetAllgpxFilepathFromfolder("D:/WorkSpace/Pharos2/Tools");

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
	m_copyTex = m_renderer->LoadTexture("Model/Earth/AtmosphericScattering.bmp");

	m_copyTech = sRenderMgr->GenerateRenderTechnique("Copy");
	m_copyShader = m_copyTech->GetPass(0)->GetShaderProgram();


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