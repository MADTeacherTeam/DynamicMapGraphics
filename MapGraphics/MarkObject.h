#ifndef MARKOBJECT_H
#define MARKOBJECT_H

#include "MapGraphicsObject.h"
#include <QImage>
#include <QDir>
#define LENGHT_IMAGES_ARRAY 3

class MarkObject : public MapGraphicsObject
{
    Q_OBJECT
public:
    explicit MarkObject(MapGraphicsObject* parent = 0);
    virtual ~MarkObject();
    void changeImage();

    //pure-virtual from MapGraphicsObject
    QRectF boundingRect() const override;

    //pure-virtual from MapGraphicsObject
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QImage image() const;

    void setMark(int rotation = 180);

    //handle mouse's move
    void mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent * event) override;

    void saveObject();
private:
    static int countImages;

    QImage _image;
    QRectF _sizeInMeters;
    int _indexImage;

};

#endif // MARKOBJECT_H
