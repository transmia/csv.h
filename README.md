# Mia's CSV

A tiny [stb-style](https://github.com/nothings/stb) header-only CSV parser in C99  
Supports [RFC 4180](https://datatracker.ietf.org/doc/html/rfc4180)  
I implemented this because on [one of tsoding's streams](https://www.youtube.com/watch?v=9n39SbRPXKQ) he was saying that there arent any *major* stb style csv libraries  
i know my lib will not be major but i consider it a fun exersize.  
i aimed at it being generic and nice to use, not the best fastest zero-alloc zero-copy streaming and etc, if you want like that
i recommend using Georgios Kiritsis' [Csv.h](https://github.com/GeorgeKiritsis/Csv.h).  

See [main.c](./examples/main.c) for usage.  

See also [examples/](./examples/).

## Limitations

- Due to C being an old stupid language i had to make it so you can only have 1 to 16 fields in a `CSV_STRUCT` (if you need more you can go and edit it in [csv.h](./csv.h#L613)).

- `csv_parse` and `csv_parse_cstr` require a writable source buffer because the parser null-terminates string fields in-place  

- `CSV_STRUCT` fields need to have the same name as the csv header names (but order doesnt matter!).

- `csv_compare` and `csv_write_*` could be imprecise because of floating-point (fp) errors.

- `csv_find_first` and `csv_find_all` cant handle inlined value for the `value` param (needs a var). If youre compiling with a compiler that provides `__typeof__`, this wont be an issue.

## Customization

| Define                     | Default value | Description                                                   |
|----------------------------|---------------|---------------------------------------------------------------|
| `MIAS_CSV_DEF`             | `static`      | Goes before declarations and definitions of the functions     |
| `CSV_ALLOC`                | `malloc`      | Allocator alloc                                               |
| `CSV_FREE`                 | `free`        | Allocator free                                                |
| `CSV_REALLOC`              | `realloc`     | Allocator realloc                                             |
| `CSV_INIT_CAP`             | `16`          | Default capacity for arrays (if estimated fails)              |
| `CSV_BACKSLASH_ESCAPE`     | `false`       | Enables parsing for `\"` style escapes                        |
| `CSV_TRIM_WHITESPACE`      | `false`       | Trims whitespace in string fields                             |
| `CSV_NUMBER_FMT`           | `%.15g`       | `sprintf` format arg for numbers, 15 used to reduce fp errors |
| `CSV_NUMBER_CMP_EPS`       | `1e-15`       | Epsilon for `csv_compare` for numbers to prevent fp errors    |
| `CSV_ALLOW_YES_AS_BOOLEAN` | `0`           | Allow `yes` for true in boolean fields                        |

## License

[MIT or Public Domain](./LICENSE)
