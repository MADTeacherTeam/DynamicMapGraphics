#include "RouteObject.h"
#include "guts/MapGraphicsNetwork.h"
#include <cmath>
#include <Position.h>
#include <QStack>
#include <QPair>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QEventLoop>
#include <QPolygon>

RouteObject::RouteObject(QPointF posBegin, QPointF posEnd, MapGraphicsObject *parent)
    : MapGraphicsObject(true, parent), _posBegin(posBegin), _posEnd(posEnd)
{

}

RouteObject::~RouteObject()
{
    qDebug() << "succ";
    thread.quit();
    requestThread->Finish();
    delete requestThread;
    delete _routePoints;
    delete _visiblePoints;
}

QRectF RouteObject::boundingRect() const
{

    auto boundingRect = _route.controlPointRect();
    boundingRect.setX(3* boundingRect.x());
    boundingRect.setY(3 *boundingRect.y());
    boundingRect.setWidth(boundingRect.width() * 4);
    boundingRect.setHeight(boundingRect.height() * 4);
    return boundingRect;
}

void RouteObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    auto pen = painter->pen();
    pen.setWidth(6);
    QColor color("blue");
    color.setAlpha(130);
    pen.setColor(color);
    painter->setPen(pen);
    updateRoute();
    painter->drawPath(_route);
}

void RouteObject::requestRoute()
{
    setVisible(true);
    MapGraphicsNetwork * network = MapGraphicsNetwork::getInstance();
    QString host = "http://router.project-osrm.org/route/v1/driving/%1,%2;%3,%4?overview=full&geometries=geojson";

    QNetworkRequest request(QUrl(host.arg(QString::number(_posBegin.x()),
                                          QString::number(_posBegin.y()),
                                          QString::number(_posEnd.x()),
                                          QString::number(_posEnd.y()))));
    requestThread = new RequestThread(request);
    requestThread->moveToThread(&thread);
    connect(&thread, SIGNAL(started()), requestThread, SLOT(run()));
    thread.start();
    //reply = network->get(request);

//    QEventLoop loop;

//    connect(reply,
//            SIGNAL(finished()),
//            this,
//            SLOT(handleNetworkRequestFinished()));
//    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
//    loop.exec();
}

void RouteObject::setRoute()
{
    //simplifyPoints();
    std::vector<QPointF> tmpVec;
    std::copy(_routePoints->begin(), _routePoints->end(), std::inserter(tmpVec, std::begin(tmpVec)));
    auto resultVec = simplifyWithRDP(tmpVec, 0.00005);
    _routePoints->erase(_routePoints->begin(), _routePoints->end());
//    delete _routePoints;
//    _routePoints = new QVector<QPointF>(resultVec.size());
//    _routePoints->clear();
    for (auto & elem : resultVec)
        _routePoints->append(elem);
//    std::copy(std::begin(tmpVec), std::end(tmpVec), std::inserter(_routePoints, _routePoints->begin()));
    setPos(QPointF((_posBegin.x() + _posEnd.x()) / 2, (_posBegin.y() + _posEnd.y()) / 2));
    updateRoute();
    emit redrawRequested();
}

void RouteObject::updateRoute()
{
    _route = QPainterPath();
    QPointF qgsPos = ll2qgs(pos());
    Position lastPoint(_routePoints->at(0).x(), _routePoints->at(0).y(), 0.0);
    if (_visiblePoints == nullptr) {
        _visiblePoints = new QVector<QPointF>();
        _visiblePoints->reserve(_routePoints->size());
    } else {
        _visiblePoints->clear();
    }

    QPointF temp = ll2qgs(lastPoint.lonLat());
    double offX = temp.x() - qgsPos.x();
    double offY = -(temp.y() - qgsPos.y());
    QPointF firstPoint(offX, offY);
    _route.moveTo(firstPoint);
    _visiblePoints->append(firstPoint);

    for (int i = 1; i < _routePoints->size(); ++i)
    {
        Position currentPoint(_routePoints->at(i).x(), _routePoints->at(i).y(), 0.0);
        temp = ll2qgs(currentPoint.lonLat());
        offX = temp.x() - qgsPos.x();
        offY = -(temp.y() - qgsPos.y());
        QPointF endPoint(offX, offY);
        _route.lineTo(endPoint);
        _visiblePoints->append(endPoint);
    }
}

void RouteObject::extendRoute(QPointF point)
{

}

void RouteObject::FinishRouteCreation()
{
    setVisible(true);
    setRoute();
    emit redrawRequested();
}

void RouteObject::simplifyPoints(qreal tolerance)
{
    qint32 anchor = 0;
    qint32 floater = _routePoints->size() - 1;
    QStack<QPair<qint32, qint32>> stack;
    QList<qint32> keep;

    stack.append(qMakePair(anchor, floater));
    auto assignAnchorAndFloater = [&anchor, &floater](QPair<qint32, qint32> const p) {
        anchor = p.first;
        floater = p.second;
    };
    while (!stack.isEmpty()) {
        assignAnchorAndFloater(stack.pop());

        qreal anchorX = 0.0, anchorY = 0.0, seg_len = 0.0;
        if (_routePoints->at(floater) != _routePoints->at(anchor))
        {
            anchorX = _routePoints->at(floater).x() - _routePoints->at(anchor).x();
            anchorY = _routePoints->at(floater).y() - _routePoints->at(anchor).y();
            seg_len = sqrt((anchorX * anchorX) + (anchorY * anchorY));
            anchorX /= seg_len;
            anchorY /= seg_len;
        }

        qreal max_dist = 0.0;
        qint32 farthest = anchor + 1;
        for (qint32 i = anchor + 1; i < floater; ++i)
        {
            qreal dist_to_seg = seg_len;
            qreal vecX = _routePoints->at(i).x() - _routePoints->at(anchor).x();
            qreal vecY = _routePoints->at(i).y() - _routePoints->at(anchor).y();
            seg_len = sqrt((vecX * vecX) + (vecY * vecY));

            qreal proj = (vecX * anchorX) + (vecY * anchorY);
            if (proj < 0.0) {
                dist_to_seg = seg_len;
            }
            else {
                vecX = _routePoints->at(i).x() - _routePoints->at(floater).x();
                vecY = _routePoints->at(i).y() - _routePoints->at(floater).y();
                seg_len = sqrt((vecX * vecX) + (vecY * vecY));
                proj = (vecX * (-anchorX)) + (vecY * (-anchorY));
                dist_to_seg = proj < 0.0 ? seg_len : sqrt(abs((seg_len * seg_len) - (proj * proj)));

                if (max_dist < dist_to_seg) {
                    max_dist = dist_to_seg;
                    farthest = i;
                }
            }
        }

        if (max_dist <= tolerance) {
            keep.append(anchor);
           // keep.append(floater);
        }
        else {
            stack.append(qMakePair(anchor, farthest));
            stack.append(qMakePair(farthest, floater));
        }
    }

    qSort(keep);
    QVector<QPointF> * toRet = new QVector<QPointF>();
    toRet->reserve(keep.size());
    foreach (qint32 temp, keep){
        toRet->append(_routePoints->at(temp));
    }
    delete _routePoints;
    _routePoints = toRet;
}

void RouteObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//    for (int i = 1; i < _visiblePoints->size(); ++i){
//        QRectF pointRect(QPointF((*_visiblePoints)[i - 1].x(), (*_visiblePoints)[i - 1].y()),
//                         QPointF((*_visiblePoints)[i].x(), (*_visiblePoints)[i].y()));
//        QPointF center = pointRect.center();
//        pointRect.setSize(pointRect.size() * (19 -_zoomLevel));
//        pointRect.moveаCenter(center);
//        if (pointRect.contains(QPointF(event->pos().x(), -event->pos().y()))){
//            emit getMovementObject(this);
//            // TODO: обдумать, как сделать удаление объекта
//            emit removeRequested(this);
//            event->setAccepted(true);
//            return;
//            //break;
//        }
//    }
    event->setAccepted(false);
}

void RouteObject::saveObject()
{

}

QPointF RouteObject::posBegin() const
{
    return _posBegin;
}

void RouteObject::setPosBegin(const QPointF &posBegin)
{
    _posBegin = posBegin;
}

QPointF RouteObject::posEnd() const
{
    return _posEnd;
}

void RouteObject::setPosEnd(const QPointF &posEnd)
{
    _posEnd = posEnd;
}

QPointF RouteObject::ll2qgs(const QPointF &ll) const
{
    constexpr qreal PI = 3.14159265358979323846;
    constexpr qreal deg2rad = PI / 180.0;
    quint32 tilesOnOneEdge = pow(2, _zoomLevel);
    constexpr quint16 tileSize = 256;
    qreal x = (ll.x() + 180) * (tilesOnOneEdge * tileSize) / 360;
    qreal y = (1 - (log(tan(PI / 4 + (ll.y() * deg2rad) / 2)) / PI)) / 2 * (tilesOnOneEdge * tileSize);

    return QPointF(int(x), int(y));
}

QPointF RouteObject::qgs2ll(const QPointF &qgs, quint8 zoomLevel) const
{
    constexpr qreal PI = 3.14159265358979323846;
    constexpr qreal rad2deg = 180.0 / PI;
    const qreal tilesOnOneEdge = pow(2.0,zoomLevel);
    constexpr quint16 tileSize = 256;    qreal longitude = (qgs.x()*(360.0/(tilesOnOneEdge*tileSize)))-180.0;
    qreal latitude = rad2deg*(atan(sinh((1.0-qgs.y()*(2.0/(tilesOnOneEdge*tileSize)))*PI)));

    return QPointF(longitude, latitude);
}

void RouteObject::handleNetworkRequestFinished()
{
//    QObject * sender = QObject::sender();
//    QNetworkReply * reply = qobject_cast<QNetworkReply *>(sender);
    reply = requestThread->getReply();


    if (reply == 0)
    {
        qWarning() << "QNetworkReply cast failure";
        return;
    }

    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Network Error:" << reply->errorString();
        return;
    }

    QString strReply = (QString)reply->readAll();
    if (strReply == "")
        return;
    thread.quit();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    auto tempArray = jsonObject["routes"].toArray()[0].toObject()["geometry"].toObject()["coordinates"].toArray();
    _routePoints = new QVector<QPointF>();
    _routePoints->reserve(tempArray.size());
    foreach (QJsonValue && temp, tempArray) {
        QJsonArray test = temp.toArray();
        _routePoints->append(QPointF(test[0].toDouble(), test[1].toDouble()));
    }
    _distance = jsonObject["routes"].toArray()[0].toObject()["distance"].toDouble();
}

qreal RouteObject::getDistance() const
{
    return _distance;
}

QVector<QPointF> *RouteObject::visiblePoints() const
{
    return _visiblePoints;
}

size_t RouteObject::sizeOfRoutePoints()
{
    if (_routePoints)
        return _routePoints->size();
    else
        return 0;
}

bool RouteObject::checkPosBegin() const
{
    return !(_posBegin.x() == -1.0 && _posBegin.y() == -1.0);
}

//void RouteObject::wheelEvent(QGraphicsSceneWheelEvent *event)
//{
//    if (event->delta() > 0)
//    {
//        if (_zoomLevel < 18)
//            ++_zoomLevel;
//    } else {
//        if (_zoomLevel > 0)
//            --_zoomLevel;
//    }
//    event->ignore();
//}

QVector<QPointF> *RouteObject::routePoints() const
{
    return _routePoints;
}

quint16 RouteObject::zoomLevel() const
{
    return _zoomLevel;
}

void RouteObject::setZoomLevel(const quint16 &zoomLevel)
{
    _zoomLevel = zoomLevel;
}

const std::pair<int, double> RouteObject::findMaximumDistance(const std::vector<QPointF>& Points)const{
    QPointF firstpoint=Points[0];
    QPointF lastpoint=Points[Points.size()-1];
  int index=0;  //index to be returned
  double Mdist=-1; //the Maximum distance to be returned

  //distance calculation
  QPointF p=lastpoint-firstpoint;
  for(int i=1;i<Points.size()-1;i++){ //traverse through second point to second last point
  QPointF pp=Points[i]-firstpoint;
  double Dist=fabs(pp.x() * p.y() - p.x() * pp.y()) / sqrt(p.x() * p.x() + p.y() * p.y()); //formula for point-to-line distance
  if (Dist>Mdist){
    Mdist=Dist;
    index=i;
  }
  }
  return std::make_pair(index, Mdist);
}



std::vector<QPointF> RouteObject::simplifyWithRDP(std::vector<QPointF>& Points, double epsilon)const{
  if(Points.size()<3){  //base case 1
    return Points;
  }
    std::pair<int, double> maxDistance=findMaximumDistance(Points);
    if(maxDistance.second>=epsilon){
    int index=maxDistance.first;
    auto it=Points.begin();
    std::vector<QPointF> path1(Points.begin(),it+index+1); //new path l1 from 0 to index
    std::vector<QPointF> path2(it+index,Points.end()); // new path l2 from index to last

   std::vector<QPointF> r1 =simplifyWithRDP(path1,epsilon);
    std::vector<QPointF> r2=simplifyWithRDP(path2,epsilon);

    //Concat simplified path1 and path2 together
    std::vector<QPointF> rs(r1);
    rs.pop_back();
    rs.insert(rs.end(),r2.begin(),r2.end());
    return rs;
  }
  else { //base case 2, all points between are to be removed.
    std::vector<QPointF> r(1,Points[0]);
      r.push_back(Points[Points.size()-1]);
    return r;
  }
}
