#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define pi 3.1415926//先定義計算時會用到的pi值

/**建立.wav的Header檔格式**/
struct Wave{
    char RIFF[4];
    uint32_t ChunkSize;
    char WAVE[4];
    char fmt[4];
    uint32_t Subchunk1Size;
    uint16_t AudioFromat;
    uint16_t Channels;
    uint32_t SampleRate;
    uint32_t ByteRate;
    uint16_t BlockAlign;
    uint16_t BitsPerSample;
    char dataID[4];
    uint32_t data_size;
};

int main()
{
    /**建立Cosine的.wav Header檔形式**/
    struct Wave wav;
    wav.RIFF[0]='R';
    wav.RIFF[1]='I';
    wav.RIFF[2]='F';
    wav.RIFF[3]='F';
    wav.WAVE[0]='W';
    wav.WAVE[1]='A';
    wav.WAVE[2]='V';
    wav.WAVE[3]='E';
    wav.fmt[0]='f';
    wav.fmt[1]='m';
    wav.fmt[2]='t';
    wav.fmt[3]=' ';
    wav.Subchunk1Size=16;
    wav.AudioFromat=1;
    wav.Channels=1;
    wav.BlockAlign=2;
    wav.BitsPerSample=16;
    wav.dataID[0]='d';
    wav.dataID[1]='a';
    wav.dataID[2]='t';
    wav.dataID[3]='a';

    /**將Header寫入每個Cosine的.wav檔**/
    int i,j,k,Hz;
    char filename[25];
    FILE *fp;

    for(j=0;j<=7;j++){
        if(j==0||j==4)Hz=50;
        if(j==1||j==5)Hz=55;
        if(j==2||j==6)Hz=200;
        if(j==3||j==7)Hz=220;
        if(j<=3)k=8000;
        if(j>=4)k=16000;
        if(Hz<100&&k==8000)sprintf(filename,"cos_0%dHz-8k.wav",Hz);
        if(Hz>100&&k==8000)sprintf(filename,"cos_%dHz-8k.wav",Hz);
        if(Hz<100&&k==16000)sprintf(filename,"cos_0%dHz-16k.wav",Hz);
        if(Hz>100&&k==16000)sprintf(filename,"cos_%dHz-16k.wav",Hz);
        wav.ChunkSize=2*k+8;
        wav.SampleRate=k;
        wav.ByteRate=2*k;
        wav.data_size=2*k;
        fp = fopen(filename,"wb");//依序開檔
        short x[k];
        fwrite(&wav.RIFF,4,1,fp);
        fwrite(&wav.ChunkSize,4,1,fp);
        fwrite(&wav.WAVE,4,1,fp);
        fwrite(&wav.fmt,4,1,fp);
        fwrite(&wav.Subchunk1Size,4,1,fp);
        fwrite(&wav.AudioFromat,2,1,fp);
        fwrite(&wav.Channels,2,1,fp);
        fwrite(&wav.SampleRate,4,1,fp);
        fwrite(&wav.ByteRate,4,1,fp);
        fwrite(&wav.BlockAlign,2,1,fp);
        fwrite(&wav.BitsPerSample,2,1,fp);
        fwrite(&wav.dataID,4,1,fp);
        fwrite(&wav.data_size,4,1,fp);
        for(i=0;i<=k;i++){
            x[i]=10000*cos((2*pi*Hz*i)/k);//計算Cosine .wav的data
            fwrite(&x[i],2,1,fp);//寫入.wav檔
        }
        fclose(fp);
    }
    /**讀取每個檔案的data，然後開始做Setting**/
    int M,frame,P,N,dftframe,a,b,c,datasize,samplerate;
    FILE *fp1;
    FILE *fp2;
    FILE *fp3;
    FILE *fp4;
    short data[112000];
    char textname1[25],textname2[25],textname3[25],textname4[25];
    float FRAME[600],Xre[600],Xim[600],APT[600];
    for(j=0;j<=9;j++){
        if(j==0||j==4)Hz=50;
        if(j==1||j==5)Hz=55;
        if(j==2||j==6)Hz=200;
        if(j==3||j==7)Hz=220;
        if(j<=3)k=8000;
        if(j>=4)k=16000;
        if(Hz<100&&k==8000){
           sprintf(filename,"cos_0%dHz-8k.wav",Hz);
           sprintf(textname1,"cos_0%dHz-8k{Set1}.txt",Hz);
           sprintf(textname2,"cos_0%dHz-8k{Set2}.txt",Hz);
           sprintf(textname3,"cos_0%dHz-8k{Set3}.txt",Hz);
           sprintf(textname4,"cos_0%dHz-8k{Set4}.txt",Hz);
        }
        if(Hz>100&&k==8000){
            sprintf(filename,"cos_%dHz-8k.wav",Hz);
            sprintf(textname1,"cos_%dHz-8k{Set1}.txt",Hz);
            sprintf(textname2,"cos_%dHz-8k{Set2}.txt",Hz);
            sprintf(textname3,"cos_%dHz-8k{Set3}.txt",Hz);
            sprintf(textname4,"cos_%dHz-8k{Set4}.txt",Hz);
        }
        if(Hz<100&&k==16000){
            sprintf(filename,"cos_0%dHz-16k.wav",Hz);
            sprintf(textname1,"cos_0%dHz-16k{Set1}.txt",Hz);
            sprintf(textname2,"cos_0%dHz-16k{Set2}.txt",Hz);
            sprintf(textname3,"cos_0%dHz-16k{Set3}.txt",Hz);
            sprintf(textname4,"cos_0%dHz-16k{Set4}.txt",Hz);
        }
        if(Hz>100&&k==16000){
            sprintf(filename,"cos_%dHz-16k.wav",Hz);
            sprintf(textname1,"cos_%dHz-16k{Set1}.txt",Hz);
            sprintf(textname2,"cos_%dHz-16k{Set2}.txt",Hz);
            sprintf(textname3,"cos_%dHz-16k{Set3}.txt",Hz);
            sprintf(textname4,"cos_%dHz-16k{Set4}.txt",Hz);
        }
        if(j==8){
            sprintf(filename,"vowel-8k.wav");
            sprintf(textname1,"vowel-8k{Set1}.txt");
            sprintf(textname2,"vowel-8k{Set2}.txt");
            sprintf(textname3,"vowel-8k{Set3}.txt");
            sprintf(textname4,"vowel-8k{Set4}.txt");
        }
        if(j==9){
            sprintf(filename,"vowel-16k.wav");
            sprintf(textname1,"vowel-16k{Set1}.txt");
            sprintf(textname2,"vowel-16k{Set2}.txt");
            sprintf(textname3,"vowel-16k{Set3}.txt");
            sprintf(textname4,"vowel-16k{Set4}.txt");
        }

        fp=fopen(filename,"rb");//依序開檔
        printf("%d. Running %s:\n",j+1,filename);
        fp1=fopen(textname1,"wb");
        fp2=fopen(textname2,"wb");
        fp3=fopen(textname3,"wb");
        fp4=fopen(textname4,"wb");
        fseek(fp,24,SEEK_SET);
        fread(&samplerate,4,1,fp);//讀取SampleRate
        fseek(fp,40,SEEK_SET);
        fread(&datasize,4,1,fp);//讀取datasize
        for(i=0;i<=datasize/2;i++){
            fread(&data[i],2,1,fp);
        }
        /**Set1**/
        M=samplerate*0.005;//frame size
        frame=(datasize/2)/M;//frame 總數
        P=samplerate*0.005;//window size
        N=samplerate*0.008;//dft window size
        printf("Setting 1 ...\n");
        for(a=0;a<frame;a++){//每個frame 跑一次
            for(i=0;i<N;i++){//每個dft window 跑一次
                    if(i<P)FRAME[i]=data[i+a*M]*1;//Retangular Window Function
                    else FRAME[i]=0;//大於 p 的 zero padding
            }
            for(i=0;i<N;i++){//DFT
                    Xre[i]=0;
                    Xim[i]=0;
                    for(c=0;c<N;c++){
                    Xre[i]+=FRAME[c]*cos(i*c*2*pi/N);
                    Xim[i]-=FRAME[c]*sin(i*c*2*pi/N);
                    }
                    APT[i]=20*log10(sqrt(Xre[i]*Xre[i]+Xim[i]*Xim[i]));
                    fprintf(fp1,"%f  ",APT[i]);//寫檔
               }
               fprintf(fp1,"\n");
        }
        /**Set2**/
        printf("Setting 2 ...\n");
        for(a=0;a<frame;a++){
            for(i=0;i<N;i++){
                    if(i<P)FRAME[i]=data[i+a*M]*(0.54-0.46*cos((2*pi*i)/(P-1)));//Hammimg Window Function
                    else FRAME[i]=0;
               }
               for(i=0;i<N;i++){
                    Xre[i]=0;
                    Xim[i]=0;
                    for(c=0;c<N;c++){
                        Xre[i]+=FRAME[c]*cos(i*c*2*pi/N);
                        Xim[i]-=FRAME[c]*sin(i*c*2*pi/N);
                    }
                    APT[i]=20*log10(sqrt(Xre[i]*Xre[i]+Xim[i]*Xim[i]));
                    fprintf(fp2,"%f  ",APT[i]);
               }
               fprintf(fp2,"\n");
        }
        /**Set3**/
        M=samplerate*0.01;//Setting 3&4設定不同，須重設
        frame=(datasize/2)/M;
        P=samplerate*0.02;
        N=samplerate*0.032;
        printf("Setting 3 ...\n");
        for(a=0;a<frame;a++){
            for(i=0;i<N;i++){
                    if(i<P)FRAME[i]=data[i+a*M]*1;
                    else FRAME[i]=0;
            }
            for(i=0;i<N;i++){
                    Xre[i]=0;
                    Xim[i]=0;
                    for(c=0;c<N;c++){
                        Xre[i]+=FRAME[c]*cos(i*c*2*pi/N);
                        Xim[i]-=FRAME[c]*sin(i*c*2*pi/N);
                    }
                    APT[i]=20*log10(sqrt(Xre[i]*Xre[i]+Xim[i]*Xim[i]));
                    fprintf(fp3,"%f  ",APT[i]);
               }
               fprintf(fp3,"\n");
        }
        /**Set4**/
        printf("Setting 4 ...\n\n");
        for(a=0;a<frame;a++){
            for(i=0;i<N;i++){
                    if(i<P)FRAME[i]=data[i+a*M]*(0.54-0.46*cos((2*pi*i)/(P-1)));
                    else FRAME[i]=0;
               }
               for(i=0;i<N;i++){
                    Xre[i]=0;
                    Xim[i]=0;
                    for(c=0;c<N;c++){
                        Xre[i]+=FRAME[c]*cos(i*c*2*pi/N);
                        Xim[i]-=FRAME[c]*sin(i*c*2*pi/N);
                    }
                    APT[i]=20*log10(sqrt(Xre[i]*Xre[i]+Xim[i]*Xim[i]));
                    fprintf(fp4,"%f  ",APT[i]);
               }
               fprintf(fp4,"\n");
        }
        fclose(fp);//關闢所有檔案
        fclose(fp1);
        fclose(fp2);
        fclose(fp3);
        fclose(fp4);
    }
    printf("\n\tProgram END.\n\n");
    return 0;
}
