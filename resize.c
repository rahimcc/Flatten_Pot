// Program takes bmp image and txt file as input
// resizes image with shifting values inside txt file  
// Author: Rahim Sharifov Sara Shamilova 
// January 2019

#include "AzPot.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char* argv[]){

    int SourceWidth, SourceHeight, TargetWidth, SubTargetWidth;
    FILE *fSource , *fTarget, *shiftingSource;
    char TargetName[50]="resized", preNAme[50]="AzCroppedPot";
    unsigned char identity[3]={'\0','\0','\0'};   
    unsigned char byteBuffer;
    SPixel pixel1 , pixel2;
    float psf[22], coeff , coeffBuffer, maxPsf;


        if (argc != 3){                                   
            printf("Usage: program <file.bmp>  <shifting source>\n");
            exit(1);
         }

        if ((fSource = fopen(argv[1], "r")) == NULL) {          
            printf("Cannot open source file: %s\n\n",argv [1]);
            exit(1);
         }
        if ((shiftingSource=fopen(argv[2],"r"))==NULL){
            printf("Cannot open file %s \n", argv[2]);
            return 0;
        }
   
        fread(&identity,1,2,fSource ); 
             if (strcmp(identity, "BM") != 0) {    
                printf("File is not BITMAP\n");
                exit(1);
        }
         strcat(TargetName,argv[1]);

          if ((fTarget= fopen (TargetName, "w"))==NULL){
            printf("Cannot open target file\n");
            exit(1);
        }
    printf("%s \n ", argv[1]);

    readPSF(shiftingSource,psf);  //opens files and fill array psf with psf values 
    fseek(fSource,18,SEEK_SET);  
    fread(&SourceWidth,1,4,fSource );
    fread(&SourceHeight,1,4,fSource);  // taking width and height of image 
    SourceHeight=abs(SourceHeight);
    
    fseek(fSource,0,SEEK_SET);  //copying all information into new file 
    for (int i=0;i<54;i++){
              fread(&byteBuffer,1,1,fSource);
              fwrite(&byteBuffer,1,1,fTarget);
    }

   TargetWidth= SourceWidth*findMaxFloat(psf);  // finding width of resized image  
  if (TargetWidth%4) TargetWidth=TargetWidth+(4-TargetWidth%4);  // completing new width to be divisible to 4 just in case 

 fseek(fTarget,18,SEEK_SET); 
 fwrite(&TargetWidth,4,1,fTarget); //writing new size to file 
 fseek(fTarget,54,SEEK_SET);

 
for (int j=0;j<22;j++){
    // printf("dfa");  
   SubTargetWidth=SourceWidth*psf[j]; // SubTargetWidth is width each step 
   if (SubTargetWidth%4) SubTargetWidth=SubTargetWidth+(4-SubTargetWidth%4);  // completing new width to be divisible to 4 just in case 
    maxPsf=(float) SubTargetWidth/ (float)SourceWidth;  //precising psf value after completing new width to be visible to 4 
    coeffBuffer=maxPsf; // at the beginning of every step coeffBuffer is equal to psf value
    fseek(fSource,480*j*100*4+54,SEEK_SET); // shifting pointer of fSource to the beginning of jth step. every step is 100 line 
     pixel1=readPixel(fSource,pixel1); // read first pixel and assings to pixel1 
   
    for(int k=0;k<100;k++){

          fseek(fTarget,((TargetWidth-SubTargetWidth)/2)*4,SEEK_CUR); // shifting pointer to center  relative to its subwidth  to obtain centered image 
           for (int i=0;i<SubTargetWidth;i++) {   

        if (coeffBuffer > 1 ){  //checking conditions of coeffBuffer to get value of coeff 
            coeff=1;
            coeffBuffer=coeffBuffer-coeff;
        } else { 
            coeff=coeffBuffer;
            coeffBuffer=maxPsf-(1-coeff);   
        }

        pixel1.red= coeff*pixel1.red+(1-coeff)*pixel2.red;  
        pixel1.green= coeff*pixel1.green+(1-coeff)*pixel2.green;
        pixel1.blue=coeff*pixel1.blue+(1-coeff)*pixel2.blue ;
        pixel1.idk=coeff*pixel1.idk+(1-coeff)*pixel2.idk ;   //getting ith pixel of fTarget using formula 

     
        writePixel(fTarget,pixel1); //writing new obtained pixel to fTarget 

        if (coeff<1 || (coeff==1 && coeffBuffer==maxPsf)){ // checking we are done with old pixel in order to read new pixel 
            pixel1=pixel2;  
            pixel2=readPixel(fSource,pixel2);           
        }
        //   printf("counter= %d , coeff= %f  coeffbuffer=%f, psf= %f \n", i, coeff, coeffBuffer, maxPsf);

    }
            fseek(fTarget,((TargetWidth-SubTargetWidth)/2)*4,SEEK_CUR); //shifting pointer to make image centered 
            pixel1=readPixel(fSource,pixel1); // reading first pixel of new step 
            fseek(fSource,-4,SEEK_CUR); // I forgot why i did this
        
        }

    }
   
    
        fseek (fSource,2200*4+54,SEEK_SET); 

         for (int i=0;i<30*480;i++){ //we have 22 step with 100 line this means 2200 pixel. last 30 pixels are copied into fTarget as it is because last psf value of last step is always 1 
        
             fseek(fTarget,((TargetWidth-SubTargetWidth)/2)*4,SEEK_CUR);
              pixel1=readPixel(fSource,pixel1);
              writePixel(fTarget,pixel1);    
               fseek(fTarget,((TargetWidth-SubTargetWidth)/2)*4,SEEK_CUR);
         } 
}



