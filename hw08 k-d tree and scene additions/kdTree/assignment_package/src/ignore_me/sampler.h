#pragma once
#include <ignore_me/enums.h>
#include <vector>
#include <ignore_me/la.h>
#include <pcg32.h>

class Sampler
{
public:
    Sampler();

    void generateSamples(int& numSamples, SampleMode sampleMode);

    unsigned int getNumSamples();

    std::vector<glm::vec3>& peekSamples();
    std::vector<glm::vec3> &peekSampleColors();

    static float rejectionSampleDisk();


    std::vector<glm::vec3> samples; // Every 3 floats is a point
    std::vector<glm::vec3> sampleColors; // Every 3 floats is a point's color
};

// PDF of disc is 1/PI because area of disc is PI*r^2, and r = 1.
