import QtQuick 2.7
import QtCharts 2.0

Item {

    ChartView {
        width: parent.width
        height: parent.height
        theme: ChartView.ChartThemeDark
        antialiasing: true

        SplineSeries {
            id: seq1
            name: "Sequence 1"
//            XYPoint { x: 0; y: 0.0 }
//            XYPoint { x: 1.1; y: 3.2 }
//            XYPoint { x: 1.9; y: 2.4 }
//            XYPoint { x: 2.1; y: 2.1 }
//            XYPoint { x: 2.9; y: 2.6 }
//            XYPoint { x: 3.4; y: 2.3 }
//            XYPoint { x: 4.1; y: 3.1 }
        }
        SplineSeries {
            id: seq2
            name: "Sequence 2"
//            XYPoint { x: 0; y: 0 }
//            XYPoint { x: 1; y: 3 }
//            XYPoint { x: 1; y: 2 }
//            XYPoint { x: 2; y: 2 }
//            XYPoint { x: 2; y: 2 }
//            XYPoint { x: 3; y: 2 }
//            XYPoint { x: 4; y: 3 }
        }
        SplineSeries {
            id: seq3
            name: "Convolution"
//            XYPoint { x: 0; y: 0 }
//            XYPoint { x: 1; y: 2 }
//            XYPoint { x: 9; y: 4 }
//            XYPoint { x: 1; y: 1 }
//            XYPoint { x: 9; y: 6 }
//            XYPoint { x: 4; y: 3 }
//            XYPoint { x: 1; y: 1 }
        }
    }

}
