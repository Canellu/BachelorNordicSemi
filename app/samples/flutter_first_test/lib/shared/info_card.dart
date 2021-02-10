import 'package:flutter/material.dart';
import 'package:hexcolor/hexcolor.dart';
import 'package:provider/provider.dart';
import 'package:flutter_first_test/models/sensor.dart';

class InfoCard extends StatefulWidget {
  String field;
  String value;
  Icon icon;

  InfoCard({this.field, this.value, this.icon});
  @override
  _InfoCardState createState() => _InfoCardState();
}

class _InfoCardState extends State<InfoCard> {
  @override
  Widget build(BuildContext context) {
    final sensor = Provider.of<List<Sensor>>(context);
    if (sensor != null) {
      sensor.forEach((sensor) {
        switch (widget.field) {
          case "Battery":
            {
              widget.value = sensor.battery;
            }
            break;
          case "Status":
            {
              widget.value = sensor.status;
            }
            break;
          case "Duration":
            {
              widget.value = sensor.duration;
            }
            break;
          case "Temperature":
            {
              widget.value = sensor.temp;
            }
            break;
        }
      });
    }

    return Expanded(
      child: Container(
        height: 120,
        margin: EdgeInsets.only(left: 2, top: 5, right: 2, bottom: 5),
        // color: HexColor('#0081AA'),
        child: Card(
            shape: RoundedRectangleBorder(
              borderRadius: BorderRadius.circular(20),
            ),
            elevation: 20,
            child: Column(
              children: [
                Container(
                  decoration: BoxDecoration(
                    borderRadius: BorderRadius.only(
                        topLeft: Radius.circular(20),
                        topRight: Radius.circular(20)),
                    color: HexColor('#0081AA'),
                  ),
                  child: ListTile(
                    // tileColor: HexColor('#0081AA'),
                    dense: true,
                    trailing: widget.icon,
                    title: Text(
                      widget.field,
                      maxLines: 1,
                      overflow: TextOverflow.clip,
                      style: TextStyle(
                        fontSize: 16,
                        fontFamily: 'Poppins',
                        fontWeight: FontWeight.w700,
                        color: Colors.white,
                      ),
                    ),
                  ),
                ),
                Expanded(
                  child: Container(
                    alignment: Alignment.center,
                    child: Text(widget.value,
                        style: TextStyle(
                            fontSize: 22,
                            fontFamily: 'Poppins',
                            fontWeight: FontWeight.w800,
                            color: Colors.grey[800])),
                  ),
                ),
              ],
            )),
      ),
    );
  }
}
