import secrets
import sqlite3


TIMESTAMP_1 = 1_600_000_000
TIMESTAMP_2 = 1_600_008_000
TIMESTAMP_NOW = 1_600_010_000
TTL = 3600


def main():
    conn = sqlite3.connect(":memory:")

    make_table(conn)
    make_data(conn)

    cur = conn.cursor()
    cur.execute(
        r"""
        SELECT card.id, board.user
        FROM card
        JOIN reaper ON card.board == reaper.board
        JOIN board ON card.board == board.id
        WHERE ? - card.update_time > reaper.time_to_live
        """,
        [TIMESTAMP_NOW],
    )

    for card_id, user_id in cur.fetchall():
        print(f"Archive card:{card_id} owned by user:{user_id}")


def make_table(conn):
    with conn:
        conn.execute(r"""
            CREATE TABLE user (
                id      TEXT PRIMARY KEY,
                name    TEXT NOT NULL,
                token   TEXT
            )
        """)

        conn.execute(r"""
            CREATE TABLE board (
                id      TEXT PRIMARY KEY,
                name    TEXT,
                user    TEXT REFERENCES user(id)
            )
        """)

        conn.execute(r"""
            CREATE TABLE card (
                id          TEXT PRIMARY KEY,
                update_time INTEGER,
                board       TEXT REFERENCES board(id)
            )
        """)

        conn.execute(r"""
            CREATE TABLE reaper (
                id              TEXT PRIMARY KEY,
                time_to_live    INTEGER,
                board           TEXT REFERENCES board(id)
            )
        """)

    with conn:
        conn.execute(r"CREATE INDEX IX_board_user ON board (user)")
        conn.execute(r"CREATE INDEX IX_card_board ON card (board)")
        conn.execute(r"CREATE INDEX IX_reaper_board ON reaper (board)")


def make_data(conn):
    user_id = make_id()
    board_id = make_id()
    card_id_1 = make_id()
    card_id_2 = make_id()
    reaper_id = make_id()

    with conn:
        conn.execute(
            r"INSERT INTO user VALUES (?, ?, ?)",
            [user_id, "alice", "029399ac38850ec728cc1e24a754a84b"],
        )

        conn.execute(
            r"INSERT INTO board VALUES (?, ?, ?)",
            [board_id, "Work", user_id],
        )

        conn.execute(
            r"INSERT INTO card VALUES (?, ?, ?)",
            [card_id_1, TIMESTAMP_1, board_id],
        )

        conn.execute(
            r"INSERT INTO card VALUES (?, ?, ?)",
            [card_id_2, TIMESTAMP_2, board_id],
        )

        conn.execute(
            r"INSERT INTO reaper VALUES (?, ?, ?)",
            [reaper_id, TTL, board_id],
        )


def make_id():
    return secrets.token_hex(4)


main()
