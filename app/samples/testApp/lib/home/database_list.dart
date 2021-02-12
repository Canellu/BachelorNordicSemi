import 'package:oasys_app/models/data.dart';
import 'package:flutter/material.dart';
import 'package:oasys_app/services/database.dart';
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

    var health;
    var duration;
    var battery;

    print(datas.length);

    datas.forEach((data) {
      health = data.health;
      duration = data.duration;
      battery = data.battery;

      print(data.health);
      print(data.duration);
      print(data.battery);
    });

    db.updateData(battery, duration, health);

    return Container(
      alignment: Alignment.center,
      child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          crossAxisAlignment: CrossAxisAlignment.center,
          children: [
            Text('$health',
                style: TextStyle(fontSize: 20, fontWeight: FontWeight.bold)),
            Text('$duration',
                style: TextStyle(fontSize: 20, fontWeight: FontWeight.bold)),
            Text('$battery',
                style: TextStyle(fontSize: 20, fontWeight: FontWeight.bold))
          ]),
    );
  }
}
