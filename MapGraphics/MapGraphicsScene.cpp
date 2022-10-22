#include "MapGraphicsScene.h"

#include <QtDebug>

MapGraphicsScene::MapGraphicsScene(QObject * parent)
    : QObject(parent)
{
    _objects.insert("RouteObject", new QLinkedList<MapGraphicsObject *>());
    _objects.insert("MapGeoObject", new QLinkedList<MapGraphicsObject *>());
    _objects.insert("MovementObject", new QLinkedList<MapGraphicsObject *>());
    _objects.insert("MarkObject", new QLinkedList<MapGraphicsObject *>());
    _objects.insert("LineObject", new QLinkedList<MapGraphicsObject *>());
    _objects.insert("CircleObject", new QLinkedList<MapGraphicsObject *>());
}

MapGraphicsScene::~MapGraphicsScene()
{
    qDebug() << "MapGraphicsScene dest";
//    foreach(auto list, _objects){
//        foreach(MapGraphicsObject * obj, list){

//            this->removeObject(obj);
//            // TODO: check delete memory from objects
//            delete obj;
//        }
//    }
}

void MapGraphicsScene::addObject(MapGraphicsObject *object)
{
    if (object == 0 || _objects[object->metaObject()->className()]->contains(object))
    {
        qDebug() << "kek";
        return;
    }
    //qDebug() << object->metaObject()->className();
    connect(object,
            SIGNAL(newObjectGenerated(MapGraphicsObject*)),
            this,
            SLOT(handleNewObjectGenerated(MapGraphicsObject*)));
//    connect(object,
//            SIGNAL(destroyed(QObject*)),
//            this,
//            SLOT(handleObjectDestroyed(QObject*)));

    _objects[object->metaObject()->className()]->append(object);
    qDebug()<<"addObject"<<endl;
    this->objectAdded(object);
}


void MapGraphicsScene::removeObject(MapGraphicsObject *object)
{
    qDebug()<<"removeObject"<<endl;
    _objects[object->metaObject()->className()]->removeOne(object);
    this->objectRemoved(object);
    object->deleteLater();
}

//private slot
void MapGraphicsScene::handleNewObjectGenerated(MapGraphicsObject *newObject)
{
    qDebug()<<"handleNewObjectGenerated"<<endl;
    this->addObject(newObject);
}

//private slot
void MapGraphicsScene::handleObjectDestroyed(QObject *object)
{
    /*
      We have to be careful with this casted pointer as technically at this point the MapGraphicsObject
      portion of it has been destroyed. This signal comes from the QObject destructor.
    */
    MapGraphicsObject *mgObj = (MapGraphicsObject*)object;
    qDebug()<<"handleObjectDestroyed"<<endl;

    this->removeObject(mgObj);
}

QHash<QString, QLinkedList<MapGraphicsObject *> *> MapGraphicsScene::objects() const
{
    return _objects;
}

QLinkedList<MapGraphicsObject *> *MapGraphicsScene::objects(const QString name) const
{
    return _objects[name];
}

