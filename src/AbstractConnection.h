#ifndef ABSTRACTCONNECTION_H
#define ABSTRACTCONNECTION_H

#include <QObject>

namespace UJ
{

class Terminal;

namespace Connection
{

class QDate;
class Site;

class AbstractConnection : public QObject
{
    Q_OBJECT

public:
    explicit AbstractConnection(QObject *parent = 0);
    enum Port
    {
        PortDefault = -1
    };
    virtual bool connectTo(Site *s) = 0;
    virtual bool connectTo(QString &address, Port port = PortDefault) = 0;

public slots:
    virtual void close() = 0;
    virtual void reconnect() = 0;
    virtual void receivedBytes(uchar *bytes, uint length) = 0;
    virtual void sendBytes(uchar *bytes, uint length) = 0;

protected:
    Site *_site;
    QString _name;
    QString _address;
    QDate *_lastDate;
    // NOTE: Need an image member for tab icon...
    //       Haven't decided which class, maybe QIcon?
    bool _isConnected;
    bool _isProcessing;

public: // Getters & Setters
    virtual inline Site *site()
    {
        return _site;
    }
    virtual void setSite(Site *site);
    virtual inline QString name()
    {
        return _name;
    }
    virtual inline void setName(QString &name)
    {
        _name = name;
    }
    virtual inline QString address()
    {
        return _address;
    }
    virtual inline void setAddress(QString &address)
    {
        _address = address;
    }
    virtual inline bool isConnected()
    {
        return _isConnected;
    }
    virtual inline void setConnected(bool isConnected)
    {
        // NOTE: Should change the icon image here...
        _isConnected = isConnected;
    }
    virtual inline bool isProcessing()
    {
        return _isProcessing;
    }
    virtual inline void setProcessing(Terminal *isProcessing)
    {
        _isProcessing = isProcessing;
    }
    virtual inline QDate *lastDate()
    {
        return _lastDate;
    }
};

}   // namespace Connection

}   // namespace UJ

#endif // ABSTRACTCONNECTION_H