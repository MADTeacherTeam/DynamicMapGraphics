#ifndef ROUTEOBJECT_H
#define ROUTEOBJECT_H

#include "MapGraphics_global.h"
#include "MapGraphicsObject.h"
#include "guts/PrivateQGraphicsView.h"
#include "RequestThread.h"

#include <QNetworkReply>

class MAPGRAPHICSSHARED_EXPORT RouteObject : public MapGraphicsObject
{
    Q_OBJECT
public:
    RouteObject(QPointF posBegin=QPointF(-1.0, -1.0), QPointF posEnd=QPointF(-1.0, -1.0), MapGraphicsObject *parent=0);
    virtual ~RouteObject();

    QRectF boundingRect() const override;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget=0) override;

    void requestRoute();
    void setRoute();
    void updateRoute();
    void extendRoute(QPointF point);
    void FinishRouteCreation();

    void simplifyPoints(double tolerance = 0.00005);
    const std::pair<int, double> findMaximumDistance(const std::vector<QPointF>& Points)const;
    std::vector<QPointF> simplifyWithRDP(std::vector<QPointF>& Points, double epsilon)const;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void saveObject(); // пока void, надо будет обдумать

    QPointF posBegin() const;
    void setPosBegin(const QPointF &posBegin);

    QPointF posEnd() const;
    void setPosEnd(const QPointF &posEnd);

    quint16 zoomLevel() const;
    void setZoomLevel(const quint16 &zoomLevel) override;

    QVector<QPointF> *routePoints() const;

    QVector<QPointF> *visiblePoints() const;

    size_t sizeOfRoutePoints();

    /**
     * @brief checkPosBegin
     * @return true if posBegin correct
     */
    bool checkPosBegin() const;

//protected:
//    void wheelEvent(QGraphicsSceneWheelEvent *event) override;

    qreal getDistance() const;

signals:
    void getMovementObject(MapGraphicsObject *);

private:
    QPointF ll2qgs(const QPointF &ll) const;
    QPointF qgs2ll(const QPointF &qgs, quint8 zoomLevel) const;

public slots:
    void handleNetworkRequestFinished();

private:
    qreal _distance;
    quint16 _zoomLevel;
    QPointF _posBegin;
    QPointF _posEnd;
    QPainterPath _route;
    QVector<QPointF> *_routePoints = nullptr;
    QVector<QPointF> *_visiblePoints = nullptr;
    QGraphicsView * _childGraphView {nullptr};
    QNetworkReply * reply {nullptr};
    QThread thread;
    RequestThread * requestThread;
};

#endif // ROUTEOBJECT_H
