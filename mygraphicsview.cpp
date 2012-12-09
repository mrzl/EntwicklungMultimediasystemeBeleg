#include "mygraphicsview.h"
#include "mainwindow.h"
#include <QtGui>

#define RED 1
#define GREEN 2
#define BLUE 3

MyGraphicsView::MyGraphicsView(QMainWindow *parent) :
    QGraphicsView(parent)
{
    par = parent;

    this->setCursor(Qt::OpenHandCursor);

    imageItem = new QGraphicsPixmapItem();
    backupItem = new QGraphicsPixmapItem();
    setAcceptDrops(true);
    oldR = oldG = oldB = 1;
    currentR = currentG = currentB = 1;
    isPreviewed = false;
}

void MyGraphicsView::rValue(int r)
{
    if(isPreviewed){
        changeImage(r, RED);
    } else {
        currentR = r;
    }
}

void MyGraphicsView::gValue(int g)
{
    if(isPreviewed){
        changeImage(g, GREEN);
    } else {
        currentG = g;
    }
}

void MyGraphicsView::bValue(int b)
{
    if(isPreviewed){
        changeImage(b, BLUE);
    } else {
       currentB = b;
    }
}

void MyGraphicsView::changeImage(int sampleRate, int color)
{
    QPixmap origiMap = backupItem->pixmap();
    QImage image = origiMap.toImage();
    QRgb pixelColor;
    for(int curWidth = 0; curWidth < image.width() - sampleRate; curWidth += sampleRate)
    {
        for(int curHeight = 0; curHeight < image.height() - sampleRate; curHeight += sampleRate)
        {
            int rSum = 0;
            rSum = getColorSum(curWidth, curWidth + sampleRate,curHeight, curHeight + sampleRate, image, color);
            rSum = rSum / (sampleRate * sampleRate);
            //set the colors around the pixel
            for(int k = curWidth; k < curWidth + sampleRate; k++)
            {
                for(int o = curHeight; o < curHeight + sampleRate; o++)
                {
                    switch(color)
                    {
                     case 1:
                        pixelColor = qRgb(rSum, qGreen(image.pixel(k, o)), qBlue(image.pixel(k, o)));
                        break;
                     case 2:
                        pixelColor = qRgb(qRed(image.pixel(k,o)), rSum, qBlue(image.pixel(k, o)));
                        break;
                     case 3:
                        pixelColor = qRgb(qRed(image.pixel(k,o)), qGreen(image.pixel(k, o)), rSum);
                        break;
                     default:
                        break;
                    }
                    image.setPixel(k, o, pixelColor);
                }
            }
        }
    }
    imageItem->setPixmap(QPixmap::fromImage(image));
}

int MyGraphicsView::getColorSum(int startW, int endW, int startH, int endH, QImage image, int color)
{
    QRgb pixelColor;
    int sum = 0;
    int value = 0;
    for(int k = startW; k < endW; k++)
    {
        for(int  o = startH; o < endH; o++)
        {
            pixelColor = image.pixel(k, o);
            switch(color)
            {
             case 1:
                value = qRed(pixelColor);
                break;
             case 2:
                value = qGreen(pixelColor);
                break;
             case 3:
                value = qBlue(pixelColor);
                break;
             default:
                break;
            }
            sum += value;
        }
    }
    return sum;
}

void MyGraphicsView::preview(bool preview){
    isPreviewed = preview;
}

void MyGraphicsView::okayButton(){
    bool oldIsPrevied = isPreviewed;
    isPreviewed = true;
    qDebug() << "r:"+currentR;
    qDebug() << "g:"+currentG;
    qDebug() << "b:"+currentB;
    rValue(currentR);
    gValue(currentG);
    bValue(currentB);
    isPreviewed = oldIsPrevied;
}

void MyGraphicsView::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasImage() || event->mimeData()->hasFormat("text/uri-list")){
        event->acceptProposedAction();
        event->setAccepted(true);
        update();
    }
}

void MyGraphicsView::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
    event->setAccepted(true);
}

void MyGraphicsView::dropEvent(QDropEvent *event)
{
    getImageFromMimeData(event->mimeData());
    event->acceptProposedAction();
    event->setAccepted(true);
    update();
}

void MyGraphicsView::mousePressEvent(QMouseEvent *event){
    event->setAccepted(true);
    oldX = event->x();
    oldY = event->y();
}

void MyGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    this->setCursor(Qt::ClosedHandCursor);
    event->setAccepted(true);

    int newX = event->x();
    int newY = event->y();
    imageItem->translate(newX-oldX, newY-oldY);
    oldX = newX;
    oldY = newY;
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent *event){
    this->setCursor(Qt::OpenHandCursor);
}

void MyGraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    resetTransform();
    QClipboard *clipboard = QApplication::clipboard();
     //QString originalText = clipboard->text();
    clipboard->setText("newText");
}


void MyGraphicsView::wheelEvent(QWheelEvent *wheelEvent)
{
    if(wheelEvent->delta() > 0){
        zoomIn(1.25);
    } else {
        zoomOut(0.8);
    }
}

void MyGraphicsView::getImageFromMimeData(const QMimeData *mimeData){
   if( mimeData->hasUrls() ){
        QImage image(mimeData->urls().first().toLocalFile());
        if(!image.isNull()){
            setImage(QPixmap::fromImage(image));
            dynamic_cast<MainWindow*>(par)->updateStatusBar(tr("Image sucessfully loaded."));
            dynamic_cast<MainWindow*>(par)->setWindowTitle(mimeData->urls().first().toString());
        } else {
             dynamic_cast<MainWindow*>(par)->updateStatusBar(tr("Image loading failed."));
        }
    } else {
       dynamic_cast<MainWindow*>(par)->updateStatusBar(tr("Image loading failed."));
   }
}

void MyGraphicsView::setImage(QPixmap map){
    imageItem->setPos(0, 0);
    imageItem->setPixmap(map);

    backupItem->setPos(0, 0);
    backupItem->setPixmap(map);
    resetMatrix();

    if(scene()->items().isEmpty()){
      scene()->addItem(imageItem);
    }
}

void MyGraphicsView::zoomIn(float factor){
    if(matrix().m11() < 3.8){
        scale(factor, factor);
        dynamic_cast<MainWindow*>(par)->updateStatusBar(tr("Image zoomed in."));
    }
}

void MyGraphicsView::zoomOut(float factor){
    if(matrix().m11() > 0.27){
        scale(factor, factor);
        dynamic_cast<MainWindow*>(par)->updateStatusBar(tr("Image zoomed out."));
    }
}
