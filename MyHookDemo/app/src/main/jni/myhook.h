//
// Created by flamingo on 2018/11/14.
//

#ifndef MYHOOKDEMO_MYHOOK_H
#define MYHOOKDEMO_MYHOOK_H

#include <stdio.h>
#include <android/log.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

    void _init(void);

#ifdef __cplusplus
}
#endif


using namespace std;

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "kkk_native", __VA_ARGS__))
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "kkk_native", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "kkk_native", __VA_ARGS__))

class hookLibc {
public:
    static void __attribute__((constructor)) init_kkk();
    hookLibc()
    {
        LOGI("c constructor");
    }
};

#endif //MYHOOKDEMO_MYHOOK_H
