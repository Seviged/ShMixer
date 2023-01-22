#include "device.h"
#include <QDebug>

Device::Device(GUID guid, DeviceInfo info, IAudioEndpointVolume *endpoint, QObject *parent)
    : QObject{parent},
      _guid(guid),
      _info(info),
      _endpoint(endpoint),
      _endpointNotify(new EndpointNotificationInterface(this))
{
    qRegisterMetaType<DeviceInfo>("DeviceInfo");

    _endpoint->AddRef();
}

Device::~Device()
{
    _endpoint->Release();
}

void Device::notify(PAUDIO_VOLUME_NOTIFICATION_DATA data)
{
    _info.volume = data->fMasterVolume;
    _info.guid = data->guidEventContext;
    _info.isMuted = data->bMuted;
    _info.nChannels = data->nChannels;

    qInfo() << _info.guid.Data1 << _info.guid.Data2 << _info.guid.Data3 << _info.guid.Data4;// << this;
    if(_info.guid.Data1 != _guid.Data1 && _info.guid.Data2 != _guid.Data2 && _info.guid.Data3 != _guid.Data3)
    {
        emit updateWidget(_info);
    }
}

EndpointNotificationInterface *Device::endpointNotify() const
{
    return _endpointNotify;
}

DeviceInfo& Device::info()
{
    return _info;
}

void Device::slotSetVolume(int volume)
{
    float v = volume / 100.0;
    _info.currentSliderVolume = volume;
    _endpoint->SetMasterVolumeLevelScalar(v, &_guid);
}

void Device::slotSetMute(bool mute)
{
    BOOL m = mute;
    _endpoint->SetMute(m, &_guid);
}
