#include "sessionnotificationinterface.h"
#include <QObject>
#include <QMetaObject>
#include "processcatcher.h"

SessionNotificationInterface::SessionNotificationInterface(ProcessCatcher *catcher) :
    _catcher(catcher)
{
    qRegisterMetaType<IAudioSessionControl*>("IAudioSessionControl*");
}

HRESULT SessionNotificationInterface::OnSessionCreated(IAudioSessionControl *NewSession)
{
    QMetaObject::invokeMethod(_catcher, "slotNewSession", Qt::DirectConnection,
                              Q_ARG(IAudioSessionControl*, NewSession));
    return S_OK;
}

HRESULT SessionNotificationInterface::QueryInterface(const IID &riid, LPVOID *ppvObj)
{
    if (!ppvObj)
        return E_INVALIDARG;
    *ppvObj = NULL;
    if (riid == IID_IUnknown || riid == __uuidof(IAudioSessionNotification))
    {
        // Increment the reference count and return the pointer.
        *ppvObj = (LPVOID)this;
        AddRef();
        return NOERROR;
    }
    return E_NOINTERFACE;
}

ULONG SessionNotificationInterface::AddRef()
{
    return ++m_cRef;
}

ULONG SessionNotificationInterface::Release()
{
    if (--m_cRef == 0)
    {
        delete this;
        return 0;
    }
    return m_cRef;
}
