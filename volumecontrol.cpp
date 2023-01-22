#include "volumecontrol.h"
#include "ui_volumecontrol.h"
#include "audiosession.h"
#include <QMessageBox>
#include <QFileInfo>
#include <QFileSystemModel>

VolumeControl::VolumeControl(AudioSession *session, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VolumeControl)
{
    ui->setupUi(this);

    _session = session;
    _lastInfo = _session->info();
    _update();

    //QPixmap pic(_lastInfo.processIconPath);
    //ui->labelIcon->setPixmap(pic);

    QFileInfo  fin(_lastInfo.processIconPath);
    QFileSystemModel model;// = new QFileSystemModel;
    model.setRootPath(fin.path());
    QIcon ic = model.fileIcon(model.index(fin.filePath()));
    ui->labelIcon->setPixmap(ic.pixmap(QSize(32, 32)));

    ui->labelVolume->setText(QString::number(ui->verticalSlider->value()));

    connect(_session, SIGNAL(updateWidget(SessionInfo)),
            this, SLOT(slotUpdateWidget(SessionInfo)));

    connect(ui->checkBox, SIGNAL(toggled(bool)),
            _session, SLOT(slotSetMute(bool)));

    connect(ui->verticalSlider, SIGNAL(valueChanged(int)),
            _session, SLOT(slotSetVolume(int)), Qt::DirectConnection);

    connect(ui->verticalSlider, SIGNAL(valueChanged(int)),
            this, SLOT(slotUpdateVolumeLabel(int)));
}

VolumeControl::~VolumeControl()
{
    delete ui;
}

void VolumeControl::slotUpdateWidget(SessionInfo info)
{
    _lastInfo = info;
    _update();
}

void VolumeControl::slotUpdateVolumeLabel(int v)
{
    ui->labelVolume->setText(QString::number(v));
}


void VolumeControl::on_toolButton_clicked()
{
    QString str = printSessionInfo(_lastInfo);
    QMessageBox msgBox;
    msgBox.setText(str);
    msgBox.exec();

}

void VolumeControl::_update()
{
    ui->verticalSlider->setValue(_lastInfo.simpleVolume * 100);
    ui->labelName->setText(_lastInfo.processName);
    ui->labelVolume->setText(QString::number(_lastInfo.simpleVolume * 100, 'h', 0));
    ui->checkBox->setChecked(_lastInfo.isMute);
}

