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
Sphere s01(fvec3(0.f, 0.f, 13.f), 2.f, fvec3(1.f, 1.f, 1.f), 0.5f, 0.0);
Sphere s02(fvec3(0.f, -3.f, 13.f), 1.f, fvec3(1.f, 0.f, 0.f), 0.1f, 0.5);
Sphere s03(fvec3(2.f, -2.f, 10.f), 2.f, fvec3(0.0f, 0.3f, 1.f), 0.0f, 0.8, 1.003);
Plane p01(fvec3(12.f, 0.f, 0.f), fvec3(-1.f, 0.f, 0.f), fvec3(1.f, 1.f, 1.f), 0.0f, 0);
Plane p02(fvec3(-12.f, 0.f, 0.f), fvec3(1.f, 0.f, 0.f), fvec3(1.f, 1.f, 1.f), 0.0f, 0);
Plane p03(fvec3(0.f, 12.f, 0.f), fvec3(0.f, -1.f, 0.f), fvec3(1.f, 1.f, 1.f), 0.0f, 0);
Plane p04(fvec3(0.f, -12.f, 0.f), fvec3(0.f, 1.f, 0.f), fvec3(1.f, 1.f, 1.f), 0.0f, 0);
Plane p05(fvec3(0.f, 0.f, -2.f), fvec3(0.f, 0.f, 1.f), fvec3(1.f, 1.f, 1.f),  0.7f, 0);
Plane p06(fvec3(0.f, 0.f, 22.f), fvec3(0.f, 0.f, -1.f), fvec3(1.f, 1.f, 1.f), 0.7f, 0);

Sphere s11(fvec3(0.f, 0.f, 13.f), 2.f, fvec3(1.f, 1.f, 1.f), 0.0f, 0.0);
Plane p11(fvec3(0.f, 0.f, 15.f), fvec3(0.f, 0.f, -1.f), fvec3(1.f, 1.f, 1.f), 0.0f, 0);

Sphere s21(fvec3(0.f, 0.f, 13.f), 2.f, fvec3(1.f, 1.f, 1.f), 1.f);

// Window/event management
Application fw;

// Raytracer logic. Initialized with 10 max objects,
// 15 max point lights, 10 max directional lights, and
// recursion depth 10
RayTracer<10, 15, 10> rt(10);

// Camera
Camera cam(fw.width(), fw.height(), 10.f, 5.f);

void initScene(int);

int main(int argc, char* argv[])
{
    util::srand();
    int scene = 0;
    config::setFromFlags(argc, argv,
        "-s", [&](const char* s) -> void {util::srand(atol(s));},
        "-r", [&](const char* r) -> void {rt.setRecursionDepth(atol(r));},
        "-sc", [&](const char* sc) -> void {scene = atol(sc);}
    );

    util::debugPrint("Building scene");
    initScene(scene);

    // Clear the screen in preparation of drawing
    util::debugPrint("Clearing");
    fw.clear({0.0f, 0.0f, 0.0f});

    // Draw scene
    util::debugPrint("Drawing objects");
    if (!rt.render(&fw, &cam)) return 0;

    // Idle
    util::debugPrint("Finished drawing");
    bool running = true;
    while (running)
        running = fw.idle();

    return 0;
}

void initScene(int scene)
{
    // Camera is at (-10, -10, 0), looking at (0, 0, 10)
    cam.lookAt({ -10, -10, 0}, { 0, 0, 10 });

    if (scene == 0 || scene == 2)
    {
        if (scene == 2)
        {
            cam.lookAt({0, 0, 0}, { 0, 0, 10 });
        }
        // Add objects to scene
        rt.addObject(&s01);
        rt.addObject(&s02);
        rt.addObject(&s03);
        rt.addObject(&p01);
        rt.addObject(&p02);
        rt.addObject(&p03);
        rt.addObject(&p04);
        rt.addObject(&p05);
        rt.addObject(&p06);

        // Create random lights, and add them to the scene
        for (int i = 0; i < 15; ++i)
        {
            fvec3 pos(util::random(20)-10, util::random(20)-10, util::random(20));
            rt.addPointLight(pos, 30.f, fvec3((util::random(100)/100.f), (util::random(100)/100.f), (util::random(100)/100.f)));
        }
    }
    else if (scene == 1)
    {
        rt.addObject(&s11);
        rt.addObject(&p11);

        rt.addDirectionalLight(
            normalize(fvec3(1.f, 0.f, 1.f)),
            fvec3(.5f, .5f, .5f)
        );

        rt.setAmbientLight(
            fvec3(0.3f, 0.3f, 0.3f)
        );
    }
    else if (scene == 3)
    {
        cam.lookAt({ 0, 0, 0}, { 0, 0, 10 });

        rt.addObject(&s21);
        rt.addObject(&p11);

        for(int i=0; i<10; ++i)
        {
            fvec3 pos(util::random(20)-10, util::random(20)-10, util::random(5)+10);
            rt.addPointLight(pos, 30.f, fvec3((util::random(100)/100.f), (util::random(100)/100.f), (util::random(100)/100.f)));
        }

    }
    else
    {
        util::debugPrint("Unrecognized scene: ", scene, " -- reverting to scene 0");
        initScene(0);
    }
}
