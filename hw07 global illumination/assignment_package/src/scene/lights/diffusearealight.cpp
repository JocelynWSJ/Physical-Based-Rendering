#include "diffusearealight.h"

Color3f DiffuseAreaLight::L(const Intersection &isect, const Vector3f &w) const
{
    //TODO
    if (this->twoSided || glm::dot(w,isect.normalGeometric) > 0.f) {
        return emittedLight;
    }
    return Color3f(0.f);
}

Color3f DiffuseAreaLight::Sample_Li(const Intersection &ref, const Point2f &xi, Vector3f *wi, Float *pdf) const {
    //Get an Intersection on the surface of its Shape by invoking shape->Sample.
    Intersection lightsample = this->shape->Sample(ref,xi,pdf);
    //Check if the resultant PDF is zero or that the reference Intersection and the resultant Intersection are the
    //same point in space, and return black if this is the case.
    if( (*pdf) <= 0.f || (ref.point == lightsample.point)) {
        *pdf = 0.f;
        return Color3f(0.f);
    }
    //Set ωi to the normalized vector from the reference Intersection's point to the Shape's intersection point.
    *wi = glm::normalize(lightsample.point - ref.point);
    //Return the light emitted along ωi from our intersection point.
    if(*pdf > 0) {
        return this->L(lightsample, -(*wi));
    }
}

//returns the PDF of the light's Shape given an Intersection from which the light
//is being viewed and a viewing direction wi
float DiffuseAreaLight::Pdf_Li(const Intersection &ref, const Vector3f &wi) const {
    Intersection isect_thislight = Intersection();
    Ray wiWray = ref.SpawnRay(wi);
    if(!this->shape->Intersect(wiWray,&isect_thislight)) {
        return 0.f;
    }

    float coslight = glm::dot(isect_thislight.normalGeometric, -wi);
    if(this->twoSided){
        coslight = glm::abs(coslight);
    }
    float denominator = coslight * this->shape->Area();
    if(denominator > 0.f) {
        return glm::distance2(ref.point, isect_thislight.point) / denominator;
    } else {
        return 0.f;
    }
}
