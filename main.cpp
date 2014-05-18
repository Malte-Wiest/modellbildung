#include "math.h"
#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Schwingung.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>

int zahl_kanal_elemente;
int *kanalEins;
int *kanalZwei;
int *kanalDrei;
int *kanalVier;
const double pi = 3.14159265359;
int arraySize;

int byteToInt(int num_bytes, char * bytes) {
	int result = 0;

	if(num_bytes == 2) {
		result = bytes[0] | bytes[1] << 8;
	} else if(num_bytes == 4) {
		result = (result << 8) + bytes[3];
		result = (result << 8) + bytes[2];
		result = (result << 8) + bytes[1];
		result = (result << 8) + bytes[0];
	}

	return result;
}

unsigned int endian_swap(unsigned int x)
{
    x = (x>>24) | 
        ((x<<8) & 0x00FF0000) |
        ((x>>8) & 0x0000FF00) |
        (x<<24);
	return x;
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
	
	double tmp = abtastrate*time;
	arraySize = (int)ceil(tmp);
	int * signal = new int[arraySize];
	float freq_radians_per_sample = freq*2*pi/abtastrate;
	for(int i = 0; i < arraySize; i++) {
		signal[i] = (int)amp*sin((freq_radians_per_sample*i)+phase);
		printf("%i",signal[i]);
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
	FILE* oldFile = fopen(filename.c_str(), "r");
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

void readSoundFile(char * filename) {

	FILE* oldFile = fopen(filename, "r");
	
	char * buffer = new char[1000];

	// 	'RIFF'
	fread(buffer, 1, 4, oldFile);
	
	// dateigroesse
	fread(buffer, 1, 4, oldFile);
	
	//WAVE
	fread(buffer, 1, 4, oldFile);
	
	//fmt
	fread(buffer, 1, 4, oldFile);
	
	//fmt length
	fread(buffer, 1, 4, oldFile);
    
	//format tag
	fread(buffer, 1, 2, oldFile);
	
	//channels
	fread(buffer, 1, 2, oldFile);
	int channels = byteToInt(2, buffer);
	
	//sample rate
	fread(buffer, 1, 4, oldFile);
	int sample_rate = byteToInt(4, buffer);

	switch (channels) {
		case 1:
			kanalEins = new int[600*sample_rate];
			break;
		case 2:
			kanalEins = new int[600*sample_rate];
			kanalZwei = new int[600*sample_rate];
			break;
		case 3:
			kanalEins = new int[600*sample_rate];
			kanalZwei = new int[600*sample_rate];
			kanalDrei = new int[600*sample_rate];
			break;		
		case 4:
			kanalEins = new int[600*sample_rate];
			kanalZwei = new int[600*sample_rate];
			kanalDrei = new int[600*sample_rate];
			kanalVier = new int[600*sample_rate];
			break;				
		default:
			break;
	}

	//bytes/sec
	fread(buffer, 1, 4, oldFile);

	//block align
	fread(buffer, 1, 2, oldFile);
	int block_align = byteToInt(2, buffer);
	
	//bits/sample
	fread(buffer, 1, 2, oldFile);
	int bits_sample = byteToInt(2, buffer);
	
	//data
	fread(buffer, 1, 4, oldFile);
	fread(buffer, 1, 4, oldFile);

	zahl_kanal_elemente = 0;
	while(!feof(oldFile)) {
		fread(buffer,1, block_align, oldFile);
		/*switch (channels) {
			case 1:
			case 2:
			case 3:
			case 4:
			default:
		}*/
		zahl_kanal_elemente++;
	}
	
	fclose(oldFile);
}

int main(int argc, char* argv[])
{	
	//char * abtast = new char[4];
	//abtast = intToByte(8000);
	//for(int i = 0; i < 4; i++)
		//printf("%x", abtast[i]);	
	//write_wav("test3.wav", arraySize, sinusSignalGenerator(32000, 440, 0, 5, 16000), 16000);
	/*manipulateWavHeader("blah.wav", 8000);

	arraySize = (int)ceil(abtastrate*time);
 * //sinusSignalGenerator(3,3,3,3,3);
	std::ifstream ifs(".wav", std::ios::binary | std::ifstream::in);
	char buff[36];
	ifs.seekg (0, std::ios::beg);
	ifs.read(buff, 36);
	for(int i = 0; i < 36; i++) {
		printf("%x \n", buff[i] );
	}*/

/*	readSoundFile("blah.wav");
	readSoundFile("manipulated.wav");
	readSoundFile("test.wav");
	readSoundFile("test2.wav");
	readSoundFile("test3.wav");
	readSoundFile("writeTest.wav");*/
	
	int * blah = sinusSignalGenerator(32000, 440, 0, 5, 16);
	
	std::string a;
	std::cin >> a;
    return 0;
}