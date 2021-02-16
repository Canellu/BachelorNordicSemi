import 'dart:io';

import 'package:flutter/cupertino.dart';
import 'package:oasys_app/models/data.dart';
import 'package:flutter/material.dart';
import 'package:oasys_app/services/database.dart';
import 'package:oasys_app/shared/constants.dart';
import 'package:provider/provider.dart';

class DataList extends StatefulWidget {

  @override
  _DataListState createState() => _DataListState();
}

class _DataListState extends State<DataList> {

  String newBattery;
  int newDuration;
  bool newHealth;

  @override
  Widget build(BuildContext context) {

    final datas = Provider.of<List<Data>>(context) ?? [];
    final db = DatabaseService();
    final formKey = GlobalKey<FormState>();

    bool health;
    int duration;
    String battery;

    datas.forEach((data) {
      health = data.health;
      duration = data.duration;
      battery = data.battery;
    });

    return Form(
      key: formKey,
      child: Column(
        children: <Widget>[
          Text(
            'Battery : ',
            style: TextStyle(fontSize: 20,fontWeight: FontWeight.bold),
          ),
          SizedBox(height: 20.0),
          TextFormField(
            initialValue: newBattery ?? battery,
            decoration: textInputDecoration,
            onChanged: (v) {
              newBattery = v;
            },
          ),
          SizedBox(height: 20.0),
          Slider(
            value: (newDuration ?? duration ?? 0.0).toDouble(),
            min: 0.0,
            max: 100.0,
            divisions: 10,
            activeColor: Colors.blueAccent[500],
            onChanged: (val) => setState(() => newDuration = val.round()),
          ),
          SizedBox(height: 20.0),
          Text(
            'Health Status :',
            style: TextStyle(fontSize: 20,fontWeight: FontWeight.bold),
          ),
          RaisedButton(
            color: newHealth ?? health ?? false ? Colors.green : Colors.red,
            onPressed: (){
              setState(() {
                if(newHealth == null){
                  newHealth = !health;
                }else{
                  newHealth = !newHealth;
                }
              });
            },
          ),
          SizedBox(height: 30.0),
          FloatingActionButton(
            onPressed: (){
              db.updateData(newBattery, newDuration, newHealth);
              newDuration = null;
              newHealth = null;
              newBattery = null;
            },
            child: Icon(Icons.navigation),
            backgroundColor: Colors.green,
          ),
        ],
      ),

    );

  }
}
