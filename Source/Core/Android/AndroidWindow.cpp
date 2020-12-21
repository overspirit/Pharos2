#include "PreCompile.h"
#include "Pharos.h"

AndroidWindow::AndroidWindow(ANativeActivity* activity, ANativeWindow*& window, AConfiguration* config) : m_handle{window}
{
	m_activity = activity;
	m_config = config;
	m_isFinish = false;
}

AndroidWindow::~AndroidWindow()
{

}

VkSurfaceKHR AndroidWindow::create_surface(VkInstance instance)
{
	if (instance == VK_NULL_HANDLE || !m_handle)
	{
		return VK_NULL_HANDLE;
	}

	VkAndroidSurfaceCreateInfoKHR info = {};
	info.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
	info.window = m_handle;

	VkSurfaceKHR surface = VK_NULL_HANDLE;
	vkCreateAndroidSurfaceKHR(instance, &info, nullptr, &surface);

	return surface;
}

bool AndroidWindow::should_close()
{
	return m_isFinish ? true : m_handle == nullptr;
}

const char* AndroidWindow::get_surface_extension()
{
    return VK_KHR_ANDROID_SURFACE_EXTENSION_NAME;
}

void AndroidWindow::close()
{
	jint result = JNI_ERR;

	JavaVM* javaVM = m_activity->vm;
	JNIEnv* javaEnv = NULL;

	result = javaVM->AttachCurrentThread(&javaEnv, NULL);
	if (result == JNI_ERR)
	{
		return;
	}

	jobject activityClazz = m_activity->clazz;
	jclass nativeActivityClass = javaEnv->GetObjectClass(activityClazz);

	jmethodID finishActivityMethodId = javaEnv->GetMethodID(nativeActivityClass, "finishActivity", "()V");
	if(finishActivityMethodId == 0)
	{
		return;
	}

	javaEnv->CallVoidMethod(activityClazz, finishActivityMethodId);

	javaVM->DetachCurrentThread();

    //ANativeActivity_finish(m_activity);

	m_isFinish = true;
}

float AndroidWindow::get_dpi_factor() const
{
    return AConfiguration_getDensity(m_config) / static_cast<float>(ACONFIGURATION_DENSITY_MEDIUM);
}