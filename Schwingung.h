#pragma once

#include <vector>
#include <math.h>

#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr 

class Schwingung
{
private:
	static const double pi;
	int sampleRate;
	int usedComplexElements;
	std::vector<double> mSchwingung;
	std::vector<double> sinusPart;
	std::vector<double> cosinusPart;
	std::vector<double> inverseFourierResult;
	std::vector<double> betragsspectrum;
	std::vector<double> phasenspectrum;
	float* vector;
	double Mittelert;
	double Varianz;
	double Standardabweichung;

public:
	Schwingung(void);
	Schwingung(int size, int sampleRate);
	~Schwingung(void);

	void setSize(int size);
	void fourier(bool inverse = false);
	void swapElements(int i, int j);
	void addElement(int index, double re, double im = 0);
	double realSignalRauschAbstand(Schwingung& vergleich);
	double getRealPart(int index);
	double getImaginaryPart(int index);
	double getAbsoluteValue(int index);
	void qunatisierungRealValues(double delta);
	double getMaxValue();
	double getMinValue();
	int maxFrequency();
	void calculateStatistics();
	double getRealMittelwert();
	double getRealVarianz();
	double getRealStandardabweichung();
	void neue_fourier_transformation();
	void neue_inverse_fourier();
	int getMaxSinusPart();
	int getMaxCosPart();
	void calculateBetragsspectrum();
	void calculatePhasenspectrum();
	std::vector<double>& getBetragsspectrum();
	std::vector<double>& getPhasenspectrum();
};

