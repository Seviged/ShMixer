#pragma once

#include <QWidget>
#include "audiosession.h"

namespace Ui {
class VolumeControl;
}



class VolumeControl : public QWidget
{
    Q_OBJECT

public:
    explicit VolumeControl(AudioSession *session, QWidget *parent = nullptr);
    ~VolumeControl();

public slots:
    void slotUpdateWidget(SessionInfo);
    void slotUpdateVolumeLabel(int);

private slots:
    void on_toolButton_clicked();

private:
    Ui::VolumeControl *ui;
    AudioSession *_session;
    SessionInfo _lastInfo;

    void _update();
};

