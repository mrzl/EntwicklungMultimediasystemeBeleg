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
    isPreviewed = edited = false;
}

void MyGraphicsView::rValue(int r)
{
    currentR = r;
    if(isPreviewed){
        changeImage();
    }
}
void MyGraphicsView::gValue(int g)
{
    currentG = g;
    if(isPreviewed){
        changeImage();
    }
}
void MyGraphicsView::bValue(int b)
{
    currentB = b;
    if(isPreviewed){
        changeImage();
    }
}

void MyGraphicsView::changeImage()
{
    setEdited(true);
    QPixmap origiMap = backupItem->pixmap();
    QImage showI = imageItem->pixmap().toImage();
    QImage image = origiMap.toImage();
    QRgb pixelColor;
    int sum = 0;

    //Sets all Red Pixel from original Picture.
    for(int curWidth = 0; curWidth < image.width() - currentR; curWidth += currentR)
    {
        for(int curHeight = 0; curHeight < image.height() - currentR; curHeight += currentR)
        {
            sum = 0;
            sum = getColorSum(curWidth, curWidth + currentR,curHeight, curHeight + currentR, image, RED);
            sum = sum / (currentR * currentR);
            //set the colors around the pixel
            for(int k = curWidth; k < curWidth + currentR; k++)
            {
                for(int o = curHeight; o < curHeight + currentR; o++)
                {
                    pixelColor = qRgb(sum, 0, 0);
                    showI.setPixel(k, o, pixelColor);
                }
            }
        }
    }

    //Set ALL GREEN
    for(int curWidth = 0; curWidth < image.width() - currentG; curWidth += currentG)
    {
        for(int curHeight = 0; curHeight < image.height() - currentG; curHeight += currentG)
        {
            sum = 0;
            sum = getColorSum(curWidth, curWidth + currentG,curHeight, curHeight + currentG, image, GREEN);
            sum = sum / (currentG * currentG);
            //set the colors around the pixel
            for(int k = curWidth; k < curWidth + currentG; k++)
            {
                for(int o = curHeight; o < curHeight + currentG; o++)
                {
                    pixelColor = qRgb(qRed(showI.pixel(k, o)), sum, 0);
                    showI.setPixel(k, o, pixelColor);
                }
            }
        }
    }

    //Set All blue
    for(int curWidth = 0; curWidth < image.width() - currentB; curWidth += currentB)
    {
        for(int curHeight = 0; curHeight < image.height() - currentB; curHeight += currentB)
        {
            sum = 0;
            sum = getColorSum(curWidth, curWidth + currentB,curHeight, curHeight + currentB, image, BLUE);
            sum = sum / (currentB * currentB);
            //set the colors around the pixel
            for(int k = curWidth; k < curWidth + currentB; k++)
            {
                for(int o = curHeight; o < curHeight + currentB; o++)
                {
                    pixelColor = qRgb(qRed(showI.pixel(k, o)), qGreen(showI.pixel(k, o)), sum);
                    showI.setPixel(k, o, pixelColor);
                }
            }
        }
    }
    imageItem->setPixmap(QPixmap::fromImage(showI));
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
    changeImage();
    backupItem = imageItem;

}

void::MyGraphicsView::rejectedButton(){
    currentR = currentG = currentB = 1;
    changeImage();
    if(edited){
        dynamic_cast<MainWindow*>(par)->setWindowTitle( dynamic_cast<MainWindow*>(par)->windowTitle().remove( dynamic_cast<MainWindow*>(par)->windowTitle().length()-1, 1));
        edited = false;
    }
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

void MyGraphicsView::setEdited(bool edited){
    QString windowTitle = dynamic_cast<MainWindow*>(par)->windowTitle();
    QString lastCharacterOfTitle = windowTitle.mid(windowTitle.length()-1, 1);
    if(edited){
       if(lastCharacterOfTitle != "*"){
           windowTitle.append("*");
       }
    } else {
        if(lastCharacterOfTitle == "*"){
            windowTitle = windowTitle.mid(0, windowTitle.length()-1);
        }
    }
    dynamic_cast<MainWindow*>(par)->setWindowTitle(windowTitle);
    this->edited = edited;
}

bool MyGraphicsView::isEdited(){
    return edited;
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
