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
    void preview(bool preview);
    void okayButton();
    void rejectedButton();

private:
    QMainWindow *par;
    void getImageFromMimeData(const QMimeData *mimeData);
    int oldR, oldG, oldB;
    int currentR, currentG, currentB;
    bool isPreviewed;
    int oldX, oldY;
    int getColorSum(int startW, int endW, int startH, int endH, QImage image, int color);
    void changeImage();
    bool edited;
    
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
