import 'package:bachelor_app/UI/pages/device_page.dart';
import 'package:bachelor_app/models/device.dart';
import 'package:flutter/material.dart';
import 'package:hexcolor/hexcolor.dart';

class DeviceTile extends StatelessWidget {

  //take the value from deviceList
  final Device oneDevice;

  const DeviceTile({this.oneDevice});

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: EdgeInsets.only(top: 5),
      child: Card(
        child: ListTile(
          leading: CircleAvatar(
            child: Text(
              '${oneDevice.alias}',
              style: TextStyle(
                fontWeight: FontWeight.bold,
                color: Colors.white,
              ),
            ),
            radius: 25,
            backgroundColor: HexColor('#1f2937'),
          ),
          contentPadding: EdgeInsets.fromLTRB(20,5,0,0),
          title: Text('ID: ' + oneDevice.deviceId),
          subtitle: Text('Last sync: ' + oneDevice.lastSync),
          isThreeLine: true,
          onTap: (){
            //Navigate to Device page
            Navigator.push(
              context, MaterialPageRoute(builder: (context) =>
                DevicePage(selectedDevice:oneDevice),
              ),
            );
          },
        ),
      ),
    );
  }


}