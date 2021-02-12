import 'package:oasys_app/models/data.dart';
import 'package:flutter/material.dart';
import 'package:oasys_app/services/database.dart';
import 'package:provider/provider.dart';

class DataList extends StatefulWidget {
  @override
  _DataListState createState() => _DataListState();
}

class _DataListState extends State<DataList> {

  //form values
  String currentBattery;
  int currentDuration;
  bool currentHealth;

  @override
  Widget build(BuildContext context) {

    final datas = Provider.of<List<Data>>(context) ?? [];

    print(datas.length);


    datas.forEach((data) {
      print(data.health);
      print(data.duration);
      print(data.battery);
    });

    //return Container();*/
    return StreamBuilder<Data>(
      stream: datas,

    );

    return ListView.builder(
      itemCount: datas.length,
      itemBuilder: (context, index) {
        return
      },
    );
  }
}
