#define CSV_IMPLEMENTATION
#include "../csv.h"
#include <stdio.h>
#include <stdint.h>

/* wine-quality.csv from https://archive.ics.uci.edu/dataset/186/wine+quality (red) */

Csv(Wine,
    number, fixed_acidity,
    number, volatile_acidity,
    number, citric_acid,
    number, residual_sugar,
    number, chlorides,
    number, free_sulfur_dioxide,
    number, total_sulfur_dioxide,
    number, density,
    number, pH,
    number, sulphates,
    number, alcohol,
    number, quality
)

int high_quality_wine(const void* item) {
    const Wine* wine = (const Wine*)item;
    return wine->alcohol > 12.0 && wine->quality >= 5.0;
}

int low_alcohol_wine(const void* item) {
    const Wine* wine = (const Wine*)item;
    return wine->alcohol < 10.0;
}

int main() {
    WineArray* wines = csv_parse_file("csv/wine-quality.csv", Wine);
    printf("Parsed %zu records\n\n", wines->count);

    size_t min_quality_idx = csv_find_min(wines, Wine, quality);
    if (min_quality_idx != CSV_INVALID_INDEX) {
        Wine w = wines->items[min_quality_idx];
        printf("Minimum quality: %.0f (alcohol: %.2f)\n\n", w.quality, w.alcohol);
    }

    size_t max_alcohol_idx = csv_find_max(wines, Wine, alcohol);
    if (max_alcohol_idx != CSV_INVALID_INDEX) {
        Wine w = wines->items[max_alcohol_idx];
        printf("Maximum alcohol: %.2f (quality: %.0f)\n\n", w.alcohol, w.quality);
    }

    double target_quality = 5.0;
    size_t first_quality_5 = csv_find_first(wines, Wine, quality, target_quality);
    if (first_quality_5 != CSV_INVALID_INDEX) {
        Wine w = wines->items[first_quality_5];
        printf("First wine with quality=5: alcohol=%.2f, acidity=%.2f\n\n", 
               w.alcohol, w.fixed_acidity);
    }
    /*                              this wont work on actual C99 --- v */
    WineArray* quality_6_wines = csv_find_all(wines, Wine, quality, 6.0);
    if (quality_6_wines) {
        printf("Found %zu wines with quality=6\n", quality_6_wines->count);
        if (quality_6_wines->count > 0) {
            printf("  First one: alcohol=%.2f, sulphates=%.2f\n", 
                   quality_6_wines->items[0].alcohol, 
                   quality_6_wines->items[0].sulphates);
        }
        printf("\n");
        csv_free(quality_6_wines);
    }

    WineArray* high_quality = csv_find_all_where(wines, Wine, high_quality_wine);
    if (high_quality) {
        printf("Found %zu high-quality wines\n", high_quality->count);
        if (high_quality->count > 0) {
            printf("  Samples:\n");
            for (size_t i = 0; i < (high_quality->count < 3 ? high_quality->count : 3); ++i) {
                printf("    alcohol=%.2f, quality=%.0f\n", 
                       high_quality->items[i].alcohol, 
                       high_quality->items[i].quality);
            }
        }
        printf("\n");
        csv_free(high_quality);
    }

    WineArray* low_alcohol = csv_find_all_where(wines, Wine, low_alcohol_wine);
    if (low_alcohol) {
        printf("Found %zu low-alcohol wines\n\n", low_alcohol->count);
        csv_free(low_alcohol);
    }

    csv_free(wines);
    return 0;
}
