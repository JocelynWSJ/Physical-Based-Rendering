#ifndef MULTIPLEIMPORTANCEINTEGRATOR_H
#define MULTIPLEIMPORTANCEINTEGRATOR_H
#pragma once
#include "integrator.h"

class MultipleImportanceIntegrator : public Integrator
{
public:
    MultipleImportanceIntegrator(Bounds2i bounds, Scene* s, std::shared_ptr<Sampler> sampler, int recursionLimit)
        : Integrator(bounds, s, sampler, recursionLimit)
    {}

    // Evaluate the energy transmitted along the ray back to
    // its origin using multiple importance sampling
    virtual Color3f Li(const Ray &ray, const Scene &scene, std::shared_ptr<Sampler> sampler, int depth) const;

};

//float BalanceHeuristic(int nf, Float fPdf, int ng, Float gPdf);
//float PowerHeuristic(int nf, Float fPdf, int ng, Float gPdf);


#endif // MULTIPLEIMPORTANCEINTEGRATOR_H
