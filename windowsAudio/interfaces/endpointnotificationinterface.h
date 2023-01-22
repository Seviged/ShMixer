#pragma once

#include <endpointvolume.h>
#include <windows.h>

class Device;

class EndpointNotificationInterface : public IAudioEndpointVolumeCallback
{
public:
    EndpointNotificationInterface(Device *device);

    HRESULT OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify) override;

    HRESULT QueryInterface(REFIID riid, LPVOID * ppvObj) override;
    ULONG AddRef() override;
    ULONG Release() override;
    DWORD m_cRef = 0;

private:
    Device *_device;
};

