#pragma once

#include <QObject>
#include "deviceinfo.h"
#include <endpointvolume.h>
#include "endpointnotificationinterface.h"

Q_DECLARE_METATYPE(DeviceInfo)

class Device : public QObject
{
    Q_OBJECT
public:
    explicit Device(GUID guid, DeviceInfo info, IAudioEndpointVolume *endpoint, QObject *parent = nullptr);
    ~Device();

    void notify(PAUDIO_VOLUME_NOTIFICATION_DATA data);

    EndpointNotificationInterface *endpointNotify() const;

    DeviceInfo& info();

signals:
    void updateWidget(DeviceInfo);

public slots:
    void slotSetVolume(int);
    void slotSetMute(bool);

private:
    GUID _guid;
    DeviceInfo _info;
    EndpointNotificationInterface *_endpointNotify;
    IAudioEndpointVolume *_endpoint;

};

