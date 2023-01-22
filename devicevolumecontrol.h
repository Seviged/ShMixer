#pragma once

#include <QWidget>
#include "device.h"

namespace Ui {
class VolumeControl;
}


class DeviceVolumeControl : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceVolumeControl(Device *device, QWidget *parent = nullptr);
    ~DeviceVolumeControl();

public slots:
    void slotUpdateWidget(DeviceInfo);
    void slotUpdateVolumeLabel(int);

private slots:
    void on_toolButton_clicked();

private:
    Ui::VolumeControl *ui;
    Device *_device;
    DeviceInfo _lastInfo;

    void _update();
};

