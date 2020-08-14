#ifndef SAMPLERCONTROLS_H
#define SAMPLERCONTROLS_H

#include <QWidget>
#include "enums.h"

namespace Ui {
class SamplerControls;
}

class SamplerControls : public QWidget
{
    Q_OBJECT

public:
    explicit SamplerControls(QWidget *parent = 0);
    ~SamplerControls();

private:
    Ui::SamplerControls *ui;

    //Control variables
    unsigned int numSamples;     // How many samples to generate when warping
    SampleMode squareSampleMode; // Which type of square sampling method to use when generating samples
    WarpMethod warpMethod;       // Which shape to warp the square samples onto

public slots:
    void slot_setSamples(int); // Connected to the slider bar and spinbox, and emits sig_setSamples to the main window.
    void slot_setSampleMode(int);
    void slot_updateNumSampleControls(int);
    void slot_setDisplayGrid();
    void slot_fixSampleModeBox(int);
    void slot_buildKDTree();
    void slot_updateSphereX(double);
    void slot_updateSphereY(double);
    void slot_updateSphereZ(double);
    void slot_updateSphereR(double);
    void slot_searchPoints();


signals:
    void sig_setSamples(int);
    void sig_setSampleMode(SampleMode);
    void sig_setDisplayGrid();
    void sig_buildKDTree();

    void sig_updateSphereX(double);
    void sig_updateSphereY(double);
    void sig_updateSphereZ(double);
    void sig_updateSphereR(double);

    void sig_searchPoints();
};

#endif // SAMPLERCONTROLS_H
