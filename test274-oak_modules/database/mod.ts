interface ConnectionInfo {
  host: string;
  port: number;
  username: string;
  password: string;
}

class Conn {
  async close() {
  }
}

class Pool {
  constructor(info: ConnectionInfo) {
  }

  async connect(): Conn {
    return new Conn();
  }
}
