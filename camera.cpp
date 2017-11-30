#include "camera.h"

Camera::Camera(int nPixelsX, int nPixelsY, float viewportWidth, float viewportDepth, fvec3 up = { 0,1,0 }) noexcept
  : m_nPixelsX(nPixelsX), m_nPixelsY(nPixelsY),
    m_viewportWidth(viewportWidth), m_viewportHeight(m_viewportWidth / float(m_nPixelsX)*m_nPixelsY),
    m_viewportDepth(viewportDepth),
    m_up(up)
{
}

void Camera::lookAt(fvec3 eye, fvec3 target) noexcept
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

Ray Camera::getPixelRay(int pixelX, int pixelY) const noexcept
{
    // Camera space coordinates
    float x = (pixelX - m_nPixelsX / 2)*m_viewportWidth / m_nPixelsX;
    float y = (pixelY - m_nPixelsY / 2)*m_viewportHeight / m_nPixelsY;
    // Direction vector in camera space coordinates
    fvec3 dirToPixel{ x, y, m_viewportDepth };
    // Transform to worldspace
    return { m_pos.transform({0, 0, 0}), normalize(m_dir*dirToPixel) };
}

int Camera::nPixelsX() const noexcept { return m_nPixelsX; }
int Camera::nPixelsY() const noexcept { return m_nPixelsY; }