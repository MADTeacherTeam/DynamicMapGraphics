#include "RequestThread.h"
#include "guts/MapGraphicsNetwork.h"
#include <QEventLoop>

RequestThread::RequestThread(QNetworkRequest & request) : QObject(nullptr), _request(request)
{

}

RequestThread::~RequestThread()
{

}

void RequestThread::run()
{
    MapGraphicsNetwork * network = MapGraphicsNetwork::getInstance();
    _reply = network->get(_request);
    QEventLoop loop;

    connect(_reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
}

QNetworkReply *RequestThread::getReply()
{
    return _reply;
}

void RequestThread::Finish()
{
    _reply->abort();
}
