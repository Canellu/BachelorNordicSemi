import 'package:flutter/material.dart';
import 'package:hexcolor/hexcolor.dart';

class InfoCard extends StatelessWidget {
  final String field;
  final String value;
  final Icon icon;

  const InfoCard({this.field, this.value, this.icon});

  @override
  Widget build(BuildContext context) {
    return Expanded(
      child: Container(
        height: 140,
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
                    trailing: icon,
                    title: Text(
                      field,
                      maxLines: 1,
                      overflow: TextOverflow.clip,
                      style: TextStyle(
                        fontSize: 18,
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
                    child: Text(value,
                        style: TextStyle(
                            fontSize: 30,
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
