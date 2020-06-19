#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define double_pi 6.283185//���w�qDFT�ഫ�ɷ|�Ψ쪺��

//�إ�.wav��Header�ɮ榡
struct Wave{
    unsigned char RIFF[4];
    unsigned int overall_size;
    unsigned char wave[4];
    unsigned char fmt_chunk_maker[4];
    unsigned int length_of_fmt;
    unsigned int format_type;
    unsigned int channels;
    unsigned int sample_rate;
    unsigned int byterate;
    unsigned int block_align;
    unsigned int bits_per_sample;
    unsigned char data_chunk_header[4];
    unsigned int data_size;
    unsigned int data;
};

int main(int argc,char *argv[])
{
    //buffer�O�Ψӥ�����Ʀ�m,�]���b�x�s�ɦ���32bits��16bits��
    unsigned char buffer2[2];
    unsigned char buffer4[4];
    unsigned int i=0;
    struct Wave wav;
    FILE *input;
    FILE *output_magnitude;
    FILE *output_phase;
    input = fopen(argv[1],"rb");
    if(argc!=4){
        printf("Argumeht count error!\n");
        return -1;
    }
    //Ū�� Header��
    fread(&wav.RIFF,4,1,input);
    printf("ChunkID:\t%.4s\n",wav.RIFF);
    wav.overall_size = buffer4[0] |//int���ŧi�����ݭn���ƥ�쥿�T����m
                                (buffer4[1]<<8) |
                                (buffer4[2]<<16) |
                                (buffer4[3]<<24);
    fread(&wav.overall_size,4,1,input);
    printf("ChunkSize:\t%u\n",wav.overall_size);
    fread(&wav.wave,4,1,input);
    printf("Format:\t\t%.4s\n",wav.wave);
    fread(&wav.fmt_chunk_maker,4,1,input);
    printf("Subchunk1ID:\t%.4s\n",wav.fmt_chunk_maker);
    wav.length_of_fmt = buffer4[0] |
                                (buffer4[1]<<8) |
                                (buffer4[2]<<16) |
                                (buffer4[3]<<24);
    fread(&wav.length_of_fmt,4,1,input);
    printf("Subchunk1Size:\t%u\n",wav.length_of_fmt);
    wav.format_type = buffer2[0] |
                                (buffer2[1]<<8);
    fread(&wav.format_type,2,1,input);
    printf("AudioFormat:\t%u\n",wav.format_type);
    wav.channels = buffer2[0] |
                                (buffer2[1]<<8);
    fread(&wav.channels,2,1,input);
    printf("NumChannel:\t%u\n",wav.channels);
    wav.sample_rate = buffer4[0] |
                                (buffer4[1]<<8) |
                                (buffer4[2]<<16) |
                                (buffer4[3]<<24);
    fread(&wav.sample_rate,4,1,input);
    printf("SampleRate:\t%u\n",wav.sample_rate);
    wav.byterate = buffer4[0] |
                                (buffer4[1]<<8) |
                                (buffer4[2]<<16) |
                                (buffer4[3]<<24);
    fread(&wav.byterate,4,1,input);
    printf("ByteRate:\t%u\n",wav.byterate);
    wav.block_align = buffer2[0] |
                                (buffer2[1]<<8);
    fread(&wav.block_align,2,1,input);
    printf("BlockAlign:\t%u\n",wav.block_align);
    wav.bits_per_sample = buffer2[0] |
                                (buffer2[1]<<8);
    fread(&wav.bits_per_sample,2,1,input);
    printf("BitsPerSample:\t%u\n",wav.bits_per_sample);
    fread(&wav.data_chunk_header,4,1,input);
    printf("SubChunk2ID:\t%.4s\n",wav.data_chunk_header);
    wav.data_size = buffer4[0] |
                                (buffer4[1]<<8) |
                                (buffer4[2]<<16) |
                                (buffer4[3]<<24);
    fread(&wav.data_size,4,1,input);
    printf("SubChunk2Size:\t%u\n",wav.data_size);
    //Ū��data
    unsigned long datasize = wav.data_size;
    float x[datasize/2];
    for(i=1;i<=datasize/2;i=++i){//�C��Sample����bytes�ҥH�`�@��56000/2��Sample
            wav.data = buffer2[0] |
                                (buffer2[1]<<8);
            fread(&wav.data,2,1,input);
            unsigned long intdata = wav.data;
            long complement = 0;
            if(intdata>32765)//32765����}�l���̤p�t��,�ҥH�n�N�t�ƴ��^��
            {
                complement = intdata - 65536;
            }
            else
            {
                complement = intdata;//��L���N�O����
            }
            double float_data = (double)(complement/(double)32768);//��j�p����-1~1
            x[i]=float_data;
    }
    fclose(input);
    //DFT�ഫ
    float Xre[datasize/2];
    float Xim[datasize/2];
    float P[datasize/2];
    float A[datasize/2];
    float D[datasize/2];
    int k;
    output_magnitude = fopen(argv[2],"wb");
    output_phase = fopen(argv[3],"wb");
    printf("Runing......\n");
    for(k=0;k<=datasize/2;++k){
            Xre[k]=0;
            Xim[k]=0;
            for(i=0;i<=datasize/2;++i){//DFT����
                Xre[k]+=x[i]*cos(i*k*double_pi/(datasize/2));
                Xim[k]-=x[i]*sin(i*k*double_pi/(datasize/2));
            }
            P[k] = Xre[k]*Xre[k]+Xim[k]*Xim[k];
            A[k]=20*log10(P[k]/20);//�p��_�T
            D[k]=atan(Xim[k]/Xre[k]);//�p��ۦ�
            fwrite(&A[k],sizeof(A[k]),1,output_magnitude);
            fwrite(&D[k],sizeof(D[k]),1,output_phase);
        }
    fclose(output_magnitude);
    fclose(output_phase);
    printf("End.\n");
    return 0;
}
