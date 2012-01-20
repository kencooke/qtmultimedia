/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: http://www.qt-project.org/
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

/* Camera snippets */

#include "qcamera.h"
#include "qcameraviewfinder.h"
#include "qmediarecorder.h"
#include "qcameraimagecapture.h"
#include "qcameraimageprocessing.h"
#include "qabstractvideosurface.h"

/* Globals so that everything is consistent. */
QCamera *camera = 0;
QCameraViewfinder *viewfinder = 0;
QMediaRecorder *recorder = 0;
QCameraImageCapture *imageCapture = 0;

void overview_viewfinder()
{
    //! [Camera overview viewfinder]
    camera = new QCamera;
    viewfinder = new QCameraViewfinder;
    camera->setViewfinder(viewfinder);
    viewfinder->show();

    camera->start(); // to start the viewfinder
    //! [Camera overview viewfinder]
}

// -.-
class MyVideoSurface : public QAbstractVideoSurface
{
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
    {
        Q_UNUSED(handleType);
        return QList<QVideoFrame::PixelFormat>();
    }
    bool present(const QVideoFrame &frame)
    {
        Q_UNUSED(frame);
        return true;
    }
};

void overview_surface()
{
    MyVideoSurface *mySurface;
    //! [Camera overview surface]
    camera = new QCamera;
    mySurface = new MyVideoSurface;
    camera->setViewfinder(mySurface);

    camera->start();
    // MyVideoSurface::present(..) will be called with viewfinder frames
    //! [Camera overview surface]
}

void overview_still()
{
    //! [Camera overview capture]
    imageCapture = new QCameraImageCapture(camera);

    camera->setCaptureMode(QCamera::CaptureStillImage);
    camera->start(); // Viewfinder frames start flowing

    //on half pressed shutter button
    camera->searchAndLock();

    //on shutter button pressed
    imageCapture->capture();

    //on shutter button released
    camera->unlock();
    //! [Camera overview surface]
}

void overview_movie()
{
    //! [Camera overview movie]
    camera = new QCamera;
    recorder = new QMediaRecorder(camera);

    camera->setCaptureMode(QCamera::CaptureVideo);
    camera->start();

    //on shutter button pressed
    recorder->record();

    // sometime later, or on another press
    recorder->stop();
    //! [Camera overview movie]
}

void camera_blah()
{
    //! [Camera]
    camera = new QCamera;

    viewfinder = new QCameraViewfinder();
    viewfinder->show();

    camera->setViewfinder(viewfinder);

    imageCapture = new QCameraImageCapture(camera);

    camera->setCaptureMode(QCamera::CaptureStillImage);
    camera->start();
    //! [Camera]

    //! [Camera keys]
    //on half pressed shutter button
    camera->searchAndLock();

    //on shutter button pressed
    imageCapture->capture();

    //on shutter button released
    camera->unlock();
    //! [Camera keys]
}

void cameraimageprocessing()
{
    //! [Camera image whitebalance]
    camera = new QCamera;
    QCameraImageProcessing *imageProcessing = camera->imageProcessing();

    if (imageProcessing->isAvailable()) {
        imageProcessing->setWhiteBalanceMode(QCameraImageProcessing::WhiteBalanceFluorescent);
    }
    //! [Camera image whitebalance]

    //! [Camera image denoising]
    if (imageProcessing->isDenoisingSupported()) {
        imageProcessing->setDenoisingLevel(3);
    }
    //! [Camera image denoising]
}

void camerafocus()
{
    //! [Camera custom zoom]
    QCameraFocus *focus = camera->focus();
    focus->setFocusPointMode(QCameraFocus::FocusPointCustom);
    focus->setCustomFocusPoint(QPointF(0.25f, 0.75f)); // A point near the bottom left, 25% away from the corner, near that shiny vase
    //! [Camera custom zoom]

    //! [Camera combined zoom]
    focus->zoomTo(3.0, 4.0); // Super zoom!
    //! [Camera combined zoom]

    //! [Camera focus zones]
    focus->setFocusPointMode(QCameraFocus::FocusPointAuto);
    QList<QCameraFocusZone> zones = focus->focusZones();
    foreach (QCameraFocusZone zone, zones) {
        if (zone.status() == QCameraFocusZone::Focused) {
            // Draw a green box at zone.area()
        } else if (zone.status() == QCameraFocusZone::Selected) {
            // This area is selected for autofocusing, but is not in focus
            // Draw a yellow box at zone.area()
        }
    }
    //! [Camera focus zones]
}
