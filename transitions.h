#ifndef TRANSITIONS_H
#define TRANSITIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FRAMES 100

typedef struct PIXEL {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} PIXEL;

typedef struct IMG {
    int h;
    int w;
    PIXEL **pixels;
} IMG;

static IMG *IMG_new(int h, int w) {
    // Returns a black image
    IMG *img = malloc(sizeof(IMG));
    img->h = h;
    img->w = w;
    img->pixels = malloc(h * sizeof(PIXEL*));
    for (int i=0; i<h; i++) {
        img->pixels[i] = malloc(w * sizeof(PIXEL));
        for (int j=0; j<w; j++) {
            PIXEL p = {.r=0, .g=0, .b=0};
            img->pixels[i][j] = p;
        }
    }
    return img;
}

static void freeIMG(IMG *img) {
    for (int i=0; i<img->h; i++) {
        free(img->pixels[i]);
    }
    free(img->pixels);
    free(img);
}

static IMG *generateIMG(IMG *I1, IMG *I2, int f) {
    if (I1->h != I2->h || I1->w != I2->w) {
        perror("[generateIMG()] Reference Images have different sizes.\n");
        return NULL;
    }
    IMG *nextI = IMG_new(I1->h, I1->w);
    for (int i=0; i<nextI->h; i++) {
        for (int j=0; j<nextI->w; j++) {
            int pr, pg, pb;
            pr = ((int)I2->pixels[i][j].r - (int)I1->pixels[i][j].r) * f / (FRAMES);
            pg = ((int)I2->pixels[i][j].g - (int)I1->pixels[i][j].g) * f / (FRAMES);
            pb = ((int)I2->pixels[i][j].b - (int)I1->pixels[i][j].b) * f / (FRAMES);
            
            pr = (int)I1->pixels[i][j].r + pr;
            pg = (int)I1->pixels[i][j].g + pg;
            pb = (int)I1->pixels[i][j].b + pb;

            if (pr < 0) pr = 0; 
            if (pr > 255) pr = 255;
            if (pg < 0) pg = 0; 
            if (pg > 255) pg = 255;
            if (pb < 0) pb = 0; 
            if (pb > 255) pb = 255;

            PIXEL p = {.r = (unsigned char)pr,
                       .b = (unsigned char)pb, 
                       .g = (unsigned char)pg};
            nextI->pixels[i][j] = p;
        }
    }
    return nextI;
}

static IMG *ppmToIMG(FILE *f) {
    char header[100];
    if (!fgets(header, sizeof(header), f)) return NULL;
    if (strncmp(header, "P6", 2) != 0) {
        perror("[ppmToIMG()] Erro: file not a PPM P6.\n");
        return NULL;
    }

    int h, w, max_color;
    fscanf(f, "%d %d %d", &w, &h, &max_color);
    fgetc(f);

    IMG *img = IMG_new(h, w);
    for (int i=0; i<h; i++) {
        for (int j=0; j<w; j++) {
            fread(&(img->pixels[i][j]), sizeof(PIXEL), 1, f);
        }
    }
    return img;
}

static void createPpmFile(FILE *file, IMG *img) {
    if (!file) {
        perror("[createPpmFile()] file handle is NULL.\n");
        return;
    }
    if (!img) {
        perror("[createPpmFile()] IMG Object is NULL.\n");
        return;
    }
    fprintf(file, "P6\n%d %d %d\n", img->w, img->h, 255);
    for (int i=0; i<img->h; i++) {
        for (int j=0; j<img->w; j++) {
            fwrite(&(img->pixels[i][j]), sizeof(PIXEL), 1, file);
        }
    }
}

static int createTransition(IMG *start, IMG *end) {
    for (int i=0; i<FRAMES; i++) {
        char newFileName[100];
        sprintf(newFileName, "transition-img-%d.ppm", i);
        printf("Creating %s\n", newFileName);
        FILE *newImgFile = fopen(newFileName, "wb");

        IMG *tempIMG = generateIMG(start, end, i);
        if (tempIMG == NULL) return 1;

        createPpmFile(newImgFile, tempIMG);
        fclose(newImgFile);

        freeIMG(tempIMG);
    }
    return 0;
}

#endif
