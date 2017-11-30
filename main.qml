import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

ApplicationWindow {
    visible: true
    width: 640
    height: 480

    signal qmlComputeApriory( string array1, string array2 );
    signal qmlComputeOverlapAdd( string array1, string array2 );
    signal qmlComputeOverlapSave( string array1, string array2 );

    header: Label {
            text: qsTr("CONVOLUTION")
            font.pixelSize: 22
            font.bold:  true
            horizontalAlignment: Text.AlignHCenter
        }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex
        interactive: false

        Page1 {
        }

        Page2 {
        }

        Page3 {
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex
        TabButton {
            text: qsTr("A-priory")
        }
        TabButton {
            text: qsTr("Overlap-add")
        }
        TabButton {
            text: qsTr("Overlap-save")
        }
    }
}
