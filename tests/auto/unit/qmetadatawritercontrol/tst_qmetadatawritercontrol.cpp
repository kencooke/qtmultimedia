/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: http://www.qt-project.org/
**
** This file is part of the test suite of the Qt Toolkit.
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
#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>
#include "qmetadatawritercontrol.h"

#include "mockmetadatawritercontrol.h"

class tst_QMetaDataWriterControl: public QObject
{
    Q_OBJECT

public slots:
    void initTestCase();
    void cleanupTestCase();

private slots:
    void constructor();
};

void tst_QMetaDataWriterControl::initTestCase()
{

}

void tst_QMetaDataWriterControl::cleanupTestCase()
{

}

//MaemoAPI-1862:test constructor
void tst_QMetaDataWriterControl::constructor()
{
    QMetaDataWriterControl *mock = new MockMetaDataWriterControl();
    mock->availableMetaData();
    mock->isMetaDataAvailable();
    mock->isWritable();
    mock->metaData(QtMultimedia::MetaData::Title);
    mock->setMetaData(QtMultimedia::MetaData::Title, QVariant());
    ((MockMetaDataWriterControl*)mock)->setWritable();
    ((MockMetaDataWriterControl*)mock)->setMetaDataAvailable();
    delete mock;
}

QTEST_MAIN(tst_QMetaDataWriterControl);

#include "tst_qmetadatawritercontrol.moc"
