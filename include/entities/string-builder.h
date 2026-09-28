#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

typedef struct {
    char *data;
    int length;
    int capacity;
} StringBuilder;

int builderInit(StringBuilder *builder);
int builderAppendChar(StringBuilder *builder, char c); 
int builderAppendString(StringBuilder *builder, const char *str); 

#endif