import 'dart:io';

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

    bool health;
    int duration;
    String battery;

    datas.forEach((data) {
      print(data.health);
      print(data.duration);
      print(data.battery);

      health = data.health;
      duration = data.duration;
      battery = data.battery;
    });

    var healthStatus = health ?? false;


    /*
    void setHealthColor() {
      if(health){
        healthStatus = Colors.green;
      }else{
        healthStatus = Colors.red;
      }
    }
    setHealthColor();*/

    //db.updateData(battery, duration, health);
    /*
    return Container(
      alignment: Alignment.center,
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        crossAxisAlignment: CrossAxisAlignment.center,
        children: [
          Text('$health',
          style: TextStyle),
          Text('$duration',
              style: TextStyle(fontSize: 20,fontWeight: FontWeight.bold)),
          Text('$battery',
              style: TextStyle(fontSize: 20,fontWeight: FontWeight.bold))
        ],
      )
    );*/

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
            decoration: textInputDecoration,
            initialValue: battery,
            onChanged: (val) => setState(() => battery = val),
          ),
          SizedBox(height: 20.0),
          Slider(
            value: (duration ?? 0.0).toDouble(),
            min: 0.0,
            max: 100.0,
            divisions: 100,
            activeColor: Colors.blueAccent[500],
            onChanged: (val) => setState(() => duration = val.round()),
          ),
          SizedBox(height: 20.0),
          Text(
            'Health Status :',
            style: TextStyle(fontSize: 20,fontWeight: FontWeight.bold),
          ),
          RaisedButton(
            color: healthStatus ? Colors.green : Colors.red,
            onPressed: (){
              setState(() {
                health = !health;
                print(health);
            });
            /*
                health = !health;
                setHealthColor();
                print(health);
                print(healthStatus);*/
            },
          ),
          FloatingActionButton(
            onPressed: (){
              //DatabaseService().updateData("0", 100, true);
            },
            child: Icon(Icons.navigation),
            backgroundColor: Colors.green,
          ),
        ],
      ),

    );

  }
}
