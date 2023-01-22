#include "endpointnotificationinterface.h"
#include "device.h"

EndpointNotificationInterface::EndpointNotificationInterface(Device *device)
{
    _device = device;
}

HRESULT EndpointNotificationInterface::OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify)
{
    _device->notify(pNotify);

    return S_OK;
}

HRESULT EndpointNotificationInterface::QueryInterface(const IID &riid, LPVOID *ppvObj)
{
    if (!ppvObj)
        return E_INVALIDARG;
    *ppvObj = NULL;
    if (riid == IID_IUnknown || riid == __uuidof(IAudioEndpointVolumeCallback))
    {
        // Increment the reference count and return the pointer.
        *ppvObj = (LPVOID)this;
        AddRef();
        return NOERROR;
    }
    return E_NOINTERFACE;
}

ULONG EndpointNotificationInterface::AddRef()
{
    return ++m_cRef;
}

ULONG EndpointNotificationInterface::Release()
{
    if (--m_cRef == 0)
    {
        delete this;
        return 0;
    }
    return m_cRef;
}
