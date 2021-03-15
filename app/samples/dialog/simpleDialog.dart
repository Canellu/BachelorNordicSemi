import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
/*
Future<T> showDialog<T> ({
  @required BuildContext context,
  //click outside of the dialog will close
  boolbarrierDismissible = true,
  WidgetBuilder builder,
});
*/

class ShowSimpleDialog extends Dialog {
  String text;
  bool backMiss;

  ShowSimpleDialog({Key key, @required this.text, this.backMiss}) : super(key:key);
  @override
  Widget build(BuildContext context) {
    // TODO: implement build
    return GestureDetector(
      /*
      onTap: (){
        if(backMiss){
          Navigator.of(context).pop();
        }
      },*/
      child: Material(
        type: MaterialType.transparency,
        child: Center(
          child: SizedBox(
            child: Container(
              height: 300,
              width: 300,
              decoration: ShapeDecoration(
                color: Color(0xffffffff),
                shape: RoundedRectangleBorder(
                  borderRadius: BorderRadius.all(
                    Radius.circular(10),
                  ),
                ),
              ),
              child: Column(
                mainAxisAlignment: MainAxisAlignment.center,
                children: <Widget>[
                  Icon(
                    Icons.ac_unit,
                    size: 50,
                    color: Colors.blue[200],
                  ),
                  Padding(padding: EdgeInsets.only(top:10)),
                  Text(
                    'Dialog box, can use to signup page',
                    style: TextStyle(
                      fontSize: 17,
                      fontWeight: FontWeight.bold
                    ),
                  ),
                ],
              ),
            ),
          ),
        ),
      ),
    );
  }
}
