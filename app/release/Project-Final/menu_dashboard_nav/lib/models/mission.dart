class Mission {
  final String missionId;
  final String endTime;
  final String C;
  final String P;
  final String T;
  final int maxD;
  final int minD;
  final String startTime;
  final int nett;
  final List wayPoint;

  Mission({this.wayPoint, this.nett, this.missionId, this.endTime, this.C, this.P, this.T, this.maxD, this.minD, this.startTime});
}