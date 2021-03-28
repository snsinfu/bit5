interface UserData {
  id: string;
  name: string;
}

class MockDatabase {
  users: UserData[] = [];
}

const database = new MockDatabase();

database.users.push({ id: "0d8fe819", name: "alice" });
database.users.push({ id: "9d43b0e4", name: "bob" });

console.log(database);
