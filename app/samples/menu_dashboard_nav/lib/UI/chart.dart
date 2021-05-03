import 'dart:math';

import 'package:bachelor_app/service/database.dart';
import 'package:flutter/material.dart';
import 'package:charts_flutter/flutter.dart' as charts;
import 'package:charts_flutter/src/text_style.dart' as ChartStyle;
import 'package:charts_flutter/src/text_element.dart' as ChartText;
import 'package:intl/intl.dart';


//Needs to change MinuteTimeStepper and _autoViewportDefault in library

class Chart extends StatelessWidget {
  final String dataType;
  final String missionID;

  //requiring the selected mission's id
  Chart({Key key, this.missionID, this.dataType}) : super(key: key);

  @override



  Widget build(BuildContext context) {
    return FutureBuilder(
      future: DatabaseService("311910", "Mission 1").datas,
        builder: (context, snapshot) {

          if (!snapshot.hasData) return const Text("Loading....");
          print("HENT DATA !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
          Map dataset = new Map<String, dynamic>.from(snapshot.data);
          var dataForChart = new Map();
          dataset.forEach((key, value) {
            if(key == dataType){
              dataForChart = value;
            }
          });

          return Container(
            child: ChartPage(dataForChart: dataForChart,),
          );
        }
    );
  }
}

class ChartPage extends StatefulWidget {

  final Map dataForChart;
  const ChartPage({Key key, this.dataForChart}) : super(key: key);

  @override
  _ChartPageState createState() => new _ChartPageState();
}



class TimeSeriesSales {
  final DateTime time;
  final double value;

  TimeSeriesSales(this.time, this.value);
}

final _selectPointData = [];

class _ChartPageState extends State<ChartPage> {
  // Line chart
  Widget _chartWidget() {

    final List<TimeSeriesSales> serial1data = [];

    widget.dataForChart.forEach((key, value) {
      serial1data.add(
        new TimeSeriesSales(DateTime.parse(key), double.parse(value))
      );
    });

    List<charts.Series<TimeSeriesSales, DateTime>> seriesList = [
      new charts.Series<TimeSeriesSales, DateTime>(
        id: 'Glider Data',
        colorFn: (_, __) => charts.MaterialPalette.blue.shadeDefault,
        data: serial1data,
        domainFn: (TimeSeriesSales sales, _) => sales.time,
        measureFn: (TimeSeriesSales sales, _) => sales.value,
      )
    ];

    var chart = new charts.TimeSeriesChart(
      seriesList,
      primaryMeasureAxis: new charts.NumericAxisSpec(
        viewport: new charts.NumericExtents(0, 15),
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
      animate: true,
      behaviors: [
        new charts.SlidingViewport(),
        new charts.PanAndZoomBehavior(),
        new charts.LinePointHighlighter(
          symbolRenderer: CustomCircleSymbolRenderer()
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

    return new Card(
      child: Container(
        padding: new EdgeInsets.fromLTRB(16, 10, 16, 20),
        margin: new EdgeInsets.only(bottom: 20),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: <Widget>[
            new Center(
              child: new Text(
                'freqC line chart',
                style: TextStyle(
                  color: Color.fromRGBO(0, 0, 0, 1.0), //opacity：不透明度
                  fontFamily: 'PingFangBold',
                  fontSize: 15.0,
                ),
              ),
            ),
            new Padding(
              padding: new EdgeInsets.all(32.0),
              child: new SizedBox(
                height: 200.0,
                child: chart,
              ),
            )
          ],
        ),
      ),
    );
  }

  @override
  void initState() {
    print("INIT STATE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    super.initState();
  }

  @override
  Widget build(BuildContext context) {
    return new Scaffold(
      body: new Center(
        child: _chartWidget(),
      ),
    );
  }
}

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
      Rectangle(bounds.left +5, bounds.top-70, bounds.width + 165, bounds.height + 45),
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