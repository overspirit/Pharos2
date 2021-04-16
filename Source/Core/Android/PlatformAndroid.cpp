#include "PreCompile.h"
#include "Pharos.h"

#include <jni.h>
#include <string.h>
#include <iostream>
#include <dirent.h>

PlatformAndroid::PlatformAndroid()
{
}

PlatformAndroid::~PlatformAndroid()
{

}

bool PlatformAndroid::Init(android_app* app)
{
    m_app = app;
    ANativeActivity* activity = app->activity;

    m_window = new AndroidWindow(activity, app->window, app->config);

    sKernel->SetEngineHomePath("");
    sKernel->SetEngineBundlePath("");

    int width = ANativeWindow_getWidth(app->window);
    int height = ANativeWindow_getHeight(app->window);

    if (!sKernel->Init(m_window, width, height)) return false;

    m_timer.Reset();

	return true;
}

void PlatformAndroid::Destroy()
{
    sKernel->Destroy();

	SAFE_DELETE(m_window);
}

int32 PlatformAndroid::Run()
{
    float32 fElapsed = m_timer.GetElapsedTime();

    int ret = sKernel->Run(fElapsed);

    if (ret < 0 && !m_window->should_close())
    {
        m_window->close();
    }
    else if (ret > 0)
    {
        this->Destroy();

        m_app->userData = NULL;
    }

	return ret;
}

void PlatformAndroid::onWindowChangeSize(int32 width, int32 height)
{
    sKernel->onWindowChangeSize(width, height);
}

void PlatformAndroid::onMouseEvent(int action, int32 posX, int32 posY, int32 wheel)
{
    CODE_STATE codeState = STATE_KEEP;

    if (action == AMOTION_EVENT_ACTION_DOWN)
    {
        codeState = STATE_DOWN;

        m_mousePos.x = posX;
        m_mousePos.y = posY;
    }
    else if(action == AMOTION_EVENT_ACTION_UP)
    {
        codeState = STATE_UP;
    }
    else if(action == AMOTION_EVENT_ACTION_MOVE)
    {
        codeState = STATE_KEEP;
    }

    MouseEvent event;
    event.button = MOUSE_LEFT;
    event.state = codeState;
    event.x = posX;
    event.y = posY;
    event.ox = posX - m_mousePos.x;
    event.oy = posY - m_mousePos.y;
    event.wheel = wheel * 120;
    event.shift = false;
    event.ctrl = false;
    event.alt = false;

    m_mousePos.x = posX;
    m_mousePos.y = posY;

    sKernel->onMouseEvent(event);

    LOGV("Event x:%d y:%d ox:%d oy:%d", posX, posY, event.ox, event.oy);
}