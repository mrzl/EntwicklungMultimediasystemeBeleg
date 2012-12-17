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

    connect(ui->yValueSlider,
            SIGNAL(valueChanged(int)),
            this,
            SIGNAL(yValueChanged(int)));

    connect(ui->uValueSlider,
            SIGNAL(valueChanged(int)),
            this,
            SIGNAL(uValueChanged(int)));

    connect(ui->vValueSlider,
            SIGNAL(valueChanged(int)),
            this,
            SIGNAL(vValueChanged(int)));

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
    connect(ui->rgbRadioButton,
            SIGNAL(toggled(bool)),
            this,
            SIGNAL(rgbRadio(bool)));
    connect(ui->yuvRadioButton,
            SIGNAL(toggled(bool)),
            this,
            SIGNAL(yuvRadio(bool)));

    enableRGBSliders(true);
}

void MySliderDialog::reset(){
    ui->redValueSlider->setValue(1);
    ui->greenValueSlider->setValue(1);
    ui->blueValueSlider->setValue(1);
    ui->yValueSlider->setValue(1);
    ui->uValueSlider->setValue(1);
    ui->vValueSlider->setValue(1);
}

void MySliderDialog::enableRGBSliders(bool rgb){
    if(rgb){
        //disable yuv sliders
        ui->yValueSlider->setEnabled(false);
        ui->uValueSlider->setEnabled(false);
        ui->vValueSlider->setEnabled(false);

        //enable rgb sliders
        ui->redValueSlider->setEnabled(true);
        ui->greenValueSlider->setEnabled(true);
        ui->blueValueSlider->setEnabled(true);

        //hide yuv sliders
        ui->yValueSlider->setHidden(true);
        ui->uValueSlider->setHidden(true);
        ui->vValueSlider->setHidden(true);

        //show rgbsliders
        ui->redValueSlider->setHidden(false);
        ui->greenValueSlider->setHidden(false);
        ui->blueValueSlider->setHidden(false);

        //reset yuv sliders
        ui->yValueSlider->setValue(1);
        ui->uValueSlider->setValue(1);
        ui->vValueSlider->setValue(1);
    } else {
        //enable yuv sliders
        ui->yValueSlider->setEnabled(true);
        ui->uValueSlider->setEnabled(true);
        ui->vValueSlider->setEnabled(true);

        //disable rgb sliders
        ui->redValueSlider->setEnabled(false);
        ui->greenValueSlider->setEnabled(false);
        ui->blueValueSlider->setEnabled(false);

        //hide rgb sliders
        ui->redValueSlider->setHidden(true);
        ui->greenValueSlider->setHidden(true);
        ui->blueValueSlider->setHidden(true);

        //show yuv sliders
        ui->yValueSlider->setHidden(false);
        ui->uValueSlider->setHidden(false);
        ui->vValueSlider->setHidden(false);

        //reset rgb sliders
        ui->redValueSlider->setValue(1);
        ui->greenValueSlider->setValue(1);
        ui->blueValueSlider->setValue(1);
    }
}

MySliderDialog::~MySliderDialog()
{
    delete ui;
}

