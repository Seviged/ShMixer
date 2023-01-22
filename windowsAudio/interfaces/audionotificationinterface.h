#pragma once

#include <audiopolicy.h>
#include <windows.h>
#include <QObject>

class AudioSession;

class AudioNotificationInterface : public IAudioSessionEvents
{
public:
    AudioNotificationInterface(AudioSession *parentSession);

    HRESULT OnDisplayNameChanged(LPCWSTR NewDisplayName, LPCGUID EventContext) override;
    HRESULT OnIconPathChanged(LPCWSTR NewIconPath, LPCGUID EventContext) override;
    HRESULT OnSimpleVolumeChanged(float NewVolume, BOOL NewMute, LPCGUID EventContext) override;
    HRESULT OnChannelVolumeChanged(DWORD ChannelCount, float NewChannelVolumeArray[  ],
                                   DWORD ChangedChannel, LPCGUID EventContext) override;
    HRESULT OnGroupingParamChanged(LPCGUID NewGroupingParam, LPCGUID EventContext) override;
    HRESULT OnStateChanged(AudioSessionState NewState) override;
    HRESULT OnSessionDisconnected(AudioSessionDisconnectReason DisconnectReason) override;

    HRESULT QueryInterface(REFIID riid, LPVOID * ppvObj) override;
    ULONG AddRef() override;
    ULONG Release() override;
    DWORD m_cRef = 0;

private:
    AudioSession *_session;
};

