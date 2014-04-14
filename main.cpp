#include "math.h"
#include <assert.h>
#include <iostream>
#include <fstream>
#include "Schwingung.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>

const double pi = 3.14159265359;
int arraySize;

int writeWav(double* signal) {
	
	return 0;
}

void write_little_endian(unsigned int word, int num_bytes, FILE *wav_file)
{
    unsigned buf;
    while(num_bytes>0)
    {   buf = word & 0xff;
        fwrite(&buf, 1,1, wav_file);
        num_bytes--;
    word >>= 8;
    }
}
 
void write_wav(char * filename, unsigned long num_samples, int * data, int s_rate)
{
    FILE* wav_file;
    unsigned int sample_rate;
    unsigned int num_channels;
    unsigned int bytes_per_sample;
    unsigned int byte_rate;
    unsigned long i;    /* counter for samples */
 
    num_channels = 1;   /* monoaural */
    bytes_per_sample = 2;
 
    if (s_rate<=0) sample_rate = 44100;
    else sample_rate = (unsigned int) s_rate;
 
    byte_rate = sample_rate*num_channels*bytes_per_sample;
 
    wav_file = fopen(filename, "w");
    assert(wav_file);   /* make sure it opened */
 
    /* write RIFF header */
    fwrite("RIFF", 1, 4, wav_file);
    write_little_endian(36 + bytes_per_sample* num_samples*num_channels, 4, wav_file);
    fwrite("WAVE", 1, 4, wav_file);
 
    /* write fmt  subchunk */
    fwrite("fmt ", 1, 4, wav_file);
    write_little_endian(16, 4, wav_file);   /* SubChunk1Size is 16 */
    write_little_endian(1, 2, wav_file);    /* PCM is format 1 */
    write_little_endian(num_channels, 2, wav_file);
    write_little_endian(sample_rate, 4, wav_file);
    write_little_endian(byte_rate, 4, wav_file);
    write_little_endian(num_channels*bytes_per_sample, 2, wav_file);  /* block align */
    write_little_endian(8*bytes_per_sample, 2, wav_file);  /* bits/sample */
 
    /* write data subchunk */
    fwrite("data", 1, 4, wav_file);
    write_little_endian(bytes_per_sample* num_samples*num_channels, 4, wav_file);
    for (i=0; i< num_samples; i++)
    {   write_little_endian((unsigned int)(data[i]),bytes_per_sample, wav_file);
    }
 
    fclose(wav_file);
}

int* sinusSignalGenerator(double amp, double freq, double phase, int time, int abtastrate) {
	
	arraySize = (int)ceil(abtastrate*time);
	int * signal = new int[arraySize];
	float freq_radians_per_sample = freq*2*M_PI/abtastrate;
	for(int i = 0; i < arraySize; i++) {
		signal[i] = (int)amp*sin((freq_radians_per_sample*i)+phase);
	}
	return signal;
}

char* intToByte(int n) {
	char * bytes = new char[4];

	bytes[0] = (n >> 24) & 0xFF;
	bytes[1] = (n >> 16) & 0xFF;
	bytes[2] = (n >> 8) & 0xFF;
	bytes[3] = n & 0xFF;

	return bytes;
}

int manipulateWavHeader(std::string filename, int abtastrate) {
	
	char * bytes = new char[4];
	bytes = intToByte(abtastrate);
	FILE* oldFile = fopen("blah.wav", "r");
	FILE* newFile = fopen("manipulated.wav", "w");
	
	char * buffer = new char[1000];

	// 	'RIFF'
	fread(buffer, 1, 4, oldFile);
    fwrite(buffer, 1, 4, newFile);
	
	// dateigroesse
	fread(buffer, 1, 4, oldFile);
    fwrite(buffer, 1, 4, newFile);
	
	//WAVE
	fread(buffer, 1, 4, oldFile);
    fwrite(buffer, 1, 4, newFile);
	
	//fmt
	fread(buffer, 1, 4, oldFile);
    fwrite(buffer, 1, 4, newFile);
	
	//fmt length
	fread(buffer, 1, 4, oldFile);
    fwrite(buffer, 1, 4, newFile);
    
	//format tag
	fread(buffer, 1, 2, oldFile);
    fwrite(buffer, 1, 2, newFile);
	
	//channels
	fread(buffer, 1, 2, oldFile);
    fwrite(buffer, 1, 2, newFile);
	
	//sample rate
	fread(buffer, 1, 4, oldFile);
    write_little_endian(8000,4,newFile);
	//fwrite(bytes, 1, 4, newFile);

	//bytes/sec
	fread(buffer, 1, 4, oldFile);
    write_little_endian(16000,4,newFile);
	//fwrite(buffer, 1, 4, newFile);

	//block align
	fread(buffer, 1, 2, oldFile);
    fwrite(buffer, 1, 2, newFile);
	
	//bits/sample
	fread(buffer, 1, 2, oldFile);
    fwrite(buffer, 1, 2, newFile);

	//data
	fread(buffer, 1, 4, oldFile);
    fwrite(buffer, 1, 4, newFile);
	fread(buffer, 1, 4, oldFile);
    fwrite(buffer, 1, 4, newFile);

	while(!feof(oldFile)) {
		fread(buffer,1, 1000, oldFile);
		fwrite(buffer,1,1000,newFile);
	}
	
	fclose(oldFile);
	fclose(newFile);
	
	return 0;
}

int main(int argc, char* argv[])
{	
	//char * abtast = new char[4];
	//abtast = intToByte(8000);
	//for(int i = 0; i < 4; i++)
		//printf("%x", abtast[i]);	
	//write_wav("test3.wav", arraySize, sinusSignalGenerator(32000, 440, 0, 5, 16000), 16000);
	manipulateWavHeader("blah.wav", 8000);

	//sinusSignalGenerator(3,3,3,3,3);
	std::ifstream ifs(".wav", std::ios::binary | std::ifstream::in);
	char buff[36];
	ifs.seekg (0, std::ios::beg);
	ifs.read(buff, 36);
	for(int i = 0; i < 36; i++) {
		printf("%x \n", buff[i] );
	}

	std::string a;
	std::cin >> a;
    return 0;
}