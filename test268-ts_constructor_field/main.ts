class Client {
  constructor(public username: string, private password: string) {
  }

  auth() {
    console.log("Auth:", this.username, this.password);
  }
}

const client = new Client("alice", "ISe852o4yym5ztpm");
client.auth();
console.log("User:", client.username);
