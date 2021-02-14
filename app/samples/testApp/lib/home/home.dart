import 'package:flutter/material.dart';
import 'package:oasys_app/home/database_list.dart';
import 'package:oasys_app/models/data.dart';
import 'package:provider/provider.dart';
import 'package:oasys_app/services/database.dart';

final datalist = DataList();

class Home extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return StreamProvider<List<Data>>.value(
      value: DatabaseService().datas,
      child: Scaffold(
        appBar: AppBar(
          title: Text('Test Connection'),
        ),
        body: DataList(),
        /*
        floatingActionButton: FloatingActionButton(
          onPressed: (){
            DatabaseService().updateData("0", 100, true);
          },
          child: Icon(Icons.navigation),
          backgroundColor: Colors.green,
        ),*/
      ),
    );
  }
}
