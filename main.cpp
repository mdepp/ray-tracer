#include "application.h"
#include "raytracer.h"
#include "sphere.h"
#include "util.h"
#include "vec.h"

int main(int argc, char* argv[])
{
    Application fw;
    RayTracer<5, 5> rt;

    util::debugPrint("Clearing");
    fw.clear({0.f, 0.f, 0.f});

    util::debugPrint("Creating objects");
    rt.addObject<Sphere>(vec3<>(0.f, 0.f, 13.f), 2.f, vec3<>(1.f, 1.f, 1.f), 0.7f);
    rt.addObject<Sphere>(vec3<>(0.f, 3.f, 13.f), 1.f, vec3<>(1.f, 0.f, 0.f), 0.7f);
    rt.addObject<Sphere>(vec3<>(3.f, 2.f, 14.f), 2.f, vec3<>(0.0f, 0.3f, 1.f), 0.7f);

    //rt.addObject<Sphere>(vec3<>(0.f, -3.f, 10.f), 2.f, vec3<>(0.f, 1.f, 0.f), 0.7f);
    //rt.addObject<Sphere>(vec3<>(3.f, 3.f, 14.f), 2.f, vec3<>(1.f, 0.f, 0.f), 0.7f);
    //rt.addObject<Sphere>(vec3<>(-3.f, 3.f, 18.f), 2.f, vec3<>(0.f, 0.f, 1.f), 0.7f);
    //rt.addObject<Sphere>(vec3<>(0.f, 0.f, 14.f), 1.f, vec3<>(1.f, 1.f, 1.f), 0.3f);

    rt.addLight(vec3<>(0.f, 10.f, 15.f), 10.f, vec3<>(1.f, 1.f, 1.f));
    rt.addLight(vec3<>(10.f, 0.f, 10.f), 10.f, vec3<>(1.f, 1.f, 1.f));

    util::debugPrint("Drawing objects");
    if (!rt.render(&fw)) return 0;

    util::debugPrint("Finshed drawing");
    bool running = true;
    while (running)
        running = fw.tick();

    return 0;
}
