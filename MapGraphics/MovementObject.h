#ifndef MOVEMENTOBJECT_H
#define MOVEMENTOBJECT_H

#include "MapGraphicsObject.h"
#include "guts/Conversions.h"

#include <QTimer>
#include <QtMath>
#include <RouteObject.h>

class MovementObject : public MapGraphicsObject
{
    Q_OBJECT
public:
    explicit MovementObject(RouteObject* route = nullptr, int travelTime = 0,MapGraphicsObject* parent = 0);
    virtual ~MovementObject();

    //pure-virtual from MapGraphicsObject
    QRectF boundingRect() const;

    //pure-virtual from MapGraphicsObject
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QImage image() const;

    void setMovement(RouteObject* route);

    void saveObject();

    void getState();

    //handle mouse's double click
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);

    RouteObject *getRoute() const;

private slots:
    void routeDistance();

signals:
    void setSettings();

private:
    RouteObject*        _route;
    QImage              _image;
    QRectF              _sizeInMeters;
    QVector <qreal>*    _routePointTime = nullptr;
    QVector <QPointF>*  _linePoints = nullptr;
    int                 _travelTime;
    int                 _currentState;
    int                 _speed;                     // measure in m/s
};

#endif // MOVEMENTOBJECT_H
