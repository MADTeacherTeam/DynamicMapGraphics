#ifndef MAPOBJECTSMANAGER_H
#define MAPOBJECTSMANAGER_H

#include <QObject>
#include <QPointer>


#include "MapGraphicsScene.h"
#include "MapGraphics_global.h"

#include "CircleObject.h"
#include "MapGeoObject.h"
#include "RouteObject.h"
#include "MovementObject.h"
#include "MarkObject.h"
#include "MapGraphicsObject.h"

class MAPGRAPHICSSHARED_EXPORT MapObjectsManager : public QObject
{
    Q_OBJECT
public:
    enum ManagerMode
    {
        NoCreation = 0,
        MarkCreation = 1,
        RouteCreation = 2,
        MarkRemove = 3,
        RouteRemove = 4,
        LinkRoute = 5,
        CustomRouteCreation = 6,
        GeoObjectCreation = 7,
        CircleObjectCreation = 8
    };

    explicit MapObjectsManager(MapGraphicsScene * scene, QObject *parent = nullptr);
    ~MapObjectsManager();

    ManagerMode state() const;
    void setState(const ManagerMode &state);
    ManagerMode getState();
    void addObjectToScene();
    void setObjectMovable(const QString &name, const bool flag=true);
    void clearTempObject();
    void changeImage(QImage *img=nullptr);

    RouteObject* createRoute(QPointF pnt1, QPointF pnt2);

public slots:
    void createObject();
    void deleteObject(MapGraphicsObject *);
    void linkRouteAndObject(MapGraphicsObject * obj);
    void addObjectToScene(MapGraphicsObject*);
    void setInitialRadius(quint32 radius);

signals:
    void managerModeChanged(ManagerMode mode);
    void roadChildViewRequest(RouteObject * route);

public:
    MapGraphicsObject * _tempObject = nullptr;

private:
    QPointer<MapGraphicsScene> _scene;
    ManagerMode _state = NoCreation;
    quint32 _initialRadius {10};

};

#endif // MAPOBJECTSMANAGER_H
