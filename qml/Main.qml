import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: window

    visible: true
    width: 1440
    height: 900
    minimumWidth: 1100
    minimumHeight: 700

    title: "WorkSense"
    color: "#101820"

    property string currentPage: "Overview"

        function severityColor(severity)
        {
            switch (severity) {
                case "NORMAL": return "#35C98B"
                case "WARNING": return "#F5B942"
                case "CRITICAL": return "#F05B65"
                case "DISCONNECTED": return "#8995A3"
                default: return "#8995A3"
            }
        }

        component MetricBar: ColumnLayout {
        property string label: ""
            property real value: 0
                property color accent: "#35B9D0"

                    spacing: 7

                    RowLayout {
                        Layout.fillWidth: true

                        Label {
                            text: label
                            color: "#A7B3C1"
                            font.pixelSize: 12
                        }

                        Item { Layout.fillWidth: true }

                        Label {
                            text: Math.round(value) + "%"
                            color: "#E8EEF5"
                            font.pixelSize: 12
                            font.bold: true
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        height: 7
                        radius: 4
                        color: "#303D49"

                        Rectangle {
                            width: parent.width * Math.max(0, Math.min(100, value)) / 100
                            height: parent.height
                            radius: parent.radius
                            color: accent
                        }
                    }
                }

                RowLayout {
                    anchors.fill: parent
                    spacing: 0

                    // SIDEBAR
                    Rectangle {
                        Layout.preferredWidth: 225
                        Layout.fillHeight: true
                        color: "#151F29"

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 20
                            spacing: 25

                            ColumnLayout {
                                spacing: 4

                                Label {
                                    text: "WORKSENSE"
                                    color: "#EAF2F8"
                                    font.pixelSize: 21
                                    font.bold: true
                                    font.letterSpacing: 2
                                }

                                Label {
                                    text: "ENGINEERING MONITOR"
                                    color: "#6F8294"
                                    font.pixelSize: 9
                                    font.letterSpacing: 1.5
                                }
                            }

                            Rectangle {
                                Layout.fillWidth: true
                                height: 1
                                color: "#2B3946"
                            }

                            Label {
                                text: "WORKSPACE"
                                color: "#718294"
                                font.pixelSize: 10
                                font.bold: true
                            }

                            Repeater {
                                model: ["Overview", "Devices", "Alarms", "Settings"]

                                delegate: Rectangle {
                                    required property string modelData

                                    Layout.fillWidth: true
                                    height: 43
                                    radius: 7
                                    color: currentPage === modelData
                                    ? "#243D4D" : "transparent"

                                    RowLayout {
                                        anchors.fill: parent
                                        anchors.leftMargin: 12
                                        spacing: 12

                                        Rectangle {
                                            width: 3
                                            height: 22
                                            radius: 2
                                            color: currentPage === modelData
                                            ? "#35B9D0" : "transparent"
                                        }

                                        Label {
                                            text: modelData
                                            color: currentPage === modelData
                                            ? "#EAF7FC" : "#A0ADBA"
                                            font.pixelSize: 13
                                            Layout.fillWidth: true
                                        }
                                    }

                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: currentPage = modelData
                                        cursorShape: Qt.PointingHandCursor
                                    }
                                }
                            }

                            Item { Layout.fillHeight: true }

                            Rectangle {
                                Layout.fillWidth: true
                                height: 65
                                radius: 8
                                color: "#1C2A35"

                                Column {
                                    anchors.centerIn: parent
                                    spacing: 5

                                    Label {
                                        text: "WORKSENSE"
                                        color: "#DCE7EF"
                                        font.pixelSize: 11
                                        font.bold: true
                                    }

                                    Label {
                                        text: "Prototype 0.1"
                                        color: "#718294"
                                        font.pixelSize: 10
                                    }
                                }
                            }
                        }
                    }

                    // MAIN CONTENT
                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.margins: 27
                        spacing: 22

                        RowLayout {
                            Layout.fillWidth: true

                            ColumnLayout {
                                spacing: 4

                                Label {
                                    text: currentPage
                                    color: "#EDF3F8"
                                    font.pixelSize: 27
                                    font.bold: true
                                }

                                Label {
                                    text: "Engineering systems monitoring"
                                    color: "#8595A5"
                                    font.pixelSize: 12
                                }
                            }

                            Item { Layout.fillWidth: true }

                            Rectangle {
                                radius: 7
                                color: "#1C302D"
                                border.color: "#285344"
                                implicitWidth: 125
                                implicitHeight: 34

                                RowLayout {
                                    anchors.centerIn: parent
                                    spacing: 8

                                    Rectangle {
                                        width: 7
                                        height: 7
                                        radius: 4
                                        color: "#35C98B"
                                    }

                                    Label {
                                        text: "SIMULATION"
                                        color: "#7BE0B1"
                                        font.pixelSize: 10
                                        font.bold: true
                                    }
                                }
                            }
                        }

                        // SUMMARY CARDS
                        GridLayout {
                            Layout.fillWidth: true
                            columns: 3
                            columnSpacing: 15

                            Repeater {
                                model: [
                                {
                                    title: "CONNECTED DEVICES",
                                    value: deviceController.activeCount,
                                    accent: "#35B9D0"
                                },
                            {
                                title: "WARNINGS",
                                value: deviceController.warningCount,
                                accent: "#F5B942"
                            },
                        {
                            title: "CRITICAL / OFFLINE",
                            value: deviceController.criticalCount,
                            accent: "#F05B65"
                        }
                    ]

                    delegate: Rectangle {
                        required property var modelData

                        Layout.fillWidth: true
                        Layout.preferredHeight: 105
                        radius: 9
                        color: "#1A2631"
                        border.color: "#2B3946"

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 17
                            spacing: 8

                            Label {
                                text: modelData.title
                                color: "#8B9BAA"
                                font.pixelSize: 10
                                font.bold: true
                            }

                            Label {
                                text: modelData.value
                                color: modelData.accent
                                font.pixelSize: 27
                                font.bold: true
                            }
                        }
                    }
                }
            }

            // DEVICE SECTION
            RowLayout {
                Layout.fillWidth: true

                Label {
                    text: currentPage === "Devices"
                    ? "Device inventory" : "Device overview"
                    color: "#E5EDF4"
                    font.pixelSize: 17
                    font.bold: true
                }

                Item { Layout.fillWidth: true }

                Label {
                    text: deviceController.devices.length + " DEVICES"
                    color: "#8293A3"
                    font.pixelSize: 10
                }
            }

            GridLayout {
                Layout.fillWidth: true
                columns: 2
                columnSpacing: 15
                rowSpacing: 15

                Repeater {
                    model: deviceController.devices

                    delegate: Rectangle {
                        property var deviceData: deviceController.devices[index]

                        Layout.fillWidth: true
                        Layout.preferredHeight: 190
                        radius: 9
                        color: "#1A2631"
                        border.color: "#2B3946"

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 17
                            spacing: 13

                            RowLayout {
                                Layout.fillWidth: true

                                ColumnLayout {
                                    spacing: 3
                                    Layout.fillWidth: true

                                    Label {
                                        text: deviceData.name
                                        color: "#E8EFF5"
                                        font.pixelSize: 14
                                        font.bold: true
                                    }

                                    Label {
                                        text: deviceData.id
                                        color: "#8091A1"
                                        font.pixelSize: 10
                                    }
                                }

                                Rectangle {
                                    radius: 5
                                    color: window.severityColor(deviceData.severity)
                                    opacity: 0.15
                                    implicitWidth: statusLabel.implicitWidth + 16
                                    implicitHeight: 25

                                    Label {
                                        id: statusLabel
                                        anchors.centerIn: parent
                                        text: deviceData.severity
                                        color: window.severityColor(deviceData.severity)
                                        font.pixelSize: 9
                                        font.bold: true
                                    }
                                }
                            }

                            MetricBar {
                                Layout.fillWidth: true
                                label: "CPU USAGE"
                                value: deviceData.cpu
                                accent: deviceData.cpu >= 90
                                ? "#F05B65"
                                : deviceData.cpu >= 75
                                ? "#F5B942" : "#35B9D0"
                            }

                            MetricBar {
                                Layout.fillWidth: true
                                label: "MEMORY USAGE"
                                value: deviceData.memory
                                accent: deviceData.memory >= 95
                                ? "#F05B65"
                                : deviceData.memory >= 80
                                ? "#F5B942" : "#35B9D0"
                            }
                        }
                    }
                }
            }

            // ALARMS
            Label {
                text: "Recent alarms"
                color: "#E5EDF4"
                font.pixelSize: 17
                font.bold: true
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                radius: 9
                color: "#1A2631"
                border.color: "#2B3946"
                clip: true

                ListView {
                    anchors.fill: parent
                    anchors.margins: 12
                    model: deviceController.alarms
                    spacing: 7
                    clip: true

                    delegate: RowLayout {
                        required property var modelData

                        width: ListView.view.width
                        height: 30
                        spacing: 12

                        Rectangle {
                            width: 6
                            height: 6
                            radius: 3
                            color: window.severityColor(modelData.severity)
                        }

                        Label {
                            text: modelData.timestamp
                            color: "#8091A1"
                            font.pixelSize: 10
                        }

                        Label {
                            text: modelData.severity
                            color: window.severityColor(modelData.severity)
                            font.pixelSize: 10
                            font.bold: true
                        }

                        Label {
                            text: modelData.deviceId + " — " + modelData.message
                            color: "#C4D0DA"
                            font.pixelSize: 11
                            Layout.fillWidth: true
                            elide: Text.ElideRight
                        }
                    }

                    Label {
                        anchors.centerIn: parent
                        visible: deviceController.alarms.length === 0
                        text: "No recent alarms"
                        color: "#718294"
                    }
                }
            }
        }
    }
}