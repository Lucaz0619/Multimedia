#include <stdio.h>
#include <stdlib.h>

/**�سy����|�Ψ쪺�榡**/
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
    int i=0,j=0,k=0,getb,Temp[35],count=0,out=0;
    /**Ū�ɡA��Ū�r��ASCII�AŪ��\t��Ūcode�AŪ��\r���U�@��**/
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
                    h[i].lenth=k;
                    k=0;
                    j=0;
                    i++;
                }
            }
    }

    fclose(fcb);
    /**Ūinput�A���r���A�ഫ��code�A8�Ӥ@����X**/
    fin=fopen(argv[1],"rb");
    fout=fopen(argv[3],"wb");
    while(!feof(fin)){
        fread(&getb,1,1,fin);
        for(i=0;i<=101;i++){
            if(getb==h[i].cha){
                    for(j=count;j<count+h[i].lenth;j++)Temp[j]=h[i].code[j-count];
                    count=count+h[i].lenth;
                while(count>=8){
                    out = Temp[7]*1+Temp[6]*2+Temp[5]*4+Temp[4]*8+Temp[3]*16+Temp[2]*32+Temp[1]*64+Temp[0]*128;//�ন10�i��
                    fwrite(&out,1,1,fout);
                    for(j=8;j<count;j++)Temp[j-8]=Temp[j];
                    count=count-8;
                }
            }
        }
    }
    /**�̫ᤣ��8�Ӫ��ɹs���X**/
        if(count!=0){
            for(i=1;i<=count;i++)Temp[8-count]=Temp[count-i];
            for(i=0;i<=7-count;i++)Temp[i]=0;
            out = Temp[7]*1+Temp[6]*2+Temp[5]*4+Temp[4]*8+Temp[3]*16+Temp[2]*32+Temp[1]*67+Temp[0]*128;
            count=0;
            fwrite(&out,1,1,fout);
        }
    fclose(fin);
    fclose(fout);
    return 0;
}
