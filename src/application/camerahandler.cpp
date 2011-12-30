/******************************************************************************
 *  Copyright (C) 2005-2011 by                                                *
 *    Bjoern Erik Nilsen (bjoern.nilsen@bjoernen.com),                        *
 *    Fredrik Berg Kjoelstad (fredrikbk@hotmail.com),                         *
 *    Ralf Lange (ralf.lange@longsoft.de)                                     *
 *                                                                            *
 *  This program is free software; you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by      *
 *  the Free Software Foundation; either version 2 of the License, or         *
 *  (at your option) any later version.                                       *
 *                                                                            *
 *  This program is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *  GNU General Public License for more details.                              *
 *                                                                            *
 *  You should have received a copy of the GNU General Public License         *
 *  along with this program; if not, write to the                             *
 *  Free Software Foundation, Inc.,                                           *
 *  59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                 *
 ******************************************************************************/

#include "camerahandler.h"

#include <QtCore/QtDebug>
#include <QtGui/QIcon>
#include <QtGui/QImage>
#include <QtGui/QInputDialog>
#include <QtGui/QWhatsThis>


CameraHandler::CameraHandler(Frontend *f,
                             QObject *parent,
                             const QString &name)
    : QObject(parent)
{
    frontend = f;

    PreferencesTool *pref = frontend->getPreferences();
    captureFunction = (PreferencesTool::captureButtonFunction)pref->getBasicPreference("capturebutton", PreferencesTool::captureButtonAfter);

    isCameraOn = false;
    QString rootDir;
    rootDir.append(frontend->getUserDirName());
    rootDir.append(QLatin1String("/"));
    rootDir.append(QLatin1String("capturedfile.jpg"));
    temp.append(rootDir.toLatin1().constData());

    timer = new QTimer(this);
    timer->setSingleShot(true);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(storeFrame()));
    setObjectName(name);
}


CameraHandler::~CameraHandler()
{
    frontend = NULL;
}


void CameraHandler::changeCaptureButtonFunction(PreferencesTool::captureButtonFunction newFunction)
{
    // The function of the camera button is changed
    captureFunction = newFunction;
}


bool CameraHandler::isCameraRunning()
{
    return isCameraOn;
}


void CameraHandler::cameraStateChanged(bool isOn)
{
    qDebug("RecordingTab::cameraOn --> Start");

    isCameraOn = isOn;

    qDebug("RecordingTab::cameraOn --> Stop");
}


void CameraHandler::captureFrame()
{
    qDebug("CameraHandler::captureFrame --> Start");

    timer->start(60);

    qDebug("CameraHandler::captureFrame --> End");
}


void CameraHandler::storeFrame()
{
    qDebug("CameraHandler::storeFrame --> Start");

    QImage i;
    i.load(temp);
    if (!i.isNull()) {

        int sceneIndex = frontend->getProject()->getActiveSceneIndex();
        int takeIndex = frontend->getProject()->getActiveTakeIndex();
        int exposureIndex = frontend->getProject()->getActiveExposureIndex();
        switch (captureFunction) {
        case PreferencesTool::captureButtonBevor:
            frontend->getProject()->insertExposureToUndo(sceneIndex, takeIndex, exposureIndex, false, temp);
            break;
        case PreferencesTool::captureButtonAfter:
            frontend->getProject()->insertExposureToUndo(sceneIndex, takeIndex, exposureIndex, true, temp);
            break;
        case PreferencesTool::captureButtonAppend:
            frontend->getProject()->addExposureToUndo(sceneIndex, takeIndex, temp);
            break;
        }

        emit capturedFrame();
    } else {
        timer->start(60);
    }

    qDebug("CameraHandler::storeFrame --> End");
}
