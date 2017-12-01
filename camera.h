/*
 * File: camera.h
 * Authors: Alexander Epp (1487716) and Mitchell Epp (1498821)
 * Project: CMPUT274 Final Project
 * Description: A camera class for translating pixels to rays
 */

#pragma once
#include "transformation.h"
#include "ray.h"

/*
 * Camera converts screen pixels to rays in worldspace. Since this is the 
 * opposite of most 3D cameras (which convert points in worldspace to camera
 * space), the transformations used are the inverse of what might be expected.
 * 
 * The camera works by setting up a virtual screen at some distance behind the
 * origin, with pixels corresponding to the real screen pixels. Then when given
 * a real screen pixel, it transforms the corresponding virtual screen pixel to
 * worldspace and returns a ray from the origin to the corresponding spot on the
 * virtual screen.
 */
class Camera
{
public:
    /*
     * Initialize the camera. This has several parameters
     *     nPixelsX: length in pixels of virtual screen
     *     nPixelsY: height in pixels of virtual screen
     *     viewportWidth: width of virtual screen (height is determined by aspect
     *                    ratio). A larger width will stretch out the virtual
     *                    screen pixels to take up a larger area.
     *     viewportDepth: the distance of the (untransformed) virtual screen
     *                    from the origin. Along with 'viewportWidth' this
     *                    effectively controls the fov of the camera.
     *     up: needed for new coordinate calculations in lookAt, etc.
     */
    Camera(int nPixelsX, int nPixelsY, float viewportWidth, float viewportDepth, fvec3 up = { 0,1,0 }) noexcept;

    /*
     * Set up the camera transforms to look at a target from a certain eye
     * position.
     */
    void lookAt(fvec3 eye, fvec3 target) noexcept;

    /*
     * Get the ray cast from the camera (in worldspace) correspoding to a given
     * real screen pixel.
     */
    Ray getPixelRay(int pixelX, int pixelY) const noexcept;
    
    // Get dimensions of virtual (and also real) screen
    int nPixelsX() const noexcept;
    int nPixelsY() const noexcept;

private:
    int m_nPixelsX, m_nPixelsY;
    float m_viewportWidth, m_viewportHeight, m_viewportDepth;

	ftrans3 m_dir; // Direction of camera
    ftrans3 m_pos; // Position of camera

    fvec3 m_up;
};
