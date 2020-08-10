#include "directlightingintegrator.h"

Color3f DirectLightingIntegrator::Li(const Ray &ray, const Scene &scene, std::shared_ptr<Sampler> sampler, int depth) const
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
        Vector3f wiW(0.f);
        Float pdf = 1.f;

        //randomly select a light source from scene.lights and call its Sample_Li function
        uint32_t pos = (sampler->Get1D() * scene.lights.size());
        //std::cout << pos << std::endl;
        Color3f L = scene.lights[pos]->Sample_Li(isect, sampler->Get2D(), &wiW, &pdf);

        if (pdf == 0)  {
            return Color3f(0.f);
        }
        //divide the PDF you obtain from Sample_Li by the number of light sources
        pdf /= scene.lights.size();
        L /= pdf;

        Ray wiWray = isect.SpawnRay(wiW);
        Intersection inter = Intersection();
        scene.Intersect(wiWray, &inter);
        if (inter.objectHit == nullptr || inter.objectHit->areaLight != scene.lights.at(pos)) {
            L = Color3f(0.f);
        }

        L *= AbsDot(wiW,isect.normalGeometric);
        L *= isect.bsdf->f(woW, wiW);
        return isect.Le(woW) + L;
    }
    return isect.Le(woW) + L;
}
