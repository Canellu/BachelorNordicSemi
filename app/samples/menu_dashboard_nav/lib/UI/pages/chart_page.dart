import 'package:bachelor_app/UI/oneChart.dart';
import 'package:bachelor_app/service/database.dart';
import 'package:charts_flutter/flutter.dart';
import 'package:flutter/cupertino.dart';
import 'package:flutter/foundation.dart';
import 'package:flutter/gestures.dart';
import 'package:flutter/material.dart';
import 'package:flutter/painting.dart' as prefix;
import 'package:hexcolor/hexcolor.dart';

import '../missionRouteMap.dart';
import 'loadingPage.dart';


class ChartPage extends StatefulWidget {
  final List missionList;
  final String gid;

  const ChartPage({Key key, this.missionList, this.gid}) : super(key: key);

  @override
  _ChartPageState createState() => _ChartPageState();
}

class _ChartPageState extends State<ChartPage> {
  bool pressP = false;
  bool pressT = false;
  bool pressC = true;
  bool pressMap = false;

  String selectedMission;
  String dataType = "C";
  String newType;
  String cardType = "chart";
  Map dataset;

  changeType({String newChartType, String newCardType}) {
    setState(() {
      if(newChartType != null) {
        dataType = newChartType;
      }
      cardType = newCardType;
    });
  }

  initButtonBool() {
    pressP = false;
    pressT = false;
    pressC = false;
    pressMap = false;
  }


  @override
  Widget build(BuildContext context) {
    List<String> _missions = widget.missionList;
    return Container(
      color: HexColor("F3F4F6"),
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
                if (!snapshot.hasData || snapshot.hasError){
                  return ErrorLoading();
                }else {
                  dataset = new Map<String,dynamic>.from(snapshot.data);
                  return Column(
                    children: [/*
                      Container(
                        child:  ShowRouteMap(dataset: dataset),
                      ),
                      Container(
                        padding: EdgeInsets.fromLTRB(10, 0, 10, 0),
                        child: OneChart(dataType: dataType,dataset: dataset),
                      ),*/
                      Container(
                          child: _showCard()
                      ),
                      Row(
                        //mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                        children: <Widget>[
                          Padding(
                            padding: const EdgeInsets.fromLTRB(20, 0, 0, 0),
                            child: ElevatedButton(
                              style: ElevatedButton.styleFrom(
                                primary: pressC ? HexColor('#1f2937') : Colors.white,
                                elevation: 5,
                                //shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(20)),
                                //side: BorderSide(color: Colors.black),
                              ),
                              child: Text(
                                'Chart C',
                                style: prefix.TextStyle(
                                  color: pressC ? Colors.white : Colors.black,
                                ),
                              ),
                              onPressed: (){
                                setState(() {
                                  if(cardType != "chart" || dataType != "C") {
                                    initButtonBool();
                                    pressC = true;
                                  }
                                });
                                changeType(newChartType:"C",newCardType: "chart");
                                //changeDataSet();
                              },
                            ),
                          ),
                          ElevatedButton(
                            style: ElevatedButton.styleFrom(
                              primary: pressT ? HexColor('#1f2937') : Colors.white,
                              elevation: 5,
                              //shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(20)),
                            ),
                            child: Text(
                              'Chart T',
                              style: prefix.TextStyle(
                                color: pressT ? Colors.white : Colors.black,
                              ),
                            ),
                            onPressed: (){
                              setState(() {
                                if(cardType != "chart" || dataType != "T") {
                                  initButtonBool();
                                  pressT = true;
                                }
                              });
                              changeType(newChartType:"T",newCardType: "chart");
                            },
                          ),
                          ElevatedButton(
                            style: ElevatedButton.styleFrom(
                              //primary: Colors.white,
                              primary: pressP ? HexColor('#1f2937') : Colors.white,
                              elevation: 5,
                              //shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(20)),
                            ),
                            child: Text(
                              'Chart P',
                              style: prefix.TextStyle(
                                color: pressP ? Colors.white : Colors.black,
                              ),
                            ),
                            onPressed: (){
                              setState(() {
                                if(cardType != "chart" || dataType != "P") {
                                  initButtonBool();
                                  pressP = true;
                                }
                              });
                              changeType(newChartType:"P",newCardType: "chart");
                            },
                          ),
                          Padding(
                            padding: const EdgeInsets.fromLTRB(40, 0, 0, 0),
                            child: ElevatedButton(
                              style: ElevatedButton.styleFrom(
                                //primary: Colors.white,
                                shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(15)),
                                //side: BorderSide(color: Colors.black),
                                onPrimary: Colors.black45,
                                elevation: 5,
                                primary: pressMap ? HexColor('#1f2937') : Colors.white,
                              ),
                              child: Text(
                                'Route',
                                style: prefix.TextStyle(
                                  color: pressMap ? Colors.white : Colors.black,
                                ),
                              ),
                              onPressed: (){
                                setState(() {
                                  if(cardType != "map") {
                                    initButtonBool();
                                    pressMap = true;
                                  }
                                });
                                changeType(newCardType: "map");
                              },
                            ),
                          ),
                        ],
                      )
                    ],
                  );
                }
              }
          ),
        ],
      ),
    );
  }

  Widget _showCard(){
    if(cardType == "chart" ) {
      return OneChart(dataType: dataType,dataset: dataset);
    } else if(cardType == "map") {
      return ShowRouteMap(dataset: dataset);
    }
  }
}
