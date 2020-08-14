#include "mainwindow.h"
#include <ui_mainwindow.h>
#include "samplercontrols.h"
#include <iostream>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), controlWindow(new SamplerControls())
{
    ui->setupUi(this);
    ui->mygl->setFocus();

    connect(controlWindow, SIGNAL(sig_setSamples(int)), ui->mygl, SLOT(slot_setSamples(int)));
    connect(controlWindow, SIGNAL(sig_setSampleMode(SampleMode)), ui->mygl, SLOT(slot_setSampleMode(SampleMode)));

    connect(this, SIGNAL(sig_updateNumSamples(int)), controlWindow, SLOT(slot_updateNumSampleControls(int)));
    connect(controlWindow, SIGNAL(sig_setDisplayGrid()), ui->mygl, SLOT(slot_setDisplayGrid()));

    connect(ui->mygl, SIGNAL(sig_fixSampleModeBox(int)), controlWindow, SLOT(slot_fixSampleModeBox(int)));

    connect(controlWindow, SIGNAL(sig_buildKDTree()), ui->mygl, SLOT(slot_buildKDTree()));

    connect(controlWindow, SIGNAL(sig_updateSphereX(double)), ui->mygl, SLOT(slot_updateSphereX(double)));
    connect(controlWindow, SIGNAL(sig_updateSphereY(double)), ui->mygl, SLOT(slot_updateSphereY(double)));
    connect(controlWindow, SIGNAL(sig_updateSphereZ(double)), ui->mygl, SLOT(slot_updateSphereZ(double)));
    connect(controlWindow, SIGNAL(sig_updateSphereR(double)), ui->mygl, SLOT(slot_updateSphereR(double)));

    connect(controlWindow, SIGNAL(sig_searchPoints()), ui->mygl, SLOT(slot_searchPoints()));

    controlWindow->show();
    controlWindow->move(QApplication::desktop()->screen()->rect().center() - this->rect().center() + QPoint(this->width(), 0));
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    controlWindow->close();
    ui->mygl->errorWindow.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionDisplay_Controls_triggered()
{
    controlWindow->show();
}

void MainWindow::slot_updateNumSamples(int i)
{
    emit sig_updateNumSamples(i);
}
