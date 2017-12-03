/*
 * File: main.cpp
 * Authors: Alexander Epp (1487716) and Mitchell Epp (1498821)
 * Project: CMPUT274 Final Project
 * Description: Program entry point. Constructs a scene from Spheres,
 *              Planes and a Camera, which it renders using RayTracer onto
 *              a window managed by Application.
 */

#include "application.h"
#include "config.h"
#include "raytracer.h"
#include "sphere.h"
#include "plane.h"
#include "util.h"
#include "vec.h"
#include "camera.h"

// Declaring objects in the global scope allows avr-g++ to
// more accurately estimate the program's memory requirements,
// potentially catching memory errors at build time.

// Geometry to be rendered
Sphere s1(fvec3(0.f, 0.f, 13.f), 2.f, fvec3(1.f, 1.f, 1.f), 0.0f, 0.5);
Sphere s2(fvec3(0.f, -3.f, 13.f), 1.f, fvec3(1.f, 0.f, 0.f), 0.1f, 0.5);
Sphere s3(fvec3(2.f, -2.f, 10.f), 2.f, fvec3(0.0f, 0.3f, 1.f), 0.1f, 0.8);

Plane p1(fvec3(12.f, 0.f, 0.f), fvec3(-1.f, 0.f, 0.f), fvec3(1.f, 1.f, 1.f), 0.0f, 0);
Plane p2(fvec3(-12.f, 0.f, 0.f), fvec3(1.f, 0.f, 0.f), fvec3(1.f, 1.f, 1.f), 0.0f, 0);
Plane p3(fvec3(0.f, 12.f, 0.f), fvec3(0.f, -1.f, 0.f), fvec3(1.f, 1.f, 1.f), 0.0f, 0);
Plane p4(fvec3(0.f, -12.f, 0.f), fvec3(0.f, 1.f, 0.f), fvec3(1.f, 1.f, 1.f), 0.0f, 0);
Plane p5(fvec3(0.f, 0.f, -2.f), fvec3(0.f, 0.f, -1.f), fvec3(1.f, 1.f, 1.f),  0.7f, 0);
Plane p6(fvec3(0.f, 0.f, 22.f), fvec3(0.f, 0.f, 1.f), fvec3(1.f, 1.f, 1.f), 0.7f, 0);

// Window/event management
Application fw;

// Raytracer logic. Initialized with 10 max objects,
// 15 max point lights, 10 max directional lights, and
// recursion depth 10
RayTracer<10, 15, 10> rt(10);

// Camera
Camera cam(fw.width(), fw.height(), 10.f, 5.f);

int main(int argc, char* argv[])
{
    util::srand();
    config::setFromFlags(argc, argv, "-s", [&](auto s) {util::srand(atol(s));}, "-r", [&](auto r) {rt.setRecursionDepth(atol(r));});

    // Camera is at (-10, -10, 0), looking at (0, 0, 10)
    cam.lookAt({ -10, -10, 0}, { 0, 0, 10 });

    // Clear the screen in preparation of drawing
    util::debugPrint("Clearing");
    fw.clear({0.f, 0.f, 0.f});

    // Add objects to scene
    util::debugPrint("Adding objects");
    rt.addObject(&s1);
    rt.addObject(&s2);
    rt.addObject(&s3);
    rt.addObject(&p1);
    rt.addObject(&p2);
    rt.addObject(&p3);
    rt.addObject(&p4);
    rt.addObject(&p5);
    rt.addObject(&p6);

    // Create random lights, and add them to the scene
    for (int i = 0; i < 15; ++i)
    {
        fvec3 pos(util::random(20)-10, util::random(20)-10, util::random(20));
        rt.addPointLight(pos, 30.f, fvec3((util::random(100)/100.f), (util::random(100)/100.f), (util::random(100)/100.f)));
    }

    // Draw scene
    util::debugPrint("Drawing objects");
    if (!rt.render(&fw, &cam)) return 0;

    // Idle
    util::debugPrint("Finshed drawing");
    bool running = true;
    while (running)
        running = fw.idle();

    return 0;
}
