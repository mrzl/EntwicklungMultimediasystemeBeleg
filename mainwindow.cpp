#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mygraphicsview.h"
#include "mysliderdialog.h"
#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    resize(QSize(800, 600));
    setMinimumSize(200, 200);

    //setMaximumSize(QApplication::desktop()->screenGeometry().width(), QApplication::desktop()->screenGeometry().height());

    scene = new QGraphicsScene(this);

    view = new MyGraphicsView(this);
    view->setScene(scene);
    dialog = new MySliderDialog(this);

    gridLayout = new QGridLayout(ui->centralWidget);
    gridLayout->addWidget(view);


    updateStatusBar(tr("Programm started."));

    connect(dialog, SIGNAL(rValueChanged(int)), view, SLOT(rValue(int)));
    connect(dialog, SIGNAL(gValueChanged(int)), view, SLOT(gValue(int)));
    connect(dialog, SIGNAL(bValueChanged(int)), view, SLOT(bValue(int)));
}

void MainWindow::open(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath(), tr("Images")+" (*.png *.bmp *.jpg *.jpeg);;"+tr("All Files")+" (*.*)");
    originalFileName = fileName;
    if(!fileName.isEmpty()){
        QImage image(fileName);
        if(image.isNull()){
            QMessageBox::information(this, tr("Image Viewer"), tr("Could not open the file %1.").arg(fileName));
            updateStatusBar(tr("Could not open File."));
        } else {
            view->setImage(QPixmap::fromImage(image));

            setWindowTitle(fileName);
            updateStatusBar(tr("Image successfully loaded."));
        }
    }
}

void MainWindow::save(){
    QString fileFormat = getFileFormat(originalFileName);
    if(!originalFileName.isEmpty()){
        QPixmap pixmap = view->imageItem->pixmap();
        pixmap.save(originalFileName, fileFormat.toStdString().c_str());
        updateStatusBar(tr("Image successfully saved."));
    } else {
        updateStatusBar(tr("Image could not be saved."));
    }
}

void MainWindow::saveAs(){
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::currentPath(), tr("PNG")+" (*.png);;"+tr("JPEG")+" (*.jpg *.jpeg);;"+tr("BMP")+" (*.bmp)");
    QString fileFormat = getFileFormat(fileName);
    if(!fileName.isEmpty()){
        QPixmap pixmap = view->imageItem->pixmap();
        pixmap.save(fileName, fileFormat.toStdString().c_str());
        updateStatusBar(tr("Image successfully saved."));
    } else {
        updateStatusBar(tr("Image could not be saved."));
    }
}

QString MainWindow::getFileFormat(QString strImageFileName){
    QString returnString = "";
    QStringList splittedStrList = strImageFileName.split(".");
    if( ((splittedStrList.size())-1) >= 0) {
        returnString = splittedStrList[((splittedStrList.size())-1)];
    }
    return returnString;
}

/*
 *
*/
void MainWindow::zoomIn(){
    view->zoomIn(1.25);
}

/*
 *
*/
void MainWindow::zoomOut(){
    view->zoomOut(0.8);
}

void MainWindow::normalSize(){
    view->resetTransform();
    updateStatusBar(tr("Image restored to original size."));
}

void MainWindow::openDialog(){
    dialog->show();
}

void MainWindow::about(){
    QMessageBox::about(this, tr("About Belegarbeit"), tr("Belegarbeit im Fach Entwicklung von Multimediasystemen \nMarcel Schwittlick (s0529494) \nAlexander Marten (s0529999)\nDennis Hägler (s0529999)"));
}

void MainWindow::aboutQt(){
    QMessageBox::aboutQt(this);
}

void MainWindow::updateStatusBar(QString string)
{
    statusBar()->showMessage(string);
}

void MainWindow::rValue(int r)
{
    view->rValue(r);
}

void MainWindow::gValue(int g)
{
    view->gValue(g);
}

void MainWindow::bValue(int b)
{
    view->bValue(b);
}

void MainWindow::copyImage(){
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setImage(view->imageItem->pixmap().toImage());
    updateStatusBar(tr("Copied image to clipboard."));
}

void MainWindow::pasteImage(){
    QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    //checks if the clipboard contains an url to a file and sets it
    //in the view if it is an image
    if(mimeData->hasUrls()){
        QImage image(mimeData->urls().first().toLocalFile());
        if(!image.isNull()){
            setWindowTitle(mimeData->urls().first().toString());
            view->setImage(QPixmap::fromImage(image));
            updateStatusBar(tr("Pasting from clipboard succeded."));
        } else {
            updateStatusBar(tr("Pasting from clipboard failed."));
        }
    }

    //checks if an image is in the clipboard and sets in the view
    if(clipboard->image().isNull()){
        updateStatusBar(tr("Pasting from clipboard failed."));
    } else {
        view->setImage(QPixmap::fromImage(clipboard->image()));
        updateStatusBar(tr("Pasting from clipboard succeded."));
        setWindowTitle("*");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

