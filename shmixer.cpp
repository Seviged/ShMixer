#include "shmixer.h"
#include "./ui_shmixer.h"
#include "processcatcher.h"
#include "audiosession.h"
#include "volumecontrol.h"
#include "devicevolumecontrol.h"

ShMixer::ShMixer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ShMixer)
{
    ui->setupUi(this);

    _minSize = this->size();

    ProcessCatcher *catcher = new ProcessCatcher(this);

    connect(catcher, SIGNAL(signalNewSession(AudioSession*)),
            this, SLOT(slotAddNewSession(AudioSession*)), Qt::QueuedConnection);

    connect(catcher, SIGNAL(signalNewDevice(Device*)),
            this, SLOT(slotAddNewDevice(Device*)), Qt::QueuedConnection);

    catcher->initialize();


}

ShMixer::~ShMixer()
{
    delete ui;
}

void ShMixer::slotAddNewSession(AudioSession *session)
{
    auto volumeControl = new VolumeControl(session);

    connect(session, SIGNAL(deleteWidget()),
            volumeControl, SLOT(deleteLater()));

    connect(volumeControl, SIGNAL(destroyed(QObject*)),
            this, SLOT(slotResize()));

    ui->horizontalLayout->addWidget(volumeControl);

    volumeControl->show();
    slotResize();
}

void ShMixer::slotAddNewDevice(Device *device)
{
    auto volumeControl = new DeviceVolumeControl(device);

    ui->horizontalLayoutDevice->addWidget(volumeControl);

    volumeControl->show();
    slotResize();
}

void ShMixer::slotResize()
{
    this->resize(0, 0);
}


