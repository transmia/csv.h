# Mia's CSV

A tiny [stb-style](https://github.com/nothings/stb) header-only CSV parser in C99  
Supports [RFC 4180](https://datatracker.ietf.org/doc/html/rfc4180)  
I implemented this because on [one of tsoding's streams](https://www.youtube.com/watch?v=9n39SbRPXKQ) he was saying that there arent any *major* stb style csv libraries  
i know my lib will not be major but i consider it a fun excersize.  
i aimed at it being generic and nice to use, not the best fastest zero-alloc zero-copy streaming and etc, if you want like that
i reccomend using Georgios Kiritsis'es [Csv.h](https://github.com/GeorgeKiritsis/Csv.h).  

See [main.c](./main.c) for usage.  

See [test/main.c](./test/main.c) for speed test (11k records).

## Limitations

Due to C being an old stupid language i had to made it so you can only have 1 to 16 fields in a CSV_STRUCT (if you need more you can go and edit it in [csv.h](./csv.h#L613)).

## Customization

| Define                 | Default value | Description                                               |
|------------------------|---------------|-----------------------------------------------------------|
| `MIAS_CSV_DEF`         | `static`      | Goes before declarations and definitions of the functions |
| `CSV_ALLOC`            | `malloc`      | Allocator alloc                                           |
| `CSV_FREE`             | `free`        | Allocator free                                            |
| `CSV_REALLOC`          | `realloc`     | Allocator realloc                                         |
| `CSV_INIT_CAP`         | `16`          | Default capacity for arrays (if estimated fails)          |
| `CSV_BACKSLASH_ESCAPE` | `false`       | Enables parsing for `\"` style escapes                    |
| `CSV_TRIM_WHITESPACE`  | `false`       | Trims whitespace in string fields                         |

## License

[MIT or Public Domain](./LICENSE)
