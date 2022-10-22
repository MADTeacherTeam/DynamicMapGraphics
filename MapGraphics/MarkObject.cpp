#include "MarkObject.h"
#include <QDebug>

MarkObject::MarkObject(MapGraphicsObject *parent): MapGraphicsObject(true, parent), _indexImage(0)
{
    _image = QImage(":/marks/mark0.png");
    _sizeInMeters = QRectF(_image.width() / 2, _image.height() / 2, _image.width() / 15, _image.height() / 15);

    this->setFlag(MapGraphicsObject::ObjectIsSelectable, false);
    this->setFlag(MapGraphicsObject::ObjectIsMovable, false);
    this->setFlag(MapGraphicsObject::ObjectIsFocusable, false);
}

MarkObject::~MarkObject()
{
    qDebug() << "remove mark object";
}

/* Change mark's image to next */
void MarkObject::changeImage()
{
    _image = QImage(":/marks/mark"+ QString::number((++_indexImage) % countImages) + ".png");
    this->redrawRequested();
}
QRectF MarkObject::boundingRect() const
{
    return _sizeInMeters;
}

void MarkObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->drawImage(this->boundingRect(), this->image());
}

QImage MarkObject::image() const
{
    return _image;
}

void MarkObject::setMark(int rotation)
{
    int width = _sizeInMeters.width();
    int height = _sizeInMeters.height();
    _sizeInMeters = QRectF(-0.25 * width,
                           -0.5 * height,
                           width, height);
    this->redrawRequested();
    this->setRotation(rotation);

}

void MarkObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    this->redrawRequested();
    this->setPos(event->scenePos());
}

void MarkObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit removeRequested(this);
    MapGraphicsObject::mousePressEvent(event);
}

void MarkObject::saveObject()
{

}

int MarkObject::countImages = QDir(":/marks").count();
