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

    setAcceptDrops(true);
    resize(QSize(800, 600));

    scene = new QGraphicsScene(this);

    view = new MyGraphicsView(this);
    view->setScene(scene);
    dialog = new MySliderDialog(this);

    gridLayout = new QGridLayout(ui->centralWidget);
    gridLayout->addWidget(view);


    updateStatusBar("Programm started");

    connect(dialog, SIGNAL(rValueChanged(int)), view, SLOT(rValue(int)));
    connect(dialog, SIGNAL(gValueChanged(int)), view, SLOT(gValue(int)));
    connect(dialog, SIGNAL(bValueChanged(int)), view, SLOT(bValue(int)));
}

void MainWindow::open(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
    if(!fileName.isEmpty()){
        QImage image(fileName);
        if(image.isNull()){
            QMessageBox::information(this, tr("Image Viewer"), tr("Could not open the file %1.").arg(fileName));
            return;
        }
        view->setImage(QPixmap::fromImage(image));
    }
}

void MainWindow::save(){
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::currentPath());
    if(!fileName.isEmpty()){
        QPixmap pixmap = view->imageItem->pixmap();
        pixmap.save(fileName, "PNG");
    }
}

void MainWindow::zoomIn(){
    view->scale(1.25, 1.25);
}

void MainWindow::zoomOut(){
    view->scale(0.8, 0.8);
}

void MainWindow::normalSize(){
    view->resetTransform();
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

void MainWindow::wheelEvent(QWheelEvent *wheelEvent)
{
    if(wheelEvent->delta() > 0){
        zoomIn();
    } else {
        zoomOut();
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasImage() || event->mimeData()->hasFormat("text/uri-list")){
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    getImageFromMimeData(event->mimeData());
    event->acceptProposedAction();
}

void MainWindow::getImageFromMimeData(const QMimeData *mimeData){
   if( mimeData->hasUrls() ){
        QImage image(mimeData->urls().first().toLocalFile());
        scene = new QGraphicsScene(this);
        view->setImage(QPixmap::fromImage(image));
    }
}

void MainWindow::updateStatusBar(const char* string)
{
    this->statusBar()->showMessage(tr(string));
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

MainWindow::~MainWindow()
{
    delete ui;
}

