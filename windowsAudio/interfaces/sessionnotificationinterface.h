#pragma once

#include <audiopolicy.h>
#include <windows.h>
#include <QObject>

Q_DECLARE_METATYPE(IAudioSessionControl*);
class ProcessCatcher;

class SessionNotificationInterface : public IAudioSessionNotification
{
public:
    SessionNotificationInterface(ProcessCatcher *catcher);
    virtual ~SessionNotificationInterface() {}

    HRESULT OnSessionCreated(IAudioSessionControl *NewSession) override;

    HRESULT QueryInterface(REFIID riid, LPVOID * ppvObj) override;
    ULONG AddRef() override;
    ULONG Release() override;
    DWORD m_cRef = 0;

    IAudioSessionControl *session() const;

private:
    IAudioSessionControl *_session = nullptr;
    ProcessCatcher *_catcher;
};

