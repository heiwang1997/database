import QtQuick 2.4
import QtQuick.Layouts 1.1
import Material 0.2
import Material.ListItems 0.1 as ListItem
import thjdb.QMLif 1.0
import QtQuick.Controls 1.3 as Controls

Item {
    id: execute
    implicitHeight: column.height

    QMLif {
        id: testSearch
        onResultReady: {
//            command.text = testSearch.gettext()
            resultLable.text = testSearch.gettext()
            cyclicColorProgress.visible = false
            column.enabled = true
            resultLable.visible = true
        }
    }

    ColumnLayout {
        id: column
        anchors.centerIn: parent
        spacing: dp(32)

        TextField {
            id: command
            placeholderText: "Command..."
            anchors.horizontalCenter: parent.horizontalCenter

            font.family: "Roboto"
            font.weight: Font.Light
            font.pixelSize: dp(25)

            onAccepted: {
                testSearch.doSomething(command.text)
                cyclicColorProgress.visible = true
                column.enabled = false
            }
        }

        ProgressCircle {
            id: cyclicColorProgress
            visible: false
            Layout.alignment: Qt.AlignCenter
            anchors.horizontalCenter: parent.horizontalCenter

            SequentialAnimation {
                running: true
                loops: Animation.Infinite

                ColorAnimation {
                    from: "red"
                    to: "blue"
                    target: cyclicColorProgress
                    properties: "color"
                    easing.type: Easing.InOutQuad
                    duration: 2400
                }

                ColorAnimation {
                    from: "blue"
                    to: "green"
                    target: cyclicColorProgress
                    properties: "color"
                    easing.type: Easing.InOutQuad
                    duration: 1560
                }

                ColorAnimation {
                    from: "green"
                    to: "#FFCC00"
                    target: cyclicColorProgress
                    properties: "color"
                    easing.type: Easing.InOutQuad
                    duration:  840
                }

                ColorAnimation {
                    from: "#FFCC00"
                    to: "red"
                    target: cyclicColorProgress
                    properties: "color"
                    easing.type: Easing.InOutQuad
                    duration:  1200
                }
            }
        }

        Text {
            id: resultLable
            textFormat: Text.RichText
            anchors.horizontalCenter: parent.horizontalCenter

            font.family: "Roboto"
            font.weight: Font.Light
            font.pixelSize: dp(20)
        }
    }
}