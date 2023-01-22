#include "audionotificationinterface.h"
#include "audiosession.h"
#include <QDebug>

AudioNotificationInterface::AudioNotificationInterface(AudioSession *parentSession) :
    _session(parentSession)
{

}

HRESULT AudioNotificationInterface::OnDisplayNameChanged(LPCWSTR NewDisplayName, LPCGUID EventContext)
{
    qInfo() << "OnDisplayNameChanged";
    return S_OK;
}

HRESULT AudioNotificationInterface::OnIconPathChanged(LPCWSTR NewIconPath, LPCGUID EventContext)
{
    qInfo() << "OnIconPathChanged";
    return S_OK;
}

HRESULT AudioNotificationInterface::OnSimpleVolumeChanged(float NewVolume, BOOL NewMute, LPCGUID EventContext)
{
    qInfo() << "OnSimpleVolumeChanged" << NewVolume << NewMute;
    _session->updateSimpleVolume(NewVolume, NewMute, EventContext);
    return S_OK;
}

HRESULT AudioNotificationInterface::OnChannelVolumeChanged(DWORD ChannelCount, float NewChannelVolumeArray[], DWORD ChangedChannel, LPCGUID EventContext)
{
    qInfo() << "OnChannelVolumeChanged";
    return S_OK;
}

HRESULT AudioNotificationInterface::OnGroupingParamChanged(LPCGUID NewGroupingParam, LPCGUID EventContext)
{
    qInfo() << "OnGroupingParamChanged";
    return S_OK;
}

HRESULT AudioNotificationInterface::OnStateChanged(AudioSessionState NewState)
{
    qInfo() << "OnStateChanged" << NewState;
    if(NewState == AudioSessionStateExpired)
    {
        _session->deleteSession();
    }
    return S_OK;
}

HRESULT AudioNotificationInterface::OnSessionDisconnected(AudioSessionDisconnectReason DisconnectReason)
{
    qInfo() << "OnSessionDisconnected" << DisconnectReason;
    return S_OK;
}


HRESULT AudioNotificationInterface::QueryInterface(const IID &riid, LPVOID *ppvObj)
{
    if (!ppvObj)
        return E_INVALIDARG;
    *ppvObj = NULL;
    if (riid == IID_IUnknown || riid == __uuidof(IAudioSessionEvents))
    {
        // Increment the reference count and return the pointer.
        *ppvObj = (LPVOID)this;
        AddRef();
        return NOERROR;
    }
    return E_NOINTERFACE;
}

ULONG AudioNotificationInterface::AddRef()
{
    return ++m_cRef;
}

ULONG AudioNotificationInterface::Release()
{
    if (--m_cRef == 0)
    {
        delete this;
        return 0;
    }
    return m_cRef;
}
