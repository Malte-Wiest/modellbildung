#include "Schwingung.h"

const double Schwingung::pi = 3.14159265358979323846;

Schwingung::Schwingung(void)
{
}


Schwingung::~Schwingung(void)
{
}

Schwingung::Schwingung(int size)
{
	this->setSize(size);
}

void Schwingung::setSize(int size)
{
	this->usedComplexElements = size;
	int vecSize = 2;
	while (vecSize < (this->usedComplexElements * 2))
	{
		vecSize *= 2;
	}
	this->mSchwingung.resize(vecSize, 0);
}

void Schwingung::addElement(int index, double re, double im)
{
	this->mSchwingung[2 * index] = re;
	this->mSchwingung[2 * index + 1] = im;
}

double Schwingung::getRealPart(int index)
{
	return this->mSchwingung.at(index * 2);
}

double Schwingung::getImaginaryPart(int index)
{
	return this->mSchwingung.at(index * 2 + 1);
}

void Schwingung::swapElements(int i, int j)
{
	double help = this->mSchwingung[i];
	this->mSchwingung[i] = this->mSchwingung[j];
	this->mSchwingung[j] = help;
}

void Schwingung::fourier(bool inverse)
{
    unsigned long n, mmax, m, j, istep, i;
    double wtemp, wr, wpr, wpi, wi, theta;
    double tempr, tempi;
 
    // reverse-binary reindexing
    n = this->usedComplexElements << 1;
    j = 1;
    for (i = 1; i < n; i += 2) {
        if (j > i) {
            swapElements(this->mSchwingung[j-1], this->mSchwingung[i-1]);
            swapElements(this->mSchwingung[j], this->mSchwingung[i]);
        }
        m = this->usedComplexElements;
        while (m >= 2 && j > m) {
            j -= m;
            m >>= 1;
        }
        j += m;
    };
 
    // here begins the Danielson-Lanczos section
    mmax = 2;
    while (n > mmax) 
	{
        istep = mmax<<1;
        theta = -(2*this->pi / mmax);
		if (inverse)
		{
			theta *= (-1);
		}
        wtemp = sin(0.5 * theta);
        wpr = -2.0 * wtemp * wtemp;
        wpi = sin(theta);
        wr = 1.0;
        wi = 0.0;
        for (m = 1; m < mmax; m += 2) {
            for (i = m; i <= n; i += istep) {
                j = i + mmax;
                tempr = wr * this->mSchwingung[j-1] - wi * this->mSchwingung[j];
                tempi = wr * this->mSchwingung[j] + wi * this->mSchwingung[j-1];
 
                this->mSchwingung[j-1] = this->mSchwingung[i-1] - tempr;
                this->mSchwingung[j] = this->mSchwingung[i] - tempi;
                this->mSchwingung[i-1] += tempr;
                this->mSchwingung[i] += tempi;
            }
            wtemp = wr;
            wr += wr * wpr - wi * wpi;
            wi += wi * wpr + wtemp * wpi;
        }
        mmax = istep;
    }
}

double Schwingung::realSignalRauschAbstand(Schwingung& vergleich)
{
	double sumOrig = 0;
	double sumVergleichMinusOrig = 0;
	double help;
	for (int i = 0; i < this->usedComplexElements; ++i)
	{
		sumOrig += (this->getRealPart(i) * this->getRealPart(i));
		help = vergleich.getRealPart(i) - this->getRealPart(i);
		sumVergleichMinusOrig += (help * help);
	}
	if (sumVergleichMinusOrig == 0)
	{
		return 10000000.0;
	}
	double erg = sumOrig / sumVergleichMinusOrig;
	erg = std::log(erg);
	erg *= 10;
	return erg;
}
