#include <QDialog>

#ifndef MYSLIDERDIALOG_H
#define MYSLIDERDIALOG_H

namespace Ui {
class MySliderDialog;
}

class MySliderDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MySliderDialog(QWidget *parent = 0);
    ~MySliderDialog();
    bool previewOkay;

signals:
    void rValueChanged(int newRValue);
    void gValueChanged(int newGValue);
    void bValueChanged(int newBValue);
    void previewChanged(bool preview);
    void buttonAccepted();

private:
    Ui::MySliderDialog *ui;
};

#endif MYSLIDERDIALOG_H
