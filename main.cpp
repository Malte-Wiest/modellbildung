#include <QApplication>
#include <QFont>
#include <QPushButton>
#include "math.h"
#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <fstream>

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
	char bytes[4];

	bytes[0] = (n >> 24) & 0xFF;
	bytes[1] = (n >> 16) & 0xFF;
	bytes[2] = (n >> 8) & 0xFF;
	bytes[3] = n & 0xFF;

	return bytes;
}

int manipulateWavHeader(std::string filename, int abtastrate) {
	
  std::ofstream ofs (filename.c_str(), std::ofstream::out | std::ofstream::binary | std::ofstream::app);
  ofs.seekp(std::ofstream::beg);
  ofs.seekp(28);
  ofs.write (intToByte(abtastrate), 4);
  ofs.close();
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QPushButton quit("Quit");

    quit.resize(500, 500);
    quit.setFont(QFont("Times", 18, QFont::Bold));

    QObject::connect(&quit, SIGNAL(clicked()), &app, SLOT(quit()));

    quit.show();
	
	//sinusSignalGenerator(3,3,3,3,3);

	//write_wav("test3.wav", arraySize, sinusSignalGenerator(32000, 440, 0, 5, 16000), 16000);
	manipulateWavHeader("TestdateiBlah.wav", 8000);

    return app.exec();
}