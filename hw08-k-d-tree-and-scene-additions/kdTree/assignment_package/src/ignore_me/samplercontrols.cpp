#include "samplercontrols.h"
#include "ui_samplercontrols.h"

SamplerControls::SamplerControls(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SamplerControls)
{
    ui->setupUi(this);
    setFixedSize(size());
}

SamplerControls::~SamplerControls()
{
    delete ui;
}

void SamplerControls::slot_setSamples(int i)
{
    this->numSamples = i;
    emit sig_setSamples(i);
}

void SamplerControls::slot_setSampleMode(int i)
{
    SampleMode mode = RANDOM;
    switch(i)
    {
    case 1:
        mode = GRID;
        break;
    case 2:
        mode = STRATIFIED;
        break;
    }
    emit sig_setSampleMode(mode);
}

void SamplerControls::slot_updateNumSampleControls(int i)
{
//    ui->numSamplesSlider->setValue(i);
    ui->numSamplesSpinBox->setValue(i);
}

void SamplerControls::slot_setDisplayGrid()
{
    emit sig_setDisplayGrid();
}

void SamplerControls::slot_fixSampleModeBox(int i)
{
    ui->comboBox->setCurrentIndex(i);
}

void SamplerControls::slot_buildKDTree()
{
    emit sig_buildKDTree();
}

void SamplerControls::slot_updateSphereX(double d)
{
    emit sig_updateSphereX(d);
}
void SamplerControls::slot_updateSphereY(double d)
{
    emit sig_updateSphereY(d);
}
void SamplerControls::slot_updateSphereZ(double d)
{
    emit sig_updateSphereZ(d);
}
void SamplerControls::slot_updateSphereR(double d)
{
    emit sig_updateSphereR(d);
}

void SamplerControls::slot_searchPoints()
{
    emit sig_searchPoints();
}
