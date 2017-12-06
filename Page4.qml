import QtQuick 2.7
import QtQuick.Controls 2.0
import QtCharts 2.0

Item {

    ChartView {
        id: chart
        width: parent.width
        height: parent.height
        antialiasing: true
        backgroundColor: "#303030"
        legend.labelColor: "#111111" // no, it has :3

        property int maxX: 1
        property int maxY: 1
        property int countSeq1: 0
        property int countSeq2: 0
        property int countSeq3: 0

        ValueAxis {
            id: axisX
            min: -1
            max: 1
            gridLineColor: "black"
            labelsColor: "black"
            color: "black"
        }
        ValueAxis {
            id: axisY
            min: -1
            max: 1
            gridLineColor: "black"
            labelsColor: "black"
            color: "black"
        }

        Connections {
            target: convolutioner
            onPointChanged: {
                chart.maxY = (coordY > chart.maxY)? coordY : chart.maxY
                axisY.max = chart.maxY + 1

                if (seq == "seq1") {
                    seq1.append(chart.countSeq1, coordY)
                    seq11.append(chart.countSeq1, coordY)
                    chart.countSeq1++

                    chart.maxX += (chart.countSeq1 > chart.maxX)? 1 : 0
                    axisX.max = chart.maxX
                }
                else if (seq == "seq2") {
                    seq2.append(chart.countSeq2, coordY)
                    seq21.append(chart.countSeq2, coordY)
                    chart.countSeq2++

                    chart.maxX += (chart.countSeq2 > chart.maxX)? 1 : 0
                    axisX.max = chart.maxX
                }
                else if (seq == "seq3") {
                    seq3.append(chart.countSeq3, coordY)
                    seq31.append(chart.countSeq3, coordY)
                    chart.countSeq3++

                    chart.maxX += (chart.countSeq3 > chart.maxX)? 1 : 0
                    axisX.max = chart.maxX
                }
                else if (mode.position == 0) {
                    seq1.removePoints(0, seq1.count)
                    seq2.removePoints(0, seq2.count)
                    seq3.removePoints(0, seq3.count)
                    seq11.clear()
                    seq21.clear()
                    seq31.clear()

                    chart.maxY = 1
                    axisY.max = chart.maxY

                    chart.maxX = 1
                    axisX.max = chart.maxX

                    chart.countSeq1 = chart.countSeq2 = chart.countSeq3 = 0
                }
                else {
                    chart.countSeq1 = chart.countSeq2 = chart.countSeq3 = 0
                    seq11.clear()
                    seq21.clear()
                    seq31.clear()
                }

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
            markerSize: 7
        }
        ScatterSeries {
            id: seq2
            name: "Sequence 2"
            axisX: axisX
            axisY: axisY
            markerSize: 7
        }
        ScatterSeries {
            id: seq3
            name: "Convolution"
            axisX: axisX
            axisY: axisY
            markerSize: 7
        }
        SplineSeries {
            id: seq11
            name: "Sequence 1"
            axisX: axisX
            axisY: axisY
        }
        SplineSeries {
            id: seq21
            name: "Sequence 2"
            axisX: axisX
            axisY: axisY
        }
        SplineSeries {
            id: seq31
            name: "Convolution"
            axisX: axisX
            axisY: axisY
        }

        Switch {
            id: mode
            text: position ? "Save mode-on" : "Save mode-off"
            anchors.right: parent.right
        }
        Button {
            id: test
            text: "Show statistics"
            anchors.left: parent.left
            anchors.leftMargin: 10
            onClicked: convolutioner.statistic()
        }
        Button {
            id: clear
            text: "Clear"
            anchors.left: test.right
            anchors.leftMargin: 10
            onClicked: convolutioner.cleaning()
        }
    }
}
