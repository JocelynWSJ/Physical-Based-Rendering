#include "kdtree.h"
#include <iostream>
KDNode::KDNode()
    : leftChild(nullptr), rightChild(nullptr), axis(0), minCorner(), maxCorner(), particles()
{}

KDNode::~KDNode()
{
    delete leftChild;
    delete rightChild;
}

KDTree::KDTree()
    : root(nullptr)
{}

KDTree::~KDTree()
{
    delete root;
}

// Comparator functions you can use with std::sort to sort vec3s along the cardinal axes
bool xSort(glm::vec3* a, glm::vec3* b) { return a->x < b->x; }
bool ySort(glm::vec3* a, glm::vec3* b) { return a->y < b->y; }
bool zSort(glm::vec3* a, glm::vec3* b) { return a->z < b->z; }


void KDTree::build(const std::vector<glm::vec3*> &points)
{
    minCorner = glm::vec3(0.f);
    maxCorner = glm::vec3(0.f);

    std::vector<glm::vec3*> cornerPtr = points;// vector used to computer minCorner and maxCorner
    std::sort(cornerPtr.begin(), cornerPtr.end(), xSort);
    minCorner.x = cornerPtr[0]->x;
    maxCorner.x = cornerPtr[cornerPtr.size()-1]->x;

    std::sort(cornerPtr.begin(), cornerPtr.end(), ySort);
    minCorner.y = cornerPtr[0]->y;
    maxCorner.y = cornerPtr[cornerPtr.size()-1]->y;

    std::sort(cornerPtr.begin(), cornerPtr.end(), zSort);
    minCorner.z = cornerPtr[0]->z;
    maxCorner.z = cornerPtr[cornerPtr.size()-1]->z;

    root = buildKdTree(points, 0);
}

KDNode* KDTree::buildKdTree(const std::vector<glm::vec3*> &points, int depth) {
    //if point does not contain point
    if (points.size() == 0) {
        KDNode* leaf = new KDNode();
        return leaf;
    }
    //if point contain only 1 point
    else if (points.size() == 1) {
        KDNode* leaf = new KDNode();
        leaf->axis = depth % 3;
        leaf->minCorner = *points[0];
        leaf->maxCorner = *points[0];
        leaf->particles.push_back(points[0]);

        return leaf;
    } else {
        KDNode* node = new KDNode();
        node->axis = depth % 3;
        std::vector<glm::vec3*> comparePtr = points; //vector used to sort
        int median = (int)(points.size()/2.f);
        //std::cout << median << " " << points.size() << std::endl;
        //sort the vector
        if (node->axis == 0) { //x axis
            std::sort(comparePtr.begin(), comparePtr.end(), xSort);
        } else if (node->axis == 1) { //y axis
            std::sort(comparePtr.begin(), comparePtr.end(), ySort);
        } else if (node->axis == 2) { //z axis
            std::sort(comparePtr.begin(), comparePtr.end(), zSort);
        }

        node->minCorner = glm::vec3(0.f);
        node->maxCorner = glm::vec3(0.f);

        //assign minCorner and maxCorner
        std::vector<glm::vec3*> cornerPtr = points;// vector used to computer minCorner and maxCorner


        std::sort(cornerPtr.begin(), cornerPtr.end(), xSort);
        node->minCorner.x = cornerPtr[0]->x;
        node->maxCorner.x = cornerPtr[cornerPtr.size()-1]->x;

        std::sort(cornerPtr.begin(), cornerPtr.end(), ySort);
        node->minCorner.y = cornerPtr[0]->y;
        node->maxCorner.y = cornerPtr[cornerPtr.size()-1]->y;

        std::sort(cornerPtr.begin(), cornerPtr.end(), zSort);
        node->minCorner.z = cornerPtr[0]->z;
        node->maxCorner.z = cornerPtr[cornerPtr.size()-1]->z;

        //std::cout << node->minCorner.x << ", "<< node->minCorner.x << ", "<< node->minCorner.x << std::endl;
        //std::cout << node->maxCorner.x << ", "<< node->maxCorner.x << ", "<< node->maxCorner.x << std::endl<< std::endl;

        //leftPtr
        std::vector<glm::vec3*> leftPtr;
        leftPtr.assign(comparePtr.begin(), comparePtr.begin()+median);

        node->leftChild = buildKdTree(leftPtr, depth+1);


        //rightPtr
        std::vector<glm::vec3*> rightPtr;
        rightPtr.assign(comparePtr.begin()+median, comparePtr.end());
        node->rightChild = buildKdTree(rightPtr, depth+1);


        return node;
    }
    return nullptr;
}

std::vector<glm::vec3> KDTree::particlesInSphere(glm::vec3 c, float r)
{
    //TODO
    std::vector<glm::vec3> result;
    getParticlesInSphere(root, c, r, result);
    return result;

}

void KDTree::getParticlesInSphere(KDNode* node, glm::vec3 c, float r, std::vector<glm::vec3> &result) {
    if (node == nullptr) {
        return;
    }
    if (node->particles.size() == 0) {
        getParticlesInSphere(node->leftChild, c, r, result);
        getParticlesInSphere(node->rightChild, c, r, result);
    } else {
        if (glm::length(*node->particles[0]-c) <= r) {
            result.push_back(*node->particles[0]);
        }
    }
}

void KDTree::clear()
{
    delete root;
    root = nullptr;
}
