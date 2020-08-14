#include "searchsphere.h"

SearchSphere::SearchSphere(GLWidget277 *context, glm::vec3 p, float r)
    : Drawable(context), center(p), radius(r)
{}

void SearchSphere::create()
{
    std::vector<glm::vec3> pos, col;
    std::vector<GLuint> idx;

    glm::vec3 p;
    // X
    for(int i = 0; i < 20; ++i)
    {
        p = glm::vec3(glm::rotate(glm::mat4(), glm::radians(18.f * i), glm::vec3(1,0,0)) * glm::vec4(0,this->radius,0,1));
        p += this->center;
        pos.push_back(p);
        col.push_back(glm::vec3(0,1,1));
    }
    // Y
    for(int i = 0; i < 20; ++i)
    {
        p = glm::vec3(glm::rotate(glm::mat4(), glm::radians(18.f * i), glm::vec3(0,1,0)) * glm::vec4(this->radius,0,0,1));
        p += this->center;
        pos.push_back(p);
        col.push_back(glm::vec3(0,1,1));
    }
    // Z
    for(int i = 0; i < 20; ++i)
    {
        p = glm::vec3(glm::rotate(glm::mat4(), glm::radians(18.f * i), glm::vec3(0,0,1)) * glm::vec4(0,this->radius,0,1));
        p += this->center;
        pos.push_back(p);
        col.push_back(glm::vec3(0,1,1));
    }
    //Indices X
    for(int i = 1; i < 20; ++i){
        idx.push_back(i - 1);
        idx.push_back(i);
    }
    idx.push_back(19);
    idx.push_back(0);
    //Indices Y
    for(int i = 21; i < 40; ++i){
        idx.push_back(i - 1);
        idx.push_back(i);
    }
    idx.push_back(39);
    idx.push_back(20);
    //Indices Z
    for(int i = 41; i < 60; ++i){
        idx.push_back(i - 1);
        idx.push_back(i);
    }
    idx.push_back(59);
    idx.push_back(40);

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

GLenum SearchSphere::drawMode()
{
    return GL_LINES;
}
