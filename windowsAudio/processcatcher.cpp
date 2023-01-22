#include "processcatcher.h"
#include <comdef.h>
#include <QDebug>


ProcessCatcher::ProcessCatcher(QObject *parent) :
    QObject(parent)
{
    qRegisterMetaType<AudioSession*>("AudioSession*");
    qRegisterMetaType<Device*>("Device*");
}

ProcessCatcher::~ProcessCatcher()
{
    if(_rootSessionManager != nullptr)
    {
        _rootSessionManager->Release();
    }

    if(_sessionEnumerator != nullptr)
    {
        _sessionEnumerator->Release();
    }
}

bool ProcessCatcher::initialize()
{
    IMMDeviceEnumerator* deviceEnumerator = nullptr;

    const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
    const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
    _hr = CoCreateInstance(
                CLSID_MMDeviceEnumerator, nullptr,
                CLSCTX_ALL, IID_IMMDeviceEnumerator,
                (void**)&deviceEnumerator);
    if(_error("")) return false;

    IMMDevice* device = nullptr;
    _hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &device);
    if(_error("")) return false;

    _hr = device->Activate(__uuidof(IAudioSessionManager), CLSCTX_ALL, nullptr, (void**)&_rootSessionManager);
    if(_error("")) return false;

    IAudioSessionManager2* subSessionManager = nullptr;
    _hr = device->Activate(__uuidof(IAudioSessionManager2), CLSCTX_ALL, nullptr, (void**)&subSessionManager);
    if(_error("")) return false;


    _notyfyIntf = new SessionNotificationInterface(this);
    subSessionManager->RegisterSessionNotification(_notyfyIntf);

    _hr = subSessionManager->GetSessionEnumerator(&_sessionEnumerator);
    if(_error("")) return false;

    _getDeviceInfo(device);

    deviceEnumerator->Release();
    device->Release();
    subSessionManager->Release();

    _getSubSessionsInfo();

    return true;
}

void ProcessCatcher::slotNewSession(IAudioSessionControl *newSession)
{
    qInfo() << "newSession!!!!" << newSession;
    _getSessionInfo(newSession);
    newSession->AddRef();
    //if(newSession != nullptr) newSession->Release();
}

bool ProcessCatcher::_getSubSessionsInfo()
{
    int sessionCount = 0;
    _hr = _sessionEnumerator->GetCount(&sessionCount);
    if(_error("")) return false;


    for (int i = 0; i < sessionCount; i++) {
        IAudioSessionControl* sessionControl = nullptr; //вроде информация о процессе + точка запроса остальных интерфейсов потока
        _hr = _sessionEnumerator->GetSession(i, &sessionControl);
        if(_error("")) return false;

        _getSessionInfo(sessionControl);

        if(sessionControl != nullptr) sessionControl->Release();
    }

    return true;
}

bool ProcessCatcher::_getSessionInfo(IAudioSessionControl *sessionControl)
{
    IAudioSessionControl2* sessionControl2 = nullptr; //получения пида процесса
    ISimpleAudioVolume *simpleAudioVolume = nullptr;

    _hr = sessionControl->QueryInterface(__uuidof(IAudioSessionControl2), (void**)&sessionControl2);
    if(_error("")) return false;

    _hr = sessionControl->QueryInterface(__uuidof(ISimpleAudioVolume), (void**) &simpleAudioVolume);
    if(_error("")) return false;



    SessionInfo info;

    _getSessionInfo(sessionControl, info);
    _getSession2Info(sessionControl2, info);
    _getVolumeInfo(simpleAudioVolume, info);
    _fillProcessInfo(info.processId, info);

    GUID guid;
    _hr = CoCreateGuid(&guid);
    if(_error("")) return false;

    AudioSession *session = new AudioSession(guid, info, simpleAudioVolume);
    session->moveToThread(this->parent()->thread());

    _hr = sessionControl->RegisterAudioSessionNotification(session->audioNotify());
    if(_error("")) return false;

    emit signalNewSession(session);

    qInfo() << "---------------------------";
    printSessionInfo(info);

    if(sessionControl2 != nullptr) sessionControl2->Release();
    //if(simpleAudioVolume != nullptr) simpleAudioVolume->Release();

    return true;
}

void ProcessCatcher::_getSessionInfo(IAudioSessionControl *sessionControl, SessionInfo &info)
{
    if(sessionControl == nullptr) return;

    wchar_t *tmp = nullptr;

    sessionControl->GetDisplayName(&tmp);
    info.DisplayName = QString::fromWCharArray(tmp);

    GUID guid;
    sessionControl->GetGroupingParam(&guid);
    info.groupingParam = guid;

    sessionControl->GetIconPath(&tmp);
    info.IconPath = QString::fromWCharArray(tmp);

    AudioSessionState state;
    sessionControl->GetState(&state);
    info.state = state;

    CoTaskMemFree(tmp);
}

void ProcessCatcher::_getSession2Info(IAudioSessionControl2 *sessionControl2, SessionInfo &info)
{
    if(sessionControl2 == nullptr) return;

    wchar_t *tmp = nullptr;

    DWORD pid = 0;
    sessionControl2->GetProcessId(&pid);
    info.processId = pid;

    sessionControl2->GetSessionIdentifier(&tmp);
    info.sessionIdentifier = QString::fromWCharArray(tmp);

    sessionControl2->GetSessionInstanceIdentifier(&tmp);
    info.sessionInstanceIdentifier = QString::fromWCharArray(tmp);

    info.isSystemSound = sessionControl2->IsSystemSoundsSession() == S_OK;

    CoTaskMemFree(tmp);
}

void ProcessCatcher::_getVolumeInfo(ISimpleAudioVolume *simpleAudioVolume, SessionInfo &info)
{
    float vol;
    simpleAudioVolume->GetMasterVolume(&vol);

    BOOL mute;
    simpleAudioVolume->GetMute(&mute);

    info.simpleVolume = vol;
    info.isMute = mute;

}



bool ProcessCatcher::_error(QString where)
{
    if(_hr != S_OK)
    {
        _com_error err(_hr);
        LPCTSTR errMsg = err.ErrorMessage();
        qDebug() << "Error at:" << where << "Msg and info:" << QString::fromWCharArray(errMsg) << err.ErrorInfo();
    }

    return _hr == S_OK ? false : true;
}


void ProcessCatcher::_fillProcessInfo(DWORD pid, SessionInfo &info)
{
    HANDLE handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (handle) {

        WCHAR buffer[MAX_PATH];
        DWORD bufSize = MAX_PATH;
           if (QueryFullProcessImageNameW(handle, 0, buffer, &bufSize))
           {
               info.processIconPath = QString::fromWCharArray(buffer);
               info.processName = info.processIconPath.split('\\').last();
               info.processName = info.processName.left(info.processName.lastIndexOf('.'));
           }

        CloseHandle(handle);
    }

}

void ProcessCatcher::_getDeviceInfo(IMMDevice *winDevice)
{
    IAudioEndpointVolume *endpointVolume = nullptr;
    _hr = winDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, nullptr, (void**)&endpointVolume);
    if(_error("")) return;

    float var;
    _hr = endpointVolume->GetMasterVolumeLevelScalar(&var);
    if(_error("")) return;

    UINT ch;
    _hr = endpointVolume->GetChannelCount(&ch);
    if(_error("")) return;

    BOOL mute;
    _hr = endpointVolume->GetMute(&mute);
    if(_error("")) return;



    DeviceInfo info;

    info.isMuted = mute;
    info.nChannels = ch;
    info.volume = var;

    GUID guid;
    _hr = CoCreateGuid(&guid);
    if(_error("")) return;

    Device *device = new Device(guid, info, endpointVolume);
    device->moveToThread(this->parent()->thread());

    _hr = endpointVolume->RegisterControlChangeNotify(device->endpointNotify());
    if(_error("")) return;

    emit signalNewDevice(device);
}
