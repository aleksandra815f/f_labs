#include "parsing.h"

#include <errno.h>
#include <math.h>
#include <stdlib.h>

Status parse_double(const char *str, double *out)
{
    if (str == NULL || out == NULL) {
        return STATUS_ERR_NULL_ARGUMENT;
    }

    errno = 0;
    char *endptr = NULL;
    double value = strtod(str, &endptr);

    if (endptr == str || *endptr != '\0') {
        return STATUS_ERR_NUMBER_INVALID;
    }
    if (errno == ERANGE) {
        return STATUS_ERR_NUMBER_OUT_OF_RANGE;
    }
    if (!isfinite(value)) {
        return STATUS_ERR_NUMBER_NOT_FINITE;
    }

    *out = value;
    return STATUS_OK;
}
