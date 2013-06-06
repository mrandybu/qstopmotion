/******************************************************************************
 *  Copyright (C) 2005-2013 by                                                *
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

#include "importwidget.h"

#include "domain/domainfacade.h"
#include "frontends/qtfrontend/elements/flexiblelineedit.h"
#include "technical/preferencestool.h"
#include "technical/grabber/imagegrabber.h"

#include <QtCore/QtDebug>
#include <QtCore/QRect>
#include <QtGui/QFileDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QInputDialog>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>


ImportWidget::ImportWidget(Frontend *f, bool type, QWidget *parent) : QWidget(parent)
{
    qDebug("ImportWidget::Constructor --> Start");

    frontend                 = f;
    tabType                  = type;

    infoText                 = 0;
    encoderTable             = 0;

    // Grabber preferences
    grabberPrefs             = 0;
    grabberSourceLabel       = 0;
    grabberSourceCombo       = 0;
    activeGrabberSource      = ImageGrabberDevice::testSource;

    // Image preferences
    imagePrefs               = 0;
    imageFormatLabel         = 0;
    imageFormatCombo         = 0;
    activeImageFormat        = ImageGrabber::jpegFormat;
    imageQualityLabel        = 0;
    imageQualitySlider       = 0;
    activeImageQuality       = 100;
    qualityMinimumLabel      = 0;
    qualityMaximumLabel      = 0;
    imageSizeLabel           = 0;
    imageSizeCombo           = 0;
    activeImageSize          = ImageGrabber::defaultSize;

    // Transformation preferences
    transformPrefs           = 0;
    transformText            = 0;
    scaleButton              = 0;
    clipButton               = 0;

    // Adjustment preferences
    adjustmentPrefs          = 0;
    leftUpButton             = 0;
    centerUpButton           = 0;
    rightUpButton            = 0;
    leftMiddleButton         = 0;
    centerMiddleButton       = 0;
    rightMiddleButton        = 0;
    leftDownButton           = 0;
    centerDownButton         = 0;
    rightDownButton          = 0;
    activeImageAdjustment    = ImageGrabber::centerDown;

    // Live view preferences
    liveViewPrefs            = 0;
    liveViewFpsLabel         = 0;
    liveViewFpsSlider        = 0;
    activeLiveViewFps        = 20;
    fpsMinimumLabel          = 0;
    fpsMaximumLabel          = 0;

    this->setObjectName("ImportWidget");

    makeGUI();

    qDebug("ImportWidget::Constructor --> End");
}


void ImportWidget::makeGUI()
{
    qDebug("ImportWidget::makeGUI --> Start");

    infoText = new QTextEdit;
    infoText->setReadOnly(true);
    if (tabType) {
        infoText->setHtml(
            "<p>" +
            tr("Below you can set which image sources should be used "
               "for importing images to a new project.") +
            "</p>");
    }
    else {
        infoText->setHtml(
            "<p>" +
            tr("Below you can set which image sources should be used "
               "for importing images to the currently active project.") +
            "</p>");
    }

    infoText->setMinimumWidth(440);
    infoText->setMinimumHeight(55);
    infoText->setMaximumHeight(60);
    infoText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Image grabber preferences
    grabberPrefs = new QGroupBox;
    grabberPrefs->setTitle(tr("Image grabber settings"));

    grabberSourceLabel = new QLabel(tr("Image Grabber:"));
    grabberSourceCombo = new QComboBox();
    grabberSourceLabel->setBuddy(grabberSourceCombo);
    grabberSourceCombo->setFocusPolicy(Qt::NoFocus);
    grabberSourceCombo->setMinimumWidth(300);
    grabberSourceCombo->setMaximumWidth(300);
    connect(grabberSourceCombo, SIGNAL(activated(int)), this, SLOT(changeImageGrabberSources(int)));

    grabberSourceCombo->addItem(tr("Test Source"));

#ifdef Q_WS_X11
    grabberSourceCombo->addItem(tr("Video 4 Linux (USB WebCam)"));
    grabberSourceCombo->addItem(tr("IEEE 1394 (FireWire DigiCam)"));
    grabberSourceCombo->addItem(tr("gphoto (USB Compact Camera)"));
#endif

#ifdef Q_WS_WIN
    grabberSourceCombo->addItem(tr("Direct Show (USB WebCam)"));
    grabberSourceCombo->addItem(tr("Direct Show (FireWire DigiCam)"));
#endif

    QGridLayout *grabberPrefsLayout = new QGridLayout;
    grabberPrefsLayout->setColumnStretch(0, 1);
    grabberPrefsLayout->addWidget(grabberSourceLabel, 0, 0, Qt::AlignLeft);
    grabberPrefsLayout->addWidget(grabberSourceCombo, 0, 1, Qt::AlignRight);
    grabberPrefs->setLayout(grabberPrefsLayout);

    // Image import preferences
    imagePrefs = new QGroupBox;
    imagePrefs->setTitle(tr("Image import settings"));

    imageFormatLabel = new QLabel(tr("Image Format:"));
    imageFormatCombo = new QComboBox();
    imageFormatLabel->setBuddy(imageFormatCombo);
    imageFormatCombo->setFocusPolicy(Qt::NoFocus);
    imageFormatCombo->setMinimumWidth(300);
    imageFormatCombo->setMaximumWidth(300);
    connect(imageFormatCombo, SIGNAL(activated(int)), this, SLOT(changeImageFormat(int)));
    imageFormatCombo->addItem(tr("JPEG"));
    imageFormatCombo->addItem(tr("TIFF"));
    imageFormatCombo->addItem(tr("BMP"));

    imageQualityLabel = new QLabel(tr("Image Quality:"));
    imageQualitySlider = new QSlider();
    imageQualitySlider->setMaximum(0);
    imageQualitySlider->setMaximum(100);
    imageQualitySlider->setOrientation(Qt::Horizontal);
    imageQualitySlider->setMinimumWidth(300);
    imageQualitySlider->setMaximumWidth(300);
    imageQualitySlider->setTickPosition(QSlider::TicksBelow);
    imageQualitySlider->setTickInterval(20);
    imageQualitySlider->setSingleStep(10);
    imageQualitySlider->setPageStep(20);
    connect(imageQualitySlider, SIGNAL(sliderReleased()), this, SLOT(changeImageQuality()));
    qualityMinimumLabel = new QLabel(tr("Min"));
    qualityMaximumLabel = new QLabel(tr("Max"));

    imageSizeLabel = new QLabel(tr("Image Size:"));
    imageSizeCombo = new QComboBox();
    imageSizeLabel->setBuddy(imageSizeCombo);
    imageSizeCombo->setFocusPolicy(Qt::NoFocus);
    imageSizeCombo->setMinimumWidth(300);
    imageSizeCombo->setMaximumWidth(300);
    connect(imageSizeCombo, SIGNAL(activated(int)), this, SLOT(changeImageSize(int)));
    imageSizeCombo->addItem(tr("Default Grabber Size"));
    imageSizeCombo->addItem(tr("QVGA (320x240)"));
    imageSizeCombo->addItem(tr("VGA (640x480)"));
    imageSizeCombo->addItem(tr("SVGA (800x600)"));
    imageSizeCombo->addItem(tr("PAL D (704x576)"));
    imageSizeCombo->addItem(tr("HD Ready (1280x720)"));
    imageSizeCombo->addItem(tr("Full HD (1900x1080)"));

    QHBoxLayout *icLayout = new QHBoxLayout;
    icLayout->addWidget(qualityMinimumLabel, 0, Qt::AlignLeft);
    icLayout->addStretch();
    icLayout->addWidget(qualityMaximumLabel, 0, Qt::AlignRight);

    QGridLayout *imagePrefsLayout = new QGridLayout;
    imagePrefsLayout->setColumnStretch(0, 1);
    imagePrefsLayout->addWidget(imageFormatLabel, 0, 0, Qt::AlignLeft);
    imagePrefsLayout->addWidget(imageFormatCombo, 0, 1, Qt::AlignRight);
    imagePrefsLayout->addWidget(imageQualityLabel, 1, 0, Qt::AlignLeft);
    imagePrefsLayout->addWidget(imageQualitySlider, 1, 1, Qt::AlignRight);
    imagePrefsLayout->addLayout(icLayout, 2, 1, Qt::AlignRight);
    imagePrefsLayout->addWidget(imageSizeLabel, 3, 0, Qt::AlignLeft);
    imagePrefsLayout->addWidget(imageSizeCombo, 3, 1, Qt::AlignRight);
    imagePrefs->setLayout(imagePrefsLayout);

    // Transformation preferences
    transformPrefs = new QGroupBox;
    transformPrefs->setMinimumSize(440, 300);
    transformPrefs->setTitle(tr("Transformation settings"));

    transformText = new QTextEdit;
    transformText->setReadOnly(true);
    if (tabType) {
        transformText->setHtml(
            "<p>" +
            tr("Below you can set which image transformation should be used for "
               "importing images to a new project. If you select "
               "clip a part of the image set also the adjustment for cliping.") +
            "</p>");
    }
    else {
        transformText->setHtml(
            "<p>" +
            tr("Below you can set which image transformation should be used for "
               "importing images to the currently active project. If you select "
               "clip a part of the image set also the adjustment for cliping.") +
            "</p>");
    }

    transformText->setMinimumWidth(440);
    transformText->setMinimumHeight(75);
    transformText->setMaximumHeight(90);
    transformText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    scaleButton = new QRadioButton(tr("Scale the whole image"));
    scaleButton->setChecked(true);
    scaleButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(scaleButton, SIGNAL(clicked()), this, SLOT(setScaleButtonOn()));

    clipButton = new QRadioButton(tr("Clip a part of the image"));
    clipButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    clipButton->setChecked(false);
    connect(clipButton, SIGNAL(clicked()), this, SLOT(setClipButtonOn()));

    adjustmentPrefs = new QGroupBox;
    adjustmentPrefs->setMinimumSize(200, 100);
    // adjustmentPrefs->setStyleSheet("border-width = 2px");
    // adjustmentPrefs->setStyleSheet("border-color = black");
    // adjustmentPrefs->setStyleSheet("border = 2px solid black");
    // adjustmentPrefs->setStyle("plastique");
    // QPalette p = adjustmentPrefs->palette();
    // p.setColor(adjustmentPrefs->backgroundRole(), QColor(0, 255, 255));
    // adjustmentPrefs->setPalette(p);
    // adjustmentPrefs->setTitle(tr("Adjustment settings"));

    leftUpButton = new QRadioButton();
    leftUpButton->setChecked(false);
    leftUpButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    centerUpButton = new QRadioButton();
    centerUpButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    centerUpButton->setChecked(false);

    rightUpButton = new QRadioButton();
    rightUpButton->setChecked(false);
    rightUpButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    leftMiddleButton = new QRadioButton();
    leftMiddleButton->setChecked(false);
    leftMiddleButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    centerMiddleButton = new QRadioButton();
    centerMiddleButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    centerMiddleButton->setChecked(false);

    rightMiddleButton = new QRadioButton();
    rightMiddleButton->setChecked(false);
    rightMiddleButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    leftDownButton = new QRadioButton();
    leftDownButton->setChecked(false);
    leftDownButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    centerDownButton = new QRadioButton();
    centerDownButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    centerDownButton->setChecked(false);

    rightDownButton = new QRadioButton();
    rightDownButton->setChecked(false);
    rightDownButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QVBoxLayout *adjustmentPrefsLayout = new QVBoxLayout;
    QHBoxLayout *hbLayout = new QHBoxLayout;
    hbLayout->addWidget(leftUpButton);
    hbLayout->addStretch(1);
    hbLayout->addWidget(centerUpButton);
    hbLayout->addStretch(1);
    hbLayout->addWidget(rightUpButton);
    adjustmentPrefsLayout->addLayout(hbLayout);
    adjustmentPrefsLayout->addStretch(1);

    hbLayout = new QHBoxLayout;
    hbLayout->addWidget(leftMiddleButton);
    hbLayout->addStretch(1);
    hbLayout->addWidget(centerMiddleButton);
    hbLayout->addStretch(1);
    hbLayout->addWidget(rightMiddleButton);
    adjustmentPrefsLayout->addLayout(hbLayout);
    adjustmentPrefsLayout->addStretch(1);

    hbLayout = new QHBoxLayout;
    hbLayout->addWidget(leftDownButton);
    hbLayout->addStretch(1);
    hbLayout->addWidget(centerDownButton);
    hbLayout->addStretch(1);
    hbLayout->addWidget(rightDownButton);
    adjustmentPrefsLayout->addLayout(hbLayout);

    adjustmentPrefs->setLayout(adjustmentPrefsLayout);

    // Transform preferences
    QVBoxLayout *transformPrefsLayout = new QVBoxLayout;
    hbLayout = new QHBoxLayout;
    hbLayout->setMargin(0);
    hbLayout->setSpacing(0);
    hbLayout->addStretch(1);

    transformPrefsLayout->addLayout(hbLayout);
    transformPrefsLayout->addWidget(transformText);
    hbLayout = new QHBoxLayout;
    hbLayout->addStretch(1);
    hbLayout->addWidget(scaleButton);
    hbLayout->addStretch(1);
    hbLayout->addWidget(clipButton);
    hbLayout->addStretch(1);
    transformPrefsLayout->addLayout(hbLayout);
    hbLayout = new QHBoxLayout;
    hbLayout->addStretch(1);
    hbLayout->addWidget(adjustmentPrefs);
    hbLayout->addStretch(1);
    transformPrefsLayout->addLayout(hbLayout);
    transformPrefs->setLayout(transformPrefsLayout);

    // Live view preferences
    liveViewPrefs = new QGroupBox;
    liveViewPrefs->setTitle(tr("Live view settings"));

    liveViewFpsLabel = new QLabel(tr("Frames per second:"));
    liveViewFpsSlider = new QSlider();
    liveViewFpsSlider->setMaximum(1);
    liveViewFpsSlider->setMaximum(50);
    liveViewFpsSlider->setOrientation(Qt::Horizontal);
    liveViewFpsSlider->setMinimumWidth(300);
    liveViewFpsSlider->setMaximumWidth(300);
    liveViewFpsSlider->setTickPosition(QSlider::TicksBelow);
    liveViewFpsSlider->setTickInterval(2);
    liveViewFpsSlider->setSingleStep(5);
    liveViewFpsSlider->setPageStep(10);
    connect(liveViewFpsSlider, SIGNAL(sliderReleased()), this, SLOT(changeLiveViewFps()));
    fpsMinimumLabel = new QLabel(tr("0.1"));
    fpsMaximumLabel = new QLabel(tr("5.0"));

    QHBoxLayout *fpsLayout = new QHBoxLayout;
    fpsLayout->addWidget(fpsMinimumLabel, 0, Qt::AlignLeft);
    fpsLayout->addStretch();
    fpsLayout->addWidget(fpsMaximumLabel, 0, Qt::AlignRight);

    QGridLayout *liveViewPrefsLayout = new QGridLayout;
    liveViewPrefsLayout->setColumnStretch(0, 1);
    liveViewPrefsLayout->addWidget(liveViewFpsLabel, 1, 0, Qt::AlignLeft);
    liveViewPrefsLayout->addWidget(liveViewFpsSlider, 1, 1, Qt::AlignRight);
    liveViewPrefsLayout->addLayout(fpsLayout, 2, 1, Qt::AlignRight);
    liveViewPrefs->setLayout(liveViewPrefsLayout);

    // Widget layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(infoText);
    mainLayout->addWidget(grabberPrefs);
    mainLayout->addWidget(imagePrefs);
    mainLayout->addWidget(transformPrefs);
    mainLayout->addWidget(liveViewPrefs);
    setLayout(mainLayout);
    // setMinimumHeight(500);

    qDebug("ImportWidget::makeGUI --> End");
}


void ImportWidget::initialize()
{
    qDebug("ImportWidget::initialize --> Start");

    PreferencesTool *pref = frontend->getPreferences();
    int              value;

    // Read eEncoder preferences
    if (tabType) {
        // This is a general dialog tab
        if (pref->getIntegerPreference("preferences", "defaultgrabbersource", value) == false) {
            value = ImageGrabberDevice::testSource;
        }
        activeGrabberSource = value;

        if (pref->getIntegerPreference("preferences", "defaultimageformat", value) == false) {
            value = ImageGrabber::jpegFormat;
        }
        activeImageFormat = value;

        if (pref->getIntegerPreference("preferences", "defaultimagequality", value) == false) {
            value = 100;
        }
        activeImageQuality = value;

        if (pref->getIntegerPreference("preferences", "defaultimagesize", value) == false) {
            value = ImageGrabber::defaultSize;
        }
        activeImageSize = value;

        if (pref->getIntegerPreference("preferences", "defaulttransformation", value) == false) {
            value = false;
        }
        activeTransform = value;

        if (pref->getIntegerPreference("preferences", "defaultimageadjustment", value) == false) {
            value = ImageGrabber::centerDown;
        }
        activeImageAdjustment = value;

        if (pref->getIntegerPreference("preferences", "defaultliveviewfps", value) == false) {
            value = 20;
        }
        activeLiveViewFps = value;
    }
    else {
        // This is a project dialog tab
        activeGrabberSource = frontend->getProject()->getGrabberSource();
        activeImageFormat = frontend->getProject()->getImageFormat();
        activeImageQuality = frontend->getProject()->getImageQuality();
        activeImageSize = frontend->getProject()->getImageSize();
        activeTransform = frontend->getProject()->getImageTransformation();
        activeImageAdjustment = frontend->getProject()->getImageAdjustment();
        activeLiveViewFps = frontend->getProject()->getLiveViewFps();
    }

    setImageGrabberSource(activeGrabberSource);

    imageFormatCombo->setCurrentIndex(activeImageFormat);
    enableQuality();

    imageQualitySlider->setValue(activeImageQuality);

    imageSizeCombo->setCurrentIndex(activeImageSize);

    // Transformation preferences
    if (activeTransform) {
        setScaleButtonOn();
    }
    else {
        setClipButtonOn();
    }

    setAdjustment(activeImageAdjustment);

    liveViewFpsSlider->setValue(activeLiveViewFps);

    qDebug("ImportWidget::initialize --> End");
}

/*
void ImportWidget::resizeEvent(QResizeEvent *event)
{
    qDebug("ImportWidget::resizeEvent --> Start");

    QWidget::resizeEvent(event);

    qDebug("ImportWidget::resizeEvent --> End");
}
*/

void ImportWidget::apply()
{
    qDebug("ImportWidget::apply --> Start");

    PreferencesTool *pref = frontend->getPreferences();
    int index;
    int value;
    bool changings = false;

    switch (grabberSourceCombo->currentIndex()) {
    case 0:
        index = ImageGrabberDevice::testSource;
        break;
    case 1:
#ifdef Q_WS_X11
        index = ImageGrabberDevice::video4LinuxSource;
#endif
#ifdef Q_WS_WIN
        index = ImageGrabberDevice::directShowUsbSource;
#endif
        break;
    case 2:
#ifdef Q_WS_X11
        index = ImageGrabberDevice::ieee1394Source;
#endif
#ifdef Q_WS_WIN
        index = ImageGrabberDevice::directShow1394Source;
#endif
        break;
    case 3:
#ifdef Q_WS_X11
        index = ImageGrabberDevice::gphoto2Source;
#endif
        break;
    }
    if (activeGrabberSource != index) {
        activeGrabberSource = index;
        changings = true;
    }

    index = imageFormatCombo->currentIndex();
    if (activeImageFormat != index) {
        activeImageFormat = index;
        changings = true;
    }

    value = imageQualitySlider->value();
    if (activeImageQuality != value) {
        activeImageQuality = value;
        changings = true;
    }

    index = imageSizeCombo->currentIndex();
    if (activeImageSize != index) {
        activeImageSize = index;
        changings = true;
    }

    if (clipButton->isChecked()) {
        if (activeTransform) {
            activeTransform = false;
            changings = true;
        }
    }
    else {
        if (!activeTransform) {
            activeTransform = true;
            changings = true;
        }
    }

    if (leftUpButton->isChecked()) {
        index = ImageGrabber::leftUp;
    }
    if (centerUpButton->isChecked()) {
        index = ImageGrabber::centerUp;
    }
    if (rightUpButton->isChecked()) {
        index = ImageGrabber::rightUp;
    }
    if (leftMiddleButton->isChecked()) {
        index = ImageGrabber::leftMiddle;
    }
    if (centerMiddleButton->isChecked()) {
        index = ImageGrabber::centerMiddle;
    }
    if (rightMiddleButton->isChecked()) {
        index = ImageGrabber::rightMiddle;
    }
    if (leftDownButton->isChecked()) {
        index = ImageGrabber::leftDown;
    }
    if (centerDownButton->isChecked()) {
        index = ImageGrabber::centerDown;
    }
    if (rightDownButton->isChecked()) {
        index = ImageGrabber::rightDown;
    }
    if (activeImageAdjustment != index)
    {
        activeImageAdjustment = index;
        changings = true;
    }

    value = liveViewFpsSlider->value();
    if (activeLiveViewFps != value) {
        activeLiveViewFps = value;
        changings = true;
    }

    if (changings) {
        if (tabType) {
            // This is a general dialog tab
            pref->setIntegerPreference("preferences", "defaultgrabbersource", activeGrabberSource);
            pref->setIntegerPreference("preferences", "defaultimageformat", activeImageFormat);
            pref->setIntegerPreference("preferences", "defaultimagequality", activeImageQuality);
            pref->setIntegerPreference("preferences", "defaultimagesize", activeImageSize);
            pref->setIntegerPreference("preferences", "defaulttransformation", activeTransform);
            pref->setIntegerPreference("preferences", "defaultimageadjustment", activeImageAdjustment);
            pref->setIntegerPreference("preferences", "defaultliveviewfps", activeLiveViewFps);
        }
        else {
            // This is a project dialog tab
            frontend->getProject()->setGrabberSource(activeGrabberSource);
            frontend->getProject()->setImageFormat(activeImageFormat);
            frontend->getProject()->setImageQuality(activeImageQuality);
            frontend->getProject()->setImageSize(activeImageSize);
            frontend->getProject()->setImageTransformation(activeTransform);
            frontend->getProject()->setImageAdjustment(activeImageAdjustment);
            frontend->getProject()->setLiveViewFps(activeLiveViewFps);
        }
    }

    qDebug("ImportWidget::apply --> End");
}


void ImportWidget::reset()
{
    qDebug("ImportWidget::reset --> Start");

    setImageGrabberSource(activeGrabberSource);
    imageFormatCombo->setCurrentIndex(activeImageFormat);
    enableQuality();
    imageQualitySlider->setValue(activeImageQuality);
    imageSizeCombo->setCurrentIndex(activeImageSize);
    if (activeTransform)
    {
        setScaleButtonOn();
    }
    else
    {
        setClipButtonOn();
    }
    setAdjustment(activeImageAdjustment);
    liveViewFpsSlider->setValue(activeLiveViewFps);

    qDebug("ImportWidget::reset --> End");
}


void ImportWidget::setImageGrabberSource(int newSource)
{
    qDebug() << "ImportWidget::setImageGrabberSource --> Start";

    switch (newSource) {
    case ImageGrabberDevice::testSource:
        grabberSourceCombo->setCurrentIndex(0);
        break;
    case ImageGrabberDevice::video4LinuxSource:
        grabberSourceCombo->setCurrentIndex(1);
        break;
    case ImageGrabberDevice::ieee1394Source:
        grabberSourceCombo->setCurrentIndex(2);
        break;
    case ImageGrabberDevice::gphoto2Source:
        grabberSourceCombo->setCurrentIndex(3);
        break;
    case ImageGrabberDevice::directShowUsbSource:
        grabberSourceCombo->setCurrentIndex(1);
        break;
    case ImageGrabberDevice::directShow1394Source:
        grabberSourceCombo->setCurrentIndex(2);
        break;
    }

    qDebug() << "ImportWidget::setImageGrabberSource --> End";
}


void ImportWidget::setAdjustment(int newAdjustment)
{
    qDebug() << "ImportWidget::setAdjustment --> Start";

    switch (newAdjustment) {
    case ImageGrabber::leftUp:
        leftUpButton->setChecked(true);
        break;
    case ImageGrabber::centerUp:
        centerUpButton->setChecked(true);
        break;
    case ImageGrabber::rightUp:
        rightUpButton->setChecked(true);
        break;
    case ImageGrabber::leftMiddle:
        leftMiddleButton->setChecked(true);
        break;
    case ImageGrabber::centerMiddle:
        centerMiddleButton->setChecked(true);
        break;
    case ImageGrabber::rightMiddle:
        rightMiddleButton->setChecked(true);
        break;
    case ImageGrabber::leftDown:
        leftDownButton->setChecked(true);
        break;
    case ImageGrabber::centerDown:
        centerDownButton->setChecked(true);
        break;
    case ImageGrabber::rightDown:
        rightDownButton->setChecked(true);
        break;
    }

    qDebug() << "ImportWidget::setAdjustment --> End";
}


void ImportWidget::changeImageGrabberSources(int /*index*/)
{
    // qDebug() << "ImportWidget::changeImageGrabberSources --> Start";

    // qDebug() << "ImportWidget::changeImageGrabberSources --> End";
}


void ImportWidget::changeImageFormat(int index)
{
    qDebug() << "ImportWidget::changeImageFormat --> Start";

    if (!tabType) {
        // Project settings are changed

        // Convert the existing images

        if (frontend->convertImages(index, activeImageQuality)) {
            // Images converted
            activeImageFormat = index;
        }
        else {
            // Images not converted, user break
            imageFormatCombo->setCurrentIndex(activeImageFormat);
        }
    }

    enableQuality();

    qDebug() << "ImportWidget::changeImageFormat --> End";
}


void ImportWidget::changeImageQuality()
{
    qDebug() << "ImportWidget::changeImageQuality --> Start";

    int value = imageQualitySlider->value();

    if (activeImageQuality == value) {
        return;
    }

    if (!tabType) {
        // Project settings are changed

        // Convert the existing images

        if (frontend->convertImages(activeImageFormat, value)) {
            // Images converted
            activeImageQuality = value;
        }
        else {
            // Images not converted, user break
            imageQualitySlider->setValue(activeImageQuality);
        }
    }

    qDebug() << "ImportWidget::changeImageQuality --> End";
}


void ImportWidget::changeImageSize(int /*index*/)
{
    // qDebug() << "ImportWidget::changeImageSize --> Start";

    // qDebug() << "ImportWidget::changeImageSize --> End";
}


void ImportWidget::setScaleButtonOn()
{
    scaleButton->setChecked(true);
    clipButton->setChecked(false);

    adjustmentPrefs->setEnabled(false);

    leftUpButton->setEnabled(false);
    centerUpButton->setEnabled(false);
    rightUpButton->setEnabled(false);
    leftMiddleButton->setEnabled(false);
    centerMiddleButton->setEnabled(false);
    rightMiddleButton->setEnabled(false);
    leftDownButton->setEnabled(false);
    centerDownButton->setEnabled(false);
    rightDownButton->setEnabled(false);
}


void ImportWidget::setClipButtonOn()
{
    clipButton->setChecked(true);
    scaleButton->setChecked(false);

    adjustmentPrefs->setEnabled(true);

    leftUpButton->setEnabled(true);
    centerUpButton->setEnabled(true);
    rightUpButton->setEnabled(true);
    leftMiddleButton->setEnabled(true);
    centerMiddleButton->setEnabled(true);
    rightMiddleButton->setEnabled(true);
    leftDownButton->setEnabled(true);
    centerDownButton->setEnabled(true);
    rightDownButton->setEnabled(true);
}


void ImportWidget::enableQuality()
{
    if (imageFormatCombo->currentIndex() == ImageGrabber::jpegFormat) {
        imageQualityLabel->setEnabled(true);
        imageQualitySlider->setEnabled(true);
        qualityMinimumLabel->setEnabled(true);
        qualityMaximumLabel->setEnabled(true);
    }
    else {
        imageQualityLabel->setEnabled(false);
        imageQualitySlider->setEnabled(false);
        qualityMinimumLabel->setEnabled(false);
        qualityMaximumLabel->setEnabled(false);
    }
}


void ImportWidget::changeLiveViewFps()
{
    qDebug() << "ImportWidget::changeLiveViewFps --> Start";

    int value = liveViewFpsSlider->value();

    if (activeLiveViewFps == value) {
        return;
    }

    /*
    if (!tabType) {
        // Project settings are changed

    }
    */

    qDebug() << "ImportWidget::changeLiveViewFps --> End";
}