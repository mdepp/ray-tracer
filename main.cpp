#include "application.h"
#include "raytracer.h"
#include "sphere.h"
#include "plane.h"
#include "util.h"
#include "vec.h"

int main(int argc, char* argv[])
{
    Application fw;
    RayTracer<10, 1000> rt;

    util::debugPrint("Clearing");
    fw.clear({0.f, 0.f, 0.f});

    util::debugPrint("Creating objects");
    rt.addObject<Sphere>(vec3<>(0.f, 0.f, 13.f), 2.f, vec3<>(1.f, 1.f, 1.f), 0.7f);
    rt.addObject<Sphere>(vec3<>(0.f, 3.f, 13.f), 1.f, vec3<>(1.f, 0.f, 0.f), 0.7f);
    rt.addObject<Sphere>(vec3<>(3.f, 2.f, 14.f), 2.f, vec3<>(0.0f, 0.3f, 1.f), 0.7f);
    rt.addObject<Plane>(vec3<>(10.f, 0.f, 0.f), vec3<>(-1.f, 0.f, 0.f), vec3<>(1.f, 1.f, 1.f), 0.9f);
    rt.addObject<Plane>(vec3<>(-10.f, 0.f, 0.f), vec3<>(1.f, 0.f, 0.f), vec3<>(1.f, 1.f, 1.f), 0.9f);
    rt.addObject<Plane>(vec3<>(0.f, 10.f, 0.f), vec3<>(0.f, -1.f, 0.f), vec3<>(1.f, 1.f, 1.f), 0.9f);
    rt.addObject<Plane>(vec3<>(0.f, -10.f, 0.f), vec3<>(0.f, 1.f, 0.f), vec3<>(1.f, 1.f, 1.f), 0.9f);

    //rt.addObject<Sphere>(vec3<>(0.f, -3.f, 10.f), 2.f, vec3<>(0.f, 1.f, 0.f), 0.7f);
    //rt.addObject<Sphere>(vec3<>(3.f, 3.f, 14.f), 2.f, vec3<>(1.f, 0.f, 0.f), 0.7f);
    //rt.addObject<Sphere>(vec3<>(-3.f, 3.f, 18.f), 2.f, vec3<>(0.f, 0.f, 1.f), 0.7f);
    //rt.addObject<Sphere>(vec3<>(0.f, 0.f, 14.f), 1.f, vec3<>(1.f, 1.f, 1.f), 0.3f);

    for (int i = 0; i < 100; ++i)
    {
        vec3<> pos(rand()%20-10, rand()%20-10, rand()%20);
        rt.addLight(pos, 0.9f, vec3<>((rand()%100/100.f), (rand()%100/100.f), (rand()%100/100.f)));
    }

    //rt.addLight(vec3<>(-10.f, 10.f, 15.f), 100.f, vec3<>(1.f, 1.f, 1.f));
    //rt.addLight(vec3<>(10.f, 0.f, 10.f), 100.f, vec3<>(1.f, 1.f, 1.f));
    //rt.addLight(vec3<>(0.f, 0.f, 0.f), 200.f, vec3<>(1.f, 1.f, 1.f));


    util::debugPrint("Drawing objects");
    if (!rt.render(&fw)) return 0;

    util::debugPrint("Finshed drawing");
    bool running = true;
    while (running)
        running = fw.idle();

    return 0;
}
