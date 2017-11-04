#ifdef ARDUINO_BUILD
#include "ILI9341framework.h"
typedef ILI9341Framework Application;
#else
#include "sdlframework.h"
typedef SDLFramework Application;
#endif