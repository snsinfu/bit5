import { Board, Card, Reaper, User, Session } from "./model.ts";

type PrimaryKey = string;

interface BasicEntity {
  id: PrimaryKey;
}

class BasicRepository<Entity extends BasicEntity> {
  protected _rows: Entity[] = [];

  upsert(entity: Entity) {
    const row = this.findById(entity.id);
    if (row) {
      Object.assign(row, entity);
    } else {
      this._rows.push(entity);
    }
  }

  delete(entity: Entity) {
    const index = this.indexWhere((row: Entity) => row.id === entity.id);
    if (index === this._rows.length) {
      return; // idempotency
    }
    this._rows.copyWithin(index, index + 1);
    this._rows.pop();
  }

  findById(id: PrimaryKey): Entity | undefined {
    const index = this.indexWhere((row: Entity) => row.id === id);
    if (index === this._rows.length) {
      return undefined;
    }
    return this._rows[index];
  }

  all(): Entity[] {
    return this._rows;
  }

  protected indexWhere(pred: (_: Entity) => boolean): number {
    let index = 0;
    for (; index < this._rows.length; index++) {
      if (pred(this._rows[index])) {
        break;
      }
    }
    return index;
  }
}

export class Users extends BasicRepository<User> {
}

export class Boards extends BasicRepository<Board> {
}

export class Cards extends BasicRepository<Card> {
  findByBoard(id: PrimaryKey): Card[] {
    return this._rows.filter((row: Card) => row.board === id);
  }
}

export class Reapers extends BasicRepository<Reaper> {
}

export class Sessions extends BasicRepository<Session> {
}
