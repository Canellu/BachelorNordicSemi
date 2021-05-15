import 'package:bachelor_app/models/mission.dart';
import 'package:bachelor_app/service/database.dart';
import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:intl/intl.dart';

class MissionTabPage extends StatefulWidget {
  final List missionList;
  final String gid;

  const MissionTabPage({Key key, this.missionList, this.gid}) : super(key: key);

  @override
  _MissionTabPageState createState() => _MissionTabPageState();
}

class WayPoint {
  final double Lat;
  final double Long;

  WayPoint(this.Lat, this.Long);
}

class _MissionTabPageState extends State<MissionTabPage> {
  final _formKey = GlobalKey<FormState>();
  final _latController = TextEditingController();
  final _lngController = TextEditingController();


<<<<<<< HEAD
  List<String> _freqSelections = ["High", "Medium", "Low", "None"];
=======
  List<String> _freqSelections = ["High","Medium","Low","None"];
  List<WayPoint> wayPointList = [];
  /*
  List<WayPoint> wayPointList = [
    WayPoint(1, 1),
    WayPoint(2, 2),
    WayPoint(3, 3),
  ];*/
>>>>>>> b7a094108ddbfcb4eae47ae512b277f0e7e475ea
  String _freqCvalue = "None";
  String _freqTvalue = "None";
  String _freqPvalue = "None";
  RangeValues _freqDvalue = RangeValues(0, 300);
  double _4GValue = 0;
  DateTime dateTime;

  String getText() {
    if (dateTime == null) {
      return 'Select Start date';
    } else {
      print(DateFormat('yyyyMMddHHmm').format(dateTime));
      return DateFormat('MM-dd-yyyy HH:mm').format(dateTime);
    }
  }

  //Clean up the controller when the widget is disposed
  @override
  void dispose(){
    _latController.dispose();
    _lngController.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return SingleChildScrollView(
      child: Container(
        padding: EdgeInsets.only(left: 30, top: 30, right: 30, bottom: 0),
        child: Form(
          key: _formKey,
          child: Column(
            children: <Widget>[
              Container(
                child: Column(
                  //crossAxisAlignment: CrossAxisAlignment.start,
                  children: <Widget>[
                    Text('Start dive at : '),
                    Row(
                      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                      children: [
                        Ink(
                          decoration: BoxDecoration(
                              border: Border.all(),
                              borderRadius: BorderRadius.circular(5)),
                          child: IconButton(
                            icon: Icon(Icons.calendar_today_outlined),
                            padding: EdgeInsets.zero,
                            onPressed: () => pickDateTime(context),
                          ),
                        ),
                        Container(
                          width: 200,
                          //padding: const EdgeInsets.all(20.0),
                          child: TextField(
                            decoration: InputDecoration(
                              hintText: getText(),
                            ),
                            readOnly: true,
                          ),
                        ),
                      ],
                    )
                  ],
                ),
              ),
              SizedBox(height: 20),
              Card(
                elevation: 5,
                child: Column(
                  children: <Widget>[
                    Text('4G Message Limit'),
                    Slider(
                      value: _4GValue,
                      min: 0,
                      max: 100,
                      divisions: 100,
                      label: _4GValue.round().toString(),
                      onChanged: (double value) {
                        setState(() {
                          _4GValue = value;
                        });
                      },
                    ),
                    /*
                    Row(
                      children: [
                        Slider(
                          value: _currentSliderValue,
                          min: 0,
                          max: 100,
                          divisions: 100,
                          label: _currentSliderValue.round().toString(),
                          onChanged: (double value) {
                            setState(() {
                              _currentSliderValue = value;
                            });
                          },
                        ),
                        Expanded(
                          child: TextField(

                          ),
                        )
                        /*
                  TextFormField(
                    decoration: InputDecoration(
                      hintText: 'Something'
                    ),
                  )*/
                      ],
                    ),*/
                  ],
                ),
              ),
              SizedBox(height: 20),
              _LoggingFrequencies(),
              SizedBox(height: 20),
              _buildWayPointTable(),
              ElevatedButton(
<<<<<<< HEAD
                child: Text('Submit'),
                onPressed: () {
                  var createMissionID = widget.missionList.length + 1;
                  /*DatabaseService("123456","").newMission(
=======
                child: Text(
                  'Submit'
                ),
                onPressed: () {
                  var createMissionID = widget.missionList.length + 1;

                  var wayPoint = [];
                  wayPointList.forEach((element) {
                    wayPoint.add("${element.Lat},${element.Long}");
                  });
                  //Map<String,String>
                  print(wayPoint);

                  DatabaseService("123456","").newMission(
>>>>>>> b7a094108ddbfcb4eae47ae512b277f0e7e475ea
                      Mission(
                        missionId: "Mission $createMissionID",
                        startTime: DateFormat('yyyyMMddHHmm').format(dateTime),
                        freqC: _freqCvalue,
                        freqP: _freqPvalue,
                        freqT: _freqTvalue,
                        maxD: _freqDvalue.end.round(),
                        minD: _freqDvalue.start.round(),
                        nett: _4GValue.round(),
                        wayPoint: wayPoint,
                      )
                  );*/
                },
              )
            ],
          ),
        ),
      ),
    );
  }

  Widget _buildWayPointTable() {
    return Card(
      elevation: 5,
      child: Column(
        children: <Widget>[/*
          Container(
            margin: EdgeInsets.all(20),
            decoration: BoxDecoration(
              //border: Border.all(),
              borderRadius: BorderRadius.circular(10),
            ),
            child: Column(
              mainAxisAlignment: MainAxisAlignment.spaceEvenly,
              children: <Widget>[
                SizedBox(
                  width: 100,
                  child: TextFormField(
                    controller: _latController,
                    decoration: InputDecoration(

                        hintText: 'Enter Latitude',
                        labelText: 'Lat',
                    ),
                    keyboardType: TextInputType.numberWithOptions(
                      decimal: true,
                    ),
                    inputFormatters: <TextInputFormatter>[
                      //Only allow numbers and 4 number after decimal
                      FilteringTextInputFormatter.allow(RegExp(r'^-?(?:-?(?:[0-9]+))?(?:.\d{0,4})'))
                    ],
                    validator: (value) {
                      if(value.isEmpty || value.isEmpty) {
                        return 'Please enter som text';
                      }else if(double.parse(value) > 90 || double.parse(value) < -90){
                        return 'invalid latitude';
                      }
                      return null;
                    },
                  ),
                ),
              ],
            ),
          ),*/
          DataTable(
            columns: const <DataColumn>[
              DataColumn(
                label: Text(
                  'No.',
                  style: TextStyle(fontStyle: FontStyle.italic),
                ),
              ),
              DataColumn(
                label: Text(
                  'Lat',
                  style: TextStyle(fontStyle: FontStyle.italic),
                ),
              ),
              DataColumn(
                label: Text(
                  'Long',
                  style: TextStyle(fontStyle: FontStyle.italic),
                ),
              ),
            ],
            rows: wayPointList
                  .map(
                ((element) => DataRow(
                  cells: <DataCell>[
                    DataCell(Text((wayPointList.indexOf(element)+1).toString())), //Extracting from Map element the value
                    DataCell(Text((element.Lat).toString())),
                    DataCell(Text((element.Long).toString())),
                  ],
                )),
              ).toList()
          ),
          Row(
            mainAxisAlignment: MainAxisAlignment.spaceEvenly,
            children: [
              SizedBox(
                width: 100,
                child: TextFormField(
                  controller: _latController,
                  decoration: InputDecoration(
                      hintText: 'Enter Latitude',
                      labelText: 'Lat'
                  ),
                  keyboardType: TextInputType.numberWithOptions(
                    decimal: true,
                  ),
                  inputFormatters: <TextInputFormatter>[
                    //Only allow numbers and 4 number after decimal
                    FilteringTextInputFormatter.allow(RegExp(r'^-?(?:-?(?:[0-9]+))?(?:.\d{0,4})'))
                  ],
                  validator: (value) {
                    if(value.isEmpty || value.isEmpty) {
                      return 'Please enter som text';
                    }else if(double.parse(value) > 90 || double.parse(value) < -90){
                      return 'invalid latitude';
                    }
                    return null;
                  },
                ),
              ),
              SizedBox(
                width: 100,
                child: TextFormField(
                  controller: _lngController,
                  decoration: InputDecoration(
                      hintText: 'Enter Longitude',
                      labelText: 'Long'
                  ),
                  keyboardType: TextInputType.numberWithOptions(
                    decimal: true,
                  ),
                  inputFormatters: <TextInputFormatter>[
                    //Only allow numbers and 4 number after decimal
                    FilteringTextInputFormatter.allow(RegExp(r'^-?(?:-?(?:[0-9]+))?(?:.\d{0,4})'))
                  ],
                  validator: (value) {
                    if(value.isEmpty || value.isEmpty) {
                      return 'Please enter som text';
                    }
                    else if(double.parse(value) > 180 || double.parse(value) < -180){
                      return 'invalid Longitude';
                    }
                    return null;
                  },
                ),
              ),
              IconButton(
                icon: Icon(Icons.add),
                onPressed: (){
                  if(!_formKey.currentState.validate()) {
                    ScaffoldMessenger.of(context).showSnackBar(SnackBar(content: Text("Coordinate input is wrong ! ")));
                  }
                  wayPointList.add(WayPoint(double.parse(_latController.text), double.parse(_lngController.text)));
                  print("lat: ${_latController.text}, lng: ${_latController.text}");
                  _latController.clear();
                  _lngController.clear();
                  //wayPointList.add(WayPoint(Lat, Long))
                },
              ),
            ],
          ),
          SizedBox(
            height: 20,
          )
        ],
      ),
    );
  }

  Widget _LoggingFrequencies() {
    return Card(
      elevation: 5,
      child: Column(
        mainAxisAlignment: MainAxisAlignment.spaceEvenly,
        children: [
          Text('Logging frequencies'),
          Row(
            mainAxisAlignment: MainAxisAlignment.spaceAround,
            children: [
              Text('Conductivity :'),
              DropdownButton(
                isExpanded: false,
                //dropdownColor: Colors.blue,
                elevation: 10,
                value: _freqCvalue,
                onChanged: (String newValue) {
                  setState(() {
                    _freqCvalue = newValue;
                  });
                },
                items: _freqSelections
                    .map<DropdownMenuItem<String>>((String value) {
                  return DropdownMenuItem<String>(
                    value: value,
                    child: Text(value),
                  );
                }).toList(),
              ),
            ],
          ),
          Divider(thickness: 2, indent: 35, endIndent: 35),
          Row(
            mainAxisAlignment: MainAxisAlignment.spaceAround,
            children: [
              Text('Temperature :'),
              DropdownButton(
                isExpanded: false,
                //dropdownColor: Colors.blue,
                elevation: 10,
                value: _freqTvalue,
                onChanged: (String newValue) {
                  setState(() {
                    _freqTvalue = newValue;
                  });
                },
                items: _freqSelections
                    .map<DropdownMenuItem<String>>((String value) {
                  return DropdownMenuItem<String>(
                    value: value,
                    child: Text(value),
                  );
                }).toList(),
              ),
            ],
          ),
          Divider(thickness: 2, indent: 35, endIndent: 35),
          Row(
            mainAxisAlignment: MainAxisAlignment.spaceAround,
            children: [
              Text('Pressure :     '),
              DropdownButton(
                isExpanded: false,
                //dropdownColor: Colors.blue,
                elevation: 10,
                value: _freqPvalue,
                onChanged: (String newValue) {
                  setState(() {
                    _freqPvalue = newValue;
                  });
                },
                items: _freqSelections
                    .map<DropdownMenuItem<String>>((String value) {
                  return DropdownMenuItem<String>(
                    value: value,
                    child: Text(value),
                  );
                }).toList(),
              ),
            ],
          ),
          Divider(thickness: 2, indent: 35, endIndent: 35),
          Container(
            padding: EdgeInsets.all(20),
            child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: <Widget>[
                  Padding(
                    padding: const EdgeInsets.fromLTRB(20, 0, 0, 0),
                    child: Text(
                      'Depth : ',
                      style: TextStyle(),
                    ),
                  ),
                  SliderTheme(
                      data: SliderThemeData(
                        trackHeight: 20,
                        thumbColor: Colors.white,
                        valueIndicatorColor: Colors.teal,
                      ),
                      child: RangeSlider(
                        min: 0,
                        max: 300,
                        divisions: 300,
                        labels: RangeLabels(
                            _freqDvalue.start.round().toString(),
                            _freqDvalue.end.round().toString()),
                        values: _freqDvalue,
                        onChanged: (value) {
                          setState(() {
                            _freqDvalue = value;
                          });
                        },
                      ))
                ]),
          )
        ],
      ),
    );
  }

  Future pickDateTime(BuildContext context) async {
    final date = await pickDate(context);
    if (date == null) return;

    final time = await pickTime(context);
    if (time == null) return;

    setState(() {
      dateTime = DateTime(
        date.year,
        date.month,
        date.day,
        time.hour,
        time.minute,
      );
    });
  }

  Future<DateTime> pickDate(BuildContext context) async {
    final initialDate = DateTime.now();
    final newDate = await showDatePicker(
      context: context,
      initialDate: dateTime ?? initialDate,
      firstDate: DateTime(DateTime.now().day),
      lastDate: DateTime(DateTime.now().year + 1),
    );
    if (newDate == null) return null;
    return newDate;
  }

  Future<TimeOfDay> pickTime(BuildContext context) async {
    final initialTime = TimeOfDay(hour: 12, minute: 0);
    final newTime = await showTimePicker(
      context: context,
      initialTime: dateTime != null
          ? TimeOfDay(hour: dateTime.hour, minute: dateTime.minute)
          : initialTime,
    );
    if (newTime == null) return null;
    return newTime;
  }
}