#include "entities/string-builder.h"

#include <stdlib.h>
#include <string.h>

int builderInit(StringBuilder *builder) {
    builder->capacity = 32;
    builder->length = 0;

    builder->data = malloc(builder->capacity);

    if (!builder->data) return -1;

    builder->data[0] = '\0';

    return 0;
}


int builderEnsureCapacity(StringBuilder *builder, int extra) {
    if (builder->length + extra + 1 <= builder->capacity) return 0;

    int newCapacity = builder->capacity;

    while (builder->length + extra + 1 > newCapacity) newCapacity *= 2;

    char *tmp = realloc(builder->data, newCapacity);

    if (!tmp) return -1;

    builder->data = tmp;
    builder->capacity = newCapacity;

    return 0;
}

int builderAppendChar(StringBuilder *builder, char c) {
    if (builderEnsureCapacity(builder, 1) == -1) return -1;

    builder->data[builder->length++] = c;
    builder->data[builder->length] = '\0';

    return 0;
}


int builderAppendString(StringBuilder *builder, const char *str) {
    size_t len = strlen(str);

    if (builderEnsureCapacity(builder, len) == -1)
        return -1;

    memcpy(builder->data + builder->length, str, len);

    builder->length += len;
    builder->data[builder->length] = '\0';

    return 0;
}