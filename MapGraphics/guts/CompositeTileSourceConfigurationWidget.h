#ifndef COMPOSITETILESOURCECONFIGURATIONWIDGET_H
#define COMPOSITETILESOURCECONFIGURATIONWIDGET_H

#include <QWidget>
#include <QWeakPointer>
#include <QListView>

#include "MapObjectsManager.h"
#include "MapGraphics_global.h"

#include "tileSources/CompositeTileSource.h"

namespace Ui {
class CompositeTileSourceConfigurationWidget;
}

class MAPGRAPHICSSHARED_EXPORT CompositeTileSourceConfigurationWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit CompositeTileSourceConfigurationWidget(MapObjectsManager* manager, QWidget *parent = 0);
    ~CompositeTileSourceConfigurationWidget();
    void addTab(QWidget * widget, const QString& name);
    void setLastIndexTab();

private slots:
    void on_addMark_button_clicked();
    void on_removeMark_button_clicked();
    void on_changeColor_button_clicked();
    void on_moveMark_button_clicked();
    void on_addRoute_button_clicked();
    void on_removeRoute_button_clicked();
    void on_linkRoute_button_clicked();
    void on_addGeoObject_toolButton_triggered(QAction *arg1);

signals:
    void SashaX(const QString& name);

private:
    Ui::CompositeTileSourceConfigurationWidget *ui;
    MapObjectsManager* _manager;
};

#endif // COMPOSITETILESOURCECONFIGURATIONWIDGET_H
