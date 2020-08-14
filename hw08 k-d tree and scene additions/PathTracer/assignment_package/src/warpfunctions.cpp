#define _USE_MATH_DEFINES
#include "warpfunctions.h"
#include <math.h>

glm::vec3 WarpFunctions::squareToDiskUniform(const glm::vec2 &sample)
{
    //TODO
    float r = std::sqrt(sample[0]);
    float theta = 2 * Pi * sample[1];
    return glm::vec3(r * std::cos(theta), r * std::sin(theta), 0);
    //throw std::runtime_error("You haven't yet implemented uniform disk warping!");
}

glm::vec3 WarpFunctions::squareToDiskConcentric(const glm::vec2 &sample)
{
    //TODO
    //Map uniform random numbers to
    glm::vec2 sampleOffset = 2.f * sample - glm::vec2(1.f, 1.f);

    //Handle degeneracy at the origin>>
    if (sampleOffset.x == 0 && sampleOffset.y == 0)
        return glm::vec3(0, 0, 0);

    //<<Apply concentric mapping to point>>
    float theta, r;
    if (std::abs(sampleOffset.x) > std::abs(sampleOffset.y)) {
        r = sampleOffset.x;
        theta = (PiOver4) * (sampleOffset.y / sampleOffset.x);
    } else {
        r = sampleOffset.y;
        theta = PiOver2 - PiOver4 * (sampleOffset.x / sampleOffset.y);
    }
    return r * glm::vec3(std::cos(theta), std::sin(theta), 0);
    //throw std::runtime_error("You haven't yet implemented concentric disk warping!");
}

float WarpFunctions::squareToDiskPDF(const glm::vec3 &sample)
{
    //TODO
    return InvPi;
}

glm::vec3 WarpFunctions::squareToSphereUniform(const glm::vec2 &sample)
{
    //TODO
    //Map uniform random numbers to
    glm::vec2 sampleOffset = 2.f * sample - glm::vec2(1.f, 1.f);

    float z = sampleOffset[0];
    float r = std::sqrt(std::max(0.f, 1.f - z * z));
    float phi = TwoPi * sampleOffset[1];
    return glm::vec3(r * std::cos(phi), r * std::sin(phi), z);
    //throw std::runtime_error("You haven't yet implemented uniform sphere warping!");
}

float WarpFunctions::squareToSphereUniformPDF(const glm::vec3 &sample)
{
    //TODO
    return Inv4Pi;
}

glm::vec3 WarpFunctions::squareToSphereCapUniform(const glm::vec2 &sample, float thetaMin)
{
    //TODO
    float z = sample[0];
    z = std::cos(thetaMin) + (z+1.f) * (1.f - std::cos(thetaMin))/2.f;
    float r = std::sqrt(std::max(0.f, 1.f - z * z));
    float phi = 2 * Pi * sample[1];
    return glm::vec3(r * std::cos(phi), r * std::sin(phi), z);
    //throw std::runtime_error("You haven't yet implemented sphere cap warping!");
}

float WarpFunctions::squareToSphereCapUniformPDF(const glm::vec3 &sample, float thetaMin)
{
    //TODO
    return InvPi/(2.f * (1 - glm::cos(glm::radians(180 - thetaMin))));
}

glm::vec3 WarpFunctions::squareToHemisphereUniform(const glm::vec2 &sample)
{
    //TODO
    float z = sample[0];
    float r = std::sqrt(std::max(0.f, 1.f - z * z));
    float phi = 2 * Pi * sample[1];
    return glm::vec3(r * std::cos(phi), r * std::sin(phi), z);
    //throw std::runtime_error("You haven't yet implemented uniform hemisphere warping!");
}

float WarpFunctions::squareToHemisphereUniformPDF(const glm::vec3 &sample)
{
    //TODO
    return Inv2Pi;
}

glm::vec3 WarpFunctions::squareToHemisphereCosine(const glm::vec2 &sample)
{
    //TODO
    glm::vec3 d = squareToDiskConcentric(sample);
    float z = std::sqrt(std::max((float)0, 1 - d.x * d.x - d.y * d.y));
    return glm::vec3(d.x, d.y, z);
    //throw std::runtime_error("You haven't yet implemented cosine-weighted hemisphere warping!");
}

float WarpFunctions::squareToHemisphereCosinePDF(const glm::vec3 &sample)
{
    //TODO
    float cosTheta = glm::dot(glm::normalize(sample), glm::vec3(0, 0, 1.f));
    return cosTheta*InvPi;
}
