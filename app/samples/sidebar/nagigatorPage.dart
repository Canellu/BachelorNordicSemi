import 'package:flutter/material.dart';

class NavgatorPage extends StatelessWidget {

  final String pageText;  // To save the parameter

  NavgatorPage(this.pageText);

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(pageText),
      ),
      body: Center(
        child: Text(pageText,style: TextStyle(fontSize: 30,fontWeight: FontWeight.bold),),
      ),
    );
  }
}
