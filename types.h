#include <stdint.h>

struct hrai_entry {
    struct tm date;
    uint32_t amount;            /* In cents */
    char description[768];
};   
