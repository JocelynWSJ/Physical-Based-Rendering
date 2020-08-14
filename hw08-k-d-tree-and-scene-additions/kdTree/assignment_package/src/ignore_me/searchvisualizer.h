#pragma once
#include "drawable.h"

class SearchVisualizer : public Drawable
{
public:
    SearchVisualizer(GLWidget277* context);

    void recreate(std::vector<glm::vec3>);
    void create() override;
    void destroy() override;
    GLenum drawMode() override;

    bool created;
};
