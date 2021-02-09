import 'package:flutter/material.dart';
import 'package:flutter/rendering.dart';
import 'package:hexcolor/hexcolor.dart';
import 'models/info_card.dart';

class DashBoard extends StatefulWidget {
  @override
  _DashBoardState createState() => _DashBoardState();
}

class _DashBoardState extends State<DashBoard> {
  var sliderVal = 0.0;

  @override
  Widget build(BuildContext context) {
    return SafeArea(
      child: Scaffold(
        appBar: AppBar(
          backgroundColor: HexColor('#D0E2F8'),
          elevation: 0,
          automaticallyImplyLeading: false,
          centerTitle: true,
          title: Text(
            'Dashboard'.toUpperCase(),
            style: TextStyle(
              color: Colors.grey[900],
              letterSpacing: 2,
              fontFamily: 'Poppins',
              fontWeight: FontWeight.w600,
              fontSize: 22,
            ),
          ),
        ),
        body: Container(
          color: HexColor('#D0E2F8'),
          child: Column(
            children: [
              Row(
                children: [
                  InfoCard(
                    field: 'Status',
                    value: 'Surfaced',
                    icon: Icon(Icons.anchor, color: Colors.white),
                  ),
                  InfoCard(
                    field: 'Duration',
                    value: '20 min',
                    icon: Icon(Icons.timer, color: Colors.white),
                  ),
                ],
              ),
              Row(
                children: [
                  InfoCard(
                    field: 'Temperature',
                    value: '17',
                    icon: Icon(Icons.device_thermostat, color: Colors.white),
                  ),
                  InfoCard(
                    field: 'Battery',
                    value: '88%',
                    icon: Icon(Icons.battery_charging_full_sharp,
                        color: Colors.white),
                  ),
                ],
              ),
              Column(
                children: [
                  Container(
                    margin: EdgeInsets.only(left: 22, top: 10),
                    alignment: Alignment.centerLeft,
                    child: Text(
                      'Controls',
                      style: TextStyle(
                        fontWeight: FontWeight.w600,
                        fontSize: 24,
                        fontFamily: 'Poppins',
                      ),
                    ),
                  ),
                  Container(
                    padding: EdgeInsets.only(top: 20),
                    child: Column(
                      crossAxisAlignment: CrossAxisAlignment.start,
                      children: [
                        Row(children: [
                          Container(
                            decoration: BoxDecoration(
                              borderRadius:
                                  BorderRadius.all(Radius.circular(10)),
                              color: HexColor('#0081AA'),
                            ),
                            width: 38,
                            height: 38,
                            alignment: Alignment.center,
                            margin: EdgeInsets.only(left: 22),
                            child: Text(
                              '${sliderVal.round()}',
                              style: TextStyle(
                                fontSize: 20,
                                color: Colors.white,
                                fontWeight: FontWeight.bold,
                              ),
                            ),
                          ),
                          Padding(
                            padding: EdgeInsets.only(left: 10),
                            child: Text(
                              'Pitch',
                              style: TextStyle(
                                fontSize: 20,
                                color: Colors.grey[800],
                                fontWeight: FontWeight.bold,
                              ),
                            ),
                          ),
                        ]),
                        Slider(
                          min: 0,
                          max: 100,
                          divisions: 100,
                          value: sliderVal,
                          onChanged: (newVal) {
                            setState(() {
                              sliderVal = newVal;
                            });
                          },
                        ),
                      ],
                    ),
                  ),
                ],
              ),
              Container(
                margin: EdgeInsets.only(top: 30),
                padding: EdgeInsets.only(left: 20),
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.start,
                  crossAxisAlignment: CrossAxisAlignment.center,
                  children: [
                    Text(
                      'Health Indicator: ',
                      textAlign: TextAlign.center,
                      style: TextStyle(
                        fontSize: 20,
                        color: Colors.grey[800],
                        fontWeight: FontWeight.bold,
                      ),
                    ),
                    Container(
                      margin: EdgeInsets.only(left: 20),
                      decoration: BoxDecoration(
                        border:
                            Border.all(color: HexColor('#AAB7CC'), width: 4),
                        borderRadius: BorderRadius.all(Radius.circular(30)),
                        color: HexColor('#f5665d'),
                      ),
                      width: 50,
                      height: 50,
                    ),
                  ],
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }
}
