#include <stdio.h>

#include <android/log.h>
#include <unistd.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <stdarg.h>
#include "fcntl.h"

#include "hook.h"
#include "hookzz.h"

#define define_substrate 0
#define INVALID_FUNC_ADDR NULL

uint32_t (*CCScheduler_update_ori)(void *, float) = NULL;
uint32_t CCScheduler_update_replace(void *a1, float a2)
{
    // LOGD("%s:%f", __FUNCTION__, a2);
    return CCScheduler_update_ori(a1, 5*a2);
}

int (*open_ori)(char *pathname, int flags, ...) = NULL;
int open_replace(char *pathname, int flags, ...)
{
    va_list ap = {0};
    mode_t mode = 0;

    if((flags & O_CREAT) != 0)
    {
        va_start(ap, flags);
        mode = va_arg(ap, int);
        va_end(ap);    
    }


    LOGI("open: %s", pathname);
    /** read /proc/pid/maps **/
    if(strstr(pathname, "maps") && strstr(pathname, "/proc"))
    {
        /* 构造文件 */
        char* buff[512] = {0};
        char* buff_r[512] = {0};
        FILE* fp_r = fopen(pathname, "rb");
        FILE* fp_w = fopen("/data/data/io.virtualapp/xx/fake", "wb");

        if(fp_r == NULL || fp_w == NULL)
        {
            LOGI("open file error");
        }
        else
        {
            while(fgets(buff, 512, fp_r) != NULL)
            {
                char* str = strstr(buff, "/data/data/io.virtualapp/virtual/data/app/");
                if(strstr(buff, "/data/data/io.virtualapp/virtual/data/app/"))
                {
                    memset(buff_r, 512, 0);
                    sprintf(buff_r, "/data/data/%s", str);
                    fputs(buff_r, fp_w);
                    LOGI("open: %s", buff_r);
                }
                else
                {
                    fputs(buff, fp_w);
                }
            }

        }
        
        fclose(fp_r);
        fclose(fp_w);

        return open_ori("/data/data/io.virtualapp/xx/fake", flags);
    }

    return open_ori(pathname, flags, mode);
}


void _init(char *args)
{
    LOGI("kkk _init is start");

    if(define_substrate)
    {
        void *handler1 = dlopen("/system/lib/libsubstrate.so", RTLD_NOW);
        char *err = (char *)dlerror();
        if(err != NULL) 
            LOGI("dlopen error is:%s\n",err);

        void (*MSHookFunction)(void *symbol, void *replace, void **result) = INVALID_FUNC_ADDR;
        MSHookFunction = dlsym(handler1, "MSHookFunction");
        if(MSHookFunction == INVALID_FUNC_ADDR) {
            LOGD("can't find MSHookFunction");
        }
    }

    pid_t pid = getpid();
    LOGI("the target pid is %d", pid);
    LOGI("the target name is %s", get_process_name(pid));

    void* open_target = dlsym(RTLD_DEFAULT, "open");
    if(open_target != NULL)
    {
        ZzHook(open_target, (void *)&open_replace, (void **)&open_ori, NULL, NULL, false);
    }
    else
    {
        LOGI("hook open error");
    }

    // char* so_dir = "/data/data/com.tencent.KiHan/lib/libtersafe.so";
    // void* handler = dlopen(so_dir, RTLD_NOW);
    // char* err = (char*)dlerror();
    // if (handler == NULL)
    //     LOGD("dlopen libtersafe.so error is:%s\n",err);
    // if(handler)
    // {
    //     void* module_base = get_module_base(pid, "/data/app-lib/com.tencent.KiHan-1/libtersafe.so");
    //     LOGD("libgame.so module_base :%x", (uint32_t)module_base);

    //     // void* CCScheduler_update_target = module_base + 0x2266B0 + 1;//如果是thumb指令 + 1
    //     // ZzHook((void *)CCScheduler_update_target, (void *)&CCScheduler_update_replace, (void **)&CCScheduler_update_ori, NULL, NULL, false);

    //     // void* func_target = module_base + 0xAF4A0;//如果是thumb指令 + 1
    //     // ZzHook((void *)func_target, (void *)&func_replace, (void **)&func_ori, NULL, NULL, false);
        
    // }

}


void so_entry(char *p)
{
    pid_t pid = getpid();
    LOGI("the target pid is %d", pid);
    LOGI("the target name is %s", get_process_name(pid));
}
