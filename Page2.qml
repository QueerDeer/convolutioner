import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2

Item {

    RowLayout {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 120
        anchors.top: parent.top

        ColumnLayout {

            Text {
                id: hiddenText1
                anchors.fill: textField1
                text: textField1.text
                font.pixelSize: textField1.font.pixelSize
                visible: false
            }
            Text {
                id: hiddenText2
                anchors.fill: textField2
                text: textField2.text
                font.pixelSize: textField2.font.pixelSize
                visible: false
            }

            TextField {
                id: textField1
                selectByMouse: true
                placeholderText: qsTr("Sequence №1")
                Layout.fillHeight: true
                Layout.preferredWidth: parent.width * 2.5
                validator: RegExpValidator {
                    regExp: /(\d+(\,\d+)?)+ ((\d+(\,\d+)?)+ )*(\d)+/
                }
                onTextChanged: {
                    while ((hiddenText1.contentWidth > hiddenText1.width) || (hiddenText1.contentHeight > hiddenText1.height)) {
                        font.pixelSize -= 1
                    }
                }
            }

            TextField {
                id: textField2
                selectByMouse: true
                placeholderText: qsTr("Sequence №2")
                Layout.fillHeight: true
                Layout.fillWidth: true
                validator: RegExpValidator {
                    regExp: /(\d+(\,\d+)?)+ ((\d+(\,\d+)?)+ )*(\d)+/
                }
                onTextChanged: {
                    while ((hiddenText2.contentWidth > hiddenText2.width) || (hiddenText2.contentHeight > hiddenText2.height)) {
                        font.pixelSize -= 1
                    }
                }
            }
        }

        ComboBox {
            id: box
            editable: false
            model: ListModel {
                id: model
            }
            onPressedChanged: {
                model.clear()
                qmlWound(textField1.text, textField2.text)
            }

            Connections {
                target: convolutioner
                onFooChanged: {
                    model.append({text: lenSeq})
                }
            }
        }
        Button {
            id: button1
            text: qsTr("Process")
            Material.foreground: Material.color(Material.Blue, Material.Shade100)
            onClicked: {
                convolutioner.foo = box.currentText

                if (subConvolutionMethod.value == 1)
                    qmlComputeOverlapAddLine(textField1.text, textField2.text);
                else if (subConvolutionMethod.value == 2)
                    qmlComputeOverlapAddCircle(textField1.text, textField2.text);
            }
        }
        Slider {
            id: subConvolutionMethod
            from: 1
            value: 1
            to: 2
            stepSize: 1
            snapMode: Slider.SnapOnRelease
            Text {
                text: "line"
                anchors.top: subConvolutionMethod.bottom
                anchors.left: subConvolutionMethod.left
            }
            Text {
                text: "cirlce"
                anchors.top: subConvolutionMethod.bottom
                anchors.right: subConvolutionMethod.right
            }
        }
    }

    RowLayout {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 220
        anchors.bottom: parent.bottom

        ColumnLayout {

            Text {
                id: hiddenText3
                anchors.fill: textField3
                text: textField3.text
                font.pixelSize: textField3.font.pixelSize
                visible: false
            }

            TextField {
                id: textField3
                selectByMouse: true
                placeholderText: qsTr("Convolution sequence")
                Layout.fillHeight: true
                Layout.preferredWidth: parent.width * 2.74
                enabled: true
                onTextChanged: {
                    while ((hiddenText3.contentWidth > hiddenText3.width) || (hiddenText3.contentHeight > hiddenText3.height)) {
                        font.pixelSize -= 1
                    }
                }

                Connections {
                    target: convolutioner
                    onInput1Changed: {
                        if (nFrame == "frame2")
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
                        if (nFrame == "frame2")
                            textField4.text = convolutioner.input2
                    }
                }
            }
        }
    }
}
