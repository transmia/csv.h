#define CSV_IMPLEMENTATION
#include "../csv.h"
#include <stdio.h>
#include <stdint.h>

uint64_t get_ns(void);

/* other syntax for csv struct */
Csv(Pornstar,
    string, name,
    string, link,
    number, video_upload,
    string, views,
    number, rank
)

int main() {
    uint64_t start = get_ns();
    PornstarArray* pornstars = csv_parse_file("pornstars.csv", Pornstar);
    uint64_t end = get_ns();
    printf("Parsed %zu records in %g milliseconds\n", pornstars->count, (double)(end - start) / 1e6); /* about 1.8ms on my machine */
    csv_free(pornstars);
    return 0;
}

#ifdef _WIN32
#include <windows.h>
uint64_t get_ns(void) {
    static LARGE_INTEGER freq;
    static int initialized;

    if (!initialized) {
        QueryPerformanceFrequency(&freq);
        initialized = 1;
    }

    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);

    uint64_t seconds = (uint64_t)(counter.QuadPart / freq.QuadPart);
    uint64_t remainder = (uint64_t)(counter.QuadPart % freq.QuadPart);

    return seconds * 1000000000ULL +
           remainder * 1000000000ULL / (uint64_t)freq.QuadPart;
}
#else
#include <time.h>
uint64_t get_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (uint64_t)ts.tv_sec * 1000000000ULL +
           (uint64_t)ts.tv_nsec;
}
#endif
