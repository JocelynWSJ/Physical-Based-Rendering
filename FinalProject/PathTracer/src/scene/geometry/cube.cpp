
#include "cube.h"
#include <iostream>

float Cube::Area() const
{
    //TODO
    Vector3f scale = transform.getScale();
    float xyfaces = 2 * scale.x * scale.y;
    float xzfaces = 2 * scale.x * scale.z;
    float yzfaces = 2 * scale.y * scale.z;
    return xyfaces + xzfaces + yzfaces;
//    return 6 * scale.x * scale.x; //assume uniform scaling
}

Bounds3f Cube::WorldBound() const {
    Bounds3f modelbounds(Point3f(-0.5f, -0.5f, -0.5f), Point3f(0.5f, 0.5f, 0.5f));
    return modelbounds.Apply(this->transform);
}

void Cube::getSurfaceEmissionRay(const Point2f &xi, const Point2f& xk, Ray& ray ,float& pdf) const {
    pdf = 1.f / Area();
    const float hash = xi.x + xi.y + xk.x +  xk.y;
    const float hashrand = glm::clamp( (hash - std::floor(hash)) - 0.0001f, 0.f, 1.f) ;
    glm::vec4 p;
    Vector3f dir;
    const float oneover6 = 1.f/6.f;
    const float bias = 0.005f;
    if(hashrand < 1.f*oneover6) {//xyplane, facing z+
        p = transform.T() * glm::vec4(xi.x - 0.5f, xi.y - 0.5f, bias, 1.f);
        dir = WarpFunctions::squareToHemisphereUniform(xk);
    } else if (hashrand < 2.f*oneover6) {//xyplane facing z-
        p = transform.T() * glm::vec4(xi.x - 0.5f, xi.y - 0.5f, -bias, 1.f);
        dir = WarpFunctions::squareToHemisphereUniform(xk);
        dir.z *= -1.f;
    } else if (hashrand < 3.f*oneover6) {//xz plane, facing y+
        p = transform.T() * glm::vec4(xi.x - 0.5f, bias, xi.y - 0.5f, 1.f);
        dir = WarpFunctions::squareToHemisphereUniform(xk);
        glm::vec4 rotdir = glm::rotate(glm::mat4(1.f),glm::radians(-90.f),glm::vec3(0.f, 0.f, 1.f)) * glm::vec4(dir,0.f);
        dir = glm::vec3(rotdir);
    } else if (hashrand < 4.f*oneover6) {//xz plane, facing y-
        p = transform.T() * glm::vec4(xi.x - 0.5f, -bias, xi.y - 0.5f, 1.f);
        glm::vec4 rotdir = glm::rotate(glm::mat4(1.f),glm::radians(90.f),glm::vec3(0.f, 0.f, 1.f)) * glm::vec4(dir,0.f);
        dir = glm::vec3(rotdir);
    } else if (hashrand < 5.f*oneover6) {//yz plane, facing x+
        p = transform.T() * glm::vec4(bias, xi.x - 0.5f, xi.y - 0.5f, 1.f);
        glm::vec4 rotdir = glm::rotate(glm::mat4(1.f),glm::radians(90.f),glm::vec3(1.f, 0.f, 0.f)) * glm::vec4(dir,0.f);
        dir = glm::vec3(rotdir);
    } else {//yz plane, facing x-
        p = transform.T() * glm::vec4(-bias, xi.x - 0.5f, xi.y - 0.5f, 1.f);
        glm::vec4 rotdir = glm::rotate(glm::mat4(1.f),glm::radians(-90.f),glm::vec3(1.f, 0.f, 0.f)) * glm::vec4(dir,0.f);
        dir = glm::vec3(rotdir);
    }
    ray.origin = Vector3f(p.x, p.y, p.z);
    ray.direction      = glm::normalize(transform.invTransT() * dir);
}

Intersection Cube::Sample(const Point2f &xi, Float *pdf) const
{
    return Intersection();
}

int GetFaceIndex(const Point3f& P)
{
    int idx = 0;
    float val = -1;
    for(int i = 0; i < 3; i++){
        if(glm::abs(P[i]) > val){
            idx = i * glm::sign(P[i]);
            val = glm::abs(P[i]);
        }
    }
    return idx;
}

Normal3f GetCubeNormal(const Point3f& P)
{
    int idx = glm::abs(GetFaceIndex(Point3f(P)));
    Normal3f N(0,0,0);
    N[idx] = glm::sign(P[idx]);
    return N;
}


bool Cube::Intersect(const Ray& r, Intersection* isect) const
{
    //Transform the ray
    Ray r_loc = r.GetTransformedCopy(transform.invT());

    float t_n = -1000000;
    float t_f = 1000000;
    for(int i = 0; i < 3; i++){
        //Ray parallel to slab check
        if(r_loc.direction[i] == 0){
            if(r_loc.origin[i] < -0.5f || r_loc.origin[i] > 0.5f){
                return false;
            }
        }
        //If not parallel, do slab intersect check
        float t0 = (-0.5f - r_loc.origin[i])/r_loc.direction[i];
        float t1 = (0.5f - r_loc.origin[i])/r_loc.direction[i];
        if(t0 > t1){
            float temp = t1;
            t1 = t0;
            t0 = temp;
        }
        if(t0 > t_n){
            t_n = t0;
        }
        if(t1 < t_f){
            t_f = t1;
        }
    }
    if(t_n < t_f)
    {
        float t = t_n > 0 ? t_n : t_f;
        if(t < 0)
            return false;
        //Lastly, transform the point found in object space by T
        glm::vec4 P = glm::vec4(r_loc.origin + t*r_loc.direction, 1);
        InitializeIntersection(isect, t, Point3f(P));
        return true;
    }
    else{//If t_near was greater than t_far, we did not hit the cube
        return false;
    }
}


void Cube::ComputeTBN(const Point3f& P, Normal3f* nor, Vector3f* tan, Vector3f* bit) const
{
    Normal3f modelnor = GetCubeNormal(P);
    *nor = glm::normalize(transform.invTransT() * modelnor);
    //TODO: Compute tangent and bitangent
    Vector3f modelbit(1.f,0.f,0.f);
    if(modelnor.y == 0.f) {
        modelbit.x = 0.f;
        modelbit.y = 1.f;
    }

    *bit = glm::normalize(transform.T3() * modelbit );
    *tan = glm::normalize(transform.T3() * glm::cross(modelbit,modelnor));
}

glm::vec2 Cube::GetUVCoordinates(const glm::vec3 &point) const
{
    glm::vec3 abs = glm::min(glm::abs(point), 0.5f);
    glm::vec2 UV;//Always offset lower-left corner
    if(abs.x > abs.y && abs.x > abs.z)
    {
        UV = glm::vec2(point.z + 0.5f, point.y + 0.5f)/3.0f;
        //Left face
        if(point.x < 0)
        {
            UV += glm::vec2(0, 0.333f);
        }
        else
        {
            UV += glm::vec2(0, 0.667f);
        }
    }
    else if(abs.y > abs.x && abs.y > abs.z)
    {
        UV = glm::vec2(point.x + 0.5f, point.z + 0.5f)/3.0f;
        //Left face
        if(point.y < 0)
        {
            UV += glm::vec2(0.333f, 0.333f);
        }
        else
        {
            UV += glm::vec2(0.333f, 0.667f);
        }
    }
    else
    {
        UV = glm::vec2(point.x + 0.5f, point.y + 0.5f)/3.0f;
        //Left face
        if(point.z < 0)
        {
            UV += glm::vec2(0.667f, 0.333f);
        }
        else
        {
            UV += glm::vec2(0.667f, 0.667f);
        }
    }
    return UV;
}
