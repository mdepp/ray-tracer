#include "sdlframework.h"

int main(int argc, char* argv[])
{
    WindowFramework* fw = new SDLFramework();
    while(fw.tick());
    delete fw;
}