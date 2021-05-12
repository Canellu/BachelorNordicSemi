import 'package:bachelor_app/UI/oneChart.dart';
import 'package:bachelor_app/service/database.dart';
import 'package:charts_flutter/flutter.dart';
import 'package:fl_chart/fl_chart.dart';
import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:flutter/painting.dart' as prefix;


class ChartPage extends StatefulWidget {
  final List missionList;
  final String gid;

  const ChartPage({Key key, this.missionList, this.gid}) : super(key: key);

  @override
  _ChartPageState createState() => _ChartPageState();
}

class _ChartPageState extends State<ChartPage> {

  String selectedMission;
  String dataType = "C";
  String newType;

  changeChartType({String newType}) {
    setState(() {
      dataType = newType;
    });
  }

  @override
  Widget build(BuildContext context) {
    List<String> _missions = widget.missionList;
    return Container(
      child: Column(
        mainAxisAlignment: MainAxisAlignment.spaceEvenly,
        children: <Widget>[
          Padding(
            padding: const EdgeInsets.fromLTRB(30, 0, 120, 0),
            child: Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              //crossAxisAlignment: CrossAxisAlignment.start,
              children: <Widget>[
                Expanded(
                  child: Container(
                    margin: EdgeInsets.fromLTRB(0, 0, 10, 0),
                    decoration: BoxDecoration(
                      border: Border.all(color: Colors.black26,width: 2),
                      borderRadius: BorderRadius.circular(15),
                    ),
                    child: DropdownButton(
                      items: _missions
                          .map((value) => DropdownMenuItem(
                        child: Center(
                          child: Text(
                            value,
                            style: prefix.TextStyle(color: Colors.black, fontSize: 15),
                          ),
                        ),
                        value: value,
                      )).toList(),
                      onChanged: (SelectMID) {
                        setState(() {
                          selectedMission = SelectMID;
                        });
                      },
                      value: selectedMission,
                      isExpanded: true,
                      elevation: 20,
                      underline: SizedBox(),
                      hint: Center(
                        child: Text(
                          'Mission 1',
                        ),
                      ),
                    ),
                  ),
                ),
                //RaisedButton(onPressed: (){}),
                TextButton(
                  onPressed: () {},
                  child: Text(
                    'Detail info',
                    style: prefix.TextStyle(
                      color: Colors.black45,
                    ),
                  ),
                )
              ],
            ),
          ),
          FutureBuilder(
              future: DatabaseService(widget.gid, selectedMission ?? "Mission 1").datas,
              builder: (context, snapshot) {
                if (!snapshot.hasData) return const Text("Loading....");
                Map dataset = new Map<String,dynamic>.from(snapshot.data);
                return Container(
                  padding: EdgeInsets.fromLTRB(10, 0, 10, 0),
                  child: OneChart(dataType: dataType,dataset: dataset),
                );
              }
          ),
          Row(
            mainAxisAlignment: MainAxisAlignment.spaceEvenly,
            children: <Widget>[
              ElevatedButton(
                style: ElevatedButton.styleFrom(
                  primary: Colors.black26,
                  shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(20)),
                  side: BorderSide(color: Colors.black),
                ),
                child: Text(
                  'Chart C',
                  style: prefix.TextStyle(
                    color: Colors.white,
                  ),
                ),
                onPressed: (){
                  changeChartType(newType:"C");
                },
              ),
              ElevatedButton(
                style: ElevatedButton.styleFrom(
                  primary: Colors.black26,
                  shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(20)),
                ),
                child: Text(
                  'Chart T',
                  style: prefix.TextStyle(
                    color: Colors.white,
                  ),
                ),
                onPressed: (){
                  changeChartType(newType:"T");
                },
              ),
              ElevatedButton(
                style: ElevatedButton.styleFrom(
                  primary: Colors.black26,
                  shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(20)),
                ),
                child: Text(
                  'Chart P',
                  style: prefix.TextStyle(
                    color: Colors.white,
                  ),
                ),
                onPressed: (){
                  changeChartType(newType:"P");
                },
              ),
            ],
          )
        ],
      ),
    );
  }
}
