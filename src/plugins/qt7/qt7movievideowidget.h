/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: http://www.qt-project.org/
**
** This file is part of the Qt Toolkit.
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

#ifndef QT7MOVIEVIDEOWIDGET_H
#define QT7MOVIEVIDEOWIDGET_H

#include <QtCore/qobject.h>
#include <QtCore/qmutex.h>

#include <qvideowindowcontrol.h>
#include <qmediaplayer.h>

#include "qt7videooutput.h"

#include <QuartzCore/CVOpenGLTexture.h>
#include <QuickTime/QuickTime.h>

class GLVideoWidget;

QT_BEGIN_NAMESPACE

class QCvDisplayLink;
class QT7PlayerSession;
class QT7PlayerService;

class QT7MovieVideoWidget : public QT7VideoWidgetControl
{
Q_OBJECT
public:
    QT7MovieVideoWidget(QObject *parent = 0);
    virtual ~QT7MovieVideoWidget();

    void setMovie(void *movie);
    void updateNaturalSize(const QSize &newSize);

    QWidget *videoWidget();

    bool isFullScreen() const;
    void setFullScreen(bool fullScreen);

    QSize nativeSize() const;

    Qt::AspectRatioMode aspectRatioMode() const;
    void setAspectRatioMode(Qt::AspectRatioMode mode);

    int brightness() const;
    void setBrightness(int brightness);

    int contrast() const;
    void setContrast(int contrast);

    int hue() const;
    void setHue(int hue);

    int saturation() const;
    void setSaturation(int saturation);

private Q_SLOTS:
    void updateVideoFrame(const CVTimeStamp &ts);
    
private:
    void setupVideoOutput();
    bool createVisualContext();

    void updateColors();

    void *m_movie;
    GLVideoWidget *m_videoWidget;

    QCvDisplayLink *m_displayLink;

#ifdef QUICKTIME_C_API_AVAILABLE
    QTVisualContextRef	m_visualContext;
#endif

    bool m_fullscreen;
    QSize m_nativeSize;
    Qt::AspectRatioMode m_aspectRatioMode;
    int m_brightness;
    int m_contrast;
    int m_hue;
    int m_saturation;
};

QT_END_NAMESPACE

#endif
