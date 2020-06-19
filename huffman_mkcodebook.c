#include <stdio.h>
#include <stdlib.h>

/**建造node格式，用來存要用的東西**/
struct Huff{
    unsigned char cha;//ASCII
    int freq;
    int code[102];//position
    int d[102];
    int Hcode[102];
    int lenth;//code lenth
};
struct codebook{
    unsigned char c;
    int hc[18];
    int len;
};

int main(int argc,char*argv[])
{
    FILE *fin,*fout;
    struct Huff node[102]={0};
    struct codebook cb[256]={0};
    unsigned char c;
    int f[256]={0};
    int t[102]={0};
    int N[102]={0};
    int code[102]={0};
    int k=0,a=0;
    int L=102;
    int i,j,n,Temp;
    fin = fopen(argv[1],"rb");
    fout=fopen(argv[2],"wb");
    if(argc!=3){
        printf("Argument count error!\n");
        return -1;
    }
    /**讀檔&計算算出現頻率，頻率為零的就不管**/
    while(!feof(fin)){
        fread(&c,1,1,fin);
        if(feof(fin))break;
        f[c]=f[c]+1;
    }
    for(i=0;i<=255;i++){
        if(f[i]!=0){
            node[k].cha=i;
            node[k].freq=f[i];
            k++;
        }
    }
    /**將計算好的頻率由小到大排序**/
    for(i=100;i>=0;i--){
        for(j=0;j<=i;j++){
            if(node[j].freq>node[j+1].freq){
            Temp=node[j].freq;
            node[j].freq=node[j+1].freq;
            node[j+1].freq=Temp;
            Temp=node[j].cha;
            node[j].cha=node[j+1].cha;
            node[j+1].cha=Temp;
            }
        }
    }
    /**開始使用Huffman，將最小的兩數相加，將編碼序記下來**/
    for(i=0;i<=101;i++)t[i]=node[i].freq;
    k=1;
    while(L>1){
        t[1]=t[0]+t[1];
        t[0]=0;
        N[k]=t[1];
        for(i=0;i<=L-2;i++)t[i]=t[i+1];//left shift
        L--;
        for(i=L-1;i>0;i--){
            for(j=0;j<i;j++){
                if(t[j]>t[j+1]){
                Temp=t[j];
                t[j]=t[j+1];
                t[j+1]=Temp;
                }
            }
        }
        Temp=0;
            while(a<=L){
                if(N[k]==t[a]){
                    code[k]=a;
                    Temp=1;
                }
                a++;
                if(Temp==1) break;
            }
        a=0;
        k++;
    }
    /**用編碼序算出每個ARCII在每次計算後的位置**/
    a=0;
    for(k=0;k<=101;k++){
            node[k].code[0]=k;
            for(i=1;i<=101;i++){
                if(node[k].code[i-1]==0 || node[k].code[i-1]==1){//0or1的就跟著編碼序
                    node[k].code[i]=code[i];
                }
                else if(node[k].code[i-1]-2 < code[i]){
                   node[k].code[i]= node[k].code[i-1]-2;
                }
                else{
                    node[k].code[i]= node[k].code[i-1]-1;
                }
            }
            a=0;
    }
    /**將算出的位置倒過來，且將非0或1的去掉，輸出檔案**/
    i=0;
    n=4;
    for(i=0;i<=101;i++){
            cb[node[i].cha].c=node[i].cha;
        for(j=101;j>=0;j--){
            if(node[i].code[j]==0||node[i].code[j]==1){
                cb[node[i].cha].hc[cb[node[i].cha].len] = node[i].code[j];
                cb[node[i].cha].len++;
            }
        }
    }
    /**將編碼長度相同的以ASCII順序排列**/
    for(i=0;i<=101;i++){
    }
    for(i=4;i<=19;i++){
        for(j=0;j<=255;j++){
            if(cb[j].len!=0&&cb[j].len==i){
                fprintf(fout,"%d\t",cb[j].c);
                for(k=2;k<=cb[j].len;k++)fprintf(fout,"%d",cb[j].hc[k-1]);
                fprintf(fout,"\r\n");
            }
        }
    }
    fclose(fin);
    fclose(fout);
    return 0;
}
