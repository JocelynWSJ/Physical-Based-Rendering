#include "orennayarbrdf.h"
#include <warpfunctions.h>

Color3f OrenNayarBRDF::f(const Vector3f &wo, const Vector3f &wi) const
{
    //TODO
    Float sinThetaI = SinTheta(wi);
    Float sinThetaO = SinTheta(wo);
    //<<Compute cosine term of Oren–Nayar model>>
    Float maxCos = 0;
    if (sinThetaI > 1e-4 && sinThetaO > 1e-4) {
        Float sinPhiI = SinPhi(wi), cosPhiI = CosPhi(wi);
        Float sinPhiO = SinPhi(wo), cosPhiO = CosPhi(wo);
        Float dCos = cosPhiI * cosPhiO + sinPhiI * sinPhiO;
        maxCos = std::max((Float)0, dCos);
    }

    //<<Compute sine and tangent terms of Oren–Nayar model>>
    Float sinAlpha, tanBeta;
    if (AbsCosTheta(wi) > AbsCosTheta(wo)) {
        sinAlpha = sinThetaO;
        tanBeta = sinThetaI / AbsCosTheta(wi);
    } else {
        sinAlpha = sinThetaI;
        tanBeta = sinThetaO / AbsCosTheta(wo);
    }

    return R * InvPi * (A + B * maxCos * sinAlpha * tanBeta);
}

Color3f OrenNayarBRDF::Sample_f(const Vector3f &wo, Vector3f *wi, const Point2f &u,
                        Float *pdf, BxDFType *sampledType) const
{
    //TODO
    *wi = glm::normalize(WarpFunctions::squareToHemisphereCosine(u));
    *pdf = Pdf(wo, *wi);
    return f(wo, *wi);
}

float OrenNayarBRDF::Pdf(const Vector3f &wo, const Vector3f &wi) const
{
    //TODO
    Float pdf = WarpFunctions::squareToHemisphereCosinePDF(wi);
    return SameHemisphere(wo, wi) ? pdf : 0;
}

