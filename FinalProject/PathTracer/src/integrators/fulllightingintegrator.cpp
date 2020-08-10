#include "fulllightingintegrator.h"

Color3f FullLightingIntegrator::Li(const Ray &_ray, const Scene &scene, std::shared_ptr<Sampler> sampler, int depth) const
{
    Color3f throughput(1.f); //The throughput will be used to determine when your ray path terminates via the Russian Roulette heuristic
    Color3f accumColor(0.f);
    bool specular = false;
    int bounce = 0;
    Ray ray = _ray;
    while (bounce < depth) {
        if(depth < 0){
            return Color3f(0.f);
        }

        Intersection isect = Intersection();
        //Possibly add emitted light at intersection
        bool isectExist = scene.Intersect(ray, &isect);
        if (bounce == 0||specular) {
            if (isectExist) {
                accumColor += throughput * isect.Le(-ray.direction);
            } else {
                for (const auto &light : scene.lights) {
                    accumColor += throughput * light->Le(ray);
                }
                return accumColor;
            }
        }
        //Terminate path if ray escaped
        if (!isectExist||bounce >= depth) {
            break;
        }


        Color3f MIS(0.f);
        //Compute emitted and reflected light at ray intersection point
        //Initialize common variables for Whitted integrator
        Vector3f woW = -1.f * ray.direction;

        //Compute scattering functions and skip over medium boundaries
        if (!isect.ProduceBSDF()) {
            if(isect.objectHit->GetAreaLight()) {
                break;
            }
            ray = isect.SpawnRay(ray.direction);
            bounce++;
            continue;
        }

        if(isect.objectHit->GetMaterial() == nullptr) {
            break;
        }

        //==============================
        // Direct Light
        //==============================
        Vector3f wiW(0.f);
        Float light_pdf = 1.f;
        //if the material is specular
        specular = isect.bsdf->BxDFsMatchingFlags(BSDF_SPECULAR) > 0 ? true : false;

        //Ask the light source for a random point on its surface p’
        uint32_t pos = (sampler->Get1D() * scene.lights.size());
        //Ask the light for its PDF
        MIS = scene.lights[pos]->Sample_Li(isect, sampler->Get2D(), &wiW, &light_pdf);

        //If the PDF is nonzero and the light emitted along ωj is nonzero, then we know we
        //can evaluate this function
        if (light_pdf == 0 || MIS == Color3f(0.f))  {
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
            MIS = Color3f(0.f);
        } else {
            brdf_pdf = isect.bsdf->Pdf(woW, wiW);
            //We can compute the power heuristic weight now that we have the light_pdf and brdf_pdf
            Float wp = BalanceHeuristic(1, light_pdf, 1, brdf_pdf);
            //Our (first) estimated light is equal to the formula at the top of the page for the ωj computed
            //using a sample on the light source
            MIS = wp * f * MIS * AbsDot(wiW,isect.normalGeometric)/light_pdf;

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

        if (specular) {
            MIS = L2;
        } else {
            MIS += L2;
        }
        MIS *= scene.lights.size();
        accumColor += throughput * MIS;

        //==============================
        // Global illumination
        //==============================

        Vector3f wi(0.f);
        Float Gpdf = 0.f;
        Color3f GL = isect.bsdf->Sample_f(woW,&wi,sampler->Get2D(), &Gpdf);

        Color3f current_throughput = GL * AbsDot(wi, isect.normalGeometric) / Gpdf;
        if(glm::length2(GL) <= 0 || Gpdf <= 0) {
            current_throughput = Color3f(0.f);
        }
        throughput *= current_throughput;
        ray = isect.SpawnRay(wi);


        //Possibly terminate the path with Russian roulette
        if (bounce > 5) {
            Float q = std::max(throughput.r, std::max(throughput.g, throughput.b));
            if (q < sampler->Get1D()) {
                break;
            }
            throughput /= 1 - q;
        }

        bounce++;
    }

    return accumColor;
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

