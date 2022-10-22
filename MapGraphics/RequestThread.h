#ifndef REQUESTTHREAD_H
#define REQUESTTHREAD_H

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QThread>
#include <QObject>

#include "MapGraphics_global.h"

class MAPGRAPHICSSHARED_EXPORT RequestThread : public QObject
{
    Q_OBJECT
public:
    RequestThread(QNetworkRequest&);
    ~RequestThread();
    QNetworkReply *getReply();
    void Finish();

public slots:
    void run();

private:
    QNetworkRequest _request;
    QNetworkReply* _reply{nullptr};
};

#endif // REQUESTTHREAD_H
