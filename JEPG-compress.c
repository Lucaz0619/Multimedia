#include <stdio.h>
#include <stdlib.h>
#include <math.h>
float PI=3.1415927;
int BQT[8][8]={
16, 11, 10, 16, 24, 40, 51, 61,
12, 12, 14, 19, 26, 58, 60, 55,
14, 13, 16, 24, 40, 57, 69, 56,
14, 17, 22, 29, 51, 87, 80, 62,
18, 22, 37, 56, 68, 109, 103, 77,
24, 35, 55, 64, 81, 104, 113, 92,
49, 64, 78, 87, 103, 121, 120, 101,
72, 92, 95, 98, 112, 100, 103, 99
};
int CQT[8][8]={
17, 18, 24, 47, 99, 99, 99, 99,
18, 21, 26, 66, 99, 99, 99, 99,
24, 26, 56, 99, 99, 99, 99, 99,
47, 66, 99, 99, 99, 99, 99, 99,
99, 99, 99, 99, 99, 99, 99, 99,
99, 99, 99, 99, 99, 99, 99, 99,
99, 99, 99, 99, 99, 99, 99, 99,
99, 99, 99, 99, 99, 99, 99, 99
};
int ZigzagTable[8][8]={
 0,  1,  8, 16,  9,  2,  3, 10,
17, 24, 32, 25, 18, 11,  4,  5,
12, 19, 26, 33, 40, 48, 41, 34,
27, 20, 13,  6,  7, 14, 21, 28,
35, 42, 49, 56, 57, 50, 43, 36,
29, 22, 15, 23, 30, 37, 44, 51,
58, 59, 52, 45, 38, 31, 39, 46,
53, 60, 61, 54, 47, 55, 62, 63
};
int DezigzagTable[8][8]={
0, 1, 5, 6, 14, 15, 27, 28,
2, 4, 7, 13, 16, 26, 29, 42,
3, 8, 12, 17, 25, 30, 41, 43,
9, 11, 18, 24, 31, 40, 44, 53,
10, 19, 23, 32, 39, 45, 52, 54,
20, 22, 33, 38, 46, 41, 55, 60,
21, 34, 37, 47, 50, 56, 59, 61,
35, 36, 48, 49, 57, 58, 62, 63
};
/*construct a structure of BMP header*/
typedef struct Bmpheader{
    unsigned short identifier; // 0x0000
    unsigned int filesize; // 0x0002
    unsigned short reserved; // 0x0006
    unsigned short reserved2;
    unsigned int bitmap_dataoffset; // 0x000A
    unsigned int bitmap_headersize; // 0x000E
    unsigned int width; // 0x0012
    unsigned int height; // 0x0016
    unsigned short planes; // 0x001A
    unsigned short bits_perpixel; // 0x001C
    unsigned int compression; // 0x001E
    unsigned int bitmap_datasize; // 0x0022
    unsigned int hresolution; // 0x0026
    unsigned int vresolution; // 0x002A
    unsigned int usedcolors; // 0x002E
    unsigned int importantcolors; // 0x0032
    unsigned int palette; // 0x0036
} Bitmap;
/*construct a structure of RGB*/
typedef struct RGB{
    int R;
    int G;
    int B;
} ImgRGB;
/*construct a structure of YCbCr*/
typedef struct YCbCr{
    float Y;
    float Cb;
    float Cr;
} ImgYCbCr;
/*construct a structure of block*/
typedef struct block{
    float x;
    float y;
    float z;
} Imgblock;
/*construct a structure of Run & Length*/
typedef struct RC{
    int skip;
    int value;
} runlen;

Bitmap readheader(FILE* fp);
ImgRGB** malloc_2D(int row, int col);
ImgYCbCr** malloc_2DY(int row, int col);
Imgblock** malloc_2DB(int row, int col);
void InputData(FILE* fp,ImgRGB **array,int H,int W);
void output_bmp(ImgRGB **RGB,FILE* outfile,Bitmap bmpheader);

void RGBtoYCbCr(ImgRGB **RGB,ImgYCbCr **YCbCr,Bitmap bmpheader);
void YCbCrtoRGB(ImgRGB **RGB,ImgYCbCr **YCbCr,Bitmap bmpheader);
void Blocking(ImgYCbCr **YCbCr,Imgblock **block,int h,int w,int u,int v);
void inBlocking(ImgYCbCr **YCbCr,Imgblock **block,int h,int w,int u,int v);
void FDCT(Imgblock **block,Imgblock **block2,int h,int w);
void IDCT(Imgblock **block,Imgblock **block2,int h,int w);
void Quantization(Imgblock **block2,int h,int w);
void Dequantization(Imgblock **block2,int h,int w);
void Zigzag(ImgRGB **RGB,Imgblock **block2,int h,int w);
void Dezigzag(ImgRGB **RGB,Imgblock **block2,int h,int w);
void DPCM(ImgRGB *DC,ImgRGB **RGB,Bitmap bmpheader,int u,int v,int a,int b,int c);
int RunLengthACx(runlen *xRLC,ImgRGB **RGB,Bitmap bmpheader,int h,int w,int count1);
int RunLengthACy(runlen *yRLC,ImgRGB **RGB,Bitmap bmpheader,int h,int w,int count2);
int RunLengthACz(runlen *zRLC,ImgRGB **RGB,Bitmap bmpheader,int h,int w,int count3);
void DeDC(ImgRGB *DC,ImgRGB **RGB,Bitmap bmpheader,int u,int v,int a,int b,int c);
int DeRLEACx(runlen *xRLC,ImgRGB **RGB,int h,int w,int count4);
int DeRLEACy(runlen *yRLC,ImgRGB **RGB,int h,int w,int count5);
int DeRLEACz(runlen *zRLC,ImgRGB **RGB,int h,int w,int count6);


int main(int argc,char *argv[]){

    FILE* fp=fopen("input.bmp","rb");
    FILE *fw=fopen("output.bmp","wb");

    //read header
    Bitmap bmpheader=readheader(fp);
    ImgRGB **Data_RGB=malloc_2D(bmpheader.height, bmpheader.width);

	//read data
    InputData(fp,Data_RGB,bmpheader.height,bmpheader.width);

    /**My Code starts from here**/
    ImgYCbCr **Data_YCbCr=malloc_2DY(bmpheader.height, bmpheader.width);
    Imgblock **Data_block=malloc_2DB(8, 8);//創造兩個8x8的格子之後用來blocking,DCT,Quantize
    Imgblock **Data_block2=malloc_2DB(8, 8);
    ImgRGB **Data_zz=malloc_2D(8,8);
    ImgRGB *DC= (ImgRGB*)malloc((bmpheader.height/8+1)*(bmpheader.width/8+1)*sizeof(ImgRGB));
    runlen *xRLC=(runlen*)malloc(6*(bmpheader.height/8+1)*(bmpheader.width/8+1)*sizeof(runlen));
    runlen *yRLC=(runlen*)malloc(6*(bmpheader.height/8+1)*(bmpheader.width/8+1)*sizeof(runlen));
    runlen *zRLC=(runlen*)malloc(6*(bmpheader.height/8+1)*(bmpheader.width/8+1)*sizeof(runlen));
    int h=8,w=8,u=0,v=0,x=1,y=1,i,j,a,b,c,count1=0,count2=0,count3=0,count4=0,count5=0,count6=0;

    RGBtoYCbCr(Data_RGB,Data_YCbCr,bmpheader);//RGB -> YCbCr
    while(x==1){
        if(u>bmpheader.height) x=0;
        if(u>bmpheader.height-8) h=bmpheader.height-u;//剩下的高度的像素
        else h=8;
        while(y==1){
            if(v>bmpheader.width) y=0;
            if(v>bmpheader.width-8)w=bmpheader.width-v;//剩下的寬度得像素
            else w=8;
            Blocking(Data_YCbCr,Data_block,h,w,u,v);
            FDCT(Data_block,Data_block2,h,w);
            Quantization(Data_block2,h,w);
            Zigzag(Data_zz,Data_block2,h,w);
            DPCM(DC,Data_zz,bmpheader,u,v,a,b,c);
            count1=RunLengthACx(xRLC,Data_zz,bmpheader,h,w,count1);
            count2=RunLengthACy(yRLC,Data_zz,bmpheader,h,w,count2);
            count3=RunLengthACz(zRLC,Data_zz,bmpheader,h,w,count3);
            DeDC(DC,Data_zz,bmpheader,u,v,a,b,c);
            a=Data_zz[0][0].R;
            b=Data_zz[0][0].G;
            c=Data_zz[0][0].B;
            count4=DeRLEACx(xRLC,Data_zz,h,w,count4);
            count5=DeRLEACy(yRLC,Data_zz,h,w,count5);
            count6=DeRLEACz(zRLC,Data_zz,h,w,count6);
            Dezigzag(Data_zz,Data_block2,h,w);
            Dequantization(Data_block2,h,w);
            IDCT(Data_block,Data_block2,h,w);
            inBlocking(Data_YCbCr,Data_block,h,w,u,v);//把資料丟回去
            v=v+8;
        }
        y=1;
        v=0;
        u=u+8;
    }
    YCbCrtoRGB(Data_RGB,Data_YCbCr,bmpheader);//YCbCr -> RGB*/
    //output bmp
    output_bmp(Data_RGB,fw,bmpheader);
    free(Data_RGB);//釋放空間
    free(Data_YCbCr);
    free(Data_block);
    free(Data_block2);
    free(Data_zz);

    return 0;
}

/*read header*/
Bitmap readheader(FILE* fp){
    Bitmap x;
    fread(&x.identifier,sizeof(x.identifier),1,fp);
    fread(&x.filesize,sizeof(x.filesize),1,fp);
    fread(&x.reserved,sizeof(x.reserved),1,fp);
    fread(&x.reserved2,sizeof(x.reserved2),1,fp);
    fread(&x.bitmap_dataoffset,sizeof(x.bitmap_dataoffset),1,fp);
    fread(&x.bitmap_headersize,sizeof(x.bitmap_headersize),1,fp);
    fread(&x.width,sizeof(x.width),1,fp);
    fread(&x.height,sizeof(x.height),1,fp);
    fread(&x.planes,sizeof(x.planes),1,fp);
    fread(&x.bits_perpixel,sizeof(x.bits_perpixel),1,fp);
    fread(&x.compression,sizeof(x.compression),1,fp);
    fread(&x.bitmap_datasize,sizeof(x.bitmap_datasize),1,fp);
    fread(&x.hresolution,sizeof(x.hresolution),1,fp);
    fread(&x.vresolution,sizeof(x.vresolution),1,fp);
    fread(&x.usedcolors,sizeof(x.usedcolors),1,fp);
    fread(&x.importantcolors,sizeof(x.importantcolors),1,fp);
    return x;
}

/*input data without header into RGB structure*/
void InputData(FILE* fp,ImgRGB **array,int H, int W){
    int temp,i=0,j=0;
    for(i=0;i<H;i++){
        for(j=0;j<W;j++){
            temp=fgetc(fp);
            array[i][j].B=temp;
            temp=fgetc(fp);
            array[i][j].G=temp;
            temp=fgetc(fp);
            array[i][j].R=temp;
        }
    }
}

/* A function of making two dimensions memory locate array*/
ImgRGB** malloc_2D(int row, int col){
    ImgRGB **Array, *Data;
    int i;
    Array=(ImgRGB**)malloc(row*sizeof(ImgRGB *));
    Data=(ImgRGB*)malloc(row*col*sizeof(ImgRGB));
    for(i=0; i<row; i++,Data+=col){
        Array[i] = Data;
    }
    return Array;
}
ImgYCbCr** malloc_2DY(int row, int col){
    ImgYCbCr **Array, *Data;
    int i;
    Array=(ImgYCbCr**)malloc(row*sizeof(ImgYCbCr *));
    Data=(ImgYCbCr*)malloc(row*col*sizeof(ImgYCbCr));
    for(i=0; i<row; i++,Data+=col){
        Array[i] = Data;
    }
    return Array;
}
Imgblock** malloc_2DB(int row, int col){
    Imgblock **Array, *Data;
    int i;
    Array=(Imgblock**)malloc(row*sizeof(Imgblock *));
    Data=(Imgblock*)malloc(row*col*sizeof(Imgblock));
    for(i=0; i<row; i++,Data+=col){
        Array[i] = Data;
    }
    return Array;
}

/*output header and data*/
void output_bmp(ImgRGB **RGB,FILE* outfile,Bitmap bmpheader){

    fwrite(&bmpheader.identifier, sizeof(short), 1 , outfile);
    fwrite(&bmpheader.filesize, sizeof(int), 1 , outfile);
    fwrite(&bmpheader.reserved, sizeof(short), 1 , outfile);
    fwrite(&bmpheader.reserved2, sizeof(short), 1 , outfile);
    fwrite(&bmpheader.bitmap_dataoffset, sizeof(int), 1 , outfile);
    fwrite(&bmpheader.bitmap_headersize, sizeof(int), 1 , outfile);
    fwrite(&bmpheader.width, sizeof(int), 1 , outfile);
    fwrite(&bmpheader.height, sizeof(int), 1 , outfile);
    fwrite(&bmpheader.planes, sizeof(short), 1 , outfile);
    fwrite(&bmpheader.bits_perpixel, sizeof(short), 1 , outfile);
    fwrite(&bmpheader.compression, sizeof(int), 1 , outfile);
    fwrite(&bmpheader.bitmap_datasize, sizeof(int), 1 , outfile);
    fwrite(&bmpheader.hresolution, sizeof(int), 1 , outfile);
    fwrite(&bmpheader.vresolution, sizeof(int), 1 , outfile);
    fwrite(&bmpheader.usedcolors, sizeof(int), 1 , outfile);
    fwrite(&bmpheader.importantcolors, sizeof(int), 1 , outfile);

    int x,y;
    for (x=0; x<bmpheader.height; x++){
        for(y=0; y<bmpheader.width; y++){
            fwrite(&RGB[x][y].B, sizeof(char),1,outfile);
            fwrite(&RGB[x][y].G, sizeof(char),1,outfile);
            fwrite(&RGB[x][y].R, sizeof(char),1,outfile);
        }
    }
}

/*trun RGB to YCbCr*/
void RGBtoYCbCr(ImgRGB **RGB,ImgYCbCr **YCbCr,Bitmap bmpheader){
    int x,y;
    for (x=0; x<bmpheader.height; x++){
        for(y=0; y<bmpheader.width; y++){
            YCbCr[x][y].Y=(float)(0.299*RGB[x][y].R+0.587*RGB[x][y].G+0.114*RGB[x][y].B-128);
            YCbCr[x][y].Cb=(float)(-0.16874*RGB[x][y].R-0.33126*RGB[x][y].G+0.50000*RGB[x][y].B);
            YCbCr[x][y].Cr=(float)(0.50000*RGB[x][y].R-0.41869*RGB[x][y].G-0.08131*RGB[x][y].B);
        }
    }
}

/*return YCbCr to RGB*/
void YCbCrtoRGB(ImgRGB **RGB,ImgYCbCr **YCbCr,Bitmap bmpheader){
    int x,y;
    for (x=0; x<bmpheader.height; x++){
        for(y=0; y<bmpheader.width; y++){
            YCbCr[x][y].Y=YCbCr[x][y].Y+128;
            RGB[x][y].R=(int)(YCbCr[x][y].Y+1.402*YCbCr[x][y].Cr);
            RGB[x][y].G=(int)(YCbCr[x][y].Y-0.34414*YCbCr[x][y].Cb-0.71414*YCbCr[x][y].Cr);
            RGB[x][y].B=(int)(YCbCr[x][y].Y+1.772*YCbCr[x][y].Cb);
            if(RGB[x][y].R>255) RGB[x][y].R=255;
            if(RGB[x][y].R<0) RGB[x][y].R=0;
            if(RGB[x][y].G>255) RGB[x][y].G=255;
            if(RGB[x][y].G<0) RGB[x][y].G=0;
            if(RGB[x][y].B>255) RGB[x][y].B=255;
            if(RGB[x][y].B<0) RGB[x][y].B=0;
        }
    }
}

/*Blocking*/
void Blocking(ImgYCbCr **YCbCr,Imgblock **block,int h,int w,int u,int v){
    int x,y;
    for (x=0; x<h; x++){//hxw的矩陣,主要是8x8但在邊界會變成6x8 or 8x4 or 6x4
        for(y=0; y<w; y++){
            block[x][y].x=YCbCr[u+x][v+y].Y;
            block[x][y].y=YCbCr[u+x][v+y].Cb;
            block[x][y].z=YCbCr[u+x][v+y].Cr;
        }
    }
}

/*inBlock把資料放回YCbCr*/
void inBlocking(ImgYCbCr **YCbCr,Imgblock **block,int h,int w,int u,int v){//剛剛做完IDCT,在block
    int x,y;
    for (x=0; x<h; x++){//把矩陣放回去
        for(y=0; y<w; y++){
            YCbCr[u+x][v+y].Y=block[x][y].x;
            YCbCr[u+x][v+y].Cb=block[x][y].y;
            YCbCr[u+x][v+y].Cr=block[x][y].z;
        }
    }
}

/*FDCT*/
void FDCT(Imgblock **block,Imgblock **block2,int h,int w){
    double suma,sumb,sumc,u,v;
    int i,j,x,y;
    for (i=0; i<h; i++){//hxw 矩陣做一次
        for(j=0; j<w; j++){
            suma=0;
            sumb=0;
            sumc=0;
            if(i==0) u=1.0/sqrt(2.0);
            else u=1.0;
            if(j==0) v=1.0/sqrt(2.0);
            else v=1.0;
            for (x=0; x<h; x++){
                for(y=0; y<w; y++){
                    suma=suma+block[x][y].x*cos((2*x+1)*i*PI*1.0/(2*h))*cos((2*y+1)*j*PI*1.0/(2*w));
                    sumb=sumb+block[x][y].y*cos((2*x+1)*i*PI*1.0/(2*h))*cos((2*y+1)*j*PI*1.0/(2*w));
                    sumc=sumc+block[x][y].z*cos((2*x+1)*i*PI*1.0/(2*h))*cos((2*y+1)*j*PI*1.0/(2*w));
                }
            }
            block2[i][j].x=0.25*u*v*suma;//放到block2
            block2[i][j].y=0.25*u*v*sumb;
            block2[i][j].z=0.25*u*v*sumc;
        }
    }
}

/*IDCT*/
void IDCT(Imgblock **block,Imgblock **block2,int h,int w){
    double suma,sumb,sumc,u,v;
    int i,j,x,y;
    for (x=0; x<h; x++){
        for(y=0; y<w; y++){
            suma=0;
            sumb=0;
            sumc=0;
            for (i=0; i<h; i++){
                for(j=0; j<w; j++){
                    if(i==0) u=1.0/sqrt(2.0);
                    else u=1.0;
                    if(j==0) v=1.0/sqrt(2.0);
                    else v=1.0;
                    suma=suma+u*v*block2[i][j].x*cos((2*x+1)*i*PI*1.0/(2*h))*cos((2*y+1)*j*PI*1.0/(2*w));
                    sumb=sumb+u*v*block2[i][j].y*cos((2*x+1)*i*PI*1.0/(2*h))*cos((2*y+1)*j*PI*1.0/(2*w));
                    sumc=sumc+u*v*block2[i][j].z*cos((2*x+1)*i*PI*1.0/(2*h))*cos((2*y+1)*j*PI*1.0/(2*w));
                }
            }
            block[x][y].x=0.25*suma;//剛剛做完Dequantization在block2,放回block
            block[x][y].y=0.25*sumb;
            block[x][y].z=0.25*sumc;
        }
    }
}

/*Quantization*/
void Quantization(Imgblock **block2,int h,int w){//剛剛DCT做完在block2
    int x,y;
    for (x=0; x<h; x++){
        for(y=0; y<w; y++){
            block2[x][y].x=(block2[x][y].x/BQT[x][y]);
            block2[x][y].y=(block2[x][y].y/CQT[x][y]);
            block2[x][y].z=(block2[x][y].z/CQT[x][y]);
        }
    }
}

/*Dequantization*/
void Dequantization(Imgblock **block2,int h,int w){//剛剛Quantization做完在block2
    int x,y;
    for (x=0; x<h; x++){
        for(y=0; y<w; y++){
            block2[x][y].x=(block2[x][y].x*BQT[x][y]);
            block2[x][y].y=(block2[x][y].y*CQT[x][y]);
            block2[x][y].z=(block2[x][y].z*CQT[x][y]);
        }
    }
}

/*ZigZag*/
void Zigzag(ImgRGB **RGB,Imgblock **block2,int h,int w){
    int x,y;
    for (x=0; x<h; x++){
        for(y=0; y<w; y++){
            RGB[x][y].R=(int)(block2[ZigzagTable[x][y]/8][ZigzagTable[x][y]%8].x);
            RGB[x][y].G=(int)(block2[ZigzagTable[x][y]/8][ZigzagTable[x][y]%8].y);
            RGB[x][y].B=(int)(block2[ZigzagTable[x][y]/8][ZigzagTable[x][y]%8].z);
        }
    }
}

/*Dezigzag*/
void Dezigzag(ImgRGB **RGB,Imgblock **block2,int h,int w){
    int x,y;
    for (x=0; x<h; x++){
        for(y=0; y<w; y++){
            block2[x][y].x=RGB[DezigzagTable[x][y]/8][DezigzagTable[x][y]%8].R;
            block2[x][y].y=RGB[DezigzagTable[x][y]/8][DezigzagTable[x][y]%8].G;
            block2[x][y].z=RGB[DezigzagTable[x][y]/8][DezigzagTable[x][y]%8].B;
        }
    }
}

/*DPCM*/
void DPCM(ImgRGB *DC,ImgRGB **RGB,Bitmap bmpheader,int u,int v,int a,int b,int c){
    if(u!=0||v!=0){
        DC[(u/8)*(bmpheader.width/8)+v/8].R=RGB[0][0].R-a;
        DC[(u/8)*(bmpheader.width/8)+v/8].G=RGB[0][0].G-b;
        DC[(u/8)*(bmpheader.width/8)+v/8].B=RGB[0][0].B-c;
    }
    else{
        DC[0].R=RGB[0][0].R;
        DC[0].G=RGB[0][0].G;
        DC[0].B=RGB[0][0].B;
    }
}

/*RLE*/
int RunLengthACx(runlen *xRLC,ImgRGB **RGB,Bitmap bmpheader,int h,int w,int count1){
    int x,y,i=0;
    for (x=0; x<h; x++){
        for(y=0; y<w; y++){
            if(x==0&&y==0){}
            else{
                if(RGB[x][y].R==0)i++;
                else if(RGB[x][y].R!=0){
                    xRLC[count1].value=RGB[x][y].R;
                    xRLC[count1].skip=i;
                    //printf("\t%d : %d,%d\n",count,xRLC[count].skip,xRLC[count].value);
                    count1++;
                    i=0;
                }
                if(i>20){//EOB
                    xRLC[count1].value=0;
                    xRLC[count1].skip=0;
                    y=10;
                    x=10;
                    count1++;
                    i=0;
                }
            }
        }
    }
    return count1;
}
int RunLengthACy(runlen *yRLC,ImgRGB **RGB,Bitmap bmpheader,int h,int w,int count2){
    int x,y,i=0;
    for (x=0; x<h; x++){
        for(y=0; y<w; y++){
            if(x==0&&y==0){}
            else{
                if(RGB[x][y].G==0)i++;
                else if(RGB[x][y].G!=0){
                    yRLC[count2].value=RGB[x][y].G;
                    yRLC[count2].skip=i;
                    //printf("\n%d : %d,%d\n",count,yRLC[count].skip,xRLC[count].value);
                    count2++;
                    i=0;
                }
                if(i>20){//EOB
                    yRLC[count2].value=0;
                    yRLC[count2].skip=0;
                    //printf("\n%d : %d,%d\n",count,yRLC[count].skip,xRLC[count].value);
                    count2++;
                    i=0;
                }
            }
        }
    }
    return count2;
}
int RunLengthACz(runlen *zRLC,ImgRGB **RGB,Bitmap bmpheader,int h,int w,int count3){
    int x,y,i=0;
    for (x=0; x<h; x++){
        for(y=0; y<w; y++){
            if(x==0&&y==0){}
            else{
                if(RGB[x][y].B==0)i++;
                else if(RGB[x][y].B!=0){
                    zRLC[count3].value=RGB[x][y].B;
                    zRLC[count3].skip=i;
                    //printf("\t%d : %d,%d\n",count,zRLC[count].skip,zRLC[count].value);
                    count3++;
                    i=0;
                }
                if(i>20){//EOB
                    zRLC[count3].value=0;
                    zRLC[count3].skip=0;
                    //printf("\t%d : %d,%d\n",count,zRLC[count].skip,zRLC[count].value);
                    count3++;
                    i=0;
                }
            }
        }
    }
    return count3;
}

/*DeRLE DC part*/
void DeDC(ImgRGB *DC,ImgRGB **RGB,Bitmap bmpheader,int u,int v,int a,int b,int c){


    if(u!=0||v!=0){
        RGB[0][0].R=a+DC[(u/8)*(bmpheader.width/8)+v/8].R;
        RGB[0][0].G=b+DC[(u/8)*(bmpheader.width/8)+v/8].G;
        RGB[0][0].B=c+DC[(u/8)*(bmpheader.width/8)+v/8].B;
    }
    else{
        RGB[0][0].R=DC[0].R;
        RGB[0][0].G=DC[0].G;
        RGB[0][0].B=DC[0].B;
    }
}

/*DeRLE AC parts*/
int DeRLEACx(runlen *xRLC,ImgRGB **RGB,int h,int w,int count4){
    int x,y,i=0;
    for(x=0;x<h;x++){
        for(y=0;y<w;y++){
            if(x==0&&y==0){}
            else{
                if(xRLC[count4].skip==0&&xRLC[count4].value==0){
                    RGB[x][y].R=0;
                    if(x==h-1&&y==w-1) count4++;
                }
                else if(xRLC[count4].skip==0&&xRLC[count4].value!=0){
                    RGB[x][y].R=xRLC[count4].value;
                    count4++;
                }
                else if(xRLC[count4].skip!=0){
                    RGB[x][y].R=0;
                    i++;
                    if(i>xRLC[count4].skip){
                        RGB[x][y].R=xRLC[count4].value;
                        count4++;
                        i=0;
                    }
                }
            }
        }
    }
    return count4;
}
int DeRLEACy(runlen *yRLC,ImgRGB **RGB,int h,int w,int count5){
    int x,y,i=0;
    for(x=0;x<h;x++){
        for(y=0;y<w;y++){
            if(x==0&&y==0){}
            else{
                if(yRLC[count5].skip==0&&yRLC[count5].value==0){
                    RGB[x][y].G=0;
                    if(x==h-1&&y==w-1) count5++;
                }
                else if(yRLC[count5].skip==0&&yRLC[count5].value!=0){
                    RGB[x][y].G=yRLC[count5].value;
                    count5++;
                }
                else if(yRLC[count5].skip!=0){
                    RGB[x][y].G=0;
                    i++;
                    if(i>yRLC[count5].skip){
                        RGB[x][y].G=yRLC[count5].value;
                        count5++;
                        i=0;
                    }
                }
            }
        }
    }
    return count5;
}
int DeRLEACz(runlen *zRLC,ImgRGB **RGB,int h,int w,int count6){
    int x,y,i=0;
    for(x=0;x<h;x++){
        for(y=0;y<w;y++){
            if(x==0&&y==0){}
            else{
                if(zRLC[count6].skip==0&&zRLC[count6].value==0){
                    RGB[x][y].B=0;
                    if(x==h-1&&y==w-1) count6++;
                }
                else if(zRLC[count6].skip==0&&zRLC[count6].value!=0){
                    RGB[x][y].B=zRLC[count6].value;
                    count6++;
                }
                else if(zRLC[count6].skip!=0){
                    RGB[x][y].B=0;
                    i++;
                    if(i>zRLC[count6].skip){
                        RGB[x][y].B=zRLC[count6].value;
                        count6++;
                        i=0;
                    }
                }
            }
        }
    }
    return count6;
}
