/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "QGCApplication.h"
#include "QGCCorePlugin.h"
#include "QGCOptions.h"
#include "QmlPageInfo.h"
#include "FactMetaData.h"
#include "SettingsManager.h"
#include "AppMessages.h"

#include <QtQml>
#include <QQmlEngine>

/// @file
///     @brief Core Plugin Interface for QGroundControl - Default Implementation
///     @author Gus Grubba <mavlink@grubba.com>

class QGCCorePlugin_p
{
public:
    QGCCorePlugin_p()
        : pGeneral                  (NULL)
        , pCommLinks                (NULL)
        , pOfflineMaps              (NULL)
        , pMAVLink                  (NULL)
        , pConsole                  (NULL)
    #if defined(QT_DEBUG)
        , pMockLink                 (NULL)
        , pDebug                    (NULL)
    #endif
        , defaultOptions            (NULL)
        , valuesPageWidgetInfo      (NULL)
        , cameraPageWidgetInfo      (NULL)
        , healthPageWidgetInfo      (NULL)
        , vibrationPageWidgetInfo   (NULL)
    {
    }

    ~QGCCorePlugin_p()
    {
        if(pGeneral)
            delete pGeneral;
        if(pCommLinks)
            delete pCommLinks;
        if(pOfflineMaps)
            delete pOfflineMaps;
        if(pMAVLink)
            delete pMAVLink;
        if(pConsole)
            delete pConsole;
#if defined(QT_DEBUG)
        if(pMockLink)
            delete pMockLink;
        if(pDebug)
            delete pDebug;
#endif
        if(defaultOptions)
            delete defaultOptions;
    }

    QmlPageInfo* pGeneral;
    QmlPageInfo* pCommLinks;
    QmlPageInfo* pOfflineMaps;
    QmlPageInfo* pMAVLink;
    QmlPageInfo* pConsole;
#if defined(QT_DEBUG)
    QmlPageInfo* pMockLink;
    QmlPageInfo* pDebug;
#endif
    QVariantList settingsList;
    QGCOptions*  defaultOptions;

    QmlPageInfo*    valuesPageWidgetInfo;
    QmlPageInfo*    cameraPageWidgetInfo;
    QmlPageInfo*    healthPageWidgetInfo;
    QmlPageInfo*    vibrationPageWidgetInfo;
    QVariantList    instrumentPageWidgetList;
};

QGCCorePlugin::~QGCCorePlugin()
{
    if(_p) {
        delete _p;
    }
}

QGCCorePlugin::QGCCorePlugin(QGCApplication *app, QGCToolbox* toolbox)
    : QGCTool(app, toolbox)
    , _showTouchAreas(false)
    , _showAdvancedUI(true)
{
    _p = new QGCCorePlugin_p;
}

void QGCCorePlugin::setToolbox(QGCToolbox *toolbox)
{
    QGCTool::setToolbox(toolbox);
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    qmlRegisterUncreatableType<QGCCorePlugin>("QGroundControl.QGCCorePlugin", 1, 0, "QGCCorePlugin", "Reference only");
    qmlRegisterUncreatableType<QGCOptions>("QGroundControl.QGCOptions",       1, 0, "QGCOptions",    "Reference only");
}

QVariantList &QGCCorePlugin::settingsPages()
{
    if(!_p->pGeneral) {
        _p->pGeneral = new QmlPageInfo(tr("General"),
                                       QUrl::fromUserInput("qrc:/qml/GeneralSettings.qml"),
                                       QUrl::fromUserInput("qrc:/res/gear-white.svg"));
        _p->settingsList.append(QVariant::fromValue((QmlPageInfo*)_p->pGeneral));
        _p->pCommLinks = new QmlPageInfo(tr("Comm Links"),
                                         QUrl::fromUserInput("qrc:/qml/LinkSettings.qml"),
                                         QUrl::fromUserInput("qrc:/res/waves.svg"));
        _p->settingsList.append(QVariant::fromValue((QmlPageInfo*)_p->pCommLinks));
        _p->pOfflineMaps = new QmlPageInfo(tr("Offline Maps"),
                                           QUrl::fromUserInput("qrc:/qml/OfflineMap.qml"),
                                           QUrl::fromUserInput("qrc:/res/waves.svg"));
        _p->settingsList.append(QVariant::fromValue((QmlPageInfo*)_p->pOfflineMaps));
        _p->pMAVLink = new QmlPageInfo(tr("MAVLink"),
                                       QUrl::fromUserInput("qrc:/qml/MavlinkSettings.qml"),
                                       QUrl::fromUserInput("qrc:/res/waves.svg"));
        _p->settingsList.append(QVariant::fromValue((QmlPageInfo*)_p->pMAVLink));
        _p->pConsole = new QmlPageInfo(tr("Console"),
                                       QUrl::fromUserInput("qrc:/qml/QGroundControl/Controls/AppMessages.qml"));
        _p->settingsList.append(QVariant::fromValue((QmlPageInfo*)_p->pConsole));
#if defined(QT_DEBUG)
        //-- These are always present on Debug builds
        _p->pMockLink = new QmlPageInfo(tr("Mock Link"),
                                        QUrl::fromUserInput("qrc:/qml/MockLink.qml"));
        _p->settingsList.append(QVariant::fromValue((QmlPageInfo*)_p->pMockLink));
        _p->pDebug = new QmlPageInfo(tr("Debug"),
                                     QUrl::fromUserInput("qrc:/qml/DebugWindow.qml"));
        _p->settingsList.append(QVariant::fromValue((QmlPageInfo*)_p->pDebug));
#endif
    }
    return _p->settingsList;
}

QVariantList& QGCCorePlugin::instrumentPages(void)
{
    if (!_p->valuesPageWidgetInfo) {
        _p->valuesPageWidgetInfo = new QmlPageInfo(tr("Values"), QUrl::fromUserInput("qrc:/qml/ValuePageWidget.qml"));
        _p->cameraPageWidgetInfo = new QmlPageInfo(tr("Camera"), QUrl::fromUserInput("qrc:/qml/CameraPageWidget.qml"));
        _p->healthPageWidgetInfo = new QmlPageInfo(tr("Health"), QUrl::fromUserInput("qrc:/qml/HealthPageWidget.qml"));
        _p->vibrationPageWidgetInfo = new QmlPageInfo(tr("Vibration"), QUrl::fromUserInput("qrc:/qml/VibrationPageWidget.qml"));

        _p->instrumentPageWidgetList.append(QVariant::fromValue(_p->valuesPageWidgetInfo));
        _p->instrumentPageWidgetList.append(QVariant::fromValue(_p->cameraPageWidgetInfo));
        _p->instrumentPageWidgetList.append(QVariant::fromValue(_p->healthPageWidgetInfo));
        _p->instrumentPageWidgetList.append(QVariant::fromValue(_p->vibrationPageWidgetInfo));
    }
    return _p->instrumentPageWidgetList;
}

int QGCCorePlugin::defaultSettings()
{
    return 0;
}

QGCOptions* QGCCorePlugin::options()
{
    if(!_p->defaultOptions) {
        _p->defaultOptions = new QGCOptions();
    }
    return _p->defaultOptions;
}

bool QGCCorePlugin::overrideSettingsGroupVisibility(QString name)
{
    Q_UNUSED(name);

    // Always show all
    return true;
}

bool QGCCorePlugin::adjustSettingMetaData(FactMetaData& metaData)
{
    //-- Default Palette
    if (metaData.name() == AppSettings::indoorPaletteName) {
        QVariant outdoorPalette;
#if defined (__mobile__)
        outdoorPalette = 0;
#else
        outdoorPalette = 1;
#endif
        metaData.setRawDefaultValue(outdoorPalette);
        return true;
    //-- Auto Save Telemetry Logs
    } else if (metaData.name() == AppSettings::telemetrySaveName) {
#if defined (__mobile__)
        metaData.setRawDefaultValue(false);
        return true;
#else
        metaData.setRawDefaultValue(true);
        return true;
#endif
#if defined(__ios__)
    } else if (metaData.name() == AppSettings::savePathName) {
        QString appName = qgcApp()->applicationName();
        QDir rootDir = QDir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
        metaData.setRawDefaultValue(rootDir.filePath(appName));
        return false;
#endif
    }
    return true; // Show setting in ui
}

void QGCCorePlugin::setShowTouchAreas(bool show)
{
    if (show != _showTouchAreas) {
        _showTouchAreas = show;
        emit showTouchAreasChanged(show);
    }
}

void QGCCorePlugin::setShowAdvancedUI(bool show)
{
    if (show != _showAdvancedUI) {
        _showAdvancedUI = show;
        emit showAdvancedUIChanged(show);
    }
}

void QGCCorePlugin::paletteOverride(QString colorName, QGCPalette::PaletteColorInfo_t& colorInfo)
{
    Q_UNUSED(colorName);
    Q_UNUSED(colorInfo);
}

QString QGCCorePlugin::showAdvancedUIMessage(void) const
{
    return tr("WARNING: You are about to enter Advanced Mode. "
              "If used incorrectly, this may cause your vehicle to malfunction thus voiding your warranty. "
              "You should do so only if instructed by customer support. "
              "Are you sure you want to enable Advanced Mode?");
}

void QGCCorePlugin::valuesWidgetDefaultSettings(QStringList& largeValues, QStringList& smallValues)
{
    Q_UNUSED(smallValues);
    largeValues << "Vehicle.altitudeRelative" << "Vehicle.groundSpeed" << "Vehicle.flightTime";
}

QQmlApplicationEngine* QGCCorePlugin::createRootWindow(QObject *parent)
{
    QQmlApplicationEngine* pEngine = new QQmlApplicationEngine(parent);
    pEngine->addImportPath("qrc:/qml");
    pEngine->rootContext()->setContextProperty("joystickManager", qgcApp()->toolbox()->joystickManager());
    pEngine->rootContext()->setContextProperty("debugMessageModel", AppMessages::getModel());
    pEngine->load(QUrl(QStringLiteral("qrc:/qml/MainWindowNative.qml")));
    return pEngine;
}

bool QGCCorePlugin::mavlinkMessage(Vehicle* vehicle, LinkInterface* link, mavlink_message_t message)
{
    Q_UNUSED(vehicle);
    Q_UNUSED(link);
    Q_UNUSED(message);

    return true;
}
