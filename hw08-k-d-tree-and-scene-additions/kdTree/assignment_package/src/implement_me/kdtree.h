#pragma once
#include <ignore_me/la.h>

class KDNode
{
public:
    KDNode();
    ~KDNode();

    KDNode* leftChild;
    KDNode* rightChild;
    unsigned int axis; // Which axis split this node represents
    glm::vec3 minCorner, maxCorner; // The world-space bounds of this node; 0 means X-axis, 1 means Y-axis, and 2 means Z-axis
    std::vector<glm::vec3*> particles; // A collection of pointers to the particles contained in this node.
};

class KDTree
{
public:
    KDTree();
    ~KDTree();
    void build(const std::vector<glm::vec3*> &points);
    KDNode* buildKdTree(const std::vector<glm::vec3*> &points, int depth);
    void clear();

    std::vector<glm::vec3> particlesInSphere(glm::vec3 c, float r); // Returns all the points contained within a sphere with center c and radius r
    void getParticlesInSphere(KDNode* node, glm::vec3 c, float r, std::vector<glm::vec3> &result);

    KDNode* root;
    glm::vec3 minCorner, maxCorner; // For visualization purposes
};
