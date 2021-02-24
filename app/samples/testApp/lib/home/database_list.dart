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

  @override
  Widget build(BuildContext context) {

    final datas = Provider.of<List<Data>>(context) ?? [];
    final db = DatabaseService();
    final formKey = GlobalKey<FormState>();

    String health;
    String duration;
    String battery;

    datas.forEach((data) {
      health = data.health;
      duration = data.duration;
      battery = data.battery;
    });

    bool healthToBool = false;
    if(health == "true" || health == "True"){
      healthToBool = true;
    }else{
      healthToBool = false;
    }

    return Container(
      margin: const EdgeInsets.all(20.0),
      child: Form(
        key: formKey,
        child: Column(
          children: <Widget>[
            Text(
              'Battery : ',
              style: TextStyle(fontSize: 20,fontWeight: FontWeight.bold),
            ),
            SizedBox(height: 20.0),
            TextFormField(
              initialValue: battery,
              decoration: textInputDecoration,
              onChanged: (v) {
                battery = v;
              },
              onEditingComplete: (){
                db.updateData(battery, duration, healthToBool.toString());
              },
            ),
            SizedBox(height: 20.0),
            Text(
              'Duration : ' + duration.toString(),
              style: TextStyle(fontSize: 20,fontWeight: FontWeight.bold),
            ),
            Slider(
              value: (double.parse(duration) ?? 0.0),
              min: 0.0,
              max: 100.0,
              divisions: 10,
              activeColor: Colors.blueAccent[500],
              onChanged: (val) => duration = val.toString(),
              onChangeEnd: (val){
                db.updateData(battery, duration, healthToBool.toString());
              },
            ),
            SizedBox(height: 20.0),
            Text(
              'Health Status :',
              style: TextStyle(fontSize: 20,fontWeight: FontWeight.bold),
            ),
            RaisedButton(
              color: healthToBool ?? false ? Colors.green : Colors.red,
              child: Text(healthToBool ? 'True' : 'False', style: TextStyle(fontSize: 15)),
              onPressed: (){
                healthToBool = !healthToBool;
                db.updateData(battery, duration, healthToBool.toString());
              },
            ),
          ],
        ),
      ),
    );
  }
}
