#include "mygl.h"
#include "la.h"

#include <iostream>
#include <QApplication>
#include <QKeyEvent>


MyGL::MyGL(QWidget *parent)
    : GLWidget277(parent),
      camera(),
      prog_flat(this),
      vao(),
      kdVisualizer(this, nullptr),
      sampler(),
      pointVisualizer(this, &sampler),
      numSamples(4), sampleMode(RANDOM),
      kdTree(), searchSphere(this, glm::vec3(0.5,0.5,0.5), 0.125f),
      searchVis(this),
      mX(), mY(),
      displayGrid(true),
      errorWindow()
{
    setFocusPolicy(Qt::StrongFocus);
}

MyGL::~MyGL()
{
    makeCurrent();

    glDeleteVertexArrays(1, &vao);
}

void MyGL::initializeGL()
{
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(3);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0, 0, 0, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    // Create and set up the diffuse shader
    // Create and set up the flat lighting shader
    prog_flat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");
    prog_flat.setModelMatrix(glm::mat4(1.f));

    sampler.generateSamples(numSamples, sampleMode);
    pointVisualizer.create();
    searchSphere.create();

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    //    vao.bind();
    glBindVertexArray(vao);
}

void MyGL::resizeGL(int w, int h)
{

    camera.setAspect(w, h);
    glm::mat4 viewproj = camera.getViewProj();

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)

    prog_flat.setViewProjMatrix(viewproj);

    printGLErrorLog();
}

//This function is called by Qt any time your GL window is supposed to update
//For example, when the function updateGL is called, paintGL is called implicitly.
void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    prog_flat.setViewProjMatrix(camera.getViewProj());

    prog_flat.setModelMatrix(glm::translate(glm::mat4(), glm::vec3(-0.5, -0.5, -0.5)));

    prog_flat.draw(pointVisualizer);

    if(kdVisualizer.created && displayGrid)
    {
        prog_flat.draw(kdVisualizer);
    }

    prog_flat.draw(searchSphere);

    if(searchVis.created)
    {
        glDisable(GL_DEPTH_TEST);
        prog_flat.draw(searchVis);
        glEnable(GL_DEPTH_TEST);
    }
}

void MyGL::keyPressEvent(QKeyEvent *e)
{
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    if (e->key() == Qt::Key_Escape)
    {
        QApplication::quit();
    }
    else if(e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
    {
        errorWindow.hide();
    }
}

void MyGL::mousePressEvent(QMouseEvent *e)
{
    mX = e->globalX();
    mY = e->globalY();
}

void MyGL::mouseMoveEvent(QMouseEvent *e)
{
    camera.rotatePhi(mY - e->globalY());
    camera.rotateTheta(e->globalX() - mX);
    mX = e->globalX();
    mY = e->globalY();
    update();
}

float roundToOne(float f)
{
    if(std::fabs(1.f - f) < 0.00001f)
    {
        return 1;
    }
    return f;
}

void MyGL::slot_setDisplayGrid()
{
    displayGrid = !displayGrid;
    refreshVisuals();
    if(!kdVisualizer.created && displayGrid) {
        kdVisualizer.create();
    }
}

void MyGL::refreshVisuals()
{
    sampler.generateSamples(numSamples, sampleMode);

    pointVisualizer.destroy(); pointVisualizer.create();

    update();
}

void MyGL::slot_buildKDTree()
{
    std::vector<glm::vec3*> particlePtrs(sampler.samples.size());
    for(unsigned int i = 0; i < particlePtrs.size(); ++i)
    {
        particlePtrs[i] = &(sampler.samples[i]);
    }
    kdTree.build(particlePtrs);
    kdVisualizer.setTree(&kdTree);
    if(kdVisualizer.created)
    {
        kdVisualizer.destroy();
    }
    if(displayGrid)
    {
        kdVisualizer.create();
    }
    update();
}

void MyGL::slot_searchPoints()
{
    if(kdTree.root == nullptr) return;

    std::vector<glm::vec3> pts = kdTree.particlesInSphere(searchSphere.center, searchSphere.radius);
    searchVis.recreate(pts);

    update();
}


void MyGL::slot_setSamples(int i)
{
    numSamples = i;
    refreshVisuals();
    kdTree.clear();
    kdVisualizer.destroy();
    emit sig_updateNumSamples(numSamples);
}

void MyGL::slot_updateSphereX(double d)
{
    searchSphere.center.x = d;
    searchSphere.destroy();
    searchSphere.create();
    searchVis.destroy();
    update();
}
void MyGL::slot_updateSphereY(double d)
{
    searchSphere.center.y = d;
    searchSphere.destroy();
    searchSphere.create();
    searchVis.destroy();
    update();
}
void MyGL::slot_updateSphereZ(double d)
{
    searchSphere.center.z = d;
    searchSphere.destroy();
    searchSphere.create();
    searchVis.destroy();
    update();
}
void MyGL::slot_updateSphereR(double d)
{
    searchSphere.radius = d;
    searchSphere.destroy();
    searchSphere.create();
    searchVis.destroy();
    update();
}

void MyGL::slot_setSampleMode(SampleMode m)
{
    sampleMode = m;
    refreshVisuals();
    kdVisualizer.destroy();
    searchVis.destroy();
}
