#include "mygraphicsview.h"
#include "mainwindow.h"
#include <QtGui>

MyGraphicsView::MyGraphicsView(QMainWindow *parent) :
    QGraphicsView(parent)
{
    par = parent;

    this->setCursor(Qt::OpenHandCursor);

    imageItem = new QGraphicsPixmapItem();
    backupItem = new QGraphicsPixmapItem();
    setAcceptDrops(true);
    oldR = oldG = oldB = 1;
}

void MyGraphicsView::rValue(int r)
{
    qDebug() << "r: " << r;
    QPixmap map;
    if(r > oldR){
        map = imageItem->pixmap();
    } else {
        map = backupItem->pixmap();
    }
    QImage im = map.toImage();

    for(int i=0; i<im.width()-r; i+=r){
        for(int j=0; j<im.height()-r; j+=r){
            int rSum = 0;
            for(int k=i; k<i+r; k++){
                for(int o=j; o<j+r; o++){
                    QRgb pixelColor = im.pixel(k, o);
                    int value = qRed(pixelColor);
                    rSum += value;
                }
            }

            rSum = rSum/(r*r);

            for(int k=i; k<i+r; k++){
                for(int o=j; o<j+r; o++){
                    QRgb pixelColor = qRgb(rSum, qGreen(im.pixel(k, o)), qBlue(im.pixel(k, o)));
                    im.setPixel(k, o, pixelColor);
                }
            }
        }
    }

    imageItem->setPixmap(QPixmap::fromImage(im));
    oldR = r;
}

void MyGraphicsView::gValue(int g)
{
    qDebug() << "g: " << g;

    QPixmap map;
    if(g > oldG){
        map = imageItem->pixmap();
    } else {
        map = backupItem->pixmap();
    }
    QImage im = map.toImage();

    for(int i=0; i<im.width()-g; i+=g){
        for(int j=0; j<im.height()-g; j+=g){
            int rSum = 0;
            for(int k=i; k<i+g; k++){
                for(int o=j; o<j+g; o++){
                    QRgb pixelColor = im.pixel(k, o);
                    int value = qGreen(pixelColor);
                    rSum += value;
                }
            }

            rSum = rSum/(g*g);

            for(int k=i; k<i+g; k++){
                for(int o=j; o<j+g; o++){
                    QRgb pixelColor = qRgb(qRed(im.pixel(k, o)), rSum, qBlue(im.pixel(k, o)));
                    im.setPixel(k, o, pixelColor);
                }
            }
        }
    }

    imageItem->setPixmap(QPixmap::fromImage(im));
    oldG = g;
}

void MyGraphicsView::bValue(int b)
{
    qDebug() << "b: " << b;

    QPixmap map;
    if(b > oldB){
        map = imageItem->pixmap();
    } else {
        map = backupItem->pixmap();
    }
    QImage im = map.toImage();

    for(int i=0; i<im.width()-b; i+=b){
        for(int j=0; j<im.height()-b; j+=b){
            int rSum = 0;
            for(int k=i; k<i+b; k++){
                for(int o=j; o<j+b; o++){
                    QRgb pixelColor = im.pixel(k, o);
                    int value = qBlue(pixelColor);
                    rSum += value;
                }
            }

            rSum = rSum/(b*b);

            for(int k=i; k<i+b; k++){
                for(int o=j; o<j+b; o++){
                    QRgb pixelColor = qRgb(qRed(im.pixel(k, o)), qGreen(im.pixel(k, o)), rSum);
                    im.setPixel(k, o, pixelColor);
                }
            }
        }
    }

    imageItem->setPixmap(QPixmap::fromImage(im));
    oldB = b;
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
