import 'package:fl_chart/fl_chart.dart';
import 'package:flutter/material.dart';

class BarChartSample extends StatefulWidget {
  @override
  _BarChartSampleState createState() => _BarChartSampleState();
}

class _BarChartSampleState extends State<BarChartSample> {
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
              child: Card(
                elevation: 0,
                shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(10)),
                color: const Color(0xff232d37),
                child: BarChart(
                  BarChartData(
                    alignment: BarChartAlignment.spaceAround,
                    maxY: 150,
                    barTouchData: BarTouchData(
                      enabled: false,
                      touchTooltipData: BarTouchTooltipData(
                        tooltipBgColor: Colors.transparent,
                        tooltipPadding: const EdgeInsets.all(0),
                        tooltipBottomMargin: 5,
                        getTooltipItem: (
                            BarChartGroupData group,
                            int groupIndex,
                            BarChartRodData rod,
                            int rodIndex,
                            ) {
                          return BarTooltipItem(
                            rod.y.round().toString(),
                            TextStyle(
                              color: Colors.white,
                              fontWeight: FontWeight.bold,
                            ),
                          );
                        },
                      ),
                    ),
                    titlesData: FlTitlesData(
                      show: true,
                      bottomTitles: SideTitles(
                        showTitles: true,
                        getTextStyles: (value) => const TextStyle(
                            color: Color(0xff7589a2),
                            fontWeight: FontWeight.bold,
                            fontSize: 14
                        ),
                        margin: 20,
                        getTitles: (value) {
                          if(value.toInt()%1 == 0){
                            return value.toInt().toString()+'T';
                          }
                          return '';
                        },
                      ),
                      leftTitles: SideTitles(showTitles: false),
                    ),
                    borderData: FlBorderData(
                      show: false,
                    ),
                    barGroups: [
                      BarChartGroupData(
                        x: 0,
                        barRods: [
                          BarChartRodData(
                              y: 100,
                              colors: [Colors.lightBlueAccent, Colors.greenAccent]
                          )
                        ],
                        showingTooltipIndicators: [0],
                      ),
                      BarChartGroupData(
                        x: 1,
                        barRods:  [
                          BarChartRodData(
                              y: 98,
                              colors: [Colors.lightBlueAccent, Colors.greenAccent]
                          )
                        ],
                        showingTooltipIndicators: [0],
                      ),
                      BarChartGroupData(
                        x: 2,
                        barRods:  [
                          BarChartRodData(
                              y: 80,
                              colors: [Colors.lightBlueAccent, Colors.greenAccent]
                          )
                        ],
                        showingTooltipIndicators: [0],
                      ),
                      BarChartGroupData(
                        x: 3,
                        barRods:  [
                          BarChartRodData(
                              y: 65,
                              colors: [Colors.lightBlueAccent, Colors.greenAccent]
                          )
                        ],
                        showingTooltipIndicators: [0],
                      ),
                      BarChartGroupData(
                        x: 4,
                        barRods:  [
                          BarChartRodData(
                              y: 65,
                              colors: [Colors.lightBlueAccent, Colors.greenAccent]
                          )
                        ],
                        showingTooltipIndicators: [0],
                      ),
                      BarChartGroupData(
                        x: 5,
                        barRods:  [
                          BarChartRodData(
                              y: 60,
                              colors: [Colors.lightBlueAccent, Colors.greenAccent]
                          )
                        ],
                        showingTooltipIndicators: [0],
                      ),
                      BarChartGroupData(
                        x: 6,
                        barRods:  [
                          BarChartRodData(
                              y: 48 ,
                              colors: [Colors.lightBlueAccent, Colors.greenAccent]
                          )
                        ],
                        showingTooltipIndicators: [0],
                      ),
                    ],
                  ),
                ),
              ),
            ),
          ),
        ),
        Positioned(
          top: 25,
          left: 30,
          child: Text(
            'Bar Chart',
            style: TextStyle(fontSize: 15, color: Colors.white, decoration: TextDecoration.none),
          ),
        )
      ],
    );
  }
}
