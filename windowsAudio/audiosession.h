#pragma once

#include <QObject>
#include "sessioninfo.h"
#include "audionotificationinterface.h"

Q_DECLARE_METATYPE(SessionInfo)

class AudioSession : public QObject
{
    Q_OBJECT
public:
    explicit AudioSession(GUID guid, SessionInfo info, ISimpleAudioVolume *volume, QObject *parent = nullptr);
    ~AudioSession();

    SessionInfo& info();

    void updateSimpleVolume(float volume, bool mute, LPCGUID guid);

    AudioNotificationInterface *audioNotify() const;

    void deleteSession();

signals:
    void updateWidget(SessionInfo);
    void deleteWidget();

public slots:
    void slotSetVolume(int volume);
    void slotSetMute(bool mute);

private:
    GUID _guid;
    SessionInfo _info;
    ISimpleAudioVolume *_volume;
    AudioNotificationInterface *_audioNotify;
};

