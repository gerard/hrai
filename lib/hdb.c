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
