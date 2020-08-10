#ifndef MICROFACETBTDF_H
#define MICROFACETBTDF_H
#pragma once
#include "bsdf.h"
#include "fresnel.h"
#include "microfacet.h"

class MicrofacetBTDF : public BxDF
{
public:
    MicrofacetBTDF(const Color3f &R, MicrofacetDistribution* distribution, Fresnel* fresnel)
        : BxDF(BxDFType(BSDF_TRANSMISSION | BSDF_GLOSSY)), R(R), etaA(etaA), etaB(etaB),
          distribution(distribution), fresnel(fresnel) {}

    virtual ~MicrofacetBTDF(){delete fresnel; delete distribution;}

    Color3f f(const Vector3f &wo, const Vector3f &wi) const;

    virtual Color3f Sample_f(const Vector3f &wo, Vector3f *wi,
                              const Point2f &xi, Float *pdf,
                              BxDFType *sampledType = nullptr) const;
    virtual float Pdf(const Vector3f &wo, const Vector3f &wi) const;


  private:
    const Color3f R; // The energy scattering coefficient of this BRDF (i.e. its color)
    const MicrofacetDistribution* distribution;
    float etaA, etaB;
    const Fresnel* fresnel; // A class that will determine the reflectivity coefficient at this point

};

#endif // MICROFACETBTDF_H
