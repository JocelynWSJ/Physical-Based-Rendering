#include "naiveintegrator.h"

//ray: the ray along which the incident radiance should be evaluated.
//scene: the Scene being rendered. The implementation will query the scene for information about the lights and geometry, and so on.
//sampler: a sample generator used to solve the light transport equation via Monte Carlo integration.
//arena: a MemoryArena for efficient temporary memory allocation by the integrator. The integrator should assume that any memory it allocates with the arena will be freed shortly after the Li() method returns and thus should not use the arena to allocate any memory that must persist for longer than is needed for the current ray.
//depth: the number of ray bounces from the camera that have occurred up until the current call to Li().
Color3f NaiveIntegrator::Li(const Ray &ray, const Scene &scene, std::shared_ptr<Sampler> sampler, int depth) const
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

    //<<Compute emitted and reflected light at ray intersection point>>
    //<<Initialize common variables for Whitted integrator>>
    Normal3f n = isect.normalGeometric;
    Vector3f woW = -1.f * ray.direction;

    if (isect.ProduceBSDF()) {
        Vector3f wiW(0.f);
        Float pdf = 1.f;

        L = isect.bsdf->Sample_f(woW,&wiW,sampler->Get2D(), &pdf);
        if (L == Color3f(0.f) || pdf == 0)  {
            return Color3f(0.f);
        }
        L /= pdf;
        Ray wiWray = isect.SpawnRay(wiW);
        L *= AbsDot(wiW,isect.normalGeometric);
        Color3f recurseLi = Li(wiWray,scene,sampler,--depth);
        L *= recurseLi;
    }
    return isect.Le(woW) + L;
}
