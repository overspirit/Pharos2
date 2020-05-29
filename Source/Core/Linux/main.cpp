#include "PreCompile.h"
#include "Pharos.h"

int main(int argc, char *argv[])
{
	printf("Hello Vulkan");

	if (!sPlatform->Init()) return -1;

	int32 ret = sPlatform->Run();

	sPlatform->Destroy();

	return ret;
}
