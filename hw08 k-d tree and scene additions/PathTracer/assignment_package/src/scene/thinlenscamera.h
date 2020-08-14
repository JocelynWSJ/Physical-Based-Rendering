#ifndef THINLENSCAMERA_H
#define THINLENSCAMERA_H

#pragma once

#include <globals.h>

#include <raytracing/ray.h>
#include <openGL/drawable.h>
#include "camera.h"
#include "samplers/sampler.h"


class ThinLensCamera: public Camera
{
public:
    ThinLensCamera();
    ThinLensCamera(const Camera &c);
    ThinLensCamera(unsigned int w, unsigned int h);
    ThinLensCamera(unsigned int w, unsigned int h, const Vector3f &e, const Vector3f &r, const Vector3f &worldUp);

    Float lensRadius,focalDistance;

    void CopyAttributes(const Camera &c);

    virtual Ray Raycast(const Point2f &pt) const ;         //Creates a ray in 3D space given a 2D point on the screen, in screen coordinates.
    virtual Ray Raycast(float x, float y) const ;            //Same as above, but takes two floats rather than a vec2.
    virtual Ray RaycastNDC(float ndc_x, float ndc_y) const; //Creates a ray in 3D space given a 2D point in normalized device coordinates.


};

#endif // THINLENSCAMERA_H
