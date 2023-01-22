#include "devicevolumecontrol.h"
#include "ui_volumecontrol.h"
#include <QMessageBox>
#include <QFileInfo>
#include <QFileSystemModel>

DeviceVolumeControl::DeviceVolumeControl(Device *device, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VolumeControl)
{
    ui->setupUi(this);

    _device = device;
    _lastInfo = _device->info();
    _update();

    //QPixmap pic(_lastInfo.processIconPath);
    //ui->labelIcon->setPixmap(pic);

    //QFileInfo  fin(_lastInfo.processIconPath);
    //QFileSystemModel model;// = new QFileSystemModel;
    //model.setRootPath(fin.path());
    //QIcon ic = model.fileIcon(model.index(fin.filePath()));
    //ui->labelIcon->setPixmap(ic.pixmap(QSize(64, 64)));

    ui->labelVolume->setText(QString::number(ui->verticalSlider->value()));

    connect(_device, SIGNAL(updateWidget(DeviceInfo)),
            this, SLOT(slotUpdateWidget(DeviceInfo)));

    connect(ui->checkBox, SIGNAL(toggled(bool)),
            _device, SLOT(slotSetMute(bool)));

    connect(ui->verticalSlider, SIGNAL(valueChanged(int)),
            _device, SLOT(slotSetVolume(int)));

    connect(ui->verticalSlider, SIGNAL(valueChanged(int)),
            this, SLOT(slotUpdateVolumeLabel(int)));
}

DeviceVolumeControl::~DeviceVolumeControl()
{
     delete ui;
}


void DeviceVolumeControl::slotUpdateWidget(DeviceInfo info)
{
    _lastInfo = info;
    _update();
}

void DeviceVolumeControl::slotUpdateVolumeLabel(int v)
{
    ui->labelVolume->setText(QString::number(v));
}


void DeviceVolumeControl::on_toolButton_clicked()
{
    //QString str = printSessionInfo(_lastInfo);
    QMessageBox msgBox;
    msgBox.setText("Device");
    msgBox.exec();

}

void DeviceVolumeControl::_update()
{
    if(_lastInfo.currentSliderVolume != _lastInfo.volume * 100)
    {
        ui->verticalSlider->setValue(_lastInfo.volume * 100);
    }
    ui->labelName->setText(_lastInfo.name);

}
