// Mcok database and repository classes.

export class Database {
  users: User[] = [];
  accessTokens: AccessToken[] = [];
  temporaryTokens: TemporaryToken[] = [];
}

export interface TemporaryToken {
  key: string;
  secret: string;
  createTime: number;
}

export interface AccessToken {
  key: string;
  secret: string;
  userId: string;
}

export interface User {
  id: string;
  name: string;
}

export class TemporaryTokenStore {
  private database: Database;
  private timeToLive: number = 1000;

  constructor(database: Database) {
    this.database = database;
  }

  insert(token: TemporaryToken) {
    this.database.temporaryTokens.push(token);
  }

  deleteByKey(key: string) {
    this.database.temporaryTokens = this.database.temporaryTokens.filter(
      (token: TemporaryToken) => (token.key !== key),
    );
  }

  deleteStaleTokens(now: number) {
    const tokens = this.database.temporaryTokens;
    this.database.temporaryTokens = tokens.filter(
      (token: TemporaryToken) => (now - token.createTime > this.timeToLive),
    );
  }

  findByKey(key: string): TemporaryToken | undefined {
    return this.database.temporaryTokens.find((token: TemporaryToken) => {
      return token.key === key;
    });
  }
}

export class AccessTokenStore {
  private database: Database;

  constructor(database: Database) {
    this.database = database;
  }

  insert(token: AccessToken) {
    this.database.accessTokens.push(token);
  }

  deleteByKey(key: string) {
    const tokens = this.database.accessTokens;
    this.database.accessTokens = tokens.filter((token: AccessToken) => {
      return token.key !== key;
    });
  }

  findByKey(key: string): AccessToken | undefined {
    return this.database.accessTokens.find((token: AccessToken) => {
      return token.key === key;
    });
  }

  findByUserId(userId: string): AccessToken | undefined {
    return this.database.accessTokens.find((token: AccessToken) => {
      return token.userId === userId;
    });
  }
}

export class UserStore {
  private database: Database;

  constructor(database: Database) {
    this.database = database;
  }

  upsert(user: User) {
    for (const entry of this.database.users) {
      if (entry.id === user.id) {
        return;
      }
    }
    this.database.users.push(user);
  }

  findById(id: string): User | undefined {
    return this.database.users.find((user: User) => user.id === id);
  }
}
