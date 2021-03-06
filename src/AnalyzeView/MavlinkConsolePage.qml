/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick          2.3
import QtQuick.Controls 1.2
import QtQuick.Dialogs  1.2
import QtQuick.Layouts      1.2

import QGroundControl               1.0
import QGroundControl.Palette       1.0
import QGroundControl.FactSystem    1.0
import QGroundControl.FactControls  1.0
import QGroundControl.Controls      1.0
import QGroundControl.ScreenTools   1.0
import QGroundControl.Controllers   1.0

AnalyzePage {
    id:              mavlinkConsolePage
    pageComponent:   pageComponent
    pageName:        qsTr("Mavlink Console")
    pageDescription: qsTr("Mavlink Console provides a connection to the vehicle's system shell.")

    property bool loaded: false

    Component {
        id: pageComponent

        ColumnLayout {
            id:     consoleColumn
            height: availableHeight
            width:  availableWidth

            Connections {
                target: conController

                onDataChanged: {
                    // Keep the view in sync if the button is checked
                    if (loaded) {
                        if (followTail.checked) {
                            listview.positionViewAtEnd();
                        }
                    }
                }
            }

            Component {
                id: delegateItem
                Rectangle {
                    color:  qgcPal.windowShade
                    height: Math.round(ScreenTools.defaultFontPixelHeight * 0.5 + field.height)
                    width:  listview.width

                    QGCLabel {
                        id:          field
                        text:        display
                        width:       parent.width
                        wrapMode:    Text.NoWrap
                        font.family: ScreenTools.fixedFontFamily
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }

            QGCListView {
                Component.onCompleted: {
                    loaded = true
                }
                Layout.fillHeight: true
                anchors.left:      parent.left
                anchors.right:     parent.right
                clip:              true
                id:                listview
                model:             conController
                delegate:          delegateItem

                // Unsync the view if the user interacts
                onMovementStarted: {
                    followTail.checked = false
                }
            }

            RowLayout {
                anchors.left:  parent.left
                anchors.right: parent.right
                QGCTextField {
                    id:               command
                    Layout.fillWidth: true
                    placeholderText:  "Enter Commands here..."
                    onAccepted: {
                        conController.sendCommand(text)
                        text = ""
                    }
                }

                QGCButton {
                    id:        followTail
                    text:      qsTr("Show Latest")
                    checkable: true
                    checked:   true

                    onCheckedChanged: {
                        if (checked && loaded) {
                            listview.positionViewAtEnd();
                        }
                    }
                }
            }
        }
    } // Component
} // AnalyzePage
