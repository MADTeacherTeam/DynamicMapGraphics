#include "CompositeTileSourceConfigurationWidget.h"
#include "ui_CompositeTileSourceConfigurationWidget.h"

#include "MapTileLayerListModel.h"
#include "MapTileSourceDelegate.h"

#include "tileSources/OSMTileSource.h"

#include <QtDebug>
#include <QMenu>
#include <QIcon>
#include <QAction>

CompositeTileSourceConfigurationWidget::CompositeTileSourceConfigurationWidget(MapObjectsManager* manager, QWidget *parent) :
    QWidget(parent),  ui(new Ui::CompositeTileSourceConfigurationWidget),
    _manager(manager)
{
    ui->setupUi(this);
    ui->addGeoObject_toolButton->setPopupMode(QToolButton::MenuButtonPopup);
    QMenu *menu = new QMenu();
    QAction *shopAction = new QAction(QIcon(":/shops/shop.png"), "Shop", this);
    QAction *storageAction = new QAction(QIcon(":/storages/storage.png"), "Storage", this);
    menu->addAction(shopAction);
    menu->addAction(storageAction);
    ui->addGeoObject_toolButton->setMenu(menu);

//    ui->addGeoObject_toolButton->addAction(new QAction(QIcon(":/shops/shop.png"), "Shop"));
//    ui->addGeoObject_toolButton->addAction(new QAction(QIcon(":/storages/storage.png"), "Storage"));
}

CompositeTileSourceConfigurationWidget::~CompositeTileSourceConfigurationWidget()
{
    delete ui;
}

void CompositeTileSourceConfigurationWidget::addTab(QWidget *widget, const QString &name)
{
    ui->toolBox->addItem(widget, name);
}

void CompositeTileSourceConfigurationWidget::setLastIndexTab()
{
    ui->toolBox->setCurrentIndex(ui->toolBox->count());
}

void CompositeTileSourceConfigurationWidget::on_addMark_button_clicked()
{
    if (ui->addMark_button->isChecked())
    {
        ui->moveMark_button->setChecked(false);
        ui->removeMark_button->setChecked(false);
        _manager->setState(MapObjectsManager::MarkCreation);
        _manager->createObject();
    }
    else
    {
        _manager->setState(MapObjectsManager::NoCreation);
        _manager->clearTempObject();
    }
}

void CompositeTileSourceConfigurationWidget::on_removeMark_button_clicked()
{
    _manager->clearTempObject();
    if (ui->removeMark_button->isChecked())
    {
        ui->addMark_button->setChecked(false);
        ui->moveMark_button->setChecked(false);
        _manager->setState(MapObjectsManager::MarkRemove);
    } else {
        _manager->setState(MapObjectsManager::NoCreation);
    }
}

void CompositeTileSourceConfigurationWidget::on_changeColor_button_clicked()
{
    if (_manager->_tempObject)
        _manager->changeImage();
}

void CompositeTileSourceConfigurationWidget::on_moveMark_button_clicked()
{
    //_manager->clearTempObject(true);
    if (ui->moveMark_button->isChecked())
    {
        ui->addMark_button->setChecked(false);
        ui->removeMark_button->setChecked(false);
        _manager->setState(MapObjectsManager::NoCreation);
        //TODO: write
        _manager->setObjectMovable("MarkObject");
    }
    else {
        _manager->setObjectMovable("MarkObject", false);
    }
}

void CompositeTileSourceConfigurationWidget::on_addRoute_button_clicked()
{
    if (ui->addRoute_button->isChecked())
    {
        ui->removeRoute_button->setChecked(false);
        ui->linkRoute_button->setChecked(false);
        ui->createRoute_button->setChecked(false);
        _manager->setState(MapObjectsManager::RouteCreation);
        _manager->createObject();
    }
    else
    {
        _manager->setState(MapObjectsManager::NoCreation);
        _manager->createObject();
    }
}

void CompositeTileSourceConfigurationWidget::on_removeRoute_button_clicked()
{
     _manager->createObject();
     if (ui->removeRoute_button->isChecked())
     {
         ui->addRoute_button->setChecked(false);
         ui->linkRoute_button->setChecked(false);
         ui->createRoute_button->setChecked(false);
         _manager->setState(MapObjectsManager::RouteRemove);
     }
     else
     {
         _manager->setState(MapObjectsManager::NoCreation);
     }
}

void CompositeTileSourceConfigurationWidget::on_linkRoute_button_clicked()
{
    if (ui->linkRoute_button->isChecked())
    {
        ui->addRoute_button->setChecked(false);
        ui->removeRoute_button->setChecked(false);
        ui->createRoute_button->setChecked(false);
        _manager->setState(MapObjectsManager::LinkRoute);
        _manager->createObject();
    }
    else
    {
        _manager->setState(MapObjectsManager::NoCreation);
        _manager->clearTempObject();
    }
}

void CompositeTileSourceConfigurationWidget::on_addGeoObject_toolButton_triggered(QAction *arg1)
{
    _manager->setState(MapObjectsManager::GeoObjectCreation);
    _manager->createObject();
    if (arg1->text() == "Shop") {
        static_cast<MapGeoObject *>(_manager->_tempObject)->setImage(QImage(":/shops/shop.png"));
    }
    else if (arg1->text() == "Storage") {
        static_cast<MapGeoObject *>(_manager->_tempObject)->setImage(QImage(":/storages/storage.png"));
    }
    else if (arg1->text() == "Transport") {
        static_cast<MapGeoObject *>(_manager->_tempObject)->setImage(QImage(":/cars/car_object0.png"));
    }
}
