 #include <stdio.h>
 
    typedef struct  FileHeader {
        char bfType[3]; 
        unsigned int bfSize;
        unsigned short reserved1;
        unsigned short reserved2;
        unsigned long bfOffbits; 
    } BitmapFileHeader;
    
    typedef struct  DIBHeader {
        unsigned int biSize;
        long biWidth ;
        long biHeight ;
        unsigned short biPlanes;
        unsigned short biBitCount;
    } BitmapDIBHeader;

unsigned short read2Byte(FILE *fp){ /* reading 2 bytes in little endian */
    unsigned char byte[2];
    unsigned short result=0;
    int i ;
    
    fread(byte,1,2,fp);
    for (i=1;i>=0;i--){
       result=(result << 8) | (unsigned short) byte[i];
    }
}

unsigned int read4Byte(FILE *file){  /*reading 4 bytes in little endian */
    unsigned char byte[4];
    unsigned int result=0;
    int i;
    fread (byte,1,4,file);
    for (i=3;i>=0;i--){
        result=(result<<8) | (unsigned int) byte[i];


    }
return result;
}

BitmapFileHeader *ReadFileHeader(FILE *fp){
    BitmapFileHeader *header;
    char filetype[3]={'\0','\0','\0'};
    unsigned int filesize;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned long offset;
    
    fread(&filetype,1,2,fp);
    filesize=(unsigned int) read4Byte(fp);
    fread(&reserved1,2,1,fp);
    fread(&reserved2,2,1,fp);
    offset=(unsigned long) read4Byte(fp);

    header=(BitmapFileHeader *) malloc(sizeof(BitmapFileHeader));
    strcpy(header->bfType,filetype);
    header->bfSize=filesize;
    header->reserved1=reserved1;
    header->reserved2=reserved2;
    header->bfOffbits = offset;
    
    return header;
}
/*int SizeOfInformationHeader(FILE *fp){
    int headersize;
    unsigned char buf[4];
    int i;
    fread(buf,1,4,fp);
    for (i=3;i>=0;i--){
        headersize= (headersize<<8) | (int) buf[i];
        fseek(fp,14,SEEK_SET);
        return headersize;
    }
}
*/
BitmapDIBHeader *ReadDIBHeader(FILE *fp){
    BitmapDIBHeader *header;
    unsigned int headersize;
    int width;
    int height;
    unsigned short planes;
    unsigned short bitcount;

    headersize=(unsigned int ) read4Byte(fp); //4 bytes
    width=(int) read4Byte(fp); //4 bytes 
    height=(int) read4Byte(fp);  //4 bytes 
    planes=(unsigned int) read2Byte(fp); //2 bytes 
    fread(&bitcount,2,1,fp); //2 bytes
    header= (BitmapDIBHeader *) malloc(sizeof(BitmapDIBHeader));
    header->biSize=headersize ;
    header->biWidth=width;
    header->biHeight=height;
    header->biPlanes=planes;
    header->biBitCount=bitcount; 

    return header;
}





