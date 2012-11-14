#include "mygraphicsview.h"
#include "mainwindow.h"
#include <QtGui>

MyGraphicsView::MyGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
    imageItem = new QGraphicsPixmapItem();
    setAcceptDrops(true);
}

void MyGraphicsView::rValue(int r)
{
    qDebug() << "r: " << r;

    QPixmap map = imageItem->pixmap();
    QImage im = map.toImage();

   /* for(int i=0; i<im.width()-r; i+=r){
        for(int j=0; j<im.width()-r; j+=r){
            int rSum = 0;
            for(int k=i; k<r; k++){
                for(int o=j; o<r; i++){
                    QRgb rgb = im.pixel(j, i);
                    rSum += qRed(rgb);
                }
            }
            rSum = rSum/(r*r);
            for(int k=i; k<r; k++){
                for(int o=j; o<r; i++){
                    im.setPixel(o, k, rSum);
                }
            }
        }
    }*/
}

void MyGraphicsView::gValue(int g)
{
    qDebug() << "g: " << g;
}

void MyGraphicsView::bValue(int b)
{
    qDebug() << "b: " << b;
}

void MyGraphicsView::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "dragenterevent";
    if(event->mimeData()->hasImage() || event->mimeData()->hasFormat("text/uri-list")){
        event->acceptProposedAction();
        event->setAccepted(true);
        update();
    }
}

void MyGraphicsView::dragMoveEvent(QDragMoveEvent *event)
{
    qDebug() << "dragmoveevent";
    event->acceptProposedAction();
    event->setAccepted(true);
}

void MyGraphicsView::dropEvent(QDropEvent *event)
{
    qDebug() << "dropevent";
    getImageFromMimeData(event->mimeData());
    event->acceptProposedAction();
    event->setAccepted(true);
    update();
}

void MyGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "mousemoveevent";
    this->setCursor(Qt::ClosedHandCursor);
}

void MyGraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug() << "mousedoubleclick";
    resetTransform();
}

void MyGraphicsView::getImageFromMimeData(const QMimeData *mimeData){
   if( mimeData->hasUrls() ){
        QImage image(mimeData->urls().first().toLocalFile());
        setImage(QPixmap::fromImage(image));
    }
}

void MyGraphicsView::setImage(QPixmap map){
    imageItem->setPixmap(map);
    imageItem->setPos(0, 0);
    if(scene()->items().isEmpty()){
      scene()->addItem(imageItem);
    }
}
