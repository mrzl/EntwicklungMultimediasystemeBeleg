#include "mygraphicsview.h"
#include "mainwindow.h"
#include <QtGui>

MyGraphicsView::MyGraphicsView(QMainWindow *parent) :
    QGraphicsView(parent)
{
    par = parent;

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

void MyGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    this->setCursor(Qt::ClosedHandCursor);
}

void MyGraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    resetTransform();
}

void MyGraphicsView::getImageFromMimeData(const QMimeData *mimeData){
   if( mimeData->hasUrls() ){
        QImage image(mimeData->urls().first().toLocalFile());
            setImage(QPixmap::fromImage(image));
    }
}

void MyGraphicsView::setImage(QPixmap map){
    imageItem->setPos(0, 0);
    imageItem->setPixmap(map);

    backupItem->setPos(0, 0);
    backupItem->setPixmap(map);
    qDebug() << imageItem->pos()<< " " << imageItem->pos();
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
