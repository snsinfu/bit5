import { denodb } from "./deps.ts";

const { Model, Relationships } = denodb;
const { STRING } = denodb.DataTypes;

export class User extends Model {
  static table = "users";
  static fields = {
    id: { type: STRING, primaryKey: true },
    name: STRING,
  };
}

export class Session extends Model {
  static table = "sessions";
  static fields = {
    id: { type: STRING, primaryKey: true },
    userId: STRING,
  };
}

// This adds a FOREIGN KEY constraint on the "userId" field. The field name is
// deduced as lowercase type name + "Id". I could pass { foreignKey: "userId" }
// to explicitly specify the field name.
// https://github.com/eveningkid/denodb/blob/v1.0.31/lib/relationships.ts#L34
Relationships.belongsTo(Session, User);

export class Token extends Model {
  static table = "tokens";
  static fields = {
    key: { type: STRING, primaryKey: true },
    secret: STRING,
    userId: STRING,
  };
}

Relationships.belongsTo(Token, User);
