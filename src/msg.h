#ifndef __INSTEON_MSG_H__
#define __INSTEON_MSG_H__

#include <stdint.h>

typedef enum {
    INT_FIELD
} InstFieldType;

typedef struct {
    InstFieldType type;
    int offset;
} inst_msg_field;

typedef struct {
    char *name;
    int num_fields;
    // Array of fields
    // in same order as memory
    inst_msg_field *fields;
    // Hashmap of fieldname to field
} inst_msg_fmt;

typedef struct {
    uint8_t *data;
} inst_msg;

#endif
