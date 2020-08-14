#pragma once
#include <ignore_me/drawable.h>
#include <implement_me/kdtree.h>

class KDVisualizer : public Drawable
{
public:
    KDVisualizer(GLWidget277* context, KDTree *tree);
    void create() override;
    void destroy() override;
    GLenum drawMode() override;

    void setTree(KDTree* t) { kdTree = t; }

    bool created;

private:
    KDTree* kdTree;
};
