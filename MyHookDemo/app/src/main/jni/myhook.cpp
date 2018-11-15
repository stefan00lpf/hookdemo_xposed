//
// Created by flamingo on 2018/11/14.
//

#include "myhook.h"

void hookLibc::init_kkk()
{
    LOGI("%s :kkk native hook is load ...", __FUNCTION__);
}

void _init()
{
    LOGI("%s:lib loaded ...", __FUNCTION__);
}