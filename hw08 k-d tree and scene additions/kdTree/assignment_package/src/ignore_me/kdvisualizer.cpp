#include "kdvisualizer.h"

using namespace glm;

KDVisualizer::KDVisualizer(GLWidget277 *context, KDTree *tree)
    : Drawable(context), created(false), kdTree(tree)
{}

void KDVisualizer::destroy()
{
    Drawable::destroy();
    created = false;
}

void treeVis(KDNode* curr, std::vector<glm::vec3>& pos, std::vector<glm::vec3>& col, std::vector<GLuint>& idx, GLuint& minIdx)
{
    if(curr == nullptr || curr->leftChild == nullptr || curr->rightChild == nullptr) return;

    // Find our split axis
    // Look at left and right children
    // Left's rightmost point is the X point of pos, assuming X split.

    vec3 min = curr->minCorner;
    vec3 max = curr->maxCorner;
//    vec3 len = (max - min) * 0.5f;
    vec3 color;
    float mid;
    switch(curr->axis)
    {
    case 0:
        color = vec3(1,0,0);
        mid = curr->leftChild->maxCorner.x;
        pos.push_back(vec3(mid, min.y, max.z));
        pos.push_back(vec3(mid, max.y, max.z));
        pos.push_back(vec3(mid, max.y, min.z));
        pos.push_back(vec3(mid, min.y, min.z));
        break;
    case 1:
        color = vec3(0,1,0);
        mid = curr->leftChild->maxCorner.y;
        pos.push_back(vec3(min.x, mid, max.z));
        pos.push_back(vec3(max.x, mid, max.z));
        pos.push_back(vec3(max.x, mid, min.z));
        pos.push_back(vec3(min.x, mid, min.z));
        break;
    case 2:
        color = vec3(0,0,1);
        mid = curr->leftChild->maxCorner.z;
        pos.push_back(vec3(max.x, min.y, mid));
        pos.push_back(vec3(max.x, max.y, mid));
        pos.push_back(vec3(min.x, max.y, mid));
        pos.push_back(vec3(min.x, min.y, mid));

        break;
    }
    for(int i = 0; i < 4; ++i)
    {
        col.push_back(color);
    }
    idx.push_back(minIdx + 0);
    idx.push_back(minIdx + 1);
    idx.push_back(minIdx + 1);
    idx.push_back(minIdx + 2);
    idx.push_back(minIdx + 2);
    idx.push_back(minIdx + 3);
    idx.push_back(minIdx + 3);
    idx.push_back(minIdx + 0);
    minIdx += 4;

    treeVis(curr->leftChild, pos, col, idx, minIdx);
    treeVis(curr->rightChild, pos, col, idx, minIdx);
}

void KDVisualizer::create()
{
    created = true;
    // Traverse kd tree, draw boxes based on node data
    std::vector<glm::vec3> pos, col;
    std::vector<GLuint> idx;

    // Make bounding box

    //    6-------5
    //   /       /
    //  /       /
    // 7-------4

    //    0-------3
    //   /       /
    //  /       /
    // 1-------2
    vec3 min(kdTree->minCorner);
    vec3 max(kdTree->maxCorner);
    pos.push_back(min);                         col.push_back(vec3(0.5, 0.5, 0.5)); //0
    pos.push_back(vec3(min.x, min.y, max.z));   col.push_back(vec3(0.5, 0.5, 1));       //1
    pos.push_back(vec3(max.x, min.y, max.z));   col.push_back(vec3(1, 0.5, 1));       //2
    pos.push_back(vec3(max.x, min.y, min.z));   col.push_back(vec3(1, 0.5, 0.5));       //3
    pos.push_back(max);                         col.push_back(vec3(1, 1, 1));       //4
    pos.push_back(vec3(max.x, max.y, min.z));   col.push_back(vec3(1, 1, 0.5));       //5
    pos.push_back(vec3(min.x, max.y, min.z));   col.push_back(vec3(0.5, 1, 0.5));       //6
    pos.push_back(vec3(min.x, max.y, max.z));   col.push_back(vec3(0.5, 1, 1));       //7
//    for(int i = 0; i < 8; ++i)
//    {
//        col.push_back(vec3(1,1,1));
//    }
    //Bot
    idx.push_back(0); idx.push_back(1);
    idx.push_back(1); idx.push_back(2);
    idx.push_back(2); idx.push_back(3);
    idx.push_back(3); idx.push_back(0);
    //Top
    idx.push_back(4); idx.push_back(5);
    idx.push_back(5); idx.push_back(6);
    idx.push_back(6); idx.push_back(7);
    idx.push_back(7); idx.push_back(4);
    //Side
    idx.push_back(0); idx.push_back(6);
    idx.push_back(3); idx.push_back(5);
    idx.push_back(1); idx.push_back(7);
    idx.push_back(2); idx.push_back(4);

    // Iterate over all nodes and draw the plane between them
    GLuint minIdx = 8;
    treeVis(this->kdTree->root, pos, col, idx, minIdx);

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

GLenum KDVisualizer::drawMode()
{
    return GL_LINES;
}
