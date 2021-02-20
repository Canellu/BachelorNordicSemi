class Data {

  final String battery;
  final bool health;
  final int duration;

  Data({
    this.battery,
    this.duration,
    this.health
});
}

class DataWithDocInfo {
  final String battery;
  final bool health;
  final int duration;
  final String docID;

  DataWithDocInfo({
    this.battery,
    this.duration,
    this.health,
    this.docID
  });
}