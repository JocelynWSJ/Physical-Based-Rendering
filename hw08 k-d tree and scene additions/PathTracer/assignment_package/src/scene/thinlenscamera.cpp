#include "thinlenscamera.h"
#include "warpfunctions.h"
#include <la.h>
#include <iostream>


Sampler TLsample(100, 0);

ThinLensCamera::ThinLensCamera():
    Camera(), lensRadius(0.5), focalDistance(29.5)
{
}

ThinLensCamera::ThinLensCamera(unsigned int w, unsigned int h):
    Camera(w, h), lensRadius(0.5), focalDistance(29.5)
{}

ThinLensCamera::ThinLensCamera(unsigned int w, unsigned int h, const Vector3f &e, const Vector3f &r, const Vector3f &worldUp):
    Camera(w,h,e,r,worldUp), lensRadius(0.5), focalDistance(29.5)
{
    RecomputeAttributes();
}

ThinLensCamera::ThinLensCamera(const Camera &c):
    Camera(c), lensRadius(0.5), focalDistance(29.5)
{}

void ThinLensCamera::CopyAttributes(const Camera &c)
{
    Camera::CopyAttributes(c);
}

Ray ThinLensCamera::Raycast(const Point2f &pt) const
{
    return Raycast(pt.x, pt.y);
}

Ray ThinLensCamera::Raycast(float x, float y) const
{
    float ndc_x = (2.f*x/width - 1);
    float ndc_y = (1 - 2.f*y/height);
    return RaycastNDC(ndc_x, ndc_y);
}

Ray ThinLensCamera::RaycastNDC(float ndc_x, float ndc_y) const
{
    glm::vec3 P = ref + ndc_x*H + ndc_y*V;
    Ray result(eye, glm::normalize(P - eye));


//    std::cout << "\n\nRaycastNDC lensRadius: " << lensRadius;

    //thin film camera pbrtv3 page 375
    if(lensRadius > 0) {
        //world to camera, aka inverse of camera to world aka view matrix
        glm::mat4 worldtocam = glm::lookAt(eye, ref, up);
        glm::vec4 corigin  = worldtocam * glm::vec4(result.origin, 1.0f);
        glm::vec4 cdirection = worldtocam * glm::vec4(result.direction, 0.0f);

        result = Ray(glm::vec3(corigin), glm::vec3(cdirection));

        Point3f pLens = lensRadius *
                WarpFunctions::squareToDiskConcentric(TLsample.Get2D());
//        std::cout << "\n\nlensRadius: " << lensRadius << "\npLens: " << pLens.x << " " << pLens.y << " " << pLens.z;
        Float ft = glm::abs(focalDistance / result.direction.z);
        Point3f pFocus = result.origin + ft*result.direction;
        result.origin = pLens;
        result.direction = glm::normalize(pFocus - result.origin);

        //camera to world
        glm::mat4 camtoworld = glm::inverse(worldtocam);
        glm::vec4 worigin = camtoworld * glm::vec4(result.origin, 1.0f);
        glm::vec4 wdirection = camtoworld * glm::vec4(result.direction, 0.0f);

        result = Ray(glm::vec3(worigin), glm::vec3(wdirection));
    }
    return result;
}


