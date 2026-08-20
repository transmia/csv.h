#ifndef MIAS_CSV_H
#define MIAS_CSV_H

#ifndef MIAS_CSV_DEF
#define MIAS_CSV_DEF static
#endif

#ifndef CSV_ALLOC
#define CSV_ALLOC(size) malloc(size)
#endif

#ifndef CSV_FREE
#define CSV_FREE(ptr) free(ptr)
#endif

#ifndef CSV_REALLOC
#define CSV_REALLOC(ptr, size) realloc(ptr, size)
#endif

#ifndef CSV_INIT_CAP
#define CSV_INIT_CAP 16
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef enum {
    CSV_NUMBER,
    CSV_STRING,
    CSV_BOOL,
} CsvType;

typedef struct {
    const char *name;
    CsvType type;
    size_t offset;
} CsvField;

#define csv_parse(csv_data, csv_size, struct_type) \
    (struct_type##Array*)(csv__parse(csv_data, csv_size, struct_type##_fields, struct_type##_num_fields, sizeof(struct_type)))
#define csv_parse_cstr(csv_data, struct_type) \
    (struct_type##Array*)(csv__parse(csv_data, strlen(csv_data), struct_type##_fields, struct_type##_num_fields, sizeof(struct_type)))
#define csv_parse_from_file(filepath, struct_type) \
    (struct_type##Array*)(csv__parse_from_file(filepath, struct_type##_fields, struct_type##_num_fields, sizeof(struct_type)))
#define csv_parse_file csv_parse_from_file
MIAS_CSV_DEF void* csv__parse(const char* csv_data, size_t csv_size, const CsvField* fields, size_t num_fields, size_t item_size);
MIAS_CSV_DEF void* csv__parse_from_file(const char* filepath, const CsvField* fields, size_t num_fields, size_t item_size);

#define csv_field_offset(struct_type, field_name) csv__field_offset(struct_type##_fields, struct_type##_num_fields, field_name)
MIAS_CSV_DEF size_t csv__field_offset(const CsvField* fields, size_t num_fields, const char* field_name);

#define csv_free(arr) do { \
    csv__array* _csv_a = (csv__array*)(void*)(arr); \
    csv__free_array(_csv_a ? _csv_a->arena_buf : NULL, \
                   _csv_a ? _csv_a->items      : NULL, \
                   _csv_a); \
} while(0)
MIAS_CSV_DEF void csv__free_array(void* arena_buf, void* items, void* array);

#define CSV_CTYPE_number  double
#define CSV_CTYPE_string  const char*
#define CSV_CTYPE_bool    bool
#define CSV_CTYPE_boolean bool
#define CSV_CTYPE__Bool   bool

#define CSV_TYPE_number  CSV_NUMBER
#define CSV_TYPE_string  CSV_STRING
#define CSV_TYPE_bool    CSV_BOOL
#define CSV_TYPE_boolean CSV_BOOL
#define CSV_TYPE__Bool   CSV_BOOL

#define CSV_FIELD(type, name) type, name

#define CSV_DECL_FIELD(type, name) \
    CSV_CTYPE_##type name;

#define CSV_META_FIELD(struct_type, type, name) \
    { #name, CSV_TYPE_##type, offsetof(struct_type, name) }

#define CSV_NARG(...) \
    CSV_NARG_(__VA_ARGS__, \
        32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, \
        16, 15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1)

#define CSV_NARG_( \
    _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8, \
    _9, _10, _11, _12, _13, _14, _15, _16, \
    _17, _18, _19, _20, _21, _22, _23, _24, \
    _25, _26, _27, _28, _29, _30, _31, _32, N, ...) N

#define CSV_CAT_(a, b) a##b
#define CSV_CAT(a, b) CSV_CAT_(a, b)

#define CSV_STRUCT(name, ...) \
    CSV_CAT(CSV_STRUCT_, CSV_NARG(__VA_ARGS__))(name, __VA_ARGS__)
#define Csv(name, ...) \
    CSV_CAT(CSV_STRUCT_, CSV_NARG(__VA_ARGS__))(name, __VA_ARGS__)

#define CSV_STRUCT_2(name, a1, a2) \
    typedef struct { \
        CSV_DECL_FIELD(a1, a2) \
    } name; \
    typedef struct { \
        name* items; \
        size_t count; \
        size_t capacity; \
        char*  arena_buf; \
        size_t arena_used; \
        size_t arena_cap; \
    } name##Array; \
    static const CsvField name##_fields[] = { \
        CSV_META_FIELD(name, a1, a2), \
    }; \
    static const size_t name##_num_fields = 1;

#define CSV_STRUCT_4(name, a1, a2, b1, b2) \
    typedef struct { \
        CSV_DECL_FIELD(a1, a2) \
        CSV_DECL_FIELD(b1, b2) \
    } name; \
    typedef struct { \
        name* items; \
        size_t count; \
        size_t capacity; \
        char*  arena_buf; \
        size_t arena_used; \
        size_t arena_cap; \
    } name##Array; \
    static const CsvField name##_fields[] = { \
        CSV_META_FIELD(name, a1, a2), \
        CSV_META_FIELD(name, b1, b2), \
    }; \
    static const size_t name##_num_fields = 2;

#define CSV_STRUCT_6(name, a1, a2, b1, b2, c1, c2) \
    typedef struct { \
        CSV_DECL_FIELD(a1, a2) \
        CSV_DECL_FIELD(b1, b2) \
        CSV_DECL_FIELD(c1, c2) \
    } name; \
    typedef struct { \
        name* items; \
        size_t count; \
        size_t capacity; \
        char*  arena_buf; \
        size_t arena_used; \
        size_t arena_cap; \
    } name##Array; \
    static const CsvField name##_fields[] = { \
        CSV_META_FIELD(name, a1, a2), \
        CSV_META_FIELD(name, b1, b2), \
        CSV_META_FIELD(name, c1, c2), \
    }; \
    static const size_t name##_num_fields = 3;

#define CSV_STRUCT_8(name, a1, a2, b1, b2, c1, c2, d1, d2) \
    typedef struct { \
        CSV_DECL_FIELD(a1, a2) \
        CSV_DECL_FIELD(b1, b2) \
        CSV_DECL_FIELD(c1, c2) \
        CSV_DECL_FIELD(d1, d2) \
    } name; \
    typedef struct { \
        name* items; \
        size_t count; \
        size_t capacity; \
        char*  arena_buf; \
        size_t arena_used; \
        size_t arena_cap; \
    } name##Array; \
    static const CsvField name##_fields[] = { \
        CSV_META_FIELD(name, a1, a2), \
        CSV_META_FIELD(name, b1, b2), \
        CSV_META_FIELD(name, c1, c2), \
        CSV_META_FIELD(name, d1, d2), \
    }; \
    static const size_t name##_num_fields = 4;

#define CSV_STRUCT_10(name, a1, a2, b1, b2, c1, c2, d1, d2, e1, e2) \
    typedef struct { \
        CSV_DECL_FIELD(a1, a2) \
        CSV_DECL_FIELD(b1, b2) \
        CSV_DECL_FIELD(c1, c2) \
        CSV_DECL_FIELD(d1, d2) \
        CSV_DECL_FIELD(e1, e2) \
    } name; \
    typedef struct { \
        name* items; \
        size_t count; \
        size_t capacity; \
        char*  arena_buf; \
        size_t arena_used; \
        size_t arena_cap; \
    } name##Array; \
    static const CsvField name##_fields[] = { \
        CSV_META_FIELD(name, a1, a2), \
        CSV_META_FIELD(name, b1, b2), \
        CSV_META_FIELD(name, c1, c2), \
        CSV_META_FIELD(name, d1, d2), \
        CSV_META_FIELD(name, e1, e2), \
    }; \
    static const size_t name##_num_fields = 5;

#define CSV_STRUCT_12(name, a1, a2, b1, b2, c1, c2, d1, d2, e1, e2, f1, f2) \
    typedef struct { \
        CSV_DECL_FIELD(a1, a2)  \
        CSV_DECL_FIELD(b1, b2)  \
        CSV_DECL_FIELD(c1, c2)  \
        CSV_DECL_FIELD(d1, d2)  \
        CSV_DECL_FIELD(e1, e2)  \
        CSV_DECL_FIELD(f1, f2)  \
    } name; \
    typedef struct { \
        name* items; \
        size_t count; \
        size_t capacity; \
        char*  arena_buf; \
        size_t arena_used; \
        size_t arena_cap; \
    } name##Array; \
    static const CsvField name##_fields[] = { \
        CSV_META_FIELD(name, a1, a2), \
        CSV_META_FIELD(name, b1, b2), \
        CSV_META_FIELD(name, c1, c2), \
        CSV_META_FIELD(name, d1, d2), \
        CSV_META_FIELD(name, e1, e2), \
        CSV_META_FIELD(name, f1, f2), \
    }; \
    static const size_t name##_num_fields = 6;

#define CSV_STRUCT_14(name, a1, a2, b1, b2, c1, c2, d1, d2, e1, e2, f1, f2, g1, g2) \
    typedef struct { \
        CSV_DECL_FIELD(a1, a2) \
        CSV_DECL_FIELD(b1, b2) \
        CSV_DECL_FIELD(c1, c2) \
        CSV_DECL_FIELD(d1, d2) \
        CSV_DECL_FIELD(e1, e2) \
        CSV_DECL_FIELD(f1, f2) \
        CSV_DECL_FIELD(g1, g2) \
    } name; \
    typedef struct { \
        name* items; \
        size_t count; \
        size_t capacity; \
        char*  arena_buf; \
        size_t arena_used; \
        size_t arena_cap; \
    } name##Array; \
    static const CsvField name##_fields[] = { \
        CSV_META_FIELD(name, a1, a2), \
        CSV_META_FIELD(name, b1, b2), \
        CSV_META_FIELD(name, c1, c2), \
        CSV_META_FIELD(name, d1, d2), \
        CSV_META_FIELD(name, e1, e2), \
        CSV_META_FIELD(name, f1, f2), \
        CSV_META_FIELD(name, g1, g2), \
    }; \
    static const size_t name##_num_fields = 7;

#define CSV_STRUCT_16(name, a1, a2, b1, b2, c1, c2, d1, d2, e1, e2, f1, f2, g1, g2, h1, h2) \
    typedef struct { \
        CSV_DECL_FIELD(a1, a2) \
        CSV_DECL_FIELD(b1, b2) \
        CSV_DECL_FIELD(c1, c2) \
        CSV_DECL_FIELD(d1, d2) \
        CSV_DECL_FIELD(e1, e2) \
        CSV_DECL_FIELD(f1, f2) \
        CSV_DECL_FIELD(g1, g2) \
        CSV_DECL_FIELD(h1, h2) \
    } name; \
    typedef struct { \
        name* items; \
        size_t count; \
        size_t capacity; \
        char*  arena_buf; \
        size_t arena_used; \
        size_t arena_cap; \
    } name##Array; \
    static const CsvField name##_fields[] = { \
        CSV_META_FIELD(name, a1, a2), \
        CSV_META_FIELD(name, b1, b2), \
        CSV_META_FIELD(name, c1, c2), \
        CSV_META_FIELD(name, d1, d2), \
        CSV_META_FIELD(name, e1, e2), \
        CSV_META_FIELD(name, f1, f2), \
        CSV_META_FIELD(name, g1, g2), \
        CSV_META_FIELD(name, h1, h2), \
    }; \
    static const size_t name##_num_fields = 8;

#define CSV_STRUCT_18(name, a1, a2, b1, b2, c1, c2, d1, d2, e1, e2, f1, f2, g1, g2, h1, h2, i1, i2) \
    typedef struct { \
        CSV_DECL_FIELD(a1, a2) \
        CSV_DECL_FIELD(b1, b2) \
        CSV_DECL_FIELD(c1, c2) \
        CSV_DECL_FIELD(d1, d2) \
        CSV_DECL_FIELD(e1, e2) \
        CSV_DECL_FIELD(f1, f2) \
        CSV_DECL_FIELD(g1, g2) \
        CSV_DECL_FIELD(h1, h2) \
        CSV_DECL_FIELD(i1, i2) \
    } name; \
    typedef struct { \
        name* items; \
        size_t count; \
        size_t capacity; \
        char*  arena_buf; \
        size_t arena_used; \
        size_t arena_cap; \
    } name##Array; \
    static const CsvField name##_fields[] = { \
        CSV_META_FIELD(name, a1, a2), \
        CSV_META_FIELD(name, b1, b2), \
        CSV_META_FIELD(name, c1, c2), \
        CSV_META_FIELD(name, d1, d2), \
        CSV_META_FIELD(name, e1, e2), \
        CSV_META_FIELD(name, f1, f2), \
        CSV_META_FIELD(name, g1, g2), \
        CSV_META_FIELD(name, h1, h2), \
        CSV_META_FIELD(name, i1, i2), \
    }; \
    static const size_t name##_num_fields = 9;

#define CSV_STRUCT_20(name, a1, a2, b1, b2, c1, c2, d1, d2, e1, e2, f1, f2, g1, g2, h1, h2, i1, i2, j1, j2) \
    typedef struct { \
        CSV_DECL_FIELD(a1, a2) \
        CSV_DECL_FIELD(b1, b2) \
        CSV_DECL_FIELD(c1, c2) \
        CSV_DECL_FIELD(d1, d2) \
        CSV_DECL_FIELD(e1, e2) \
        CSV_DECL_FIELD(f1, f2) \
        CSV_DECL_FIELD(g1, g2) \
        CSV_DECL_FIELD(h1, h2) \
        CSV_DECL_FIELD(i1, i2) \
        CSV_DECL_FIELD(j1, j2) \
    } name; \
    typedef struct { \
        name* items; \
        size_t count; \
        size_t capacity; \
        char*  arena_buf; \
        size_t arena_used; \
        size_t arena_cap; \
    } name##Array; \
    static const CsvField name##_fields[] = { \
        CSV_META_FIELD(name, a1, a2), \
        CSV_META_FIELD(name, b1, b2), \
        CSV_META_FIELD(name, c1, c2), \
        CSV_META_FIELD(name, d1, d2), \
        CSV_META_FIELD(name, e1, e2), \
        CSV_META_FIELD(name, f1, f2), \
        CSV_META_FIELD(name, g1, g2), \
        CSV_META_FIELD(name, h1, h2), \
        CSV_META_FIELD(name, i1, i2), \
        CSV_META_FIELD(name, j1, j2), \
    }; \
    static const size_t name##_num_fields = 10;

#define CSV_STRUCT_22(name, a1, a2, b1, b2, c1, c2, d1, d2, e1, e2, f1, f2, g1, g2, h1, h2, i1, i2, j1, j2, k1, k2) \
    typedef struct { \
        CSV_DECL_FIELD(a1, a2) \
        CSV_DECL_FIELD(b1, b2) \
        CSV_DECL_FIELD(c1, c2) \
        CSV_DECL_FIELD(d1, d2) \
        CSV_DECL_FIELD(e1, e2) \
        CSV_DECL_FIELD(f1, f2) \
        CSV_DECL_FIELD(g1, g2) \
        CSV_DECL_FIELD(h1, h2) \
        CSV_DECL_FIELD(i1, i2) \
        CSV_DECL_FIELD(j1, j2) \
        CSV_DECL_FIELD(k1, k2) \
    } name; \
    typedef struct { \
        name* items; \
        size_t count; \
        size_t capacity; \
        char*  arena_buf; \
        size_t arena_used; \
        size_t arena_cap; \
    } name##Array; \
    static const CsvField name##_fields[] = { \
        CSV_META_FIELD(name, a1, a2), \
        CSV_META_FIELD(name, b1, b2), \
        CSV_META_FIELD(name, c1, c2), \
        CSV_META_FIELD(name, d1, d2), \
        CSV_META_FIELD(name, e1, e2), \
        CSV_META_FIELD(name, f1, f2), \
        CSV_META_FIELD(name, g1, g2), \
        CSV_META_FIELD(name, h1, h2), \
        CSV_META_FIELD(name, i1, i2), \
        CSV_META_FIELD(name, j1, j2), \
        CSV_META_FIELD(name, k1, k2), \
    }; \
    static const size_t name##_num_fields = 11;

#define CSV_STRUCT_24(name, a1, a2, b1, b2, c1, c2, d1, d2, e1, e2, f1, f2, g1, g2, h1, h2, i1, i2, j1, j2, k1, k2, l1, l2) \
    typedef struct { \
        CSV_DECL_FIELD(a1, a2) \
        CSV_DECL_FIELD(b1, b2) \
        CSV_DECL_FIELD(c1, c2) \
        CSV_DECL_FIELD(d1, d2) \
        CSV_DECL_FIELD(e1, e2) \
        CSV_DECL_FIELD(f1, f2) \
        CSV_DECL_FIELD(g1, g2) \
        CSV_DECL_FIELD(h1, h2) \
        CSV_DECL_FIELD(i1, i2) \
        CSV_DECL_FIELD(j1, j2) \
        CSV_DECL_FIELD(k1, k2) \
        CSV_DECL_FIELD(l1, l2) \
    } name; \
    typedef struct { \
        name* items; \
        size_t count; \
        size_t capacity; \
        char*  arena_buf; \
        size_t arena_used; \
        size_t arena_cap; \
    } name##Array; \
    static const CsvField name##_fields[] = { \
        CSV_META_FIELD(name, a1, a2), \
        CSV_META_FIELD(name, b1, b2), \
        CSV_META_FIELD(name, c1, c2), \
        CSV_META_FIELD(name, d1, d2), \
        CSV_META_FIELD(name, e1, e2), \
        CSV_META_FIELD(name, f1, f2), \
        CSV_META_FIELD(name, g1, g2), \
        CSV_META_FIELD(name, h1, h2), \
        CSV_META_FIELD(name, i1, i2), \
        CSV_META_FIELD(name, j1, j2), \
        CSV_META_FIELD(name, k1, k2), \
        CSV_META_FIELD(name, l1, l2), \
    }; \
    static const size_t name##_num_fields = 12;

#define CSV_STRUCT_26(name, a1, a2, b1, b2, c1, c2, d1, d2, e1, e2, f1, f2, g1, g2, h1, h2, i1, i2, j1, j2, k1, k2, l1, l2, m1, m2) \
    typedef struct { \
        CSV_DECL_FIELD(a1, a2) \
        CSV_DECL_FIELD(b1, b2) \
        CSV_DECL_FIELD(c1, c2) \
        CSV_DECL_FIELD(d1, d2) \
        CSV_DECL_FIELD(e1, e2) \
        CSV_DECL_FIELD(f1, f2) \
        CSV_DECL_FIELD(g1, g2) \
        CSV_DECL_FIELD(h1, h2) \
        CSV_DECL_FIELD(i1, i2) \
        CSV_DECL_FIELD(j1, j2) \
        CSV_DECL_FIELD(k1, k2) \
        CSV_DECL_FIELD(l1, l2) \
        CSV_DECL_FIELD(m1, m2) \
    } name; \
    typedef struct { \
        name* items; \
        size_t count; \
        size_t capacity; \
        char*  arena_buf; \
        size_t arena_used; \
        size_t arena_cap; \
    } name##Array; \
    static const CsvField name##_fields[] = { \
        CSV_META_FIELD(name, a1, a2), \
        CSV_META_FIELD(name, b1, b2), \
        CSV_META_FIELD(name, c1, c2), \
        CSV_META_FIELD(name, d1, d2), \
        CSV_META_FIELD(name, e1, e2), \
        CSV_META_FIELD(name, f1, f2), \
        CSV_META_FIELD(name, g1, g2), \
        CSV_META_FIELD(name, h1, h2), \
        CSV_META_FIELD(name, i1, i2), \
        CSV_META_FIELD(name, j1, j2), \
        CSV_META_FIELD(name, k1, k2), \
        CSV_META_FIELD(name, l1, l2), \
        CSV_META_FIELD(name, m1, m2), \
    }; \
    static const size_t name##_num_fields = 13;

#define CSV_STRUCT_28(name, a1, a2, b1, b2, c1, c2, d1, d2, e1, e2, f1, f2, g1, g2, h1, h2, i1, i2, j1, j2, k1, k2, l1, l2, m1, m2, n1, n2) \
    typedef struct { \
        CSV_DECL_FIELD(a1, a2) \
        CSV_DECL_FIELD(b1, b2) \
        CSV_DECL_FIELD(c1, c2) \
        CSV_DECL_FIELD(d1, d2) \
        CSV_DECL_FIELD(e1, e2) \
        CSV_DECL_FIELD(f1, f2) \
        CSV_DECL_FIELD(g1, g2) \
        CSV_DECL_FIELD(h1, h2) \
        CSV_DECL_FIELD(i1, i2) \
        CSV_DECL_FIELD(j1, j2) \
        CSV_DECL_FIELD(k1, k2) \
        CSV_DECL_FIELD(l1, l2) \
        CSV_DECL_FIELD(m1, m2) \
        CSV_DECL_FIELD(n1, n2) \
    } name; \
    typedef struct { \
        name* items; \
        size_t count; \
        size_t capacity; \
        char*  arena_buf; \
        size_t arena_used; \
        size_t arena_cap; \
    } name##Array; \
    static const CsvField name##_fields[] = { \
        CSV_META_FIELD(name, a1, a2), \
        CSV_META_FIELD(name, b1, b2), \
        CSV_META_FIELD(name, c1, c2), \
        CSV_META_FIELD(name, d1, d2), \
        CSV_META_FIELD(name, e1, e2), \
        CSV_META_FIELD(name, f1, f2), \
        CSV_META_FIELD(name, g1, g2), \
        CSV_META_FIELD(name, h1, h2), \
        CSV_META_FIELD(name, i1, i2), \
        CSV_META_FIELD(name, j1, j2), \
        CSV_META_FIELD(name, k1, k2), \
        CSV_META_FIELD(name, l1, l2), \
        CSV_META_FIELD(name, m1, m2), \
        CSV_META_FIELD(name, n1, n2), \
    }; \
    static const size_t name##_num_fields = 14;

#define CSV_STRUCT_30(name, a1, a2, b1, b2, c1, c2, d1, d2, e1, e2, f1, f2, g1, g2, h1, h2, i1, i2, j1, j2, k1, k2, l1, l2, m1, m2, n1, n2, o1, o2) \
    typedef struct { \
        CSV_DECL_FIELD(a1, a2) \
        CSV_DECL_FIELD(b1, b2) \
        CSV_DECL_FIELD(c1, c2) \
        CSV_DECL_FIELD(d1, d2) \
        CSV_DECL_FIELD(e1, e2) \
        CSV_DECL_FIELD(f1, f2) \
        CSV_DECL_FIELD(g1, g2) \
        CSV_DECL_FIELD(h1, h2) \
        CSV_DECL_FIELD(i1, i2) \
        CSV_DECL_FIELD(j1, j2) \
        CSV_DECL_FIELD(k1, k2) \
        CSV_DECL_FIELD(l1, l2) \
        CSV_DECL_FIELD(m1, m2) \
        CSV_DECL_FIELD(n1, n2) \
        CSV_DECL_FIELD(o1, o2) \
    } name; \
    typedef struct { \
        name* items; \
        size_t count; \
        size_t capacity; \
        char*  arena_buf; \
        size_t arena_used; \
        size_t arena_cap; \
    } name##Array; \
    static const CsvField name##_fields[] = { \
        CSV_META_FIELD(name, a1, a2), \
        CSV_META_FIELD(name, b1, b2), \
        CSV_META_FIELD(name, c1, c2), \
        CSV_META_FIELD(name, d1, d2), \
        CSV_META_FIELD(name, e1, e2), \
        CSV_META_FIELD(name, f1, f2), \
        CSV_META_FIELD(name, g1, g2), \
        CSV_META_FIELD(name, h1, h2), \
        CSV_META_FIELD(name, i1, i2), \
        CSV_META_FIELD(name, j1, j2), \
        CSV_META_FIELD(name, k1, k2), \
        CSV_META_FIELD(name, l1, l2), \
        CSV_META_FIELD(name, m1, m2), \
        CSV_META_FIELD(name, n1, n2), \
        CSV_META_FIELD(name, o1, o2), \
    }; \
    static const size_t name##_num_fields = 15;

#define CSV_STRUCT_32(name, a1, a2, b1, b2, c1, c2, d1, d2, e1, e2, f1, f2, g1, g2, h1, h2, i1, i2, j1, j2, k1, k2, l1, l2, m1, m2, n1, n2, o1, o2, p1, p2) \
    typedef struct { \
        CSV_DECL_FIELD(a1, a2) \
        CSV_DECL_FIELD(b1, b2) \
        CSV_DECL_FIELD(c1, c2) \
        CSV_DECL_FIELD(d1, d2) \
        CSV_DECL_FIELD(e1, e2) \
        CSV_DECL_FIELD(f1, f2) \
        CSV_DECL_FIELD(g1, g2) \
        CSV_DECL_FIELD(h1, h2) \
        CSV_DECL_FIELD(i1, i2) \
        CSV_DECL_FIELD(j1, j2) \
        CSV_DECL_FIELD(k1, k2) \
        CSV_DECL_FIELD(l1, l2) \
        CSV_DECL_FIELD(m1, m2) \
        CSV_DECL_FIELD(n1, n2) \
        CSV_DECL_FIELD(o1, o2) \
        CSV_DECL_FIELD(p1, p2) \
    } name; \
    typedef struct { \
        name* items; \
        size_t count; \
        size_t capacity; \
        char*  arena_buf; \
        size_t arena_used; \
        size_t arena_cap; \
    } name##Array; \
    static const CsvField name##_fields[] = { \
        CSV_META_FIELD(name, a1, a2), \
        CSV_META_FIELD(name, b1, b2), \
        CSV_META_FIELD(name, c1, c2), \
        CSV_META_FIELD(name, d1, d2), \
        CSV_META_FIELD(name, e1, e2), \
        CSV_META_FIELD(name, f1, f2), \
        CSV_META_FIELD(name, g1, g2), \
        CSV_META_FIELD(name, h1, h2), \
        CSV_META_FIELD(name, i1, i2), \
        CSV_META_FIELD(name, j1, j2), \
        CSV_META_FIELD(name, k1, k2), \
        CSV_META_FIELD(name, l1, l2), \
        CSV_META_FIELD(name, m1, m2), \
        CSV_META_FIELD(name, n1, n2), \
        CSV_META_FIELD(name, o1, o2), \
        CSV_META_FIELD(name, p1, p2), \
    }; \
    static const size_t name##_num_fields = 16;

/* Custom-sized CSV_STRUCT macro:
 * Copy the code beneath this and replace everything in <> with your desired number of fields.
 * Take that 'n' is the amount of fields you want.
 * Replace all <...> with the remaining fields after the first 3.
 * For example, if you want 5 fields, replace <...> with d1, d2, e1, e2 or the CSV_DECL_FIELD and CSV_META_FIELD macros.
 * ============================================================== */
/*
#define CSV_STRUCT_<2n>(name, a1, a2, b1, b2, c1, c2, <...>) \
    typedef struct { \
        CSV_DECL_FIELD(a1, a2) \
        CSV_DECL_FIELD(b1, b2) \
        CSV_DECL_FIELD(c1, c2) \
        <...> \
    } name; \
    typedef struct { \
        name* items; \
        size_t count; \
        size_t capacity; \
        char*  arena_buf; \
        size_t arena_used; \
        size_t arena_cap; \
    } name##Array; \
    static const CsvField name##_fields[] = { \
        CSV_META_FIELD(name, a1, a2), \
        CSV_META_FIELD(name, b1, b2), \
        CSV_META_FIELD(name, c1, c2), \
        <...> \
    }; \
    static const size_t name##_num_fields = <n>;
*/

#endif /* MIAS_CSV_H */

#ifdef CSV_IMPLEMENTATION

#include <stdlib.h>
#include <stdio.h>

/* compatible with nob's da */
typedef struct {
    void*  items;
    size_t count;
    size_t capacity;
    char*  arena_buf;
    size_t arena_used;
    size_t arena_cap;
} csv__array;

typedef struct {
    char*  data;
    size_t cap;
} csv__scratch;

static char* csv__arena_alloc(csv__array* arr, size_t n) {
    if (arr->arena_used + n > arr->arena_cap) {
        size_t new_cap = arr->arena_cap ? arr->arena_cap * 2 : (1 << 20);
        while (new_cap < arr->arena_used + n) new_cap *= 2;
        char* tmp = (char*)CSV_REALLOC(arr->arena_buf, new_cap);
        if (!tmp) return NULL;
        arr->arena_buf = tmp;
        arr->arena_cap = new_cap;
    }
    char* p = arr->arena_buf + arr->arena_used;
    arr->arena_used += n;
    return p;
}

static int csv__scratch_grow(csv__scratch* s, size_t need) {
    if (need <= s->cap) return 1;
    size_t new_cap = s->cap ? s->cap * 2 : 4096;
    while (new_cap < need) new_cap *= 2;
    char* tmp = (char*)CSV_REALLOC(s->data, new_cap);
    if (!tmp) return 0;
    s->data = tmp;
    s->cap  = new_cap;
    return 1;
}

#define CSV__IS_SPACE(c) ((c) == ' ' || (c) == '\t')

static const char* csv__parse_field(
        const char* src, size_t src_size, size_t* pos,
        int* is_last_in_record, csv__scratch* scratch, size_t* out_len) {

    size_t i = *pos;

#ifndef CSV_TRIM_WHITESPACE
    size_t lead = i;
    while (lead < src_size && CSV__IS_SPACE(src[lead])) ++lead;
#else
    size_t lead = i;
    while (lead < src_size && CSV__IS_SPACE(src[lead])) ++lead;
#endif

    if (lead < src_size && src[lead] == '"') {
        i = lead + 1;
        size_t len = 0;

        {
            size_t est = 64;
            if (!csv__scratch_grow(scratch, est)) return NULL;
        }

        while (i < src_size) {
            char c = src[i];

#ifdef CSV_BACKSLASH_ESCAPE
            if (c == '\\' && i + 1 < src_size) {
                char next = src[i + 1];
                char decoded;
                switch (next) {
                    case '"':  decoded = '"';  break;
                    case '\\': decoded = '\\'; break;
                    case 'n':  decoded = '\n'; break;
                    case 'r':  decoded = '\r'; break;
                    case 't':  decoded = '\t'; break;
                    default:
                        if (!csv__scratch_grow(scratch, len + 2)) return NULL;
                        scratch->data[len++] = '\\';
                        scratch->data[len++] = next;
                        i += 2;
                        continue;
                }
                if (!csv__scratch_grow(scratch, len + 1)) return NULL;
                scratch->data[len++] = decoded;
                i += 2;
                continue;
            }
#endif
            if (c == '"') {
                if (i + 1 < src_size && src[i + 1] == '"') {
                    if (!csv__scratch_grow(scratch, len + 1)) return NULL;
                    scratch->data[len++] = '"';
                    i += 2;
                } else {
                    ++i;
                    break;
                }
            } else {
                if (!csv__scratch_grow(scratch, len + 1)) return NULL;
                scratch->data[len++] = c;
                ++i;
            }
        }

        while (i < src_size && CSV__IS_SPACE(src[i])) ++i;

        if (!csv__scratch_grow(scratch, len + 1)) return NULL;
        scratch->data[len] = '\0';
        *out_len = len;

        if (i >= src_size) {
            *is_last_in_record = 1;
        } else if (src[i] == ',') {
            *is_last_in_record = 0;
            ++i;
        } else {
            *is_last_in_record = 1;
            if (src[i] == '\r') ++i;
            if (i < src_size && src[i] == '\n') ++i;
        }
        *pos = i;
        return scratch->data;

    } else {
        const char* base  = src + i;
        const char* fence = src + src_size;
        const char* p     = base;

        while (p < fence) {
            unsigned char ch = (unsigned char)*p;
            if (ch == ',' || ch == '\n' || ch == '\r') break;
            ++p;
        }

        size_t field_len = (size_t)(p - base);

#ifdef CSV_TRIM_WHITESPACE
        while (field_len > 0 && CSV__IS_SPACE((unsigned char)base[field_len - 1]))
            --field_len;
#endif

        *out_len = field_len;

        i += (size_t)(p - base);

        if (i >= src_size) {
            *is_last_in_record = 1;
        } else if (src[i] == ',') {
            *is_last_in_record = 0;
            ++i;
        } else {
            *is_last_in_record = 1;
            if (src[i] == '\r') ++i;
            if (i < src_size && src[i] == '\n') ++i;
        }
        *pos = i;
        return base;
    }
}

static double csv__fast_atod(const char* s, size_t len) {
    const char* p   = s;
    const char* end = s + len;

    int neg = 0;
    if (p < end && *p == '-') { neg = 1; ++p; }
    else if (p < end && *p == '+') { ++p; }

    uint64_t ipart = 0;
    while (p < end && (unsigned char)(*p - '0') <= 9u) {
        ipart = ipart * 10 + (uint64_t)(*p - '0');
        ++p;
    }

    double result = (double)ipart;

    if (p < end && *p == '.') {
        ++p;
        double fscale = 0.1;
        while (p < end && (unsigned char)(*p - '0') <= 9u) {
            result += (*p - '0') * fscale;
            fscale *= 0.1;
            ++p;
        }
    }

    if (p < end && (*p == 'e' || *p == 'E')) {
        ++p;
        int eneg = 0;
        if (p < end && *p == '-') { eneg = 1; ++p; }
        else if (p < end && *p == '+') { ++p; }
        int exp = 0;
        while (p < end && (unsigned char)(*p - '0') <= 9u) {
            exp = exp * 10 + (*p - '0'); ++p;
        }
        static const double pow10[23] = {
            1e0,  1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,
            1e8,  1e9,  1e10, 1e11, 1e12, 1e13, 1e14, 1e15,
            1e16, 1e17, 1e18, 1e19, 1e20, 1e21, 1e22
        };
        if (exp <= 22) {
            result = eneg ? result / pow10[exp] : result * pow10[exp];
        } else {
            char tmp[64];
            size_t copy_len = len < 63 ? len : 63;
            memcpy(tmp, s, copy_len);
            tmp[copy_len] = '\0';
            char* endp;
            result = strtod(tmp, &endp);
            return neg ? -result : result;
        }
    } else if (p != end) {
        char tmp[64];
        size_t copy_len = len < 63 ? len : 63;
        memcpy(tmp, s, copy_len);
        tmp[copy_len] = '\0';
        char* endp;
        result = strtod(tmp, &endp);
        return neg ? -result : result;
    }

    return neg ? -result : result;
}

static void* csv__array_push(csv__array* arr, size_t item_size) {
    if (arr->count == arr->capacity) {
        size_t new_cap = arr->capacity * 2;
        void*  tmp     = CSV_REALLOC(arr->items, item_size * new_cap);
        if (!tmp) return NULL;
        arr->items    = tmp;
        arr->capacity = new_cap;
        memset((char*)arr->items + arr->count * item_size, 0,
               (new_cap - arr->count) * item_size);
    }
    void* slot = (char*)arr->items + arr->count * item_size;
    arr->count++;
    return slot;
}

static int csv__set_field(void* item, const CsvField* f,
                           const char* val, size_t val_len,
                           csv__array* arr,
                           int val_from_scratch) {
    void* dst = (char*)item + f->offset;
    switch (f->type) {
        case CSV_STRING: {
            if (val_from_scratch) {
                char* copy = csv__arena_alloc(arr, val_len + 1);
                if (!copy) return 0;
                memcpy(copy, val, val_len);
                copy[val_len] = '\0';
                *(const char**)dst = copy;
            } else {
                *(const char**)dst = val;
                ((char*)val)[val_len] = '\0';
            }
            break;
        }
        case CSV_NUMBER:
            *(double*)dst = csv__fast_atod(val, val_len);
            break;
        case CSV_BOOL: {
            const char* s   = val;
            size_t      slen = val_len;
            while (slen > 0 && CSV__IS_SPACE((unsigned char)*s)) { ++s; --slen; }
            while (slen > 0 && CSV__IS_SPACE((unsigned char)s[slen - 1])) --slen;
            *(bool*)dst = (slen == 4 && memcmp(s, "true", 4) == 0)
                       || (slen == 1 && s[0] == '1');
            break;
        }
    }
    return 1;
}

static void* csv__parse_core(
        char*       writable_src,
        const char* readonly_src,
        size_t      src_size,
        const CsvField* fields, size_t num_fields, size_t item_size)
{
    const char* src = writable_src ? writable_src : readonly_src;

    csv__array* array = (csv__array*)CSV_ALLOC(sizeof(csv__array));
    if (!array) return NULL;

    size_t estimated = src_size / 50 + 16;
    if (estimated < (size_t)CSV_INIT_CAP) estimated = CSV_INIT_CAP;
    array->items = CSV_ALLOC(item_size * estimated);
    if (!array->items) { CSV_FREE(array); return NULL; }
    array->count      = 0;
    array->capacity   = estimated;
    array->arena_buf  = writable_src;
    array->arena_used = src_size;
    array->arena_cap  = writable_src ? src_size : 0;
    memset(array->items, 0, item_size * estimated);

    csv__scratch scratch = { NULL, 0 };

    size_t pos = 0;

    size_t  header_cols = 0;
    size_t  header_cap  = 32;
    size_t* col_map     = (size_t*)CSV_ALLOC(sizeof(size_t) * header_cap);
    if (!col_map) { CSV_FREE(array->items); CSV_FREE(array); return NULL; }

    {
        int last = 0;
        while (!last && pos <= src_size) {
            int    is_last  = 0;
            size_t name_len = 0;
            const char* name_ptr = csv__parse_field(src, src_size, &pos, &is_last, &scratch, &name_len);
            if (!name_ptr) goto oom;
            last = is_last;

            if (header_cols == header_cap) {
                header_cap *= 2;
                size_t* tmp = (size_t*)CSV_REALLOC(col_map, sizeof(size_t) * header_cap);
                if (!tmp) goto oom;
                col_map = tmp;
            }

            char name_buf[256];
            size_t copy_len = name_len < 255 ? name_len : 255;
            memcpy(name_buf, name_ptr, copy_len);
            name_buf[copy_len] = '\0';

            size_t fi = num_fields;
            for (size_t k = 0; k < num_fields; ++k) {
                if (strcmp(fields[k].name, name_buf) == 0) { fi = k; break; }
            }
            col_map[header_cols++] = fi;

            if (header_cols == 0 && pos >= src_size) break;
        }
    }

    while (pos < src_size) {
        unsigned char c0 = (unsigned char)src[pos];
        if (c0 == '\r') {
            pos += (pos + 1 < src_size && src[pos + 1] == '\n') ? 2 : 1;
            continue;
        }
        if (c0 == '\n') { pos++; continue; }

        void* item = csv__array_push(array, item_size);
        if (!item) goto oom;

        size_t col  = 0;
        int    last = 0;

        while (!last) {
            int    is_last   = 0;
            size_t val_len   = 0;
            const char* val  = csv__parse_field(src, src_size, &pos, &is_last, &scratch, &val_len);
            if (!val) goto oom;
            last = is_last;

            int from_scratch = (val == scratch.data);

            if (col < header_cols) {
                size_t fi = col_map[col];
                if (fi < num_fields) {
                    const CsvField* f = &fields[fi];
                    if (f->type == CSV_STRING && !from_scratch && !writable_src) {
                        char* copy = csv__arena_alloc(array, val_len + 1);
                        if (!copy) goto oom;
                        memcpy(copy, val, val_len);
                        copy[val_len] = '\0';
                        *(const char**)((char*)item + f->offset) = copy;
                    } else {
                        if (!csv__set_field(item, f, val, val_len, array, from_scratch)) goto oom;
                    }
                }
            }
            col++;
        }

        if (col != header_cols) {
            array->count--;
        }
    }

    CSV_FREE(scratch.data);
    CSV_FREE(col_map);
    return array;

oom:
    CSV_FREE(scratch.data);
    CSV_FREE(col_map);
    if (!writable_src) CSV_FREE(array->arena_buf);
    CSV_FREE(array->items);
    CSV_FREE(array);
    return NULL;
}

MIAS_CSV_DEF void* csv__parse(const char* csv_data, size_t csv_size,
                               const CsvField* fields, size_t num_fields,
                               size_t item_size) {
    return csv__parse_core(NULL, csv_data, csv_size, fields, num_fields, item_size);
}

MIAS_CSV_DEF void* csv__parse_from_file(const char* filepath, const CsvField* fields, size_t num_fields, size_t item_size) {
    FILE* file = fopen(filepath, "rb");
    if (!file) return NULL;

    if (fseek(file, 0, SEEK_END) != 0) { fclose(file); return NULL; }
    long file_size_l = ftell(file);
    if (file_size_l < 0)                  { fclose(file); return NULL; }
    if (fseek(file, 0, SEEK_SET) != 0)  { fclose(file); return NULL; }

    size_t file_size2 = (size_t)file_size_l;

    char* data = (char*)CSV_ALLOC(file_size2 + 1);
    if (!data && file_size2 != 0) { fclose(file); return NULL; }
    data[file_size2] = '\0';

    size_t nread = fread(data, 1, file_size2, file);
    fclose(file);
    if (nread != file_size2) { CSV_FREE(data); return NULL; }

    return csv__parse_core(data, NULL, file_size2,
                           fields, num_fields, item_size);
}

MIAS_CSV_DEF size_t csv__field_offset(const CsvField* fields, size_t num_fields, const char* field_name) {
    for (size_t i = 0; i < num_fields; ++i) {
        if (strcmp(fields[i].name, field_name) == 0) {
            return fields[i].offset;
        }
    }
    return (size_t)-1;
}

MIAS_CSV_DEF void csv__free_array(void* arena_buf, void* items, void* array) {
    CSV_FREE(arena_buf);
    CSV_FREE(items);
    CSV_FREE(array);
}

#endif /* CSV_IMPLEMENTATION */
