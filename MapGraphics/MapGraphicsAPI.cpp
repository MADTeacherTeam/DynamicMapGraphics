#include "MapGraphicsAPI.h"
#include "ui_MapGraphicsAPI.h"

MapGraphicsAPI::MapGraphicsAPI(/*QVBoxLayout* layout,*/ QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapGraphicsAPI)
{
    ui->setupUi(this);

    //Setup the MapGraphics scene and view
    _scene = new MapGraphicsScene(this);
    _manager = new MapObjectsManager(_scene, this);
    _view = new MapGraphicsView(_scene, _manager, this);

    connect(_view, &MapGraphicsView::SendGeoObject,
            this, &MapGraphicsAPI::GeoObjectAdded);
    connect(_view, &MapGraphicsView::SendAdvancObject,
            this, &MapGraphicsAPI::AdvancObjectAdded);
    //The view will be our central widget
//    layout->addWidget(_view);
    this->ui->mapGraphics_Layout->addWidget(_view);

    //Setup some tile sources
    QSharedPointer<OSMTileSource> osmTiles(new OSMTileSource(OSMTileSource::OSMTiles), &QObject::deleteLater);
    //QSharedPointer<GridTileSource> gridTiles(new GridTileSource(), &QObject::deleteLater);
    //QSharedPointer<CompositeTileSource> composite(new CompositeTileSource(), &QObject::deleteLater);
    //composite->addSourceBottom(osmTiles);
    //composite->addSourceTop(gridTiles);
    _view->setTileSource(osmTiles);

    //Create a widget in the dock that lets us configure tile source layers
    _tileConfigWidget = new CompositeTileSourceConfigurationWidget(_manager, this);
//    this->ui->dockWidget->setWidget(tileConfigWidget);

//    this->ui->menuWindow->addAction(this->ui->dockWidget->toggleViewAction());
//    this->ui->dockWidget->toggleViewAction()->setText("&Layers");

    _view->setZoomLevel(18);
    _view->centerOn(30.3141, 59.9386);
}

MapGraphicsAPI::~MapGraphicsAPI()
{
    delete ui;
}

CompositeTileSourceConfigurationWidget *MapGraphicsAPI::GetTileConfigWidget() const
{
    return _tileConfigWidget;
}

MapGraphicsView *MapGraphicsAPI::GetMapGraphicsView() const
{
    return _view;
}


MapObjectsManager *MapGraphicsAPI::GetManager() const
{
    return _manager;
}

void MapGraphicsAPI::GeoObjectRemove(MapGeoObject * pObj)
{
    if (MapGraphicsObject * mgObj = dynamic_cast<MapGraphicsObject*>(pObj)){
        _scene->removeObject(mgObj);
    }
}

void MapGraphicsAPI::MapGraphObjectRemove(MapGraphicsObject *mgObj)
{
    _scene->removeObject(mgObj);
}

void MapGraphicsAPI::UpdateScene()
{
    _view->UpdateMapGraphicsObjects();;
}

RouteObject *MapGraphicsAPI::CreateRouteBtwnPoints(QPointF pnt1, QPointF pnt2)
{
    return _manager->createRoute(pnt1, pnt2);
}
