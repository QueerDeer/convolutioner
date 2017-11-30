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
                Layout.preferredWidth: parent.width * 1.5
                validator: RegExpValidator {
                    regExp: /(\d+(\,\d+)?)+ ((\d+(\,\d+)?)+ )*(\d)+/
                }
            }

            TextField {
                id: textField2
                placeholderText: qsTr("Sequence №2")
                Layout.fillHeight: true
                Layout.fillWidth: true
                validator: RegExpValidator {
                    regExp: /(\d+(\,\d+)?)+ ((\d+(\,\d+)?)+ )*(\d)+/
                }
            }
        }

        Button {
            id: button1
            text: qsTr("Process")
            Material.foreground: Material.color(Material.Blue, Material.Shade100)
            onClicked: {
                qmlComputeApriory(textField1.text, textField2.text);
            }
        }
    }

    RowLayout {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 120
        anchors.bottom: parent.bottom

        ColumnLayout {

            TextField {
                id: textField3
                placeholderText: qsTr("Convolution sequence")
                Layout.fillHeight: true
                Layout.preferredWidth: parent.width * 1.7
                enabled: true

                Connections {
                    target: convolutioner
                    onInput1Changed: {
                        textField3.text = convolutioner.input1
                    }
                }
            }

            TextField {
                id: textField4
                placeholderText: qsTr("Amount of operations")
                Layout.fillHeight: true
                Layout.fillWidth: true
                enabled: false

                Connections {
                    target: convolutioner
                    onInput2Changed: {
                        textField4.text = convolutioner.input2
                    }
                }
            }
        }
    }
}
