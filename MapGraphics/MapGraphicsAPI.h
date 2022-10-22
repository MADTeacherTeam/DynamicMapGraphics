#ifndef MAPGRAPHICSAPI_H
#define MAPGRAPHICSAPI_H

#include <QWidget>
#include <QString>
#include <QVBoxLayout>

#include "MapGraphics_global.h"
#include "MapGraphicsView.h"
#include "MapGraphicsScene.h"
#include "MapObjectsManager.h"
#include "tileSources/GridTileSource.h"
#include "tileSources/OSMTileSource.h"
#include "tileSources/CompositeTileSource.h"
#include "guts/CompositeTileSourceConfigurationWidget.h"

#include "MapGeoObject.h"

namespace Ui {
class MapGraphicsAPI;
}

class MAPGRAPHICSSHARED_EXPORT MapGraphicsAPI : public QWidget
{
    Q_OBJECT

public:
    explicit MapGraphicsAPI(/*QVBoxLayout* layout,*/ QWidget *parent = nullptr);

    ~MapGraphicsAPI();

    CompositeTileSourceConfigurationWidget *GetTileConfigWidget() const;

    MapGraphicsView * GetMapGraphicsView() const;
    RouteObject * CreateRouteBtwnPoints(QPointF, QPointF);

    MapObjectsManager *GetManager() const;

    void GeoObjectRemove(MapGeoObject *);
    void MapGraphObjectRemove(MapGraphicsObject *);

    void UpdateScene();

signals:
    void GeoObjectAdded(MapGeoObject *);
    void AdvancObjectAdded(MapGraphicsObject *);

    void AddGeoObject(QImage *);

private:
    Ui::MapGraphicsAPI *ui;
    MapGraphicsScene * _scene;
    MapGraphicsView * _view;
    MapObjectsManager * _manager;
    CompositeTileSourceConfigurationWidget * _tileConfigWidget;
};

#endif // MAPGRAPHICSAPI_H
