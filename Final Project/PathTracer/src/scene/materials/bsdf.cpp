#include "bsdf.h"
#include <warpfunctions.h>

BSDF::BSDF(const Intersection& isect, float eta /*= 1*/)
//TODO: Properly set worldToTangent and tangentToWorld
    : worldToTangent(glm::transpose(glm::mat3{isect.tangent, isect.bitangent, isect.normalGeometric})/*COMPUTE ME*/),
      tangentToWorld({isect.tangent, isect.bitangent, isect.normalGeometric}/*COMPUTE ME*/),
      normal(isect.normalGeometric),
      eta(eta),
      numBxDFs(0),
      bxdfs{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}
{}


void BSDF::UpdateTangentSpaceMatrices(const Normal3f& n, const Vector3f& t, const Vector3f b)
{
    //TODO: Update worldToTangent and tangentToWorld based on the normal, tangent, and bitangent
    //The worldToTangent matrix has these vectors as its rows, and the tangentToWorld has them as
    //its columns (in the order listed, so it would be tangent, then bitangent, then normal).
    //Remember that the constructor for a GLM matrix uses the input vectors as the columns of the
    //resultant matrix.
    tangentToWorld = {t, b, n};
    worldToTangent = glm::transpose(tangentToWorld);
}


//
Color3f BSDF::f(const Vector3f &woW, const Vector3f &wiW, BxDFType flags /*= BSDF_ALL*/) const
{
    //TODO
    //convert vectors to local space
    Vector3f wi = worldToTangent * wiW;
    Vector3f wo = worldToTangent * woW;

    //confirm if wiw and wow on the same side
    bool reflect = (glm::dot(wiW, normal) * glm::dot(woW, normal)) > 0;

    Color3f f(0.f);
    for (int i = 0; i < numBxDFs; ++i) {
        if (bxdfs[i]->MatchesFlags(flags) && ((reflect && (bxdfs[i]->type & BSDF_REFLECTION)) ||
                                              (!reflect && (bxdfs[i]->type & BSDF_TRANSMISSION))))
        {
            f += bxdfs[i]->f(wo, wi);
        }
    }


    return f;
}

// Use the input random number _xi_ to select
// one of our BxDFs that matches the _type_ flags.

// After selecting our random BxDF, rewrite the first uniform
// random number contained within _xi_ to another number within
// [0, 1) so that we don't bias the _wi_ sample generated from
// BxDF::Sample_f.

// Convert woW and wiW into tangent space and pass them to
// the chosen BxDF's Sample_f (along with pdf).
// Store the color returned by BxDF::Sample_f and convert
// the _wi_ obtained from this function back into world space.

// Iterate over all BxDFs that we DID NOT select above (so, all
// but the one sampled BxDF) and add their PDFs to the PDF we obtained
// from BxDF::Sample_f, then average them all together.

// Finally, iterate over all BxDFs and sum together the results of their
// f() for the chosen wo and wi, then return that sum.

Color3f BSDF::Sample_f(const Vector3f &woW, Vector3f *wiW, const Point2f &xi/*sample*/,
                       float *pdf, BxDFType type, BxDFType *sampledType) const
{   //1) warp ...1, ...2 to appropriate distribution based on BSDF
    //get wiw->get pdf(wiw)
    //2) return f(wow, wiw)

    //TODO
    //<<Choose which BxDF to sample>>
    //a method returns the number of BxDFs stored by the BSDF that match a particular set of BxDFType flags.
    int matchingComps = BxDFsMatchingFlags(type);
    if (matchingComps == 0) {
        *pdf = 0;
        return Color3f(0.f);
    }
    int comp = std::min((int)std::floor(xi[0] * matchingComps), matchingComps - 1);


    //<<Get BxDF pointer for chosen component>>
    BxDF *bxdf = nullptr;
    int count = comp;
    for (int i = 0; i < numBxDFs; ++i)
        if (bxdfs[i]->MatchesFlags(type) && count-- == 0) {
            bxdf = bxdfs[i];
            break;
        }


    //<<Remap BxDF sample xi to [0, 1) >>
    Point2f uRemapped(xi[0] * matchingComps - comp, xi[1]);

    //<<Sample chosen BxDF>>
    Vector3f wi = worldToTangent * *wiW;
    Vector3f wo = worldToTangent * woW;

    *pdf = 0;
    if (sampledType) {
        *sampledType = bxdf->type;
    }
    Color3f f = bxdf->Sample_f(wo, &wi, uRemapped, pdf, sampledType);
    if (*pdf == 0) {
        return Color3f(0.f);
    }
    *wiW = tangentToWorld * wi;

    //<<Compute overall PDF with all matching BxDFs>>
    if (!(bxdf->type & BSDF_SPECULAR) && matchingComps > 1)
        for (int i = 0; i < numBxDFs; ++i) {
            if (bxdfs[i] != bxdf && bxdfs[i]->MatchesFlags(type)) {
                *pdf += bxdfs[i]->Pdf(wo, wi);
            }
        }
    if (matchingComps > 1) {
        *pdf /= matchingComps;
    }

    //<<Compute value of BSDF for sampled direction>>
    if (!(bxdf->type & BSDF_SPECULAR) && matchingComps > 1) {
        bool reflect = glm::dot(*wiW, normal) * glm::dot(woW, normal) > 0;
        f = Color3f(0.f);
        for (int i = 0; i < numBxDFs; ++i) {
            if (bxdfs[i]->MatchesFlags(type) && ((reflect && (bxdfs[i]->type & BSDF_REFLECTION)) ||
                     (!reflect && (bxdfs[i]->type & BSDF_TRANSMISSION)))) {
                f += bxdfs[i]->f(wo, wi);
            }
        }
    }
    return f;
}


float BSDF::Pdf(const Vector3f &woW, const Vector3f &wiW, BxDFType flags) const
{
    //TODO
    float pdf = 0;
    int count = 0;

    Vector3f wi = worldToTangent * wiW;
    Vector3f wo = worldToTangent * woW;

    for(int i = 0; i < this->numBxDFs; i++)
    {
        if(this->bxdfs[i]->MatchesFlags(flags))
        {
            pdf += this->bxdfs[i]->Pdf(wo, wi);
            count++;
        }
    }
    return pdf / count;
}

Color3f BxDF::Sample_f(const Vector3f &wo, Vector3f *wi, const Point2f &xi,
                       Float *pdf, BxDFType *sampledType) const
{
    //TODO
    *wi = WarpFunctions::squareToHemisphereUniform(xi);
    //*wi = WarpFunctions::squareToHemisphereCosine(xi);
    if (wo.z < 0) {
        wi->z *= -1;
    }

    *pdf = Pdf(wo, *wi);
    return f(wo, *wi);
}

// The PDF for uniform hemisphere sampling
float BxDF::Pdf(const Vector3f &wo, const Vector3f &wi) const
{
    return SameHemisphere(wo, wi) ? Inv2Pi : 0;
}

BSDF::~BSDF()
{
    for(int i = 0; i < numBxDFs; i++)
    {
        delete bxdfs[i];
    }
}
