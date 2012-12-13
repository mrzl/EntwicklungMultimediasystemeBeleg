#include "mysliderdialog.h"
#include "ui_sliderdialog.h"
#include "QtGui"

MySliderDialog::MySliderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MySliderDialog)
{
    ui->setupUi(this);

    ui->radioButton->setChecked(false);

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

    connect(ui->radioButton,
            SIGNAL(clicked(bool)),
            this,
            SIGNAL(previewChanged(bool)));

    connect(ui->buttonBox,
            SIGNAL(accepted()),
            this,
            SIGNAL(buttonAccepted()));
    connect(ui->buttonBox,
            SIGNAL(rejected()),
            this,
            SIGNAL(buttonRejected()));
}

void MySliderDialog::reset(){
    ui->redValueSlider->setValue(1);
    ui->greenValueSlider->setValue(1);
    ui->blueValueSlider->setValue(1);
}

MySliderDialog::~MySliderDialog()
{
    delete ui;
}

