#include "paths.h"

#include <stddef.h>
#include <string.h>

static bool is_dir_separator(const char c)
{
#ifdef _WIN32
    return c == '/' || c == '\\';
#else
    return c == '/';
#endif
}

static bool is_name_boundary(const char c)
{
#ifdef _WIN32
    if (c == ':') {
        return true;
    }
#endif
    return is_dir_separator(c);
}

static size_t find_name_start(const char *path)
{
    size_t name_start = 0;

    for (size_t i = 0; path[i] != '\0'; i++) {
        if (is_name_boundary(path[i])) {
            name_start = i + 1;
        }
    }
    return name_start;
}

static char fold_case(const char c)
{
#ifdef _WIN32
    if (c >= 'A' && c <= 'Z') {
        return (char) (c - 'A' + 'a');
    }
#endif
    return c;
}

static const char *skip_current_dir_prefix(const char *path)
{
    while (path[0] == '.' && is_dir_separator(path[1])) {
        path += 2;
        while (is_dir_separator(*path)) {
            path++;
        }
    }
    return path;
}

Status validate_path(const char *path)
{
    if (path == NULL) {
        return STATUS_ERR_NULL_ARGUMENT;
    }
    if (path[0] == '\0') {
        return STATUS_ERR_PATH_EMPTY;
    }

    const char *name = path + find_name_start(path);

    if (name[0] == '\0' || strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
        return STATUS_ERR_PATH_NO_FILENAME;
    }
    return STATUS_OK;
}

Status paths_look_same(const char *first, const char *second, bool *out_same)
{
    if (first == NULL || second == NULL || out_same == NULL) {
        return STATUS_ERR_NULL_ARGUMENT;
    }

    const char *a = skip_current_dir_prefix(first);
    const char *b = skip_current_dir_prefix(second);

    while (*a != '\0' && *b != '\0') {
        if (is_dir_separator(*a) && is_dir_separator(*b)) {
            while (is_dir_separator(*a)) {
                a++;
            }
            while (is_dir_separator(*b)) {
                b++;
            }
            continue;
        }
        if (fold_case(*a) != fold_case(*b)) {
            *out_same = false;
            return STATUS_OK;
        }
        a++;
        b++;
    }

    *out_same = (*a == '\0' && *b == '\0');
    return STATUS_OK;
}
