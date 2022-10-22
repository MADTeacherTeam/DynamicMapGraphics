#include "MovementObject.h"

MovementObject::MovementObject(RouteObject *route, int travelTime, MapGraphicsObject *parent) : MapGraphicsObject(true, parent),
    _travelTime(travelTime)
{
    _image = QImage(":/cars/car_object0.png");
    _sizeInMeters = QRectF(0, 0, _image.width() / 4, _image.height() / 4);
    _sizeInMeters = QRectF(-0.25 * _sizeInMeters.width(),
                           -0.5 * _sizeInMeters.height(),
                           _sizeInMeters.width(), _sizeInMeters.height());

    _currentState = 1;
    _speed = 30;
    this->setRotation(180);
    this->setFlag(MapGraphicsObject::ObjectIsSelectable);
    this->setFlag(MapGraphicsObject::ObjectIsMovable);
    this->setFlag(MapGraphicsObject::ObjectIsFocusable);
}

MovementObject::~MovementObject()
{
    qDebug() << "remove move";
    delete _routePointTime;
}

QRectF MovementObject::boundingRect() const
{
    return _sizeInMeters;
}

void MovementObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->drawImage(this->boundingRect(), this->image());
}

QImage MovementObject::image() const
{
    return _image;
}

void MovementObject::setMovement(RouteObject *route)
{
    _route        = route;
    _currentState = 0;
    _travelTime   = 0;
    _routePointTime = new QVector<qreal>();
    _routePointTime->reserve(route->routePoints()->size());
    _routePointTime->append(0);
    routeDistance();
    setPos((*_linePoints)[0]);

    emit redrawRequested();
}

void MovementObject::routeDistance()
{
    qreal distance = 0;

    _linePoints = _route->routePoints();
    if (_linePoints == nullptr) {
        QTimer::singleShot(5000, this, SLOT(routeDistance()));
        return;
    }

    for(QVector<QPointF>::const_iterator iter = _linePoints->begin()+1; iter != _linePoints->end(); ++iter)
    {
        QVector3D pre_vect = Conversions::lla2xyz((*(iter-1)).y(), (*(iter-1)).x(), 0.0);
        QVector3D vect = Conversions::lla2xyz((*iter).y(), (*iter).x(), 0.0);
        distance = qSqrt(qPow(vect.x() - pre_vect.x(),2) + qPow(vect.y() - pre_vect.y(), 2));
        _routePointTime->push_back(distance / _speed);
    }
}

RouteObject *MovementObject::getRoute() const
{
    return _route;
}

void MovementObject::saveObject()
{
    
}

void MovementObject::getState()
{
    for (int i = _currentState; i != _routePointTime->size(); ++i)
    {
        if ((*_routePointTime)[i-1] <= _travelTime && _travelTime <= (*_routePointTime)[i])
        {
            _currentState = i - 1;
            qreal offset = (_travelTime - (*_routePointTime)[i -1]) * _speed;
            QPointF tempPoint = (*(_linePoints))[i -1];
            QVector3D startingPoint = Conversions::lla2xyz(tempPoint.y(), tempPoint.x(), 0.0);
            tempPoint = (*_linePoints)[i];
            QVector3D endPoint = Conversions::lla2xyz(tempPoint.y(), tempPoint.x(), 0.0);
            qreal dist = qSqrt(qPow(endPoint.x() - startingPoint.x(), 2) + qPow(endPoint.y() - startingPoint.y(),2));
            qreal x = startingPoint.x() + offset * ((endPoint.x() - startingPoint.x()) / dist);
            qreal y = startingPoint.y() + offset * ((endPoint.y() - startingPoint.y()) / dist);
            qreal z = startingPoint.z() + offset * ((endPoint.z() - startingPoint.z()) / dist);
            Position tmp_pos = Conversions::xyz2lla(x, y, z);
            QPointF toRet = QPointF(tmp_pos.longitude(), tmp_pos.latitude());
            this->setPos(toRet);
            this->redrawRequested();
            break;
        }
    }
}

void MovementObject::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    this->setSettings();
    event->setAccepted(false);
}
