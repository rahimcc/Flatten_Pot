#include <stdio.h>
#include <stdlib.h>
#include "AzPot.h"
#include <string.h>


int main (int argc, char* argv[]){
FILE** bmp=(FILE **) malloc(360*sizeof(FILE*));
FILE *fileBuffer, *flattenPOT;
int  shifting[360], POTsize=0;
unsigned char Bytebuffer; 
SPixel pixel;


printf("%s\n",argv[1]);
 if ((fileBuffer= fopen(argv[1],"r"))==NULL){
    printf("File %s doesn't exists\n", argv[1]);
    return 0;
 }

if ((flattenPOT=fopen("flattenPOT.bmp","w"))==NULL) {
    printf("Can't open file ");
    return 0;
     }

for (int i=0;i<54;i++){    //copying all information to file FlattenPOT, width will be changed 
    fread(&Bytebuffer,1,1,fileBuffer);
    fwrite (&Bytebuffer,1,1,flattenPOT);
}
 fclose(fileBuffer);


char *name;
 for (int i=0;i<argc-1;i++){
     name=argv[i+1];
     if (( bmp[i]=fopen(argv[i+1],"r"))==NULL){ //openin all bmp images 
                                  printf ("File %s doesn't exists \n", argv[i+1]);
                                  return 0;
                               } 
                               fseek(bmp[i],54,SEEK_SET);
                               fseek(bmp[i],200*4,SEEK_CUR);   
     printf(" %s \n", name);
    name[strlen(name)-9]='\0'; //string manipulation to get name correspondin txt file 
    name=name+16;
    name=strcat(name,".txt");
    printf(" %s \n", name);
   fileBuffer= fopen(name,"r");
   shifting[i]=findMaxShifting(fileBuffer); // finds max shifting value 
   fclose(fileBuffer);
   POTsize=POTsize+shifting[i]; // final pot size is sum of maximum shifting values of all images 
 }

      for (int i=0;i<2230;i++){       
            for (int k=0;k<argc-1;k++){   
                         for (int j=0;j< shifting[k]  ;j++){
                            pixel=readPixel(bmp[k],pixel);
                            writePixel(flattenPOT,pixel);
                            }
                        fseek(bmp[k],(480-shifting[k])*4,SEEK_CUR);
               }
            }

    fseek(flattenPOT,18,SEEK_SET);
    fwrite(&POTsize,4,1,flattenPOT);
    fclose(flattenPOT);
    return 0;
} 
