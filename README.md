# hookdemo_xposed
说明：
     基于xposed的hook框架，可以hook Java和 native，可以根据包名加载对应的hook.so

一、hook native

基于xposed注入android中，在加载特定app时加载自己的so，可实现native hook
1、根据目的使用JNI生成有特定hook目的的so

   1.1 native层的hook调用substrate 或者 hookZz函数
   
   1.2 dlopen目标so文件后，调用MSHookFunciotn 或者HookZZ进行hook
   
2、将生成的so copy到/data/local/kk/

3、根据要hook的包名和需要加载的so制作config.json放入/data/local/kk/

