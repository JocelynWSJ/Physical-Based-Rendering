#include "searchvisualizer.h"

SearchVisualizer::SearchVisualizer(GLWidget277 *context)
    : Drawable(context), created(false)
{}

void SearchVisualizer::destroy()
{
    Drawable::destroy();
    created = false;
}

void SearchVisualizer::create()
{
    //dummy function
}

GLenum SearchVisualizer::drawMode()
{
    return GL_POINTS;
}

void SearchVisualizer::recreate(std::vector<glm::vec3> pts)
{
    destroy();
    created = true;
    std::vector<glm::vec3> pos, col;
    std::vector<GLuint> idx;

    GLuint i = 0;
    for(glm::vec3 p : pts)
    {
        pos.push_back(p);
        col.push_back(glm::vec3(0,1,1));
        idx.push_back(i++);
    }

    // VBO stuff
    count = idx.size();

    generateIdx();
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), idx.data(), GL_DYNAMIC_DRAW);

    // Positions and colors
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::vec3), pos.data(), GL_DYNAMIC_DRAW);

    generateCol();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    context->glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::vec3), col.data(), GL_DYNAMIC_DRAW);
}
