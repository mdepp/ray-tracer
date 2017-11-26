#include "application.h"
#include "raytracer.h"
#include "sphere.h"
#include "plane.h"
#include "util.h"
#include "vec.h"
#include "reverse_sphere.h"
#include <ctime>

Sphere s1(fvec3(0.f, 0.f, 13.f), 2.f, fvec3(1.f, 1.f, 1.f), 0.1f, 0.0f);
Sphere s2(fvec3(1.f, 3.f, 16.f), 1.f, fvec3(1.f, 0.f, 0.f), 0.1f, 0.0f);
Sphere s3(fvec3(3.f, 2.f, 14.f), 2.f, fvec3(0.0f, 0.3f, 1.f), .0f, 0.5f);

Plane p1(fvec3(12.f, 0.f, 0.f), fvec3(-1.f, 0.f, 0.f), fvec3(1.f, 1.f, 1.f), 0.0f, 0.f);
Plane p2(fvec3(-12.f, 0.f, 0.f), fvec3(1.f, 0.f, 0.f), fvec3(1.f, 1.f, 1.f), 0.0f, 0.f);
Plane p3(fvec3(0.f, 12.f, 0.f), fvec3(0.f, -1.f, 0.f), fvec3(1.f, 1.f, 1.f), 0.0f, 0.f);
Plane p4(fvec3(0.f, -12.f, 0.f), fvec3(0.f, 1.f, 0.f), fvec3(1.f, 1.f, 1.f), 0.0f, 0.f);
Plane p5(fvec3(0.f, 0.f, -2.f), fvec3(0.f, 0.f, 1.f), fvec3(1.f, 1.f, 1.f),  0.0f, 0.f);
Plane p6(fvec3(0.f, 0.f, 40.f), fvec3(0.f, 0.f, -1.f), fvec3(1.f, 1.f, 1.f), 0.0f, 0.f);

Application fw;
RayTracer<10, 50, 10> rt;

int main(int argc, char* argv[])
{

    util::debugPrint("Clearing");
    fw.clear({0.f, 0.f, 0.f});

    util::debugPrint("Creating objects");

    rt.addObject(&s1);
    rt.addObject(&s2);
    rt.addObject(&s3);

    rt.addObject(&p1);
    rt.addObject(&p2);
    rt.addObject(&p3);
    rt.addObject(&p4);
    //rt.addObject(&p5);
    rt.addObject(&p6);


    /*rt.addObject<Sphere>(fvec3(0.f, 0.f, 13.f), 2.f, fvec3(1.f, 1.f, 1.f), 0.7f);
    rt.addObject<Sphere>(fvec3(0.f, 3.f, 13.f), 1.f, fvec3(1.f, 0.f, 0.f), 0.7f);
    rt.addObject<Sphere>(fvec3(3.f, 2.f, 14.f), 2.f, fvec3(0.0f, 0.3f, 1.f), 0.7f);

    //rt.addObject<ReverseSphere>(fvec3(0.f, 0.f, 13.5f), 20.f, fvec3(1.f, 1.f, 1.f), 0.3f);

    rt.addObject<Plane>(fvec3(12.f, 0.f, 0.f), fvec3(-1.f, 0.f, 0.f), fvec3(1.f, 1.f, 1.f), 0.7f);
    rt.addObject<Plane>(fvec3(-12.f, 0.f, 0.f), fvec3(1.f, 0.f, 0.f), fvec3(1.f, 1.f, 1.f), 0.7f);
    rt.addObject<Plane>(fvec3(0.f, 12.f, 0.f), fvec3(0.f, -1.f, 0.f), fvec3(1.f, 1.f, 1.f), 0.7f);
    rt.addObject<Plane>(fvec3(0.f, -12.f, 0.f), fvec3(0.f, 1.f, 0.f), fvec3(1.f, 1.f, 1.f), 0.7f);
    rt.addObject<Plane>(fvec3(0.f, 0.f, -2.f), fvec3(0.f, 0.f, 1.f), fvec3(1.f, 1.f, 1.f),  0.7f);
    rt.addObject<Plane>(fvec3(0.f, 0.f, 22.f), fvec3(0.f, 0.f, -1.f), fvec3(1.f, 1.f, 1.f), 0.7f);*/


    //rt.addObject<Plane>(fvec3(0.f, 0.f, 10.f), fvec3(0.f, 0.f, 1.f), fvec3(0.f, 0.f, 0.f), 1.f);
    //rt.addObject<Plane>(fvec3(-3.f, 0.f, 5.f), fvec3(1.f, 0.f, 0.2f), fvec3(), 1.f);
    //rt.addObject<Plane>(fvec3(3.f, 0.f, 5.f), fvec3(1.f, 0.f, 0.2f), fvec3(), 1.f);

    //rt.addObject<Sphere>(fvec3(0.f, -3.f, 10.f), 2.f, fvec3(0.f, 1.f, 0.f), 0.7f);
    //rt.addObject<Sphere>(fvec3(3.f, 3.f, 14.f), 2.f, fvec3(1.f, 0.f, 0.f), 0.7f);
    //rt.addObject<Sphere>(fvec3(-3.f, 3.f, 18.f), 2.f, fvec3(0.f, 0.f, 1.f), 0.7f);
    //rt.addObject<Sphere>(fvec3(0.f, 0.f, 14.f), 1.f, fvec3(1.f, 1.f, 1.f), 0.3f);

    srand(std::time(0));
    for (int i = 0; i < 50; ++i)
    {
        fvec3 pos(rand()%20-10, rand()%20-10, rand()%40);
        rt.addPointLight(pos, 7.f, fvec3((rand()%100/100.f), (rand()%100/100.f), (rand()%100/100.f)));
    }
    //rt.setAmbientLight(fvec3(0.1f, 0.1f, 0.1f));

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
