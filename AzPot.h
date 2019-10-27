
//Header file that contains functions that used inside resize.c shifting.c stitch.c



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct Pixel{
       unsigned char red, green, blue, idk ;
} SPixel ;  // structure to represent pixel 

 SPixel readPixel(FILE *fp, SPixel Pixel){ //read a single pixel from file fp
        fread(&Pixel.red,1,1,fp);
        fread(&Pixel.green,1,1,fp);
        fread(&Pixel.blue,1,1,fp);
        fread(&Pixel.idk ,1,1,fp);
        return Pixel;
 }


 void writePixel(FILE *fp, SPixel pixel ){ // writing a single pixel to file fp
        fwrite(&pixel.red,1,1,fp);
        fwrite(&pixel.green,1,1,fp);
        fwrite(&pixel.blue,1,1,fp);
        fwrite(&pixel.idk,1,1,fp);
}

 void readRectangle  (FILE *fp, SPixel Pixels[][350]){ // reads pixels and assings them into two dimensonal arrat which represents our rectangle  
     for (int i=0;i<50;i++){
        for (int j=0;j<350;j++){
            Pixels[i][j]=readPixel(fp,Pixels[i][j]);
        }
        fseek(fp,130*4,SEEK_CUR);
     }
 }

 int findMatch (SPixel Source[][350],SPixel Target[][350]){ // compares two different rectangles finds number of matching pixels 
  int match=0;
    for (int i=0; i<50;i++){
        for (int j=0;j<350;j++){
             if( abs(Target[i][j].red-Source[i][j].red)<15 && abs(Target[i][j].green-Source[i][j].green)<15 && abs(Target[i][j].blue-Source[i][j].blue)<15) //if difference is less than 15 it is match
            match++;
        }
    }
    return match;
 }

float findMaxFloat(float *psf){  //finds the maximum in float numbers 
     double max = psf[0]; 
     for (int i=0;i<22;i++){
         if (psf[i]>max) max=psf[i];
     }
   return max;
 }

  int  findMaxInt(int *shiftValues){ // finds the maximum in int numbers
      int max =shiftValues[0]; 
      for (int i=0;i<22;i++){
          if (shiftValues[i]>max) max=shiftValues[i];
      }
    return max;
 }


  void readPSF(FILE* fSource, float *psf){ //read all values from file writes it into array
   int shift[22];
 for(int i=0;i<22;i++){
    fscanf(fSource,"%d", &shift[i]);
  
             }

        for (int i=0;i<22;i++){
             psf[i]=(float)findMaxInt(shift)/shift[i];    
                
             }
        
  }
  

  int  findMaxShifting(FILE* fSource){ // reads all values 

     int shift[22];
 for(int i=0;i<22;i++){
    fscanf(fSource,"%d", &shift[i]);
    // printf("%d \n" ,shift[i]);
             }
 int maxShift=findMaxInt(shift);
    return maxShift;
   

  }

