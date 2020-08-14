#include "sampler.h"

Sampler::Sampler()
{}

// This function has several steps:
// 1. Generate a sample within a [0, 1) square using the sample mode specified
//      Note that if the sample mode is grid-based or stratified, the number of samples
//      generated will be rounded to the nearest square number (and will modify the input numSamples)
// 2. Warp this sample to a new surface with the method specified, then add the sample to a collection
// 3. Repeat until you've generated a number of samples equal to numSamples
// Based on code written by Wenzel Jakob for his Nori renderer

void Sampler::generateSamples(int& numSamples, SampleMode sampleMode)
{
    pcg32 rng;
    if(sampleMode == GRID || sampleMode == STRATIFIED)
    {
        int cubeRtVal = (int)(std::pow((float) numSamples, 1.f / 3.f) + 0.5);
        float invCubeRtVal = 1.f / cubeRtVal;
        numSamples = cubeRtVal * cubeRtVal * cubeRtVal;
        samples = std::vector<glm::vec3>(numSamples);
        sampleColors = std::vector<glm::vec3>(numSamples);

        int i = 0;
        for(int x = 0; x < cubeRtVal; ++x)
        {
            for(int y = 0; y < cubeRtVal; ++y)
            {
                for(int z = 0; z < cubeRtVal; ++z)
                {
                    glm::vec3 sample;
                    switch(sampleMode)
                    {
                    case GRID:
                        sample = glm::vec3((x + 0.5f) * invCubeRtVal,
                                           (y + 0.5f) * invCubeRtVal,
                                           (z + 0.5f) * invCubeRtVal);
                        break;

                    case STRATIFIED:
                        sample = glm::vec3((x + rng.nextFloat()) * invCubeRtVal,
                                           (y + rng.nextFloat()) * invCubeRtVal,
                                           (z + rng.nextFloat()) * invCubeRtVal);
                        break;
                    }

                    samples[i] = sample;
                    sampleColors[i++] = glm::vec3(1,1,1);
                }
            }
        }
    }
    else // Random sampling
    {
        int i = 0;
        samples = std::vector<glm::vec3>(numSamples);
        sampleColors = std::vector<glm::vec3>(numSamples);
        for(int s = 0; s < numSamples; ++s)
        {
            glm::vec3 sample(rng.nextFloat(), rng.nextFloat(), rng.nextFloat());

            samples[i] = sample;
            sampleColors[i++] = glm::vec3(1,1,1);
        }
    }



}

unsigned int Sampler::getNumSamples()
{
    return this->samples.size();
}

std::vector<glm::vec3> &Sampler::peekSamples()
{
    return this->samples;
}

std::vector<glm::vec3>& Sampler::peekSampleColors()
{
    return this->sampleColors;
}
