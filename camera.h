#pragma once
#include "transformation.h"
#include "ray.h"

class Camera
{
public:
    Camera(int nPixelsX, int nPixelsY, float viewportWidth, float viewportDepth, fvec3 up = { 0,1,0 }) noexcept
        : m_nPixelsX(nPixelsX), m_nPixelsY(nPixelsY),
          m_viewportWidth(viewportWidth), m_viewportHeight(m_viewportWidth / float(m_nPixelsX)*m_nPixelsY),
          m_viewportDepth(viewportDepth),
          m_up(up)
	{
	}

    void lookAt(fvec3 eye, fvec3 target) noexcept
    {
        /*
         * Loosely based off of https://github.com/toji/gl-matrix/issues/225
         * (to be precise, I copied some of the code wrong, and essentially re-wrote
         * the entire thing while fixing it).
         */

        m_pos = translate(eye);

        auto Z = normalize(target - eye);
        auto X = cross(m_up, Z);
        auto Y = cross(Z, X);

        m_dir.m_e1 = fvec4(X.x, X.y, X.z, 0);
        m_dir.m_e2 = fvec4(Y.x, Y.y, Y.z, 0);
        m_dir.m_e3 = fvec4(Z.x, Z.y, Z.z, 0);
        m_dir.m_e4 = fvec4(0, 0, 0, 1);
    }


    Ray getPixelRay(int pixelX, int pixelY) const noexcept
    {
        float x = (pixelX - m_nPixelsX / 2)*m_viewportWidth / m_nPixelsX;
        float y = (pixelY - m_nPixelsY / 2)*m_viewportHeight / m_nPixelsY;

        fvec3 dirToPixel{ x, y, m_viewportDepth };
        return { m_pos.transform({0, 0, 0}), /*reflectNormalized*/(normalize(m_dir*dirToPixel)) };
    }
    
    int nPixelsX() const noexcept { return m_nPixelsX; }
    int nPixelsY() const noexcept { return m_nPixelsY; }

private:
    int m_nPixelsX, m_nPixelsY;
    float m_viewportWidth, m_viewportHeight, m_viewportDepth;

	ftrans3 m_dir; // direction of camera
    ftrans3 m_pos; // position of camera

    fvec3 m_up;
};
