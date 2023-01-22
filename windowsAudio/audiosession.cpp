#include "audiosession.h"
#include <QMetaObject>
#include <QDebug>

AudioSession::AudioSession(GUID guid, SessionInfo info, ISimpleAudioVolume *volume, QObject *parent)
    : QObject{parent},
      _guid(guid),
      _info(info),
      _volume(volume),
      _audioNotify(new AudioNotificationInterface(this))
{
    qRegisterMetaType<SessionInfo>("SessionInfo");

    _volume->AddRef();
}

AudioSession::~AudioSession()
{
    _volume->Release();
}

SessionInfo &AudioSession::info()
{
    return _info;
}

void AudioSession::updateSimpleVolume(float volume, bool mute, LPCGUID guid)
{
    _info.simpleVolume = volume;
    _info.isMute = mute;

    qInfo() << guid->Data1 << guid->Data2 << guid->Data3 << guid->Data4;// << this;
    if(guid->Data1 != _guid.Data1 && guid->Data2 != _guid.Data2 && guid->Data3 != _guid.Data3)
    {
        emit updateWidget(_info);
    }
}

AudioNotificationInterface *AudioSession::audioNotify() const
{
    return _audioNotify;
}

void AudioSession::deleteSession()
{
    emit deleteWidget();
    this->deleteLater();
}

void AudioSession::slotSetVolume(int volume)
{
    float v = volume / 100.0;
    _volume->SetMasterVolume(v, &_guid);
}

void AudioSession::slotSetMute(bool mute)
{
    BOOL b = mute;
    _volume->SetMute(b, &_guid);
}
