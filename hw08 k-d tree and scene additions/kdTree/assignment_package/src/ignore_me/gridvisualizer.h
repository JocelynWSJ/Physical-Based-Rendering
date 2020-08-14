#pragma once
#include "drawable.h"
#include "sampler.h"

class GridVisualizer : public Drawable
{
public:
    GridVisualizer(GLWidget277 *context, Sampler* s);
    virtual void create();
    virtual GLenum drawMode();

private:
    Sampler* sampler;
};
