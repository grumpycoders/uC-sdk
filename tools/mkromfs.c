#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <dirent.h>
#include <string.h>

#define hash_init 5381

uint32_t hash_djb2(const uint8_t * str, uint32_t hash) {
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) ^ c;

    return hash;
}

void usage(const char * binname) {
    printf("Usage: %s [-d <dir>] [outfile]\n", binname);
    exit(-1);
}

void processdir(DIR * dirp, const char * curpath, FILE * outfile, const char * prefix) {
    char fullpath[1024];
    char buf[16 * 1024];
    struct dirent * ent;
    DIR * rec_dirp;
    uint32_t cur_hash = hash_djb2(curpath, hash_init);
    uint32_t size, w, hash;
    uint8_t b;
    FILE * infile;

    while ((ent = readdir(dirp))) {
        strcpy(fullpath, prefix);
        strcat(fullpath, "/");
        strcat(fullpath, curpath);
        strcat(fullpath, ent->d_name);
        switch(ent->d_type) {
        case DT_DIR:
            if (strcmp(ent->d_name, ".") == 0)
                continue;
            if (strcmp(ent->d_name, "..") == 0)
                continue;
            strcat(fullpath, "/");
            rec_dirp = opendir(fullpath);
            processdir(rec_dirp, fullpath, outfile, prefix);
            closedir(rec_dirp);
            break;
        case DT_REG:
        case DT_LNK:
            hash = hash_djb2(ent->d_name, cur_hash);
            infile = fopen(fullpath, "rb");
            if (!infile) {
                perror("opening file");
                exit(-1);
            }
            b = (hash >>  0) & 0xff; fwrite(&b, 1, 1, outfile);
            b = (hash >>  8) & 0xff; fwrite(&b, 1, 1, outfile);
            b = (hash >> 16) & 0xff; fwrite(&b, 1, 1, outfile);
            b = (hash >> 24) & 0xff; fwrite(&b, 1, 1, outfile);
            fseek(infile, 0, SEEK_END);
            size = ftell(infile);
            fseek(infile, 0, SEEK_SET);
            b = (size >>  0) & 0xff; fwrite(&b, 1, 1, outfile);
            b = (size >>  8) & 0xff; fwrite(&b, 1, 1, outfile);
            b = (size >> 16) & 0xff; fwrite(&b, 1, 1, outfile);
            b = (size >> 24) & 0xff; fwrite(&b, 1, 1, outfile);
            while (size) {
                w = size > 16 * 1024 ? 16 * 1024 : size;
                fread(buf, 1, w, infile);
                fwrite(buf, 1, w, outfile);
                size -= w;
            }
            fclose(infile);
            break;
        }
    }
}

int main(int argc, char ** argv) {
    char * binname = *argv++;
    char * o;
    char * outname = NULL;
    char * dirname = ".";
    uint64_t z = 0;
    FILE * outfile;
    DIR * dirp;

    while ((o = *argv++)) {
        if (*o == '-') {
            o++;
            switch (*o) {
            case 'd':
                dirname = *argv++;
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
        outfile = stdout;
    else
        outfile = fopen(outname, "wb");

    if (!outfile) {
        perror("opening file");
        exit(-1);
    }

    dirp = opendir(dirname);
    if (!dirp) {
        perror("opening directory");
        exit(-1);
    }

    processdir(dirp, "", outfile, dirname);
    fwrite(&z, 1, 8, outfile);
    if (outname)
        fclose(outfile);
    closedir(dirp);
}
