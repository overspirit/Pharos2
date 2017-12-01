#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
#include "SceneGlobal.h"

using namespace Pharos;
using namespace Pharos::Core;

int main(int argc, char* argv[])
{
	PlatformWindows platform;
	
	platform.Init();
	platform.Run();
	platform.Destroy();
	
	return 0;
}