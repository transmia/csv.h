#define CSV_IMPLEMENTATION
#define CSV_ALLOW_YES_AS_BOOLEAN 1
#include "../csv.h"
#include <stdio.h>
#include <stdint.h>

/* insurance.csv from https://github.com/stedy/Machine-Learning-with-R-datasets */

Csv(Insurance,
    number, age,
    string, sex,
    number, bmi,
    number, children,
    boolean, smoker,
    string, region,
    number, charges
)

int main() {
    InsuranceArray* insurances = csv_parse_file("csv/insurance.csv", Insurance);
    printf("Parsed %zu records\n", insurances->count);
    if (!csv_write_to_file("csv/insurance-out.csv", insurances, Insurance)) {
        printf("Failed to write CSV file\n");
        return 1;
    }
    InsuranceArray* insurances_reread = csv_parse_file("csv/insurance-out.csv", Insurance);
    printf("Re-read %zu records\n", insurances_reread->count);

    for (size_t i = 0; i < insurances->count; ++i) {
        if (csv_compare(&insurances->items[i], &insurances_reread->items[i], Insurance) != 0) {
            printf("Mismatch at record %zu\n", i);
            return 1;
        }
    }
    printf("All records match after re-reading\n");
    csv_free(insurances);
    csv_free(insurances_reread);
    return 0;
}
