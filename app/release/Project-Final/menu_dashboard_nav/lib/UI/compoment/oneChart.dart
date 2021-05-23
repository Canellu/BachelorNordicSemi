import 'dart:math';

import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:charts_flutter/flutter.dart' as charts;
import 'package:charts_flutter/src/text_style.dart' as ChartStyle;
import 'package:charts_flutter/src/text_element.dart' as ChartText;


//Needs to change MinuteTimeStepper and _autoViewportDefault in library

class OneChart extends StatelessWidget {
  final String dataType;
  final Map dataset;

  OneChart({Key key, this.dataType, this.dataset}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    var dataForChart = new Map();

    dataset.forEach((key, value) {
      if(key == dataType){
        dataForChart = value;
      }
    });
    return ChartCompoent(dataForChart: dataForChart,dataType: dataType,);
  }
}

class ChartCompoent extends StatefulWidget {

  final Map dataForChart;
  final String dataType;
  const ChartCompoent({Key key, this.dataForChart, this.dataType}) : super(key: key);

  @override
  _ChartCompoentState createState() => new _ChartCompoentState();
}



class TimeSeriesSales {
  final DateTime time;
  final double value;

  TimeSeriesSales(this.time, this.value);
}

final _selectPointData = [];

class _ChartCompoentState extends State<ChartCompoent> {
  //Creat line chart
  Widget _chartWidget() {
    var label;
    var lineColor;
    final List<TimeSeriesSales> serial1data = [];


    if(widget.dataType == "C") {
      label = "CONDUCTIVITY";
      lineColor = charts.ColorUtil.fromDartColor(Color(0x80FF6C3A));
    } else if(widget.dataType == "T") {
      label = "TEMPERATURE";
      lineColor = charts.ColorUtil.fromDartColor(Color(0x8088AFCC));
    } else {
      label = "PRESSURE";
      lineColor = charts.ColorUtil.fromDartColor(Color(0x80F5BA61));
    }

    widget.dataForChart.forEach((key, value) {
      serial1data.add(
          new TimeSeriesSales(DateTime.parse(key), double.parse(value))
      );
    });

    List<charts.Series<TimeSeriesSales, DateTime>> seriesList = [
      new charts.Series<TimeSeriesSales, DateTime>(
        id: 'Glider Data',
        colorFn: (_, __) => lineColor,
        data: serial1data,
        domainFn: (TimeSeriesSales sales, _) => sales.time,
        measureFn: (TimeSeriesSales sales, _) => sales.value,
      )
    ];

    var chart = new charts.TimeSeriesChart(
      seriesList,
      primaryMeasureAxis: new charts.NumericAxisSpec(
        tickProviderSpec:
        new charts.BasicNumericTickProviderSpec(zeroBound: false,desiredTickCount: 4),
      ),
      domainAxis: new charts.DateTimeAxisSpec(
        tickFormatterSpec: new charts.AutoDateTimeTickFormatterSpec(
          day: new charts.TimeFormatterSpec(
            format: 'dd',
            transitionFormat: 'dd MMM',
          ),
          minute: new charts.TimeFormatterSpec(
            format: 'mm',
            transitionFormat: 'h mm',
          ),
        ),
      ),
      animate: false,
        defaultRenderer: new charts.LineRendererConfig(includePoints: true),
      behaviors: [
        new charts.SlidingViewport(),
        new charts.PanAndZoomBehavior(),
        new charts.LinePointHighlighter(
            symbolRenderer: new CustomCircleSymbolRenderer()
        )
      ],
      selectionModels: [
        charts.SelectionModelConfig(
            type: charts.SelectionModelType.info,
            changedListener: (charts.SelectionModel model) {
              _selectPointData.clear();
              if(model.hasDatumSelection) {
                model.selectedDatum.forEach((datumPair) {
                  _selectPointData.add(datumPair.datum.time);
                  _selectPointData.add(datumPair.datum.value);
                });
                //print(_selectPointData);
              }
            }
        ),
      ],
    );

    return Container(
      margin: EdgeInsets.fromLTRB(0, 0, 0, 10),
      height: MediaQuery.of(context).size.height /2 + 70,
      child: new Card(
        elevation: 5,
        child: Container(
          margin: new EdgeInsets.only(bottom: 20),
          child: Column(
            mainAxisAlignment: MainAxisAlignment.spaceEvenly,
            children: <Widget>[
              new Center(
                child: new Text(
                  '$label',
                  textAlign: TextAlign.center,
                  style: TextStyle(
                    fontWeight: FontWeight.bold,
                    color: Color.fromRGBO(0, 0, 0, 1.0),
                    fontFamily: 'PingFangBold',
                    fontSize: 16.0,
                  ),
                ),
              ),
              //SizedBox(height: 20),
              new Padding(
                padding: new EdgeInsets.fromLTRB(20, 10, 20, 20),
                child: new Container(
                  width: MediaQuery.of(context).size.width,
                  height: MediaQuery.of(context).size.height/2.7,
                  child: chart,
                ),
              )
            ],
          ),
        ),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return _chartWidget();
  }
}

//Use paint() to draw a box with data value, when click the data point in chart
class CustomCircleSymbolRenderer extends charts.CircleSymbolRenderer {
  @override
  void paint(charts.ChartCanvas canvas, Rectangle<num> bounds, {
    List<int> dashPattern,
    charts.Color fillColor,
    charts.FillPatternType fillPattern,
    charts.Color strokeColor,
    double strokeWidthPx
  }) {
    super.paint(
        canvas,
        bounds,
        dashPattern: dashPattern,
        fillColor: fillColor,
        strokeColor: strokeColor,
        strokeWidthPx: strokeWidthPx
    );


    canvas.drawRect(
        new Rectangle(bounds.left +5, bounds.top-70, bounds.width + 165, bounds.height + 45),
        fill: charts.Color.fromHex(code: '#bfbfbf')
    );
    ChartStyle.TextStyle textStyle = ChartStyle.TextStyle();

    textStyle.color = charts.Color.black;
    textStyle.fontSize = 15;

    String showText = "${_selectPointData[0]} \nValue : ${_selectPointData[1]}";

    canvas.drawText(
        ChartText.TextElement(showText, style: textStyle),
        (bounds.left + 10).round(),
        (bounds.top - 55).round()
    );
  }
}