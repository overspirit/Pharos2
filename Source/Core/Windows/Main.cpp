#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
#include "SceneGlobal.h"

using namespace Pharos;
using namespace Pharos::Core;

int main(int argc, char* argv[])
{
	//_CrtSetBreakAlloc(10750);

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//解析命令参数
	gflags::ParseCommandLineFlags(&argc, &argv, true);

	if (!sPlatform->Init()) return -1;

	int32 ret = sPlatform->Run();

	sPlatform->Destroy();

	gflags::ShutDownCommandLineFlags();

	return ret;
}