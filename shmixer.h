#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class ShMixer; }
QT_END_NAMESPACE

class AudioSession;
class Device;

class ShMixer : public QWidget
{
    Q_OBJECT

public:
    ShMixer(QWidget *parent = nullptr);
    ~ShMixer();

public slots:
    void slotAddNewSession(AudioSession*);
    void slotAddNewDevice(Device*);
    void slotResize();

private:
    Ui::ShMixer *ui;

    QSize _minSize;
};
