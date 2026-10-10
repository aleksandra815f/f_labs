#ifndef PATHS_H
#define PATHS_H

#include <stdbool.h>

#include "status.h"

Status validate_path(const char *path);

Status paths_look_same(const char *first, const char *second, bool *out_same);

#endif
