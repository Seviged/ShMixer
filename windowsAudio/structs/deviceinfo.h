#pragma once

#include <QString>
#include <Windows.h>

struct DeviceInfo
{
    float volume = -1;
    QString name = "Device";
    QString icon = "";
    bool isMuted;
    unsigned int nChannels;
    GUID guid;

    int currentSliderVolume;
};

