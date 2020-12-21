#include "PreCompile.h"
#include "Pharos.h"

int32_t onInputEvent(android_app *app, AInputEvent *input_event)
{
    int32_t eventType = AInputEvent_getType(input_event);

    if (eventType == AINPUT_EVENT_TYPE_KEY)
    {

    }
    else if (eventType == AINPUT_EVENT_TYPE_MOTION)
    {
        int32_t actionMask = AMotionEvent_getAction(input_event);
        int32_t action = actionMask & AMOTION_EVENT_ACTION_MASK;
        int32_t pointIndex = (actionMask & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

        size_t pointerCount = AMotionEvent_getPointerCount(input_event);

        float x = AMotionEvent_getX(input_event, pointIndex);
        float y = AMotionEvent_getY(input_event, pointIndex);

        LOGV("Event pointerCount:%d pointIndex:%d action:%d x:%f y:%f", pointerCount, pointIndex, action, x, y);

        sPlatform->onMouseEvent(action, (int32)x, (int32)y, 0);
    }

    return 1;
}

void launchJavaActivity(ANativeActivity* activity);

void android_main(android_app* app)
{
    //app->onAppCmd                                  = on_app_cmd;
    app->onInputEvent                              = onInputEvent;
//    app->activity->callbacks->
//    app->activity->callbacks->onContentRectChanged = onContentRectChanged;
//    app->activity->callbacks->onWindowFocusChanged = onWindowFocusChanged;
//    app->activity->callbacks->onNativeWindowCreated = onNativeWindowCreated;
//    app->activity->callbacks->onNativeWindowResized = onNativeWindowResized;

    while (true)
    {
        android_poll_source* source = nullptr;

        int events = 0;
        int ident = 0;;

        while ((ident = ALooper_pollAll(0, nullptr, &events, (void**)&source)) >= 0)
        {
            if (source)
            {
                source->process(app, source);
            }
        }

        if (app->destroyRequested != 0)
        {
            break;
        }

        if (app->window == nullptr) continue;

        if (app->userData == nullptr)
        {
            int width = ANativeWindow_getWidth(app->window) / 2;
            int height = ANativeWindow_getHeight(app->window) / 2;

            if (width == 0 || height == 0) continue;

            ARect& contentRect = app->contentRect;
            contentRect.left = 0;
            contentRect.top = 0;
            contentRect.right = width;
            contentRect.bottom = height;

            sPlatform->Init(app);

            app->userData = (void*)0x1234;
        }

        sPlatform->Run();
    }

    sPlatform->Destroy();
}
