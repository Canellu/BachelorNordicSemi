class Glider {
  constructor(mission = [], UID, added, sync) {
    this.mission = mission;
    (this.UID = UID), (this.added = added), (this.sync = sync);
  }

  //   function getHealth(index) {
  //       return mission[index].health.health;
  //   }
}

class Mission {
  constructor(command, health, data = []) {
    (this.commands = command), (this.health = health), (this.data = data);
  }
}

class Health {
  constructor(health = "") {
    this.health = health;
  }
}

class GliderData {
  constructor(ts, T, P, C, minDepth, maxDepth, lng, lat) {
    (this.ts = ts),
      (this.T = T),
      (this.P = P),
      (this.C = C),
      (this.minD = minDepth),
      (this.maxD = maxDepth),
      (this.lng = lng),
      (this.lat = lat);
  }
}
