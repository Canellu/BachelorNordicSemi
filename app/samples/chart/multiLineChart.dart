import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:fl_chart/fl_chart.dart';

class MultiLineChartSample extends StatefulWidget {
  @override
  _MultiLineChartState createState() => _MultiLineChartState();
}

class _MultiLineChartState extends State<MultiLineChartSample> {
  List<Color> gradientColors = [
    const Color(0xff23b6e6),
    const Color(0xff02d39a),
  ];

  @override
  Widget build(BuildContext context) {
    return Stack(
      children: <Widget>[
        Container(
          padding: EdgeInsets.all(20.0),
          child: Align(
            alignment: Alignment.topCenter,
            child: AspectRatio(
              aspectRatio: 1.7,
              child: Container(
                decoration: const BoxDecoration(
                  borderRadius: BorderRadius.all(
                    Radius.circular(10),
                  ),
                  color: Color(0xff232d37),
                ),
                child: Padding(
                  padding: EdgeInsets.only(right: 18.0, left: 12.0, top: 24, bottom: 12),
                  child: LineChart(
                    mainData(),
                  ),
                ),
              ),
            ),
          ),
        ),
        Positioned(
          top: 25,
          left: 25,
          child: Text(
            'Multi Line Chart',
            style: TextStyle(fontSize: 15, color: Colors.white, decoration: TextDecoration.none),
          ),
        )
      ],
    );
  }

  LineChartData mainData() {
    return LineChartData(
      gridData: FlGridData(
        show: true,
        drawVerticalLine: true,
        getDrawingHorizontalLine: (value) {
          return FlLine(
            color: const Color(0xff37434d),
            strokeWidth: 1,
          );
        },
        getDrawingVerticalLine: (value) {
          return FlLine(
            color: const Color(0xff37434d),
            strokeWidth: 1,
          );
        },
      ),
      titlesData: FlTitlesData(
        show: true,
        bottomTitles: SideTitles(
          showTitles: true,
          reservedSize: 22,
          getTextStyles: (value) =>
          const TextStyle(color: Color(0xff68737d)),
          getTitles: (value) {
            if(value.toInt()%2 == 0){
              return value.toInt().toString()+'T';
            }
            return '';
          },
          margin: 8,
        ),
        leftTitles: SideTitles(
          showTitles: true,
          getTextStyles: (value) => const TextStyle(
            color: Color(0xff67727d),
            fontSize: 15,
            fontWeight: FontWeight.bold,
          ),
          getTitles: (value) {
            switch (value.toInt()){
              case 1:
                return '10k';
              case 3:
                return '30k';
              case 5:
                return '50k';
            }
            return '';
          },
          reservedSize: 28,
          margin: 12,
        ),
      ),
      borderData:
      FlBorderData(
        show: true,
        border: Border.all(color: const Color(0xff37434d), width: 1),
      ),
      minX: 0,
      maxX: 11,
      minY: 0,
      maxY: 6,
      lineBarsData: linesBarData(),
    );
  }

  List<LineChartBarData> linesBarData() {
    final LineChartBarData lineChartBarData1 = LineChartBarData(
      spots: [
        FlSpot(0, 3),
        FlSpot(2.6, 2),
        FlSpot(4.9, 5),
        FlSpot(6.8, 3.1),
        FlSpot(8, 4),
        FlSpot(9.6, 3),
        FlSpot(11, 4),
      ],
      isCurved: true,
      colors: gradientColors,
      barWidth: 5,
      isStrokeCapRound: true,
      dotData:  FlDotData(
        show: false,
      ),
      belowBarData: BarAreaData(
        show: true,
        colors: gradientColors.map((color) => color.withOpacity(0.3)).toList(),
      ),
    );
    final LineChartBarData lineChartBarData2 = LineChartBarData(
      spots: [
        FlSpot(0, 1),
        FlSpot(2.6, 2.8),
        FlSpot(4.9, 1.2),
        FlSpot(6.8, 2.8),
        FlSpot(8, 2.6),
        FlSpot(9.6, 3.9),
        FlSpot(11, 3),
      ],
      isCurved: true,
      colors: [const Color(0xffaa4cfc)],
      barWidth: 5,
      isStrokeCapRound: true,
      dotData:  FlDotData(
        show: false,
      ),
      belowBarData: BarAreaData(
        show: true,
        colors: gradientColors.map((color) => color.withOpacity(0.3)).toList(),
      ),
    );
    return [lineChartBarData1,lineChartBarData2];
  }
}
