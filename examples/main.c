#define CSV_IMPLEMENTATION
#include "../csv.h"
#include <stdio.h>

CSV_STRUCT(Person,
    CSV_FIELD(number, age),
    CSV_FIELD(number, height),
    CSV_FIELD(string, name),
    CSV_FIELD(boolean, gay)
)

int main() {
    PersonArray* people = csv_parse_file("csv/people.csv", Person);
    for (int i = 0; i < people->count; ++i) {
        Person p = people->items[i];
        printf("%d, %.1f, %s, %s\n", (int)p.age, p.height, p.name, p.gay ? "true" : "false");
    }
    csv_free(people);
    return 0;
}
