#include "sdlframework.h"
#include "sphere.h"
#include "vec.h"

int main(int argc, char* argv[])
{
    WindowFramework* fw = new SDLFramework();

    const float viewportWidth = 10.f;
    const float viewportHeight = viewportWidth/float(fw->width())*fw->height();
    const float viewportDepth = 10.f;

    fw->clear({0.f, 0.f, 0.f});
    
    const int nObjects = 3;
    Object* objects[nObjects];
    objects[0] = new Sphere({0.f, 0.f, 13.f}, 2.f);
    objects[1] = new Sphere({ 0.f, 3.f, 13.f }, 1.f);
    objects[2] = new Sphere({ 3.f, 2.f, 12.f }, 2.f);

    bool running = true;

    for (int pixelX=0; pixelX<fw->width(); ++pixelX)
    {
        for (int pixelY=0; pixelY<fw->height(); ++pixelY)
        {
            float x = (pixelX-fw->width()/2)*viewportWidth/fw->width();
            float y = (pixelY-fw->height()/2)*viewportHeight/fw->height();
            
            Ray ray({0, 0, 0}, normalize(vec3<>(x, y, viewportDepth)));

            vec3<> intersection;
            for(auto object : objects)
            {
                if (object && object->intersect(ray, intersection))
                {
                    fw->drawPixel(pixelX, pixelY, {1.f, 1.f, 1.f});
                }
            }

            running = fw->tick();
            if (!running) break;
        }
        if (!running) break;
    }

    while (running)
        running = fw->tick();

    delete fw;
    for(auto object : objects)
        if (object)
            delete object;
    
    return 0;
}