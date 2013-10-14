#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(int argc, char ** argv) {
    FILE * f;
    uint32_t v, c, checksum = 0;
    int i;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        exit(1);
    }

    f = fopen(argv[1], "r+");

    if (!f) {
        perror("Failed opening input file");
        exit(1);
    }

    for (i = 0; i < 7; i++) {
        v = 0;
        v >>= 8; c = fgetc(f); c <<= 24; v |= c;
        v >>= 8; c = fgetc(f); c <<= 24; v |= c;
        v >>= 8; c = fgetc(f); c <<= 24; v |= c;
        v >>= 8; c = fgetc(f); c <<= 24; v |= c;

        checksum += v;
    }

    v = -checksum;

    fseek(f, 0x1c, SEEK_SET);

    fputc(v & 0xff, f); v >>= 8;
    fputc(v & 0xff, f); v >>= 8;
    fputc(v & 0xff, f); v >>= 8;
    fputc(v & 0xff, f); v >>= 8;

    fclose(f);

    return 0;
}
