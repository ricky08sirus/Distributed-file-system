#include "metadata.h"
MYSQL *conn;

void execute_sql(const char *sql) {
    if (mysql_query(conn, sql)) {
        fprintf(stderr, "[METADATA] MySQL error: %s\n", mysql_error(conn));
    }
}



void initialize_metadata() {
    printf("[METADATA] Connecting to MySQL database...\n");

    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 0, NULL, 0)) {
        fprintf(stderr, "[METADATA] MySQL connection failed: %s\n", mysql_error(conn));
        exit(1);
    }

    // Create the metadata table if it does not exist
    execute_sql(CREATE_TABLE_QUERY);
}


void update_metadata(const char *filename, const char *location, int size, int replication_factor) {
    printf("[METADATA] Storing metadata for %s at %s\n", filename, location);

    char sql[512];
    snprintf(sql, sizeof(sql), INSERT_METADATA_QUERY, filename, location, size, replication_factor);
    execute_sql(sql);
}


void retrieve_metadata(const char *filename, char *response) {
    printf("[METADATA] Retrieving metadata for %s\n", filename);

    char sql[512];
    snprintf(sql, sizeof(sql), SELECT_METADATA_QUERY, filename);

    if (mysql_query(conn, sql)) {
        fprintf(stderr, "[METADATA] MySQL query error: %s\n", mysql_error(conn));
        strcpy(response, "NOT FOUND");
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result) {
        MYSQL_ROW row = mysql_fetch_row(result);
        if (row) {
            strcpy(response, row[0]);
        } else {
            strcpy(response, "NOT FOUND");
        }
        mysql_free_result(result);
    }
}

void delete_metadata(const char *filename) {
    printf("[METADATA] Deleting metadata for %s\n", filename);

    char sql[512];
    snprintf(sql, sizeof(sql), DELETE_METADATA_QUERY, filename);
    execute_sql(sql);
}

void close_metadata() {
    mysql_close(conn);
    printf("[METADATA] Database connection closed.\n");
}



