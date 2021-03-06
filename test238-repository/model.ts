/**
 * User represents an authorized user.
 */
export interface User {
    id: string;
    tokenKey: string;
    tokenSecret: string;
}

/**
 * UserRepository is a collection of `User` objects. This is a mockable
 * abstraction of an access to "user" table in a database.
 */
export class UserRepository {
    private _users: User[] = [];

    /**
     * Updates existing user (identified by `user.id`) or inserts a new user
     * if not exist.
     */
    upsert(user: User) {
        const entry = this.findById(user.id);
        if (entry) {
            Object.assign(entry, user);
        } else {
            this._users.push(user);
        }
    }

    /**
     * Delets existing user (identified by `user.id`) or do nothing if not
     * exist.
     */
    delete(user: User) {
        let users = this._users;
        let index = 0;

        for (; index < users.length; index++) {
            if (users[index].id === user.id) {
                break;
            }
        }
        if (index === users.length) {
            return; // idempotency
        }

        for (let i = index; i + 1 < users.length; i++) {
            users[i] = users[i + 1];
        }
        users.pop();
    }

    /**
     * Finds a user with given id.
     */
    findById(id: string): User | undefined {
        for (const user of this._users) {
            if (user.id === id) {
                return user;
            }
        }
        return undefined;
    }

    /**
     * Finds a user having given token.
     */
    findByToken(key: string): User | undefined {
        for (const user of this._users) {
            if (user.tokenKey === key) {
                return user;
            }
        }
        return undefined;
    }
}
