#ifndef CIRCLEOBJECT_H
#define CIRCLEOBJECT_H

#include "MapGraphics_global.h"
#include "MapGraphicsObject.h"

class MAPGRAPHICSSHARED_EXPORT CircleObject : public MapGraphicsObject
{
    Q_OBJECT
public:
    explicit CircleObject(qreal radius,bool sizeIsZoomInvariant=true,MapGraphicsObject *parent = 0);
    virtual ~CircleObject();

    //pure-virtual from MapGraphicsObject
    QRectF boundingRect() const;

    //pure-virtual from MapGraphicsObject
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    qreal radius() const;
    void setRadius(qreal radius);

    //handle mouse's move
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    
signals:
    void mouseDoubleClicked();
    
public slots:

protected:
    //virtual from MapGraphicsObject
    virtual void keyReleaseEvent(QKeyEvent *event);

private:
    qreal _radius;
    QColor _fillColor;
    
};

#endif // CIRCLEOBJECT_H
