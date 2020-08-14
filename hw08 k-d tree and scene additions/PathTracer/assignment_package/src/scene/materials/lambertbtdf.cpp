#include "lambertbtdf.h"
#include <warpfunctions.h>

Color3f LambertBTDF::f(const Vector3f &wo, const Vector3f &wi) const
{
    //TODO
    return R * InvPi;
}

Color3f LambertBTDF::Sample_f(const Vector3f &wo, Vector3f *wi, const Point2f &u,
                        Float *pdf, BxDFType *sampledType) const
{
    //TODO
    //std::cout << "t" <<std::endl;
    *wi = glm::normalize(WarpFunctions::squareToSphereUniform(u));
    *pdf = Pdf(wo, *wi);
    return f(wo, *wi);
}

float LambertBTDF::Pdf(const Vector3f &wo, const Vector3f &wi) const
{
    //TODO
    Vector3f nwi = wi;
    nwi.z = -wi.z;
    Float pdf = WarpFunctions::squareToSphereUniformPDF(nwi);
    if (SameHemisphere(wo, nwi)) {
        return pdf;
    }
    return 0.f;
}
