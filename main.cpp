#include "application.h"
#include "raytracer.h"
#include "sphere.h"
#include "plane.h"
#include "util.h"
#include "vec.h"

int main(int argc, char* argv[])
{
    Application fw;
    RayTracer<10, 1000, 10> rt;

    util::debugPrint("Clearing");
    fw.clear({0.f, 0.f, 0.f});

    util::debugPrint("Creating objects");
    rt.addObject<Sphere>(fvec3(0.f, 0.f, -13.f), 2.f, fvec3(1.f, 1.f, 1.f), 0.7f);
    rt.addObject<Sphere>(fvec3(0.f, 3.f, -13.f), 1.f, fvec3(1.f, 0.f, 0.f), 0.7f);
    rt.addObject<Sphere>(fvec3(3.f, 2.f, -14.f), 2.f, fvec3(0.0f, 0.3f, 1.f), 0.7f);
    //rt.addObject<Plane>(fvec3(10.f, 0.f, 0.f), fvec3(-1.f, 0.f, 0.f), fvec3(0.f, 0.f, 1.f), 0.0f);
    //rt.addObject<Plane>(fvec3(-10.f, 0.f, 0.f), fvec3(1.f, 0.f, 0.f), fvec3(0.f, 0.f, 1.f), 0.0f);
    //rt.addObject<Plane>(fvec3(0.f, 3.f, 0.f), fvec3(0.f, -1.f, 0.f), fvec3(0.f, 0.f, 0.f), 1.f);
    //rt.addObject<Plane>(fvec3(0.f, -3.f, 0.f), fvec3(0.f, 1.f, 0.f), fvec3(0.f, 0.f, 0.f), 1.f);

    rt.addObject<Plane>(fvec3(0.f, 0.f, 10.f), fvec3(0.f, 0.f, 1.f), fvec3(0.f, 0.f, 0.f), 1.f);
    rt.addObject<Plane>(fvec3(-3.f, 0.f, 5.f), fvec3(1.f, 0.f, 0.2f), fvec3(), 1.f);
    rt.addObject<Plane>(fvec3(3.f, 0.f, 5.f), fvec3(1.f, 0.f, 0.2f), fvec3(), 1.f);

    //rt.addObject<Sphere>(fvec3(0.f, -3.f, 10.f), 2.f, fvec3(0.f, 1.f, 0.f), 0.7f);
    //rt.addObject<Sphere>(fvec3(3.f, 3.f, 14.f), 2.f, fvec3(1.f, 0.f, 0.f), 0.7f);
    //rt.addObject<Sphere>(fvec3(-3.f, 3.f, 18.f), 2.f, fvec3(0.f, 0.f, 1.f), 0.7f);
    //rt.addObject<Sphere>(fvec3(0.f, 0.f, 14.f), 1.f, fvec3(1.f, 1.f, 1.f), 0.3f);

    for (int i = 0; i < 100; ++i)
    {
        fvec3 pos(rand()%20-10, rand()%20-10, -rand()%20);
        rt.addPointLight(pos, 0.9f, fvec3((rand()%100/100.f), (rand()%100/100.f), (rand()%100/100.f)));
    }
    //rt.setAmbientLight(fvec3(0.5f, 0.5f, 0.5f));

    //rt.addLight(fvec3(-10.f, 10.f, 15.f), 100.f, fvec3(1.f, 1.f, 1.f));
    //rt.addLight(fvec3(10.f, 0.f, 10.f), 100.f, fvec3(1.f, 1.f, 1.f));
    //rt.addLight(fvec3(0.f, 0.f, 0.f), 200.f, fvec3(1.f, 1.f, 1.f));


    util::debugPrint("Drawing objects");
    if (!rt.render(&fw)) return 0;

    util::debugPrint("Finshed drawing");
    bool running = true;
    while (running)
        running = fw.idle();

    return 0;
}
