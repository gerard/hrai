#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <db.h>
#include <ustr.h>

#define DEFAULT_DB_NAME     ".hrai"

const char *hrai_db_filename() {
    char *env_home = getenv("HOME");
    char *env_hdb = getenv("HRAI_DB");

    if (env_hdb) return env_hdb;

    Ustr *filename = ustr_dup_cstr(env_home ? env_home : "/tmp");
    ustr_add_cstr(&filename, "/");
    ustr_add_cstr(&filename, DEFAULT_DB_NAME);
    return ustr_cstr(filename);
}

DB *hrai_db_open()
{
    DB *dbp;
    if (db_create(&dbp, NULL, 0)) return NULL;
    if (dbp->open(dbp, NULL, hrai_db_filename(), NULL, DB_RECNO, 0, 0)) return NULL;
    return dbp;
}

int hrai_db_close(DB *dbp)
{
    if (dbp) return dbp->close(dbp, 0);
    else return 0;
}
