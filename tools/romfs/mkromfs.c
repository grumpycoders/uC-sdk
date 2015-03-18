#ifdef _WIN32
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <dirent.h>
#include <string.h>

#define hash_init 5381

static uint32_t hash_djb2(const uint8_t * str, uint32_t hash) {
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) ^ c;

    return hash;
}

static void usage(const char * binname) {
    printf("Usage: %s [-d <dir>] [-c <symbol>] [outfile]\n", binname);
    exit(-1);
}

typedef struct OUTFILE {
    FILE * outfile;
    size_t size;
} OUTFILE;

static void output(const uint8_t * buffer, size_t size, OUTFILE * outfile, int c_mode) {
    if (c_mode) {
        size_t i;
        for (i = 0; i < size; i++) {
            if (outfile->size != 0) {
                fprintf(outfile->outfile, ", ");
            }
            if ((outfile->size % 16) == 0) {
                fprintf(outfile->outfile, "\n    ");
            }
            fprintf(outfile->outfile, "0x%02x", buffer[i]);
            outfile->size++;
        }
    } else {
        fwrite(buffer, 1, size, outfile->outfile);
        outfile->size += size;
    }
}

static void processdir(DIR * dirp, const char * curpath, OUTFILE * outfile, const char * prefix, int c_mode) {
    char fullpath[1024];
    uint8_t buf[16 * 1024];
    struct dirent * ent;
    DIR * rec_dirp;
    uint32_t cur_hash = hash_djb2((const uint8_t *) curpath, hash_init);
    uint32_t size, w, hash;
    uint8_t b;
    FILE * infile;

    while ((ent = readdir(dirp))) {
        strcpy(fullpath, prefix);
        strcat(fullpath, "/");
        strcat(fullpath, curpath);
        strcat(fullpath, ent->d_name);
#ifdef _WIN32
        if (GetFileAttributes(fullpath) & FILE_ATTRIBUTE_DIRECTORY) {
#else
        struct stat st;
        stat(fullpath, &st);
        if (S_ISDIR(st.st_mode)) {
#endif
            if (strcmp(ent->d_name, ".") == 0)
                continue;
            if (strcmp(ent->d_name, "..") == 0)
                continue;
            strcat(fullpath, "/");
            rec_dirp = opendir(fullpath);
            processdir(rec_dirp, fullpath + strlen(prefix) + 1, outfile, prefix, c_mode);
            closedir(rec_dirp);
        } else {
            hash = hash_djb2((const uint8_t *) ent->d_name, cur_hash);
            infile = fopen(fullpath, "rb");
            if (!infile) {
                perror("opening input file");
                exit(-1);
            }
            b = (hash >>  0) & 0xff; output(&b, 1, outfile, c_mode);
            b = (hash >>  8) & 0xff; output(&b, 1, outfile, c_mode);
            b = (hash >> 16) & 0xff; output(&b, 1, outfile, c_mode);
            b = (hash >> 24) & 0xff; output(&b, 1, outfile, c_mode);
            fseek(infile, 0, SEEK_END);
            size = ftell(infile);
            fseek(infile, 0, SEEK_SET);
            b = (size >>  0) & 0xff; output(&b, 1, outfile, c_mode);
            b = (size >>  8) & 0xff; output(&b, 1, outfile, c_mode);
            b = (size >> 16) & 0xff; output(&b, 1, outfile, c_mode);
            b = (size >> 24) & 0xff; output(&b, 1, outfile, c_mode);
            while (size) {
                w = size > 16 * 1024 ? 16 * 1024 : size;
                w = fread(buf, 1, w, infile);
                output(buf, w, outfile, c_mode);
                size -= w;
            }
            fclose(infile);
        }
    }
}

int main(int argc, char ** argv) {
    char * binname = *argv++;
    char * o;
    char * outname = NULL;
    const char * dirname = ".";
    char * symbol;
    uint64_t z = 0;
    DIR * dirp;
    int c_mode = 0;
    OUTFILE outfile;
    outfile.size = 0;

    argc--;
    while ((o = *argv++)) {
        argc--;
        if (*o == '-') {
            o++;
            switch (*o) {
            case 'd':
                if (argc == 0) {
                    usage(binname);
                    break;
                }
                dirname = *argv++;
                argc--;
                break;
            case 'c':
                if (argc == 0) {
                    usage(binname);
                    break;
                }
                c_mode = 1;
                symbol = *argv++;
                break;
            default:
                usage(binname);
                break;
            }
        } else {
            if (outname)
                usage(binname);
            outname = o;
        }
    }

    if (!outname)
        outfile.outfile = stdout;
    else
        outfile.outfile = fopen(outname, "wb");

    if (!outfile.outfile) {
        perror("opening output file");
        exit(-1);
    }

    dirp = opendir(dirname);
    if (!dirp) {
        perror("opening directory");
        exit(-1);
    }

    if (c_mode) {
        fprintf(outfile.outfile, "const char %s[] = {", symbol);
    }

    processdir(dirp, "", &outfile, dirname, c_mode);
    output((uint8_t *) &z, 8, &outfile, c_mode);

    if (c_mode) {
        fprintf(outfile.outfile, "\n};\n\nconst unsigned %s_size = %zu;\n", symbol, outfile.size);
    }

    fclose(outfile.outfile);
    closedir(dirp);

    return 0;
}
