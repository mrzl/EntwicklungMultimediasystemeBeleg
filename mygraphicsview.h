#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QtGui>

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MyGraphicsView(QMainWindow *parent);

public slots:
    void rValue(int r);
    void gValue(int g);
    void bValue(int b);
    void yValue(int y);
    void uValue(int u);
    void vValue(int v);

    void preview(bool preview);
    void okayButton();
    void rejectedButton();

public:
    int selectedMode;

private:
    QMainWindow *par;
    void getImageFromMimeData(const QMimeData *mimeData);
    int oldR, oldG, oldB;
    int currentR, currentG, currentB, currentY, currentU, currentV;
    bool isPreviewed;
    int oldX, oldY;
    int getRedSumInArea(int starW, int endW, int startH, int endH, int maxW);
    int getGreenSumInArea(int starW, int endW, int startH, int endH, int maxW);
    int getBlueSumInArea(int starW, int endW, int startH, int endH, int maxW);
    int getYSum(int starW, int endW, int startH, int endH, int maxW);
    int getUSum(int starW, int endW, int startH, int endH, int maxW);
    int getVSum(int starW, int endW, int startH, int endH, int maxW);
    void changeImage();
    void changeYuv();
    bool edited;


    float* red;
    float* green;
    float* blue;
    float* yCh;
    float* uCh;
    float* vCh;
    void testChange();
    void initArrays();
    void reinitArrays();
    void deleteArray();
    void arrayToPixMapItem();

    void convertRgbToYuv();
    void convertYuvToRgb();
    void deleteYuvArrays();
    void calculateYuv();
    
public:
    QGraphicsPixmapItem *imageItem;
    QGraphicsPixmapItem *backupItem;
    void setImage(QPixmap map);

    void setEdited(bool edited);
    bool isEdited();

    void zoomIn(float factor);
    void zoomOut(float factor);

protected:
    virtual void dragEnterEvent(QDragEnterEvent *);
    virtual void dragMoveEvent(QDragMoveEvent *);
    virtual void dropEvent(QDropEvent *);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *wheelEvent);
    virtual void mousePressEvent(QMouseEvent *event);

};

#endif // MYGRAPHICSVIEW_H
