
import 'package:bachelor_app/UI/pages/home_page.dart';
import 'package:bachelor_app/service/database.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:provider/provider.dart';
import 'package:firebase_core/firebase_core.dart';

import 'UI/pages/loadingPage.dart';
import 'models/device.dart';

Future<void> main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp();
  runApp(MyApp());

  //Set status bar transparent
  SystemUiOverlayStyle systemUiOverlayStyle = SystemUiOverlayStyle(
    statusBarColor: Colors.transparent,
    statusBarBrightness: Brightness.dark,
    statusBarIconBrightness: Brightness.dark,

  );
  SystemChrome.setSystemUIOverlayStyle(systemUiOverlayStyle);
}

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      //Remove the debug banner at the right top
      debugShowCheckedModeBanner: false,
      title: 'Menu and Dashbord',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      builder: (BuildContext context, Widget widget) {
       //Display error page when widget build failed
        ErrorWidget.builder = (FlutterErrorDetails errorDetails) {
          return ErrorLoading();
        };
        return widget;
      },
      home: HomeRun(),
    );
  }
}

class HomeRun extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return StreamProvider<List<Device>>.value(
      value: DatabaseService("","").glider,
      child: GestureDetector(
        //close keyboard when click outside keyboard area
        onTap: (){
          FocusScopeNode currentFocus = FocusScope.of(context);
          if (!currentFocus.hasPrimaryFocus && currentFocus.focusedChild != null) {
            FocusManager.instance.primaryFocus.unfocus();
          }
        },
        child: MaterialApp(
          //Remove the debug banner at the right top
          debugShowCheckedModeBanner: false,
          home: HomePage(),
        ),
      ),
    );
  }
}

