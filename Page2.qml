import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2

Item {

    RowLayout {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 20
        anchors.top: parent.top

        ColumnLayout {

            TextField {
                id: textField1
                placeholderText: qsTr("Sequence №1")
                Layout.fillHeight: true
                Layout.preferredWidth: parent.width * 3
                validator: RegExpValidator { regExp: /(\d+(\,\d+)?)+ ((\d+(\,\d+)?)+ )*(\d)+/ }
            }

            TextField {
                id: textField2
                placeholderText: qsTr("Sequence №2")
                Layout.fillHeight: true
                Layout.fillWidth: true
                validator: RegExpValidator { regExp: /(\d+(\,\d+)?)+ ((\d+(\,\d+)?)+ )*(\d)+/ }
            }
        }

        Button {
            id: button1
            text: qsTr("Process")
            Material.foreground: Material.color(Material.Blue, Material.Shade100)
            onClicked: {
                console.log("Button Pressed on second form. Entered sequences:\n" + textField1.text + "\n" + textField2.text);
                qmlComputeOverlapAdd(textField1.text, textField2.text);
            }
        }
    }
}
