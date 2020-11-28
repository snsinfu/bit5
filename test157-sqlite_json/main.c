// Using sqlite3 as a document database.
// https://dgl.cx/2020/06/sqlite-json-support

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sqlite3.h>


int main(void)
{
    sqlite3 *db;
    int err;

    err = sqlite3_open("_output.db", &db);
    if (err) {
        fprintf(stderr, "error (%d) in sqlite3_open\n", err);
        return 1;
    }

    char const *command;
    char *errmsg;

    command = "CREATE TABLE docs (body TEXT)";
    err = sqlite3_exec(db, command, NULL, NULL, &errmsg);
    if (err) {
        fprintf(stderr, "error (%d) in sqlite3_exec: %s\n", err, errmsg);
        return 1;
    }

    // Insert some data
    {
        sqlite3_stmt *stmt;

        command = "INSERT INTO docs VALUES (?)";
        err = sqlite3_prepare_v2(db, command, -1, &stmt, NULL);
        if (err) {
            fprintf(stderr, "error (%d) in sqlite3_prepare_v2: %s\n", err, sqlite3_errmsg(db));
            return 1;
        }

        int const doc_count = 100;
        size_t const max_doc_size = 4096;
        char *doc = malloc(max_doc_size);

        for (int i = 0; i < doc_count; i++) {

            int const id = ((doc_count - i) ^ 0x1234);
            double const data = sin((double) i);
            int len;

            len = snprintf(doc, max_doc_size, "{\"id\": %d, \"data\": %g}", id, data);
            if ((size_t) len >= max_doc_size) {
                fprintf(stderr, "error in snprintf\n");
                return 1;
            }

            err = sqlite3_reset(stmt);
            if (err) {
                fprintf(stderr, "error (%d) in sqlite3_reset: %s\n", err, sqlite3_errmsg(db));
                return 1;
            }

            err = sqlite3_bind_text(stmt, 1, doc, -1, NULL);
            if (err) {
                fprintf(stderr, "error (%d) in sqlite3_bind_text: %s\n", err, sqlite3_errmsg(db));
                return 1;
            }

            err = sqlite3_step(stmt);
            if (err != SQLITE_DONE) {
                fprintf(stderr, "error (%d) in sqlite3_step: %s\n", err, sqlite3_errmsg(db));
                return 1;
            }
        }

        err = sqlite3_finalize(stmt);
        if (err) {
            fprintf(stderr, "error (%d) in sqlite3_finalize: %s\n", err, sqlite3_errmsg(db));
            return 1;
        }

        free(doc);
    }

    // Create column and index for JSON content
    command =
        "ALTER TABLE docs"
        " ADD COLUMN id GENERATED ALWAYS"
        " AS (json_extract(body, '$.id'))"
        " VIRTUAL"
        "; "
        "CREATE INDEX docs_id ON docs(id)";
    err = sqlite3_exec(db, command, NULL, NULL, &errmsg);
    if (err) {
        fprintf(stderr, "error (%d): %s\n", err, errmsg);
        return 1;
    }

    // Check if the index works
    printf("Query plan:\n");
    {
        sqlite3_stmt *stmt;

        command =
            "EXPLAIN QUERY PLAN "
            "SELECT * FROM docs WHERE id > 5000";
        err = sqlite3_prepare_v2(db, command, -1, &stmt, NULL);
        if (err) {
            fprintf(stderr, "error (%d) in sqlite3_prepare_v2: %s\n", err, sqlite3_errmsg(db));
            return 1;
        }

        for (;;) {
            err = sqlite3_step(stmt);
            if (err && err != SQLITE_ROW) {
                if (err == SQLITE_DONE) {
                    break;
                }
                fprintf(stderr, "error (%d) in sqlite3_step: %s\n", err, sqlite3_errmsg(db));
                return 1;
            }

            int const column_count = sqlite3_column_count(stmt);
            for (int column = 0; column < column_count; column++) {
                char const *column_name = sqlite3_column_name(stmt, column);
                char const *column_text = (char const *) sqlite3_column_text(stmt, column);
                printf("  %s: %s\n", column_name, column_text);
            }
        }

        err = sqlite3_finalize(stmt);
        if (err) {
            fprintf(stderr, "error (%d) in sqlite3_finalize: %s\n", err, sqlite3_errmsg(db));
            return 1;
        }
    }

    sqlite3_close(db);

    return 0;
}
