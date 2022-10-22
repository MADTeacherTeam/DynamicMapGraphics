#include "CircleObject.h"

#include <QtDebug>
#include <QKeyEvent>
#include <QRandomGenerator>

CircleObject::CircleObject(qreal radius,bool sizeIsZoomInvariant, MapGraphicsObject *parent) :
    MapGraphicsObject(sizeIsZoomInvariant,parent)
{
    _radius = qMax<qreal>(radius,0.01);

    setFlag(MapGraphicsObject::ObjectIsSelectable, false);
    setFlag(MapGraphicsObject::ObjectIsMovable, false);
    setFlag(MapGraphicsObject::ObjectIsFocusable, false);

    quint32 red = QRandomGenerator::global()->bounded(255);
    quint32 green = QRandomGenerator::global()->bounded(255);
    quint32 blue = QRandomGenerator::global()->bounded(255);
    _fillColor = QColor(red,green,blue,100);
}

CircleObject::~CircleObject()
{
}

QRectF CircleObject::boundingRect() const
{
    return QRectF(-1*_radius,
                  -1*_radius,
                  2*_radius,
                  2*_radius);
}

void CircleObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->setBrush(_fillColor);
    painter->drawEllipse(QPointF(0,0),
                         _radius,
                         _radius);
}

qreal CircleObject::radius() const
{
    return _radius;
}

void CircleObject::setRadius(qreal radius)
{
    _radius = radius;
    this->redrawRequested();
}

void CircleObject::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    emit mouseDoubleClicked();
    event->setAccepted(false);
}

//protected
//virtual from MapGraphicsObject
void CircleObject::keyReleaseEvent(QKeyEvent *event)
{
    if (event->matches(QKeySequence::Delete))
    {
        this->deleteLater();
        event->accept();
    }
    else
        event->ignore();
}
