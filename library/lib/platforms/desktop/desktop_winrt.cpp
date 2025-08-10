#ifdef __WINRT__ 
typedef int (*main_func)(int argc, char* argv[]);
#ifdef __SDL2__
#include <SDL_main.h>
#include <SDL_syswm.h>
int WinRTRunApp(main_func mainFunction) {
    return SDL_WinRTRunApp(mainFunction, NULL);
}
#else
//TODO remove
int WinRTRunApp (main_func mainFunction) {
    return 0;
}
#endif // __SDL2__

#endif