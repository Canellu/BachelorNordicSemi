import 'package:bachelor_app/UI/compoment/dialog.dart';
import 'package:bachelor_app/models/mission.dart';
import 'package:bachelor_app/service/database.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:hexcolor/hexcolor.dart';
import 'package:intl/intl.dart';

class MissionTabPage extends StatefulWidget {
  final List missionList;
  final String gid;

  const MissionTabPage({Key key, this.missionList, this.gid}) : super(key: key);

  @override
  _MissionTabPageState createState() => _MissionTabPageState();
}

class WayPoint {
  double lat;
  double lng;

  WayPoint(@required this.lat, @required this.lng);
}

class _MissionTabPageState extends State<MissionTabPage> {
  final _formKey = GlobalKey<FormState>();
  final _latController = TextEditingController();
  final _lngController = TextEditingController();


  List<String> _freqSelections = ["High","Medium","Low","None"];
  List<WayPoint> wayPointList = [];
  List<WayPoint> selectedTableList = [];
  WayPoint selectedWP;
  String _freqCvalue = "None";
  String _freqTvalue = "None";
  String _freqPvalue = "None";
  RangeValues _freqDvalue = RangeValues(40, 200);
  double _4GValue = 30;
  DateTime dateTime;
  bool tabelSelect = false;

  void defaltValue(){
    _freqCvalue = "None";
    _freqTvalue = "None";
    _freqPvalue = "None";
    _freqDvalue = RangeValues(40, 200);
    _4GValue = 30;
    dateTime = null;
    wayPointList.clear();
    selectedTableList.clear();
  }

  //Get start date
  String getText() {
    if(dateTime == null) {
      return 'Select Start date';
    } else {
      print(DateFormat('yyyyMMddHHmm').format(dateTime));
      return DateFormat('MM-dd-yyyy HH:mm').format(dateTime);
    }
  }

  //Clean up the input-controller when the widget is disposed
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
        width: MediaQuery.of(context).size.width,
        color: HexColor("F3F4F6"),
        padding: EdgeInsets.only(left: 5,top: 30,right: 5,bottom: 0),
        child: Form(
          key: _formKey,
          child: Column(
            children: <Widget>[
              Container(
                width: MediaQuery.of(context).size.width -50,
                child: Column(
                  mainAxisAlignment: MainAxisAlignment.start,
                  children: <Widget>[
                    SizedBox(height: 20,),
                    Text(
                        'Start dive at : ',
                      style: TextStyle(
                        letterSpacing: 2,
                        fontWeight: FontWeight.bold,
                        fontSize: 16,
                      ),
                    ),
                    SizedBox(height: 5,),
                    Row(
                      children: [
                        FloatingActionButton(
                            materialTapTargetSize: MaterialTapTargetSize.padded,
                            backgroundColor: HexColor('#1f2937'),
                            child: Icon(
                              Icons.calendar_today_outlined,
                              color: Colors.white,
                              size: 25,
                            ),
                            onPressed: () => pickDateTime(context)
                        ),

                        SizedBox(width: 10,),
                        Expanded(
                          child: TextField(
                            textAlign: TextAlign.justify,
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
                shape: RoundedRectangleBorder(
                  borderRadius: BorderRadius.circular(20),
                ),
                elevation: 5,
                child: Column(
                  children: <Widget>[
                    SizedBox(height: 20,),
                    Text(
                      '4G Message Limit',
                      style: TextStyle(
                        letterSpacing: 2,
                        fontWeight: FontWeight.bold,
                        fontSize: 16,
                      ),
                    ),
                    SizedBox(height: 5,),
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
                    SizedBox(height: 10,),
                  ],
                ),
              ),
              SizedBox(height: 20),
              //build WayPoint widget
              _loggWayPoint(),
              SizedBox(height: 20),
              //build frequencies widget
              _LoggingFrequencies(),
              SizedBox(height: 20,),
              ElevatedButton(
                style: ElevatedButton.styleFrom(
                  primary: HexColor('#1f2937'),
                  elevation: 5,
                ),
                child: Text(
                  'Send',
                  style: TextStyle(
                    fontSize: 16,
                    letterSpacing: 2
                  ),
                ),
                onPressed: () {
                  //Check if the mission are ok to send
                  var submitStatus = submitCheck();
                  //If something wrong, show error message at bottom
                  if(submitStatus != null) {
                    ScaffoldMessenger.of(context).showSnackBar(
                        SnackBar(
                          backgroundColor: Colors.red,
                            content: Text(
                                submitStatus
                            )
                        )
                    );
                  } else {
                    //Send new mission to firestore
                    var createMissionID = widget.missionList.length + 1;

                    var wayPoint = [];
                    wayPointList.forEach((element) {
                      wayPoint.add("${element.lat},${element.lng}");
                    });

                    DatabaseService(widget.gid,"").newMission(
                        Mission(
                          missionId: "Mission $createMissionID",
                          startTime: DateFormat('yyyyMMddHHmm').format(dateTime),
                          C: _freqCvalue,
                          P: _freqPvalue,
                          T: _freqTvalue,
                          maxD: _freqDvalue.end.round(),
                          minD: _freqDvalue.start.round(),
                          nett: _4GValue.round(),
                          wayPoint: wayPoint,
                        )
                    );

                    defaltValue();

                    //Show success message after mission created
                    ScaffoldMessenger.of(context).showSnackBar(
                        SnackBar(
                            backgroundColor: Colors.green,
                            content: Text(
                                'Mission Created ! '
                            )
                        )
                    );
                  }
                },
              ),
              SizedBox(height: 20,),
            ],
          ),
        ),
      ),
    );
  }

//-----------WayPoint widget---------
  Widget _loggWayPoint() {
    return Card(
      shape: RoundedRectangleBorder(
        borderRadius: BorderRadius.circular(20),
      ),
      elevation: 5,
      child: Container(
        width: MediaQuery.of(context).size.width,
        child: Column(
          mainAxisAlignment: MainAxisAlignment.spaceEvenly,
          children: [
            SizedBox(height: 20,),
            Text(
              'Logging waypoint',
              style: TextStyle(
                fontSize: 16,
                letterSpacing: 2,
                fontWeight: FontWeight.bold,
              ),
            ),
            SizedBox(
              width: MediaQuery.of(context).size.width-100,
              child: TextFormField(
                controller: _latController,
                decoration: InputDecoration(
                    hintText: 'Enter Latitude',
                    labelText: 'Lat'
                ),
                keyboardType: TextInputType.numberWithOptions(
                  decimal: true,
                ),
                //Input regex for lat
                inputFormatters: <TextInputFormatter>[
                  //Only allow numbers and 4 number after decimal
                  FilteringTextInputFormatter.allow(RegExp(r'^-?(?:-?(?:[0-9]+))?(?:.\d{0,4})'))
                ],
                //Input validation for lat
                validator: (value) {
                  bool isNumeric(String value) {
                    return double.tryParse(value) != null;
                  }

                  if(value.isEmpty || value.isEmpty) {
                    return 'Please enter som text';
                  }else if(!isNumeric(value)) {
                    return 'Invalid : contains letter';
                  } else  if(double.parse(value) > 90 || double.parse(value) < -90) {
                    return 'Tips : Invalid latitude';
                  }
                  return null;
                },
              ),
            ),
            SizedBox(
              width: MediaQuery.of(context).size.width-100,
              child: TextFormField(
                controller: _lngController,
                decoration: InputDecoration(
                    hintText: 'Enter Longitude',
                    labelText: 'Long'
                ),
                keyboardType: TextInputType.numberWithOptions(
                  decimal: true,
                ),
                //Input regex for lng
                inputFormatters: <TextInputFormatter>[
                  //Only allow numbers and 4 number after decimal
                  FilteringTextInputFormatter.allow(RegExp(r'^-?(?:-?(?:[0-9]+))?(?:.\d{0,4})'))
                ],
                //Input validation for lng
                validator: (value) {
                  bool isNumeric(String value) {
                    return double.tryParse(value) != null;
                  }

                  if(value.isEmpty || value.isEmpty) {
                    return 'Please enter som text';
                  }else if(!isNumeric(value)) {
                    return 'Invalid : contains letter';
                  } else  if(double.parse(value) > 180 || double.parse(value) < -180) {
                    return 'Invalid : Invalid number';
                  }
                  return null;
                },
              ),
            ),
            SizedBox(height: 10,),
            ElevatedButton(
              style: ElevatedButton.styleFrom(
                primary: HexColor('#1f2937'),
                elevation: 5,
                //shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(20)),
                //side: BorderSide(color: Colors.black),
              ),
              child: Text(
                'Add',
                style: TextStyle(
                  color: Colors.white,
                  letterSpacing: 2
                ),
              ),
              //Check input for lat and lng
              onPressed: (){
                //Input is not valid, show error message at bottom
                if(!_formKey.currentState.validate()) {
                  ScaffoldMessenger.of(context).showSnackBar(
                      SnackBar(
                          backgroundColor: Colors.red,
                          content: Text(
                              "Coordinate input is wrong !"
                          )
                      )
                  );
                } else {
                  //Input is valid, add to the way point list
                  setState(() {
                    wayPointList.add(
                        WayPoint(
                            double.parse(_latController.text),
                            double.parse(_lngController.text)
                        )
                    );
                  });
                  _latController.clear();
                  _lngController.clear();
                }
                //wayPointList.add(WayPoint(Lat, Long))
              },
            ),
            //Build data table to show all waypoints
            _buildWayPointTable(),
          ],
        ),
      ),
    );
  }

  //-----------Data table for waypoint----------
  Widget _buildWayPointTable() {
    //Build data-table if waypoint list is not empty
    if(wayPointList.isNotEmpty) {
      return Container(
        width: MediaQuery.of(context).size.width-40,
        child: Column(
          children: <Widget>[
            Divider(thickness: 2,),
            SizedBox(height: 20,),
            DataTable(
                columns: const <DataColumn>[
                  DataColumn(
                    label: Text(
                      'No.',
                      style: TextStyle(
                        fontStyle: FontStyle.italic,
                        fontSize: 16,
                      ),
                    ),
                  ),
                  DataColumn(
                    label: Text(
                      'Lat',
                      style: TextStyle(fontStyle: FontStyle.italic, fontSize: 16,),
                    ),
                  ),
                  DataColumn(
                    label: Text(
                      'Long',
                      style: TextStyle(fontStyle: FontStyle.italic, fontSize: 16,),
                    ),
                  ),
                ],
                rows: wayPointList
                    .map(
                  ((element) => DataRow(
                    selected: showSelectRow(element),
                    onSelectChanged: (select) {
                      setState(() {
                        if(select) {
                          selectedTableList.add(element);
                        } else {
                          selectedTableList.remove(element);
                        }
                      });
                    },
                    cells: <DataCell>[
                      DataCell(
                          Text((wayPointList.indexOf(element)+1).toString())
                      ),
                      DataCell(
                          Text("${(element.lat).toString()} "),
                          //Edit lat
                          showEditIcon: true,
                          onTap: () {
                            editLat((element.lat).toString(),wayPointList.indexOf(element));
                          }
                      ),
                      DataCell(
                          Text("${(element.lng).toString()} "),
                          //Edit lng
                          showEditIcon: true,
                          onTap: () {
                            editLng((element.lng).toString(),wayPointList.indexOf(element));
                          }
                      ),
                    ],
                  )),
                ).toList()
            ),
            //Show delete button
            _deleteTable(),
          ],
        ),
      );
    } else {
      return SizedBox(height: 20,);
    }
  }

  //-----------Delete button and function ------------
  Widget _deleteTable() {
    //Check if data row has selected
    if(selectedTableList.isNotEmpty) {
      return Padding(
        padding: const EdgeInsets.fromLTRB(0, 20, 0, 20),
        child: ElevatedButton(
          style: ElevatedButton.styleFrom(
            primary: Colors.red,
            elevation: 5,
            //shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(20)),
            //side: BorderSide(color: Colors.black),
          ),
          child: Text(
            'DELETE',
            style: TextStyle(
                color: Colors.white,
                letterSpacing: 2
            ),
          ),
          onPressed: () {
            setState(() {
              selectedTableList.forEach((wp) {
                if(wayPointList.contains(wp)){
                  wayPointList.remove(wp);
                }
              });
            });
          },
        ),
      );
    }else{
      return SizedBox(height: 20,);
    }
  }

  //--------Frequencices Card Widget---------------
  Widget _LoggingFrequencies() {
    return Card(
      shape: RoundedRectangleBorder(
        borderRadius: BorderRadius.circular(20),
      ),
      elevation: 5,
      child: Column(
        mainAxisAlignment: MainAxisAlignment.spaceEvenly,
        children: [
          SizedBox(height: 20,),
          Text(
              'Logging frequencies',
            style: TextStyle(
              fontSize: 16,
              letterSpacing: 2,
              fontWeight: FontWeight.bold,
            ),
          ),
          SizedBox(height: 20,),
          Row(
            mainAxisAlignment: MainAxisAlignment.spaceAround,
            children: [
              Text(
                  'Conductivity :',
                style: TextStyle(
                  fontSize: 15,
                ),
              ),
              DropdownButton(
                isExpanded: false,
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
          Divider(thickness: 2,indent: 35,endIndent: 35),
          Row(
            mainAxisAlignment: MainAxisAlignment.spaceAround,
            children: [
              Text(
                  'Temperature :',
                style: TextStyle(
                  fontSize: 15,
                ),
              ),
              DropdownButton(
                isExpanded: false,
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
          Divider(thickness: 2,indent: 35,endIndent: 35),
          Row(
            mainAxisAlignment: MainAxisAlignment.spaceAround,
            children: [
              Text(
                  'Pressure :     ',
                style: TextStyle(
                  fontSize: 15,
                ),
              ),
              DropdownButton(
                isExpanded: false,
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
          Divider(thickness: 2,indent: 35,endIndent: 35),
          Container(
            padding: EdgeInsets.all(20),
            child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: <Widget>[
                  Padding(
                    padding: const EdgeInsets.fromLTRB(20, 0, 0, 0),
                    child: Text(
                      'Depth : ',
                      style: TextStyle(
                        fontSize: 15,
                      ),
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
                          _freqDvalue.start.round().toString(),_freqDvalue.end.round().toString()
                        ),
                        values: _freqDvalue,
                        onChanged: (value) {
                          setState(() {
                            _freqDvalue = value;
                          });
                        },
                  ))
                ]
            ),
          )
        ],
      ),
    );
  }

  //----------Display Calender for select date and time----------
  Future pickDateTime(BuildContext context) async {
    final date = await pickDate(context);
    if(date == null) return;

    final time = await pickTime(context);
    if(time == null) return;

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
    if(newDate == null) return null;
    return newDate;
  }

  Future<TimeOfDay> pickTime(BuildContext context) async {
    final initialTime = TimeOfDay(hour: 12, minute:0);
    final newTime = await showTimePicker(
        context: context,
        initialTime: dateTime != null ? TimeOfDay(hour: dateTime.hour, minute: dateTime.minute)
            : initialTime,
    );
    if(newTime == null) return null;
    return newTime;
  }

  //Open dialog to edit lat ot lng
  void editLat(String latValue, int index) async {
    final latEditText = await showTextDialog(
      context,
      title: 'Change Latitude',
      value: latValue,
      latOrLng: 'lat',
    );

    setState(() {
      if(latEditText != null) {
        wayPointList[index].lat = double.parse(latEditText);
      }
    });
  }

  void editLng(String lngValue, int index) async {
    final lngEditText = await showTextDialog(
      context,
      title: 'Change Longitude',
      value: lngValue,
      latOrLng: 'lng',
    );

    setState(() {
      if(lngEditText != null) {
        wayPointList[index].lng = double.parse(lngEditText);
      }
    });
  }

  //Check if row have been selected
  bool showSelectRow(WayPoint wp) {
    if(selectedTableList.length != 1) {
      return selectedTableList.contains(wp);
    } else {
      selectedWP = selectedTableList[0];
      return wp == selectedWP;
    }
  }

  //Check if there are any empty input
  String submitCheck() {
    if(wayPointList.isEmpty) {
      return 'Enter some way point for new mission ! ';
    } else if (dateTime == null) {
      return 'Select a date to start';
    } else if (_4GValue.round() == 0 || _freqDvalue.end.round() == 0 ||
        _freqDvalue.start.round() == 0){
      return 'Check frequence again, they cannot be 0';
    }
    return null;
  }
}