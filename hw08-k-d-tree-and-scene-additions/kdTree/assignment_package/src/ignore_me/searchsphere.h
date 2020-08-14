#pragma once
#include "drawable.h"

class SearchSphere : public Drawable
{
public:
    SearchSphere(GLWidget277* context, glm::vec3 p, float r);
    void create() override;
    GLenum drawMode() override;

    glm::vec3 center;
    float radius;
};
