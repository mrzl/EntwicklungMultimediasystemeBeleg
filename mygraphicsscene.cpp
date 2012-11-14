#include "mygraphicsscene.h"
#include <QtGui>

MyGraphicsScene::MyGraphicsScene(QWidget *parent) :
    QGraphicsScene(parent)
{
}

void MyGraphicsScene::dragEnterEvent(QDragMoveEvent *event)
{
    if(event->mimeData()->hasImage() || event->mimeData()->hasFormat("text/uri-list")){
        event->acceptProposedAction();
        event->setAccepted(true);
        update();
    }
}

void MyGraphicsScene::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
    event->setAccepted(true);
}

void MyGraphicsScene::dropEvent(QDragMoveEvent *event)
{
    qDebug() << "dropevent";
    getImageFromMimeData(event->mimeData());
    event->acceptProposedAction();
    event->setAccepted(true);
    update();
}

void MyGraphicsScene::getImageFromMimeData(const QMimeData *mimeData){
   if( mimeData->hasUrls() ){
        QImage image(mimeData->urls().first().toLocalFile());
        addImage(QPixmap::fromImage(image));
    }
}

void MyGraphicsScene::addImage(QPixmap map){

    //view = new MyGraphicsView();
    //setScene(scene);
    /*imageItem->setPixmap(map);
    imageItem->setPos(0, 0);
    QGraphicsScene *currScene = scene();
    if(currScene->items().isEmpty()){
      currScene->addItem(imageItem);
    }
    MainWindow *mw = (MainWindow*) parentWidget();
    mw->statusBar()->showMessage(tr("Image successfully loaded."));*/
}
