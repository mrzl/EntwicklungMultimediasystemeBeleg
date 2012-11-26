#include "mysliderdialog.h"
#include "ui_sliderdialog.h"
#include "QtGui"

MySliderDialog::MySliderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MySliderDialog)
{
    ui->setupUi(this);
    connect(ui->redValueSlider,
            SIGNAL(valueChanged(int)),
            this,
            SIGNAL(rValueChanged(int)));

    connect(ui->greenValueSlider,
            SIGNAL(valueChanged(int)),
            this,
            SIGNAL(gValueChanged(int)));

    connect(ui->blueValueSlider,
            SIGNAL(valueChanged(int)),
            this,
            SIGNAL(bValueChanged(int)));
}

MySliderDialog::~MySliderDialog()
{
    delete ui;
}

