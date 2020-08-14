#include "fresnel.h"
#include "bsdf.h"

Color3f FresnelDielectric::Evaluate(float cosTheta) const
{
    //TODO
    float cosThetaI = Clamp(cosTheta, -1, 1);
    //<<Potentially swap indices of refraction>>
    bool entering = cosThetaI > 0.f;
    Float tmpetaI = etaI;
    Float tmpetaT = etaT;
    if (!entering) {
        Float temp = tmpetaI;
        tmpetaI = tmpetaT;
        tmpetaT = temp;
        cosThetaI = std::abs(cosThetaI);
    }

    //<<Compute cosThetaT using Snell’s law>>
    Float sinThetaI = std::sqrt(std::max((Float)0, 1 - cosThetaI * cosThetaI));
    Float sinThetaT = tmpetaI / tmpetaT * sinThetaI;
    //<<Handle total internal reflection>>
    Float cosThetaT = std::sqrt(std::max((Float)0, 1 - sinThetaT * sinThetaT));

    Float Rparl = ((tmpetaT * cosThetaI) - (tmpetaI * cosThetaT)) /
            ((tmpetaT * cosThetaI) + (tmpetaI * cosThetaT));
    Float Rperp = ((tmpetaI * cosThetaI) - (tmpetaT * cosThetaT)) /
            ((tmpetaI * cosThetaI) + (tmpetaT * cosThetaT));

    return Color3f((Rparl * Rparl + Rperp * Rperp) / 2.f);
}
