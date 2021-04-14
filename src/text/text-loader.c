#include "text-loader.h"
#include "moon/moon64.h"

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <limits.h>
#include "libs/io_utils.h"

u8 * *seg2_course_name_table;
u8 * *seg2_act_name_table;
struct DialogEntry * *dialogPool;
u8 languagesAmount = 0;

const char* get_language_name(int language) {
    return moon_get_language_name(language);
}

void set_language(int new_language) {
    moon_set_language(new_language);
}

u8 *get_key_string(char *id) {
    return moon_language_get_key(id);
}