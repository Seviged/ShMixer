#pragma once

#include <windows.h>
#include <audiopolicy.h>
#include <endpointvolume.h>
#include <mmdeviceapi.h>
#include <QString>
#include <QObject>
#include <QMap>

#include "sessioninfo.h"
#include "sessionnotificationinterface.h"
#include "audiosession.h"
#include "device.h"

Q_DECLARE_METATYPE(AudioSession*)
Q_DECLARE_METATYPE(Device*)

class ProcessCatcher : public QObject
{
    Q_OBJECT
public:
    ProcessCatcher(QObject *parent = 0);
    ~ProcessCatcher();

    bool initialize();

public slots:
    void slotNewSession(IAudioSessionControl *newSession);
    //void slotDeleteAudioSession();

signals:
    void signalNewSession(AudioSession*);
    void signalNewDevice(Device*);

private:

    bool _getSubSessionsInfo();
    bool _getSessionInfo(IAudioSessionControl *sessionControl);

    void _getSessionInfo(IAudioSessionControl *sessionControl, SessionInfo &info);
    void _getSession2Info(IAudioSessionControl2 *sessionControl2, SessionInfo &info);
    void _getVolumeInfo(ISimpleAudioVolume *simpleAudioVolume, SessionInfo &info);
    void _fillProcessInfo(DWORD pid, SessionInfo &info);

    void _getDeviceInfo(IMMDevice *winDevice);


    //QMap<int, AudioSession*> _sessionsMap;

    IAudioSessionManager* _rootSessionManager = nullptr;
    IAudioSessionEnumerator* _sessionEnumerator = nullptr;
    HRESULT _hr;
    SessionNotificationInterface *_notyfyIntf;

    bool _error(QString where);
};

