#ifndef MAPGEOOBJECT_H
#define MAPGEOOBJECT_H

#include "MapGraphics_global.h"
#include "MapGraphicsObject.h"
#include <QImage>
class MAPGRAPHICSSHARED_EXPORT MapGeoObject : public MapGraphicsObject
{
    Q_OBJECT
public:
    explicit MapGeoObject(QImage img = QImage(":/marks/mark0.png"), MapGraphicsObject* parent = 0);
    virtual ~MapGeoObject();

    //pure-virtual from MapGraphicsObject
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget=0) override ;

    //pure-virtual from MapGraphicsObject
    QRectF boundingRect() const override ;

    QImage image() const ;

    void setGeoObject(int rotation = 180);

    //handle mouse's move
    void mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    void saveObject();
    void setImage(const QImage &image);

signals:
    void mouseDoubleClicked();
private:
    QImage _image;
    QRectF _sizeInMeters;
};

#endif // MAPGEOOBJECT_H
