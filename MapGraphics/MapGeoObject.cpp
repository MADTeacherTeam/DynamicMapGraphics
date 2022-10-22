#include "MapGeoObject.h"

MapGeoObject::MapGeoObject(QImage img, MapGraphicsObject *parent) : MapGraphicsObject(false, parent), _image(img)
{
    //TODO: send container with images in /dir and check type
    _sizeInMeters = QRectF(_image.width(),_image.height(), 2* _image.width(), 2* _image.height());

    setFlag(MapGraphicsObject::ObjectIsSelectable, false);
    setFlag(MapGraphicsObject::ObjectIsMovable, false);
    setFlag(MapGraphicsObject::ObjectIsFocusable, false);
}

MapGeoObject::~MapGeoObject()
{

}

void MapGeoObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->drawImage(this->boundingRect(), this->image());
}

QRectF MapGeoObject::boundingRect() const
{
    return _sizeInMeters;
}

void MapGeoObject::setGeoObject(int rotation)
{
    int width = _sizeInMeters.width();
    int height = _sizeInMeters.height();
    _sizeInMeters = QRectF(-0.5 * width,
                           -0.5 * height,
                           width, height);
    this->redrawRequested();
    this->setRotation(rotation);

}

void MapGeoObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    this->redrawRequested();
    this->setPos(event->scenePos());
}

void MapGeoObject::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    emit mouseDoubleClicked();
    event->setAccepted(false);
}

void MapGeoObject::saveObject()
{

}

void MapGeoObject::setImage(const QImage &image)
{
    _image = image;
    _sizeInMeters = QRectF(_image.width() / 2, _image.height() / 2, _image.width() / 15, _image.height() / 15);
    setGeoObject();
}

QImage MapGeoObject::image() const
{
    return _image;
}
