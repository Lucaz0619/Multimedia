#include <stdio.h>
#include <stdlib.h>

/**建造之後會用到的格式**/
struct Huffman{
    short cha;
    int code[18];
    int lenth;
};

int main(int argc,char*argv[])
{
    FILE *fin=0;
    FILE *fcb=0;
    FILE *fout=0;
    fcb=fopen(argv[2],"rb");
        if(argc!=4){
        printf("Argument count error!\n");
        return -1;
    }
    struct Huffman h[102]={0};;
    int i=0,j=0,k=0,a=1,getb,Temp[8],count=0,out=0,cb[25];
    /**讀檔，先讀字元ASCII，讀到\t換讀code，讀到\r換下一個**/
    while(!feof(fcb)){
            fread(&getb,1,1,fcb);//get char
            if(getb!=9){
                h[i].cha=h[i].cha*10+(getb-48);
            }
            if(getb==9){//get \t
                fread(&getb,1,1,fcb);
                while(getb!=13){//get code
                    h[i].code[k]=getb-48;
                    fread(&getb,1,1,fcb);
                    k++;
                }
                if(getb==13){//get \r
                    fread(&getb,1,1,fcb);//get \n
                    h[i].lenth=k-1;
                    k=0;
                    j=0;
                    i++;
                }
            }
    }

    fclose(fcb);
/**把Compress讀進來，轉成二進位跟Codebook比較，一樣就轉成cha輸出，輸出後左移，或是不一樣的話再讀進來8個**/
    fin=fopen(argv[1],"rb");
    fout=fopen(argv[3],"wb");
    while(!feof(fin)){
        fread(&getb,1,1,fin);
        Temp[0]=getb/128;
        getb=getb-Temp[0]*128;
        Temp[1]=getb/64;
        getb=getb-Temp[1]*64;
        Temp[2]=getb/32;
        getb=getb-Temp[2]*32;
        Temp[3]=getb/16;
        getb=getb-Temp[3]*16;
        Temp[4]=getb/8;
        getb=getb-Temp[4]*8;
        Temp[5]=getb/4;
        getb=getb-Temp[5]*4;
        Temp[6]=getb/2;
        getb=getb-Temp[6]*2;
        Temp[7]=getb/1;
        for(j=0;j<=7;j++)cb[count+j]=Temp[j];
        count=count+8;//位置每次+8
        for(i=0;i<=101;i++){
            k=0;
            while(cb[k]==h[i].code[k]&&count>=h[i].lenth+1){
                k++;
                if(k==h[i].lenth+1){
                    out=h[i].cha;
                    fwrite(&out,1,1,fout);
                    count=count-h[i].lenth-1;//新的位置等於舊的扣掉輸出的長度
                    for(j=0;j<=h[i].lenth;j++)cb[j]=0;
                    for(j=0;j<=count-1;j++)cb[j]=cb[j+h[i].lenth+1];//左移
                }
                if(cb[k]!=h[i].code[k]) break;

            }
        }
    }
    fclose(fin);
    fclose(fout);
    return 0;
}
