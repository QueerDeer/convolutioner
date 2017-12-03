import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    visibility: "FullScreen"

    Shortcut {
        sequence: "Escape"
        context: Qt.ApplicationShortcut
        onActivated: Qt.quit()
    }

    signal qmlComputeAprioryLine( string array1, string array2 );
    signal qmlComputeAprioryCircle( string array1, string array2 );
    signal qmlComputeOverlapAddLine( string array1, string array2 );
    signal qmlComputeOverlapAddCircle( string array1, string array2 );
    signal qmlComputeOverlapSaveCircle( string array1, string array2 );
    signal qmlGetFactor( string array1, string array2 )

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

        Page4 {
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
        TabButton {
            text: qsTr("Chart")
        }
    }
}
