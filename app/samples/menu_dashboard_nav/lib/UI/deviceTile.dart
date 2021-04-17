import 'package:bachelor_app/UI/pages/device_page.dart';
import 'package:bachelor_app/models/device.dart';
import 'package:flutter/material.dart';

class DeviceTile extends StatelessWidget {

  //take the value from deviceList
  final Device oneDevice;

  const DeviceTile({this.oneDevice});

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: EdgeInsets.only(top: 10),
      child: Card(
        //margin: EdgeInsets.fromLTRB(20, 6, 20, 0),
        child: ListTile(
          leading: CircleAvatar(
            radius: 20,
            //show health status, need to change the dummy data after
            backgroundColor: Colors.lightGreen,
          ),
          contentPadding: EdgeInsets.fromLTRB(20,5,0,0),
          title: Text('Alias: ' + oneDevice.alias),
          subtitle: Text('Last sync: ' + oneDevice.lastSync + "\n" +
              'Added: ' + oneDevice.added),
          isThreeLine: true,
          onTap: (){
            Navigator.push(
              context, MaterialPageRoute(builder: (context) => DevicePage(gliderId:oneDevice.deviceId),
              ),
            );
          },
        ),
      ),
    );
  }


}