#include "mygraphicsview.h"
#include "mainwindow.h"
#include <QtGui>

#define RED 1
#define GREEN 2
#define BLUE 3
#define MODE_RGB 4
#define MODE_YUV 5

MyGraphicsView::MyGraphicsView(QMainWindow *parent) :
    QGraphicsView(parent)
{
    par = parent;

    this->setCursor(Qt::OpenHandCursor);

    imageItem = new QGraphicsPixmapItem();
    backupItem = new QGraphicsPixmapItem();
    setAcceptDrops(true);
    oldR = 1;
    oldG = 1;
    oldB = 1;
    currentR = 1;
    currentG = 1;
    currentB = 1;
    currentY = 1;
    currentU = 1;
    currentV = 1;
    isPreviewed = false;
    edited = false;
    selectedMode = 4; // rgb
    qDebug() << selectedMode;
    initArrays();
}

void MyGraphicsView::initArrays()
{
    QPixmap origiMap = backupItem->pixmap();
    QImage image = origiMap.toImage();
    int w = image.width();
    int h = image.height();
    int size = w * h;
    red = new float[size];
    green = new float[size];
    blue = new float[size];
    int index = 0;
    int offset = 0;
    for(int curHeight = 0; curHeight < image.height(); curHeight++)
    {
        offset = curHeight * image.width();
        for(int curWidth = 0; curWidth < image.width(); curWidth++)
        {
            index = offset + curWidth;
            red[index] = qRed(image.pixel(curWidth, curHeight));
            green[index] = qGreen(image.pixel(curWidth, curHeight));
            blue[index] = qBlue(image.pixel(curWidth, curHeight));
        }
    }
}

void MyGraphicsView::deleteArray()
{
    delete[] red;
    delete[] green;
    delete[] blue;
}

void MyGraphicsView::reinitArrays()
{
    deleteArray();
    initArrays();
}

void MyGraphicsView::convertRgbToYuv()
{
    QPixmap origiMap = imageItem->pixmap();
    QImage image = origiMap.toImage();
    int w = image.width();
    int h = image.height();
    int size = w * h;
    yCh = new float[size];
    uCh = new float[size];
    vCh = new float[size];
    for(int i = 0; i < size; i++)
    {
        yCh[i] = 0.299 * red[i] + 0.587 * green[i] + 0.114 * blue[i];
        uCh[i] = red[i] * -0.168736 + green[i] * -0.331264 + blue[i] * 0.5 + 128;
        vCh[i] = red[i] * 0.5 + green[i] * -0.418688 + blue[i] * -0.081312 + 128;
    }
}

void MyGraphicsView::deleteYuvArrays()
{
    delete[] yCh;
    delete[] uCh;
    delete[] vCh;
}

void MyGraphicsView::convertYuvToRgb()
{
    QPixmap origiMap = backupItem->pixmap();
    QImage image = origiMap.toImage();
    int w = image.width();
    int h = image.height();
    int size = w * h;
    for(int i = 0; i < size; i++)
    {
        red[i] = yCh[i] + 1.402 * (vCh[i] - 128);
        green[i] = yCh[i] - 0.3455 * (uCh[i]- 128) - (0.7169 * (vCh[i] -128));
        blue[i] = yCh[i] + 1.779 *  (uCh[i] -128);
    }
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

void MyGraphicsView::yValue(int y){
    currentY = y;
    if(isPreviewed){
        convertRgbToYuv();
        changeYuv();
        convertYuvToRgb();
        arrayToPixMapItem();
        reinitArrays();
        deleteYuvArrays();
    }
}

void MyGraphicsView::uValue(int u){
    currentU = u;
    if(isPreviewed){
        convertRgbToYuv();
        changeYuv();
        convertYuvToRgb();
        arrayToPixMapItem();
        reinitArrays();
        deleteYuvArrays();
    }
}

void MyGraphicsView::vValue(int v){
    currentV = v;
    if(isPreviewed){
        convertRgbToYuv();
        changeYuv();
        convertYuvToRgb();
        arrayToPixMapItem();
        reinitArrays();
        deleteYuvArrays();
    }
}

void MyGraphicsView::arrayToPixMapItem()
{
    QPixmap origiMap = imageItem->pixmap();
    QImage image = origiMap.toImage();
    QRgb px;
    int offset = 0;
    int index;
    for(int curHeight = 0; curHeight < image.height(); curHeight++)
    {
        offset = curHeight * image.width();
        for(int curWidth = 0; curWidth < image.width(); curWidth++)
        {
            index = offset + curWidth;
            px = qRgb(red[index],green[index],blue[index]);
            image.setPixel(curWidth, curHeight, px);
        }
    }
    imageItem->setPixmap(QPixmap::fromImage(image));
}

void MyGraphicsView::testChange()
{
    //arrayToPixMapItem();
    qDebug() << "Testing done, now set image...";
    //imageItem->setPixmap(QPixmap::fromImage(showI));
    qDebug() << "...image set in test.";
}

void MyGraphicsView::changeYuv()
{

    QPixmap origiMap = backupItem->pixmap();
    QImage image = origiMap.toImage();
    int sum = 0;
    int offset = 0;
    int index = 0;
    for(int curHeight = 0; curHeight < image.height() - currentY; curHeight += currentY)
    {
        for(int curWidth = 0; curWidth < image.width() - currentY; curWidth += currentY)
        {
            sum = 0;
            sum = getYSum(curWidth, curWidth + currentY, curHeight, curHeight + currentY, image.width());
            sum = sum / (currentY * currentY);
            //set the colors around the pixel
            for(int o = curHeight; o < curHeight + currentY; o++)
            {
                offset = o * image.width();
                for(int k = curWidth; k < curWidth + currentY; k++)
                {
                    index = offset + k;
                    yCh[index] = sum;
                }
            }
        }
    }
    for(int curHeight = 0; curHeight < image.height() - currentU; curHeight += currentU)
    {
        for(int curWidth = 0; curWidth < image.width() - currentU; curWidth += currentU)
        {
            sum = 0;
            sum = getUSum(curWidth, curWidth + currentU, curHeight, curHeight + currentU, image.width());
            sum = sum / (currentU * currentU);
            //set the colors around the pixel
            for(int o = curHeight; o < curHeight + currentU; o++)
            {
                offset = o * image.width();
                for(int k = curWidth; k < curWidth + currentU; k++)
                {
                    index = offset + k;
                    uCh[index] = sum;
                }
            }
        }
    }

    for(int curHeight = 0; curHeight < image.height() - currentV; curHeight += currentV)
    {
        for(int curWidth = 0; curWidth < image.width() - currentV; curWidth += currentV)
        {
            sum = 0;
            sum = getVSum(curWidth, curWidth + currentV, curHeight, curHeight + currentV, image.width());
            sum = sum / (currentV * currentV);
            //set the colors around the pixel
            for(int o = curHeight; o < curHeight + currentV; o++)
            {
                offset = o * image.width();
                for(int k = curWidth; k < curWidth + currentV; k++)
                {
                    index = offset + k;
                    vCh[index] = sum;
                }
            }
        }
    }
}

void MyGraphicsView::changeImage()
{
    setEdited(true);
    QPixmap origiMap = backupItem->pixmap();
    QImage showI = imageItem->pixmap().toImage();
    QImage image = origiMap.toImage();
    QRgb pixelColor;
    int sum;

    //Sets all Red Pixel from original Picture.
    for(int curHeight = 0; curHeight < image.height() - currentR; curHeight += currentR)
    {
        for(int curWidth = 0; curWidth < image.width() - currentR; curWidth += currentR)
        {
            sum = 0;
            sum = getRedSumInArea(curWidth, curWidth + currentR, curHeight, curHeight + currentR, image.width());
            sum = sum / (currentR * currentR);
            //set the colors around the pixel
            for(int o = curHeight; o < curHeight + currentR; o++)
            {
                for(int k = curWidth; k < curWidth + currentR; k++)
                {
                    pixelColor = qRgb(sum, 0, 0);
                    showI.setPixel(k, o, pixelColor);
                }
            }
        }
    }
    //Set ALL GREEN
    for(int curHeight = 0; curHeight < image.height() - currentG; curHeight += currentG)
    {
        for(int curWidth = 0; curWidth < image.width() - currentG; curWidth += currentG)
        {
            sum = 0;
            sum = getGreenSumInArea(curWidth, curWidth + currentG, curHeight, curHeight + currentG, image.width());
            sum = sum / (currentG * currentG);
            //set the colors around the pixel
            for(int o = curHeight; o < curHeight + currentG; o++)
            {
                for(int k = curWidth; k < curWidth + currentG; k++)
                {
                    pixelColor = qRgb(qRed(showI.pixel(k, o)), sum, 0);
                    showI.setPixel(k, o, pixelColor);
                }
            }
        }
    }
    //Set All blue
    for(int curHeight = 0; curHeight < image.height() - currentB; curHeight += currentB)
    {
        for(int curWidth = 0; curWidth < image.width() - currentB; curWidth += currentB)
        {
            sum = 0;
            sum = getBlueSumInArea(curWidth, curWidth + currentB, curHeight, curHeight + currentB, image.width());
            sum = sum / (currentB * currentB);
            //set the colors around the pixel
            for(int o = curHeight; o < curHeight + currentB; o++)
            {
                for(int k = curWidth; k < curWidth + currentB; k++)
                {
                    pixelColor = qRgb(qRed(showI.pixel(k, o)), qGreen(showI.pixel(k, o)), sum);
                    showI.setPixel(k, o, pixelColor);
                }
            }
        }
    }
    imageItem->setPixmap(QPixmap::fromImage(showI));
}

int MyGraphicsView::getRedSumInArea(int sw, int ew, int sh, int eh, int maxW)
{
    int sum = 0;
    int offset = 0;
    int index = 0;
    for(int height = sh; height < eh; height++)
    {
        offset = height * maxW;//CHeck
        for(int  width = sw; width < ew; width++)
        {
            index = offset + width;
            sum += red[index];
        }
    }
    return sum;
}

int MyGraphicsView::getGreenSumInArea(int sw, int ew, int sh, int eh, int maxW)
{
    int sum = 0;
    int offset = 0;
    int index = 0;
    for(int height = sh; height < eh; height++)
    {
        offset = height * maxW;
        for(int  width = sw; width < ew; width++)
        {
            index = offset + width;
            sum += green[index];
        }
    }
    return sum;
}

int MyGraphicsView::getBlueSumInArea(int sw, int ew, int sh, int eh, int maxW)
{
    int sum = 0;
    int offset = 0;
    int index = 0;
    for(int height = sh; height < eh; height++)
    {
        offset = height * maxW;
        for(int  width = sw; width < ew; width++)
        {
            index = offset + width;
            sum += blue[index];
        }
    }
    return sum;
}

int MyGraphicsView::getYSum(int sw, int ew, int sh, int eh, int maxW)
{
    int sum = 0;
    int offset = 0;
    int index = 0;
    for(int height = sh; height < eh; height++)
    {
        offset = height * maxW;//CHeck
        for(int  width = sw; width < ew; width++)
        {
            index = offset + width;
            sum += yCh[index];
        }
    }
    return sum;
}

int MyGraphicsView::getUSum(int sw, int ew, int sh, int eh, int maxW)
{
    int sum = 0;
    int offset = 0;
    int index = 0;
    for(int height = sh; height < eh; height++)
    {
        offset = height * maxW;//CHeck
        for(int  width = sw; width < ew; width++)
        {
            index = offset + width;
            sum += uCh[index];
        }
    }
    return sum;
}

int MyGraphicsView::getVSum(int sw, int ew, int sh, int eh, int maxW)
{
    int sum = 0;
    int offset = 0;
    int index = 0;
    for(int height = sh; height < eh; height++)
    {
        offset = height * maxW;//CHeck
        for(int  width = sw; width < ew; width++)
        {
            index = offset + width;
            sum += vCh[index];
        }
    }
    return sum;
}

void MyGraphicsView::preview(bool preview) {
    isPreviewed = preview;
}

void MyGraphicsView::okayButton() {
    qDebug() << selectedMode;
    if(selectedMode == MODE_RGB){
        changeImage();
    } else if(selectedMode == MODE_YUV){
        convertRgbToYuv();
        changeYuv();
        convertYuvToRgb();
        arrayToPixMapItem();
        reinitArrays();
        deleteYuvArrays();
    }
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
    reinitArrays();
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
