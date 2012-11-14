#include "mysliderdialog.h"
#include "ui_sliderdialog.h"
#include "QtGui"

MySliderDialog::MySliderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MySliderDialog)
{
    ui->setupUi(this);
    connect(ui->horizontalSlider,
            SIGNAL(valueChanged(int)),
            this,
            SIGNAL(rValueChanged(int)));

    connect(ui->horizontalSlider_2,
            SIGNAL(valueChanged(int)),
            this,
            SIGNAL(gValueChanged(int)));

    connect(ui->horizontalSlider_3,
            SIGNAL(valueChanged(int)),
            this,
            SIGNAL(bValueChanged(int)));
}

MySliderDialog::~MySliderDialog()
{
    delete ui;
}

