#ifndef METADATA_H
#define METADATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>


// MySQL Database Configuration
#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASS "6394@Aman"
#define DB_NAME "dfs_metadata"


// Metadata Table Definition
#define TABLE_NAME "metadata"

// SQL Queries
#define CREATE_TABLE_QUERY \
    "CREATE TABLE IF NOT EXISTS " TABLE_NAME " (" \
    "id INT AUTO_INCREMENT PRIMARY KEY, " \
    "filename VARCHAR(255) UNIQUE NOT NULL, " \
    "location TEXT NOT NULL, " \
    "size INT DEFAULT 0, " \
    "replication_factor INT DEFAULT 1, " \
    "timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP" \
    ");"

#define INSERT_METADATA_QUERY \
    "INSERT INTO " TABLE_NAME " (filename, location, size, replication_factor) " \
    "VALUES ('%s', '%s', %d, %d) " \
    "ON DUPLICATE KEY UPDATE location=VALUES(location), size=VALUES(size), replication_factor=VALUES(replication_factor);"

#define SELECT_METADATA_QUERY \
    "SELECT location FROM " TABLE_NAME " WHERE filename='%s';"

#define DELETE_METADATA_QUERY \
    "DELETE FROM " TABLE_NAME " WHERE filename='%s';"



void initialize_metadata();
void update_metadata(const char *filename, const char *location, int size, int replication_factor);
void retrieve_metadata(const char *filename, char *response);
void delete_metadata(const char *filename);
void close_metadata();
#endif // METADATA_H
