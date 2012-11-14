#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QtGui>

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MyGraphicsView(QWidget *parent = 0);

public slots:
    void rValue(int r);
    void gValue(int g);
    void bValue(int b);

private:
    void getImageFromMimeData(const QMimeData *mimeData);
    
public:
    QGraphicsPixmapItem *imageItem;
    void setImage(QPixmap map);

protected:
    virtual void dragEnterEvent(QDragEnterEvent *);
    virtual void dragMoveEvent(QDragMoveEvent *);
    virtual void dropEvent(QDropEvent *);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // MYGRAPHICSVIEW_H
