/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: http://www.qt-project.org/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtCore/qdir.h>
#include <QtWidgets/qfiledialog.h>

#include <qaudiorecorder.h>
#include <qmediarecorder.h>

#include "audiorecorder.h"

#if defined(Q_WS_MAEMO_6)
#include "ui_audiorecorder_small.h"
#else
#include "ui_audiorecorder.h"
#endif

AudioRecorder::AudioRecorder(QWidget *parent)
    :
    QMainWindow(parent),
    ui(new Ui::AudioRecorder),
    outputLocationSet(false)
{
    ui->setupUi(this);

    audioRecorder = new QAudioRecorder(this);

    //audio devices
    ui->audioDeviceBox->addItem(tr("Default"), QVariant(QString()));
    foreach (const QString &device, audioRecorder->audioInputs()) {
        ui->audioDeviceBox->addItem(device, QVariant(device));
    }

    //audio codecs
    ui->audioCodecBox->addItem(tr("Default"), QVariant(QString()));
    foreach (const QString &codecName, audioRecorder->supportedAudioCodecs()) {
        ui->audioCodecBox->addItem(codecName, QVariant(codecName));
    }

    //containers
    ui->containerBox->addItem(tr("Default"), QVariant(QString()));
    foreach (const QString &containerName, audioRecorder->supportedContainers()) {
        ui->containerBox->addItem(containerName, QVariant(containerName));
    }

    //sample rate:
    ui->sampleRateBox->addItem(tr("Default"), QVariant(0));
    foreach (int sampleRate, audioRecorder->supportedAudioSampleRates()) {
        ui->sampleRateBox->addItem(QString::number(sampleRate), QVariant(
                sampleRate));
    }

    ui->qualitySlider->setRange(0, int(QtMultimedia::VeryHighQuality));
    ui->qualitySlider->setValue(int(QtMultimedia::NormalQuality));

    //bitrates:
    ui->bitrateBox->addItem(QString("Default"), QVariant(0));
    ui->bitrateBox->addItem(QString("32000"), QVariant(32000));
    ui->bitrateBox->addItem(QString("64000"), QVariant(64000));
    ui->bitrateBox->addItem(QString("96000"), QVariant(96000));
    ui->bitrateBox->addItem(QString("128000"), QVariant(128000));

    connect(audioRecorder, SIGNAL(durationChanged(qint64)), this,
            SLOT(updateProgress(qint64)));
    connect(audioRecorder, SIGNAL(stateChanged(QMediaRecorder::State)), this,
            SLOT(updateState(QMediaRecorder::State)));
    connect(audioRecorder, SIGNAL(error(QMediaRecorder::Error)), this,
            SLOT(displayErrorMessage()));
    }

AudioRecorder::~AudioRecorder()
{
    delete audioRecorder;
}

void AudioRecorder::updateProgress(qint64 duration)
{
    if (audioRecorder->error() != QMediaRecorder::NoError || duration < 2000)
        return;

    ui->statusbar->showMessage(tr("Recorded %1 sec").arg(duration / 1000));
}

void AudioRecorder::updateState(QMediaRecorder::State state)
{
    QString statusMessage;

    switch (state) {
        case QMediaRecorder::RecordingState:
            ui->recordButton->setText(tr("Stop"));
            ui->pauseButton->setText(tr("Pause"));
            if (audioRecorder->outputLocation().isEmpty())
                statusMessage = tr("Recording");
            else
                statusMessage = tr("Recording to %1").arg(
                        audioRecorder->outputLocation().toString());
            break;
        case QMediaRecorder::PausedState:
            ui->recordButton->setText(tr("Stop"));
            ui->pauseButton->setText(tr("Resume"));
            statusMessage = tr("Paused");
            break;
        case QMediaRecorder::StoppedState:
            ui->recordButton->setText(tr("Record"));
            ui->pauseButton->setText(tr("Pause"));
            statusMessage = tr("Stopped");
    }

    ui->pauseButton->setEnabled(state != QMediaRecorder::StoppedState);

    if (audioRecorder->error() == QMediaRecorder::NoError)
        ui->statusbar->showMessage(statusMessage);
}

static QVariant boxValue(const QComboBox *box)
{
    int idx = box->currentIndex();
    if (idx == -1)
        return QVariant();

    return box->itemData(idx);
}

void AudioRecorder::toggleRecord()
{
    if (audioRecorder->state() == QMediaRecorder::StoppedState) {
        audioRecorder->setAudioInput(boxValue(ui->audioDeviceBox).toString());

        if (!outputLocationSet)
            audioRecorder->setOutputLocation(generateAudioFilePath());

        QAudioEncoderSettings settings;
        settings.setCodec(boxValue(ui->audioCodecBox).toString());
        settings.setSampleRate(boxValue(ui->sampleRateBox).toInt());
        settings.setBitRate(boxValue(ui->bitrateBox).toInt());
        settings.setQuality(QtMultimedia::EncodingQuality(ui->qualitySlider->value()));
        settings.setEncodingMode(ui->constantQualityRadioButton->isChecked() ?
                                 QtMultimedia::ConstantQualityEncoding :
                                 QtMultimedia::ConstantBitRateEncoding);

        QString container = boxValue(ui->containerBox).toString();

        audioRecorder->setEncodingSettings(settings, QVideoEncoderSettings(), container);
        audioRecorder->record();
    }
    else {
        audioRecorder->stop();
    }
}

void AudioRecorder::togglePause()
{
    if (audioRecorder->state() != QMediaRecorder::PausedState)
        audioRecorder->pause();
    else
        audioRecorder->record();
}

void AudioRecorder::setOutputLocation()
{
    QString fileName = QFileDialog::getSaveFileName();
    audioRecorder->setOutputLocation(QUrl(fileName));
    outputLocationSet = true;
}

void AudioRecorder::displayErrorMessage()
{
    ui->statusbar->showMessage(audioRecorder->errorString());
}

QUrl AudioRecorder::generateAudioFilePath()
{
    QDir outputDir(QDir::rootPath());

    int lastImage = 0;
    int fileCount = 0;
    foreach(QString fileName, outputDir.entryList(QStringList() << "testclip_*")) {
        int imgNumber = fileName.mid(5, fileName.size() - 9).toInt();
        lastImage = qMax(lastImage, imgNumber);
        if (outputDir.exists(fileName))
            fileCount += 1;
    }
    lastImage += fileCount;
    QUrl location(QDir::toNativeSeparators(outputDir.canonicalPath() + QString("/testclip_%1").arg(lastImage + 1, 4, 10, QLatin1Char('0'))));
    return location;
}
