import 'package:bachelor_app/UI/pages/add_device.dart';
import 'package:bachelor_app/UI/pages/home_page.dart';
import 'package:bachelor_app/UI/pages/settings.dart';
import 'package:bloc/bloc.dart';
import 'package:flutter_bloc/flutter_bloc.dart';

enum NavigationEvents {
  DashboardClickedEvent,
  DeviceClickedEvent,
  SettingsClickedEvent
}

abstract class NavigationStates {}

class NavigationBloc extends Bloc<NavigationEvents, NavigationStates> {
  final Function onMenuTap;

  NavigationBloc({this.onMenuTap})
      : super(HomePage(
          onMenuTap: onMenuTap,
        ));

  @override
  NavigationStates get initialState => HomePage(
        onMenuTap: onMenuTap,
      );

  @override
  Stream<NavigationStates> mapEventToState(NavigationEvents event) async* {
    switch (event) {
      case NavigationEvents.DashboardClickedEvent:
        yield HomePage(
          onMenuTap: onMenuTap,
        );
        break;
      case NavigationEvents.DeviceClickedEvent:
        yield AddDevice(
          onMenuTap: onMenuTap,
        );
        break;
      case NavigationEvents.SettingsClickedEvent:
        yield SettingsPage(
          onMenuTap: onMenuTap,
        );
        break;
    }
  }
}
