#include "fulllightingintegrator.h"

Color3f FullLightingIntegrator::Li(const Ray &ray, const Scene &scene, std::shared_ptr<Sampler> sampler, int depth) const
{
    //TODO
    if(depth < 0){
        return Color3f(0.f);
    }
    Color3f L(0.f);
    //<<Find closest ray intersection or return background radiance>>
    Intersection isect = Intersection();
    if (!scene.Intersect(ray, &isect)) {
        for (const auto &light : scene.lights)
            L += light->Le(ray);
        return L;
    }

    //Compute emitted and reflected light at ray intersection point
    //Initialize common variables for Whitted integrator
    Vector3f woW = -1.f * ray.direction;

    if (isect.ProduceBSDF()) {
//==============================
// Direct Light
//==============================
        Vector3f wiW(0.f);
        Float light_pdf = 1.f;

        //Ask the light source for a random point on its surface p’
        uint32_t pos = (sampler->Get1D() * scene.lights.size());
        //Ask the light for its PDF
        Color3f L = scene.lights[pos]->Sample_Li(isect, sampler->Get2D(), &wiW, &light_pdf);

        //If the PDF is nonzero and the light emitted along ωj is nonzero, then we know we
        //can evaluate this function
        if (light_pdf == 0 || L == Color3f(0.f))  {
            return Color3f(0.f);
        }

        //If this BRDF is nonzero, then we can ask p’s material for its PDF as well
        //so we can use it in our power heuristic
        Ray wiWray = isect.SpawnRay(wiW);
        Intersection inter = Intersection();
        scene.Intersect(wiWray, &inter);
        //Now evaluate f(p,ωo,ωj)
        Color3f f = isect.bsdf->f(woW, wiW);
        Float brdf_pdf = 0.f;
        if (f == Color3f(0.f) || inter.objectHit == nullptr || inter.objectHit->areaLight != scene.lights.at(pos)) {
            L = Color3f(0.f);
        } else {
            brdf_pdf = isect.bsdf->Pdf(woW, wiW);
            //We can compute the power heuristic weight now that we have the light_pdf and brdf_pdf
            Float wp = BalanceHeuristic(1, light_pdf, 1, brdf_pdf);
            //Our (first) estimated light is equal to the formula at the top of the page for the ωj computed
            //using a sample on the light source
            L = wp * f * L * AbsDot(wiW,isect.normalGeometric)/light_pdf;

        }

//==============================
// BSDF
//==============================

        Vector3f wiW2(0.f);
        Float pdf = 1.f;

        Color3f L2 = isect.bsdf->Sample_f(woW,&wiW2,sampler->Get2D(), &pdf);
        if (L2 == Color3f(0.f) || pdf == 0)  {
            L2 = Color3f(0.f);
        } else {
            float bsdfPDF = scene.lights[pos]->Pdf_Li(isect,wiW2);

            if(bsdfPDF > 0.f) {
                Float wp2 = BalanceHeuristic(1, pdf, 1, bsdfPDF);
                Ray wiWray2 = isect.SpawnRay(wiW2);
                Intersection lightisect;
                scene.Intersect(wiWray2, &lightisect);
                L2 *= wp2 * AbsDot(wiW2,lightisect.normalGeometric)/pdf;
            } else {
                L2 = Color3f(0.f);
            }

        }
        L += L2;
        L *= scene.lights.size();

        return isect.Le(woW) + L;
        }

        return isect.Le(woW);
    }




float BalanceHeuristic(int nf, Float fPdf, int ng, Float gPdf)
{
    //TODO
    return (nf * fPdf) / (nf * fPdf + ng * gPdf);
}

float PowerHeuristic(int nf, Float fPdf, int ng, Float gPdf)
{
    //TODO
    Float f = nf * fPdf, g = ng * gPdf;
    return (f * f) / (f * f + g * g);
}

