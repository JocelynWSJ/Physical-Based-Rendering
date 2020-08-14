#include "gridvisualizer.h"

GridVisualizer::GridVisualizer(GLWidget277 *context, Sampler *s) : Drawable(context), sampler(s)
{}

void GridVisualizer::create()
{

    //todo

    std::vector<glm::vec3> positions, colors;
    std::vector<GLuint> indices;

    count = indices.size();


    generateIdx();
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), indices.data(), GL_DYNAMIC_DRAW);

    // Positions and colors
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, count * 3 * sizeof(glm::vec3), positions.data(), GL_DYNAMIC_DRAW);

    generateCol();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    context->glBufferData(GL_ARRAY_BUFFER, count * 3 * sizeof(glm::vec3), colors.data(), GL_DYNAMIC_DRAW);

}

GLenum GridVisualizer::drawMode()
{
    return GL_LINES;
}
