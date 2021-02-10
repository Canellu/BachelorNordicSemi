import 'package:flutter/material.dart';
import 'package:firebase_core/firebase_core.dart';
import 'dashboard.dart';
import 'home.dart';
import 'loading.dart';

Future<void> main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp();
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  final Future<FirebaseApp> _fbInit = Firebase.initializeApp();

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'TestApp',
      debugShowCheckedModeBanner: false,
      home: FutureBuilder(
        future: _fbInit,
        builder: (context, snapshot) {
          // Check for errors
          if (snapshot.hasError) {
            print('Error initializing Firebase! ${snapshot.error.toString()}');
            return Text('Something went wrong!');
          }
          // Once complete, show Home screen
          if (snapshot.connectionState == ConnectionState.done) {
            return Home();
          }
          // If still not initialized, run loading screen
          return Loading();
        },
      ),
      routes: {'/dashboard': (_) => DashBoard()},
    );
  }
}
