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

#include "qgstreamerstreamscontrol.h"
#include "qgstreamerplayersession.h"

QGstreamerStreamsControl::QGstreamerStreamsControl(QGstreamerPlayerSession *session, QObject *parent)
    :QMediaStreamsControl(parent), m_session(session)
{
    connect(m_session, SIGNAL(streamsChanged()), SIGNAL(streamsChanged()));
}

QGstreamerStreamsControl::~QGstreamerStreamsControl()
{
}

int QGstreamerStreamsControl::streamCount()
{
    return m_session->streamCount();
}

QMediaStreamsControl::StreamType QGstreamerStreamsControl::streamType(int streamNumber)
{
    return m_session->streamType(streamNumber);
}

QVariant QGstreamerStreamsControl::metaData(int streamNumber, const QString &key)
{
    return m_session->streamProperties(streamNumber).value(key);
}

bool QGstreamerStreamsControl::isActive(int streamNumber)
{
    return streamNumber != -1 && streamNumber == m_session->activeStream(streamType(streamNumber));
}

void QGstreamerStreamsControl::setActive(int streamNumber, bool state)
{
    QMediaStreamsControl::StreamType type = m_session->streamType(streamNumber);
    if (type == QMediaStreamsControl::UnknownStream)
        return;

    if (state)
        m_session->setActiveStream(type, streamNumber);
    else {
        //only one active stream of certain type is supported
        if (m_session->activeStream(type) == streamNumber)
            m_session->setActiveStream(type, -1);
    }
}

