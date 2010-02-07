#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <limits.h>
#include <db.h>

#include "lib/hdb.h"
#include "types.h"

static const char *DATE_INPUT_FORMAT = "%Y-%m-%d";

static int is_leap_year(int year)
{
    return year%400 == 0 || (year%100 != 0 && year%4 == 0);
}

static int parse_date(const char *input, const char *format, struct tm *date)
{
    memset(date, 0, sizeof(struct tm));
    char *ret = strptime(input, format, date);
    if (ret == NULL) return 1;
    if (*ret != 0) return 1;

    /* All 31 day months have valid result */
    if (date->tm_mon == 0 || date->tm_mon == 2 || date->tm_mon == 4) return 0;
    if (date->tm_mon == 6 || date->tm_mon == 7 || date->tm_mon == 9) return 0;
    if (date->tm_mon == 11) return 0;

    /* February special cases: Normal an leap year on 29th */
    if (date->tm_mon == 1) {
        if (date->tm_mday <= 28) return 0;
        if (is_leap_year(date->tm_year + 1900) && date->tm_mday == 29) return 0;
    }

    /* Rest of the months are OK, if they are less than 30th */
    if (date->tm_mon != 1 && date->tm_mday <= 30) return 0;

    return 1;
}

static int parse_int(char *input, int *number)
{
    char *endptr = NULL;
    int ret;

    errno = 0;
    ret = strtol(input, &endptr, 10);

    if (endptr == NULL) return 1;
    if (*endptr != 0) return 1;
    if ((ret == LONG_MIN || ret == LONG_MAX) && errno == ERANGE) return 1;

    *number = ret;
    return 0;
}

int hrai_add(struct tm date, int amount, const char *desc)
{
    DBT key, data;
    DB *dbp = hrai_db_open();

    if (!dbp) {
        fprintf(stderr, "Couldn't open DB\n");
        return 1;
    }

    memset(&key, 0, sizeof(DBT));
    memset(&data, 0, sizeof(DBT));

    struct hrai_entry entry;
    memset(&entry, 0, sizeof(struct hrai_entry));
    entry.date = date;
    entry.amount = amount;
    strncpy(entry.description, desc, 768);

    data.data = &entry;
    data.size = sizeof(struct hrai_entry);
    int ret = dbp->put(dbp, NULL, &key, &data, DB_APPEND);

    if (ret == DB_KEYEXIST) return 1;
    hrai_db_close(dbp);

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 4) {
        fprintf(stderr, "E: Incorrect number of parameters\n");
        return EXIT_FAILURE;
    }
    int amount;
    struct tm date;
    char *desc = argv[3];

    if (parse_date(argv[1], DATE_INPUT_FORMAT, &date)) {
        fprintf(stderr, "E: Couldn't parse date (expected format %s): %s\n"
                      , DATE_INPUT_FORMAT, argv[1]);
        return EXIT_FAILURE;
    }

    if (parse_int(argv[2], &amount)) {
        fprintf(stderr, "E: Couldn't parse int: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    if (hrai_add(date, amount, desc)) {
        fprintf(stderr, "E: Couldn't add to DB\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
