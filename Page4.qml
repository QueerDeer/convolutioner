import QtQuick 2.7
import QtQuick.Controls 2.0
import QtCharts 2.0

Item {

    ChartView {
        id: chart
        width: parent.width
        height: parent.height
        theme: ChartView.ChartThemeDark
        antialiasing: true
        backgroundColor: grey

        property int maxX: 1
        property int maxY: 1
        property int countSeq1: 0
        property int countSeq2: 0
        property int countSeq3: 0

        ValueAxis {
                id: axisX
                min: -1
                max: 1
        }
        ValueAxis {
                id: axisY
                min: -1
                max: 1
        }

        Connections {
            target: convolutioner
            onPointChanged: {
                chart.maxY = (coordY > chart.maxY)? coordY : chart.maxY
                axisY.max = chart.maxY + 1

                if (seq == "seq1") {
                    seq1.append(chart.countSeq1, coordY)
                    chart.countSeq1++

                    chart.maxX += (chart.countSeq1 > chart.maxX)? 1 : 0
                    axisX.max = chart.maxX
                }
                else if (seq == "seq2") {
                    seq2.append(chart.countSeq2, coordY)
                    chart.countSeq2++

                    chart.maxX += (chart.countSeq2 > chart.maxX)? 1 : 0
                    axisX.max = chart.maxX
                }
                else if (seq == "seq3") {
                    seq3.append(chart.countSeq3, coordY)
                    chart.countSeq3++

                    chart.maxX += (chart.countSeq3 > chart.maxX)? 1 : 0
                    axisX.max = chart.maxX
                }
                else if (mode.position == 0) {
                    seq1.removePoints(0, seq1.count)
                    seq2.removePoints(0, seq2.count)
                    seq3.removePoints(0, seq3.count)

                    chart.maxY = 1
                    axisY.max = chart.maxY

                    chart.maxX = 1
                    axisX.max = chart.maxX

                    chart.countSeq1 = chart.countSeq2 = chart.countSeq3 = 0
                }
                else
                    chart.countSeq1 = chart.countSeq2 = chart.countSeq3 = 0

                if (convolutioner.factor == "1") {
                    seq3.brushFilename = ":/brush.png"
                    seq3.markerSize = 47
                }
            }
        }

        ScatterSeries {
            id: seq1
            name: "Sequence 1"
            axisX: axisX
            axisY: axisY
            markerSize: 10
        }
        ScatterSeries {
            id: seq2
            name: "Sequence 2"
            axisX: axisX
            axisY: axisY
            markerSize: 10
        }
        ScatterSeries {
            id: seq3
            name: "Convolution"
            axisX: axisX
            axisY: axisY
            markerSize: 10
        }
        Switch {
            id: mode
            text: position ? "Save mode-on" : "Save mode-off"
            anchors.right: parent.right
        }
    }
}