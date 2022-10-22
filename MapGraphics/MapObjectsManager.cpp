#include "MapObjectsManager.h"
#include <QLinkedList>

MapObjectsManager::MapObjectsManager(MapGraphicsScene *scene, QObject *parent) : QObject(parent), _scene(scene)
{

}

MapObjectsManager::~MapObjectsManager()
{
    qDebug() << "MapObjectsManager dest";
    // TODO: check delete tempObject
    //delete _tempObject;
}

void MapObjectsManager::createObject()
{
    switch (_state) {
    case RouteCreation:{
        _tempObject = new RouteObject();
        break;
    }
    case MarkCreation:{
        _tempObject = new MarkObject();
        emit _scene->objectAdded(_tempObject);
        break;
    }
    case LinkRoute:{
        _tempObject = new MovementObject();
        emit _scene->objectAdded(_tempObject);
        break;
    }
    case GeoObjectCreation:{
        _tempObject = new MapGeoObject();
        emit _scene->objectAdded(_tempObject);
        break;
    }
    case CircleObjectCreation:{
        _tempObject = new CircleObject(_initialRadius, false);
        emit _scene->objectAdded(_tempObject);
        break;
    }
    }
}

void MapObjectsManager::deleteObject(MapGraphicsObject * obj)
{
    switch(_state){
    case MarkRemove:{
        if (dynamic_cast<MarkObject*>(obj)!= 0)
        {
            _scene->removeObject(obj);
        }
        break;
    }
    case RouteRemove:{
        auto objects = _scene->objects("MovementObject");
        //QSet<MovementObject*>* temp = static_cast<QSet<MovementObject*>*>(static_cast<void*>(objects));
        QLinkedList<MovementObject*>* temp = reinterpret_cast<QLinkedList<MovementObject*>*>(objects);
        for(QLinkedList<MovementObject*>::const_reverse_iterator iter = temp->rbegin(); iter != temp->rend(); ++iter)
        {
            if ((*iter)->getRoute() == obj)
                _scene->removeObject(*iter);
        }
        _scene->removeObject(obj);
        break;
    }
    }
}

void MapObjectsManager::linkRouteAndObject(MapGraphicsObject *obj)
{
    if (_state == LinkRoute) {
        dynamic_cast<MovementObject *>(_tempObject)->setMovement(dynamic_cast<RouteObject *>(obj));
        addObjectToScene();
        createObject();
    }
}

MapObjectsManager::ManagerMode MapObjectsManager::state() const
{
    return _state;
}

void MapObjectsManager::setState(const ManagerMode &state)
{
    _state = state;
    emit managerModeChanged(_state);
}

MapObjectsManager::ManagerMode MapObjectsManager::getState()
{
    return _state;
}

void MapObjectsManager::addObjectToScene()
{
    _scene->addObject(_tempObject);
    connect(_tempObject,
            &MapGraphicsObject::removeRequested,
            this,
            &MapObjectsManager::deleteObject);
}

void MapObjectsManager::addObjectToScene(MapGraphicsObject * obj)
{
    _scene->addObject(obj);
    connect(obj,
            &MapGraphicsObject::removeRequested,
            this,
            &MapObjectsManager::deleteObject);
}

void MapObjectsManager::setObjectMovable(QString const &name, bool const flag)
{
    auto objects = _scene->objects(name);
    foreach (auto temp, *objects){
        temp->setFlag(MapGraphicsObject::MapGraphicsObjectFlag::ObjectIsMovable, flag);
    }
}

void MapObjectsManager::clearTempObject()
{
    if (_tempObject != nullptr) {
        emit _scene->objectRemoved(_tempObject);
        _tempObject->deleteLater();
        _tempObject = nullptr;
    }
}

void MapObjectsManager::changeImage(QImage *img)
{
    if(MapGeoObject * geoObj = dynamic_cast<MapGeoObject*>(_tempObject)){
        QImage * scaledImage = new QImage(img->scaled(500,500, Qt::IgnoreAspectRatio));
        geoObj->setImage(*scaledImage);
        delete img;
    }
}

void MapObjectsManager::setInitialRadius(quint32 radius)
{
    _initialRadius = radius;
}

RouteObject *MapObjectsManager::createRoute(QPointF pnt1, QPointF pnt2)
{
    RouteObject * route = new RouteObject(pnt1, pnt2);
    if (!route)
        return nullptr;
    else{
        route->requestRoute();
        emit roadChildViewRequest(route);
        addObjectToScene(route);
        return route;
    }
}
