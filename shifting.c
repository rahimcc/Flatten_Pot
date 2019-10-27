//Program takes 2 bmp image find shifting values
//returns txt file with shifting values inside

//Rahim Sharifov, Sara Shamilova

//January 2019




#include <stdio.h>
#include <stdlib.h>
#include "AzPot.h"

int main (int argc, char * argv[]){
    if (argc!=3){
        printf("USAGE: ./a.out picture-n picture-n+1\n");
        return 0;
    }
 FILE* fSource;
 FILE* fTarget;

    if ((fSource=fopen(argv[1],"r"))==NULL) {
            printf("\n\nFile %s doesn't exists\n", argv[1]);
            return 0;
    }
    if (( fTarget = fopen(argv[2],"r"))==NULL){
        printf("\n\nFile %s doesn't exists\n", argv[2]);
        return 0;
    }



 SPixel SourcePixel[50][350];
 SPixel TargetPixel[50][350];
 int shift[22];
 float psf[22];
 int initialPos=100;
char *name=argv[2];
name[strlen(name)-4]='\0';
 name=name+9;
strcat(name,".txt"); // creating txt file which will contain shifting values of corresponding 

printf("%s\n", name);
     for (int i=0;i<22; i++){
         fseek(fSource,i*480*100*4+54,SEEK_SET); //shifts the pointer to the beginning of ith step 
          fseek (fSource,initialPos*4,SEEK_CUR); //shifts pointer 100 pixel forward just random number 
           readRectangle(fSource,SourcePixel); // reads reactangle of pixels from fSource into SourcePixel
           int small=0;
           int shiftValue=0;
           for (int k=0;k<50;k++){
           for(int j=0;j<100;j++){
   fseek(fTarget,(i*480*100+k*480+j)*4+54,SEEK_SET); // shifts pointer vertically and horizontally to find all rectangles and its matches 
               readRectangle(fTarget,TargetPixel); //reads rectangle 
             if (findMatch(SourcePixel,TargetPixel)>small){ // compares rectangles if matching is bigger than previous it is best match so far 
             small=findMatch(SourcePixel,TargetPixel); 
             shiftValue=j;
                    }
              }
           }
               shift[i]=initialPos-shiftValue; // assings shifting value as difference between previous position and current 
          printf("shift value:[%d] %d  , %d matching pixel out of 17500  \n",i,initialPos-shiftValue,small , shiftValue);
} 

 FILE *file=fopen(name,"w");


for (int i=0;i<22;i++){
   fprintf(file,"%d\n",shift[i]); //writing these values new txt file  
//    printf("%f\n", psf[i]);
}



}