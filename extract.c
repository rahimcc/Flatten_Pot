/* Displaying BMP image information */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.h"
//#define file_name example.BMP
int main(int argc, char *argv[])
{
    FILE *fp;
    BitmapFileHeader *bmpFileHeader = NULL;
    BitmapDIBHeader *bmpDIBHeader = NULL;
        if (argc != 3){  //checking if the number of the arguments is not equal to 3 which means user have given less arguments or more arguments. if so the usage message show up 
            printf("Usage: program <file.bmp> size_of_strip\n");
            exit(1);
         }

        if ((fp = fopen(argv[1], "rb")) == NULL) {  // just to be sure that file was opened succesfully  
            printf("Cannot open file: %s\n\n", argv[1]);
            exit(1);
         }
    unsigned char identity[3]={'\0','\0','\0'};     //array of chars for storing first two bytes of bitmap file which should be "BM" in ASCII
    fread(&identity,1,2,fp); 
             if (strcmp(identity, "BM") != 0) { //checking that if the file given by user is really bitmap file 
                printf("File is not BITMAP\n");
                exit(1);
             }
    

    fseek(fp,0,SEEK_SET);  //changing location of pointer of fp to very first byte 
    bmpFileHeader=ReadFileHeader(fp); // storing informations of file header to the struct bmpFileHeader
    bmpDIBHeader=ReadDIBHeader(fp);  // storing some informations of DIB header to the struct bmpDIBHeader
    
     printf("File type          = %s\n", bmpFileHeader->bfType); //displaying some information about bitmap file 
    printf("File size          = %d bytes\n", bmpFileHeader->bfSize);
    printf("Data offset        = %ld bytes\n", bmpFileHeader->bfOffbits);
    printf("Info header size   = %d bytes\n", bmpDIBHeader->biSize);
    printf("Width              = %ld pixel  s\n", bmpDIBHeader->biWidth);
    printf("Height             = %ld pixels\n", bmpDIBHeader->biHeight);
    printf("Planes             = %d\n", bmpDIBHeader->biPlanes);
    printf("Bit count      = %d bits/pixel\n", bmpDIBHeader->biBitCount);
    fseek(fp,0,SEEK_SET);   //changing location of pointer to the first byte 
     bmpDIBHeader->biHeight=abs(bmpDIBHeader->biHeight);
   
	int paddingSize = ((atoi(argv[2])*bmpDIBHeader->biBitCount+31)/32)*4; //width of string which is given by user in bytes inculding padding 
   
    unsigned char *table=(unsigned char *) calloc(bmpFileHeader->bfOffbits,sizeof(unsigned char)); 
    fread(table,1,bmpFileHeader->bfOffbits,fp); // storing all informations of headers into array "table"
	    
    int rowSize=((bmpDIBHeader->biWidth*bmpDIBHeader->biBitCount+31)/32)*4; //width of original image given by user inculding padding 
    printf("%d\n", rowSize);

   int usedSize=(rowSize-paddingSize)/2; // creating new variable usedSize which is equal to length between beginning of orginal image and beginning of strip 
    	fseek(fp,usedSize,SEEK_CUR); // changing location of pointer to byte from which we start storing   
    int newSize=(bmpDIBHeader->biHeight)*paddingSize; // newSize variable is size of strip in bytes 
   unsigned char *newData=(unsigned char *)calloc(newSize,sizeof(unsigned char));
     
 for (int i=0;i<=bmpDIBHeader->biHeight;i++){ //storing all bytes of strip that we want to cut
       fread((newData+i*paddingSize),1,paddingSize,fp);
	   fseek(fp,2*usedSize,SEEK_CUR);
  } 


                                                                         

 fclose (fp);
       
    char filename[50];
       memset(filename, '\0', sizeof(filename));
       strncpy(filename,argv[1],strlen(argv[1])-4); // copying the string all characters from argv[1] to filename except last 4 characters that is .bmp
       strcat(filename,"_S");
       strcat(filename,argv[2]);
       strcat(filename,".bmp"); // creating filename for new bitmap file which is "original_name_Sxx.pmp"
    
       FILE *sample=fopen(filename, "wb"); //creating new bitmap file 
       fwrite(table,bmpFileHeader->bfOffbits,1,sample); //writing  all information of headers of original file into new bitmap image file 
  	   fwrite(newData,newSize,1,sample); //writing all bytes of strip to new bitmap image file 
    
    int setwidth=atoi(argv[2]);
	fseek(fp,18,SEEK_SET);
    fwrite(&setwidth,4,1, sample); // setting width of strip 
    fseek(fp,2,SEEK_SET);
    int newnewsize=bmpFileHeader->bfOffbits+newSize;
    fwrite(&newnewsize,4,1,sample); // settin size of strip

}