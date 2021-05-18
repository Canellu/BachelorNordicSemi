import 'package:flutter/material.dart';
import 'package:flutter/services.dart';

Future<T> showTextDialog<T>(
  BuildContext context, {
    String title,
      String value,
}) =>
     showDialog
       (context: context,
         builder: (context) => DialogWidget(title:title,value:value),
     );

class DialogWidget extends StatefulWidget {
  final String title;
  final String value;

  const DialogWidget({Key key, this.title, this.value}) : super(key: key);

  @override
  _DialogWidgetState createState() => _DialogWidgetState();
}

class _DialogWidgetState extends State<DialogWidget> {
  final _formKey = GlobalKey<FormState>();
  TextEditingController controller;

  @override
  void initState() {
    super.initState();

    controller = TextEditingController(text: widget.value);
  }

  @override
  Widget build(BuildContext context) {
    return Form(
      key: _formKey,
      child: AlertDialog(
        title: Text(widget.title,),
        content: TextFormField(
          controller: controller,
          decoration: InputDecoration(
            border: OutlineInputBorder(),
            hintText: 'Enter New Value',
          ),
          keyboardType: TextInputType.numberWithOptions(
            decimal: true,
          ),
          inputFormatters: <TextInputFormatter>[
            //Only allow numbers and 4 number after decimal
            FilteringTextInputFormatter.allow(RegExp(r'^-?(?:-?(?:[0-9]+))?(?:.\d{0,4})'))
          ],
          validator: (value) {
            if(value.isEmpty || value.isEmpty) {
              return 'Please enter som text';
            }else if(double.parse(value) > 90 || double.parse(value) < -90){
              return 'invalid value';
            }
            return null;
          },
        ),
        actions: [
          Center(
            child: ElevatedButton(
              child: Text(
                'Done',
              ),
              onPressed: () {
                if(_formKey.currentState.validate()) {
                  Navigator.of(context).pop(controller.text);
                }
              },
            ),
          )
        ],
      ),
    );
  }
}
