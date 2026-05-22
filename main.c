#include "transitions.h"

int main(int args, char *argv []) {

    if (args < 3) {
        perror("Two few arguments :(\n");
        for (int i=0; i<args; i++) {
            if (i>0) printf(", ");
            printf("%s", argv[i]);
        }  
        return 1;
    } 

    printf("Creating Transition from %s to %s.\n", argv[1], argv[2]);
    
    FILE * startImgFile=NULL, *endImgFile=NULL;
    startImgFile = fopen(argv[1], "rb");
    if (!startImgFile)  {
        printf("Couldn't load IMG_1");
        return 1;
    }
    endImgFile = fopen(argv[2], "rb");
    if (!endImgFile) {
        printf("Couldn't load IMG_2");
        return 1;
    }

    IMG * newIMG = ppmToIMG(startImgFile), * endIMG = ppmToIMG(endImgFile);
    fclose(startImgFile);
    fclose(endImgFile);

    createTransition(newIMG, endIMG);
    
    freeIMG(newIMG);
    freeIMG(endIMG);

    return 0;
}