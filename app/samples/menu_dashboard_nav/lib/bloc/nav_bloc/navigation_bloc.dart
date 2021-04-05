import 'package:bachelor_app/UI/pages/devicePage.dart';
import 'package:bachelor_app/UI/pages/myAccount.dart';
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

  NavigationBloc({this.onMenuTap});

  @override
  NavigationStates get initialState => MyAccountPage(
        onMenuTap: onMenuTap,
      );

  @override
  Stream<NavigationStates> mapEventToState(NavigationEvents event) async* {
    switch (event) {
      case NavigationEvents.DashboardClickedEvent:
        yield MyAccountPage(
          onMenuTap: onMenuTap,
        );
        break;
      case NavigationEvents.DeviceClickedEvent:
        yield DevicePage(
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
