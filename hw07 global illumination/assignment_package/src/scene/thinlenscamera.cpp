#include "thinlenscamera.h"
#include "warpfunctions.h"
#include <la.h>
#include <iostream>
ThinLensCamera::ThinLensCamera():
    Camera(),rng()
{
}

ThinLensCamera::ThinLensCamera(unsigned int w, unsigned int h):
    Camera(w, h),rng()
{}

ThinLensCamera::ThinLensCamera(unsigned int w, unsigned int h, const Vector3f &e, const Vector3f &r, const Vector3f &worldUp,Float lensr, Float focald):
    Camera(w,h,e,r,worldUp),lensRadius(lensr), focalDistance(focald), rng()
{
    RecomputeAttributes();
}

ThinLensCamera::ThinLensCamera(const Camera &c):
    Camera(c)
{}

void ThinLensCamera::CopyAttributes(const ThinLensCamera &c)
{
    fovy = c.fovy;
    near_clip = c.near_clip;
    far_clip = c.far_clip;
    eye = c.eye;
    ref = c.ref;
    look = c.look;
    up = c.up;
    right = c.right;
    width = c.width;
    height = c.height;
    aspect = c.aspect;
    V = c.V;
    H = c.H;
    lensRadius = c.lensRadius;
    focalDistance = c.focalDistance;
}

Ray ThinLensCamera::Raycast(const Point2f &pt)
{
    return Raycast(pt.x, pt.y);
}

Ray ThinLensCamera::Raycast(float x, float y)
{
    float ndc_x = (2.f*x/width - 1);
    float ndc_y = (1 - 2.f*y/height);
    return RaycastNDC(ndc_x, ndc_y);
}

Ray ThinLensCamera::RaycastNDC(float ndc_x, float ndc_y)
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
                WarpFunctions::squareToDiskConcentric(Point2f(rng.nextFloat(), rng.nextFloat()));
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

//ThinLensCamera::ThinLensCamera():
//    Camera()
//{
//    lensRadius = 3.5;
//    focalDistance = 19.5;
//}

//ThinLensCamera::ThinLensCamera(const Camera &c) :
//    Camera(c)
//{
//    lensRadius = 3.5;
//    focalDistance = 19.5;
//}

//ThinLensCamera::ThinLensCamera(unsigned int w, unsigned int h):
//    Camera(w, h)
//{
//    lensRadius = 3.5;
//    focalDistance = 19.5;
//}

//ThinLensCamera::ThinLensCamera(unsigned int w, unsigned int h, const Vector3f &e, const Vector3f &r, const Vector3f &worldUp, const float lensRadius, const float focalDistance):
//    Camera(w, h, e, r, worldUp),
//    lensRadius(lensRadius),
//    focalDistance(focalDistance)

//{
//    Camera::RecomputeAttributes();
//}


//void ThinLensCamera::CopyAttributes(const ThinLensCamera &c)
//{
//    fovy = c.fovy;
//    near_clip = c.near_clip;
//    far_clip = c.far_clip;
//    eye = c.eye;
//    ref = c.ref;
//    look = c.look;
//    up = c.up;
//    right = c.right;
//    width = c.width;
//    height = c.height;
//    aspect = c.aspect;
//    V = c.V;
//    H = c.H;
//    lensRadius = c.lensRadius;
//    focalDistance = c.focalDistance;
//}

//Ray ThinLensCamera::Raycast(const Point2f &pt)
//{
//    return ThinLensCamera::Raycast(pt.x, pt.y);
//}

//Ray ThinLensCamera::Raycast(float x, float y)
//{
//    float ndc_x = (2.f*x/width - 1);
//    float ndc_y = (1 - 2.f*y/height);
//    return ThinLensCamera::RaycastNDC(ndc_x, ndc_y);
//}

//Ray ThinLensCamera::RaycastNDC(float ndc_x, float ndc_y) const
//{
//    glm::vec3 P = ref + ndc_x*H + ndc_y*V;
//    Ray result(eye, glm::normalize(P - eye));

//    if(lensRadius > 0) {
//        //world to camera, aka inverse of camera to world aka view matrix
//        glm::mat4 worldtocam = glm::lookAt(eye, ref, up);
//        glm::vec4 corigin  = worldtocam * glm::vec4(result.origin, 1.0f);
//        glm::vec4 cdirection = worldtocam * glm::vec4(result.direction, 0.0f);

//        result = Ray(glm::vec3(corigin), glm::vec3(cdirection));

//        Sampler sample(100, 0);
//        Point3f pLens = lensRadius * WarpFunctions::squareToDiskConcentric(sample.Get2D());
//                std::cout << "\n\nlensRadius: " << lensRadius << "\npLens: " << pLens.x << " " << pLens.y << " " << pLens.z;
//        Float ft = glm::abs(focalDistance / result.direction.z);
//        Point3f pFocus = result.origin + ft*result.direction;
//        result.origin = pLens;
//        result.direction = glm::normalize(pFocus - result.origin);

//        //camera to world
//        glm::mat4 camtoworld = glm::inverse(worldtocam);
//        glm::vec4 worigin = camtoworld * glm::vec4(result.origin, 1.0f);
//        glm::vec4 wdirection = camtoworld * glm::vec4(result.direction, 0.0f);

//        result = Ray(glm::vec3(worigin), glm::vec3(wdirection));
//    }
//    return result;
//}

//void ThinLensCamera::create()
//{
//    std::vector<glm::vec3> pos;
//    std::vector<glm::vec3> col;
//    std::vector<GLuint> idx;

//    //0: Eye position
//    pos.push_back(eye);
//    //1 - 4: Near clip
//        //Lower-left
//        Ray r = this->RaycastNDC(-1,-1);
//        pos.push_back(eye + r.direction * near_clip);
//        //Lower-right
//        r = this->RaycastNDC(1,-1);
//        pos.push_back(eye + r.direction * near_clip);
//        //Upper-right
//        r = this->RaycastNDC(1,1);
//        pos.push_back(eye + r.direction * near_clip);
//        //Upper-left
//        r = this->RaycastNDC(-1,1);
//        pos.push_back(eye + r.direction * near_clip);
//    //5 - 8: Far clip
//        //Lower-left
//        r = this->RaycastNDC(-1,-1);
//        pos.push_back(eye + r.direction * far_clip);
//        //Lower-right
//        r = this->RaycastNDC(1,-1);
//        pos.push_back(eye + r.direction * far_clip);
//        //Upper-right
//        r = this->RaycastNDC(1,1);
//        pos.push_back(eye + r.direction * far_clip);
//        //Upper-left
//        r = this->RaycastNDC(-1,1);
//        pos.push_back(eye + r.direction * far_clip);

//    for(int i = 0; i < 9; i++){
//        col.push_back(glm::vec3(1,1,1));
//    }

//    //Frustum lines
//    idx.push_back(1);idx.push_back(5);
//    idx.push_back(2);idx.push_back(6);
//    idx.push_back(3);idx.push_back(7);
//    idx.push_back(4);idx.push_back(8);
//    //Near clip
//    idx.push_back(1);idx.push_back(2);
//    idx.push_back(2);idx.push_back(3);
//    idx.push_back(3);idx.push_back(4);
//    idx.push_back(4);idx.push_back(1);
//    //Far clip
//    idx.push_back(5);idx.push_back(6);
//    idx.push_back(6);idx.push_back(7);
//    idx.push_back(7);idx.push_back(8);
//    idx.push_back(8);idx.push_back(5);

//    //Camera axis
//    pos.push_back(eye); col.push_back(glm::vec3(0,0,1)); idx.push_back(9);
//    pos.push_back(eye + look); col.push_back(glm::vec3(0,0,1));idx.push_back(10);
//    pos.push_back(eye); col.push_back(glm::vec3(1,0,0));idx.push_back(11);
//    pos.push_back(eye + right); col.push_back(glm::vec3(1,0,0));idx.push_back(12);
//    pos.push_back(eye); col.push_back(glm::vec3(0,1,0));idx.push_back(13);
//    pos.push_back(eye + up); col.push_back(glm::vec3(0,1,0));idx.push_back(14);

//    count = idx.size();

//    bufIdx.create();
//    bufIdx.bind();
//    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
//    bufIdx.allocate(idx.data(), count * sizeof(GLuint));

//    bufPos.create();
//    bufPos.bind();
//    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
//    bufPos.allocate(pos.data(), pos.size() * sizeof(glm::vec3));

//    bufCol.create();
//    bufCol.bind();
//    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
//    bufCol.allocate(col.data(), col.size() * sizeof(glm::vec3));
//}

//GLenum ThinLensCamera::drawMode() const {return GL_LINES;}
