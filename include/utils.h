#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>

static inline bool isNumber(char* str)
{
    if (!str || *str == '\0') {
        return false; // Empty or NULL string is not a number
    }

    char* ptr = str;

    // Handle optional sign at the start
    if (*ptr == '+' || *ptr == '-') {
        ptr++;
    }

    if (*ptr == '\0') {
        return false; // A string with only a sign is not a number
    }

    bool has_digit = false;

    // Check for digits
    while (*ptr) {
        if (!isdigit((unsigned char)*ptr)) {
            return false; // Non-digit character found
        }
        has_digit = true;
        ptr++;
    }

    return has_digit; // True if at least one digit was found
}

static inline bool isEmpty(char* str)
{
    if (!str) {
        return true; // NULL strings are considered empty
    }

    while (*str) {
        if (!isspace((unsigned char)*str)) {
            return false; // Found a non-whitespace character
        }
        str++;
    }

    return true; // All characters are whitespace (or the string is empty)
}

static inline bool fileExists(const char* path)
{
    if (!path) return false;
    FILE* file = fopen(path, "r");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}

static inline bool isFloat(const char* str)
{
    if (str == NULL || *str == '\0') return false; // Null or empty string check

    const char* p = str;
    bool hasDot = false;
    bool hasDigits = false;

    // Check for optional leading sign
    if (*p == '+' || *p == '-') p++;

    // Traverse the string
    while (*p) {
        if (isdigit(*p)) {
            hasDigits = true; // At least one digit is required
        } else if (*p == '.') {
            if (hasDot) return false; // Only one dot is allowed
            hasDot = true;
        } else {
            return false; // Invalid character
        }
        p++;
    }

    return hasDigits;
}

#endif //  UTILS_H
