#pragma once
#include "transformation.h"
#include "ray.h"

class Camera
{
	Camera(ivec3 pixelDimensions, float pixelDistance, float pixelWidth)
	{

	}

	void lookAt(fvec3 origin, fvec3 pos);

	Ray getPixelRay(ivec3 pixel);

private:
	fmat3 m_dir; // direction of camera
};
