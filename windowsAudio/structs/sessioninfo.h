#pragma once

#include <QString>
#include <windows.h>
#include <AudioSessionTypes.h>

struct SessionInfo{
    QString DisplayName;
    GUID groupingParam;
    QString IconPath;
    AudioSessionState state;
    DWORD processId = 0;
    QString sessionIdentifier;
    QString sessionInstanceIdentifier;
    bool isSystemSound;

    QString  processIconPath;
    QString  processName;
    float peakVolume = -1;

    float simpleVolume = -1;
    bool isMute;
};


QString printSessionInfo(SessionInfo &info);
