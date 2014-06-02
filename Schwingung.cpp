#include "Schwingung.h"

const double Schwingung::pi=4*atan((double)1);

Schwingung::Schwingung(void)
{
	vector=NULL;  
}


Schwingung::~Schwingung(void)
{
	if(vector!=NULL)   
		 delete [] vector;
}

Schwingung::Schwingung(int size, int sampleRate)
{
	vector=NULL;  
	this->sampleRate = sampleRate;
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
	int sign;
	if (inverse)
		sign = 1;
	else
		sign = -1;
    //variables for the fft   
    unsigned long n,mmax,m,j,istep,i;   
    double wtemp,wr,wpr,wpi,wi,theta,tempr,tempi;   
   
    //the complex array is real+complex so the array    
    //as a size n = 2* number of complex samples   
    //real part is the data[index] and    
    //the complex part is the data[index+1]   
   
    //new complex array of size n=2*sample_rate   
    if(vector!=NULL)   
        delete [] vector;   
	int size = 2;
	while (size < (2*this->sampleRate))
		size*=2;
	vector=new float [size];
	for (int index=0; index<size; index++)
		vector[index] = 0;
   
    //put the real array in a complex array   
    //the complex part is filled with 0's   
    //the remaining vector with no data is filled with 0's  
	
    for(n=0; n<2*this->sampleRate;n++)   
    {   
		vector[n] = this->mSchwingung[n];
		/*if(n<this->usedComplexElements)   {
			vector[2*n]=this->getRealPart(n);   
			vector[2*n+1] = this->getImaginaryPart(n);
		}
        else   {
            vector[2*n]=0;   
			vector[2*n+1]=0;  
		}*/
		
    }   
   
    //binary inversion (note that the indexes    
    //start from 0 witch means that the   
    //real part of the complex is on the even-indexes    
    //and the complex part is on the odd-indexes)   
    n=this->sampleRate << 1;   
    j=0;   
    for (i=0;i<n/2;i+=2) {   
        if (j > i) {   
            SWAP(vector[j],vector[i]);   
            SWAP(vector[j+1],vector[i+1]);   
            if((j/2)<(n/4)){   
                SWAP(vector[(n-(i+2))],vector[(n-(j+2))]);   
                SWAP(vector[(n-(i+2))+1],vector[(n-(j+2))+1]);   
            }   
        }   
        m=n >> 1;   
        while (m >= 2 && j >= m) {   
            j -= m;   
            m >>= 1;   
        }   
        j += m;   
    }
	
    //end of the bit-reversed order algorithm   
   
    //Danielson-Lanzcos routine   
    mmax=2;   
    while (n > mmax) {   
        istep=mmax << 1;   
        theta=sign*(2*pi/mmax);   
        wtemp=sin(0.5*theta);   
        wpr = -2.0*wtemp*wtemp;   
        wpi=sin(theta);   
        wr=1.0;   
        wi=0.0;   
        for (m=1;m<mmax;m+=2) {   
            for (i=m;i<=n;i+=istep) {   
                j=i+mmax;   
                tempr=wr*vector[j-1]-wi*vector[j];   
                tempi=wr*vector[j]+wi*vector[j-1];  
				if (abs(vector[j]) > 100000000000 || abs(vector[j]) < -100000000000)
					int a = 0;
				if (abs(vector[i]) > 100000000000 || abs(vector[i]) < -100000000000)
					int a = 0;
				if (abs(vector[j-1]) > 100000000000 || abs(vector[j-1]) < -100000000000)
					int a = 0;
				if (abs(vector[i]-1) > 100000000000 || abs(vector[i]-1) < -100000000000)
					int a = 0;
                vector[j-1]=vector[i-1]-tempr;   
                vector[j]=vector[i]-tempi;   
                vector[i-1] += tempr;   
                vector[i] += tempi;   
            }   
            wr=(wtemp=wr)*wpr-wi*wpi+wr;   
            wi=wi*wpr+wtemp*wpi+wi;   
        }   
        mmax=istep;   
    }   
	float temp = vector[0];
   float temp2 = vector[1];
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

double Schwingung::getAbsoluteValue(int index)
{
	double erg = this->getRealPart(index) * this->getRealPart(index);
	double temp = this->getImaginaryPart(index) * this->getImaginaryPart(index);
	erg += temp;
	erg = std::sqrt(erg);
	return erg;
}

void Schwingung::qunatisierungRealValues(double delta)
{
	int signum;
	double quantValue, temp;
	for (int i=0; i<this->usedComplexElements; ++i)
	{
		signum = 1;
		if (this->getRealPart(i) < 0)
			signum = (-1);
		quantValue = delta;
		temp = this->getAbsoluteValue(i) / delta;
		temp += 0.5;
		quantValue *= std::floor(temp);
		quantValue *= signum;
		this->addElement(i, quantValue);
	}
}

double Schwingung::getMaxValue()
{
	double max = this->getRealPart(0);
	for (int i=1; i<this->usedComplexElements; ++i)
	{
		if (max < this->getRealPart(i))
			max = this->getRealPart(i);
	}
	return max;
}

double Schwingung::getMinValue()
{
	double min = this->getRealPart(0);
	for (int i=1; i<this->usedComplexElements; ++i)
	{
		if (min > this->getRealPart(i))
			min = this->getRealPart(i);
	}
	return min;
}

int Schwingung::maxFrequency()
{
	int fundamental_frequency=0;   
	double max = sqrt(pow(vector[0],2)+pow(vector[0+1],2));
	double temp;
    for(int i=2; i<=this->sampleRate/2; i+=2)   
    {   
		temp = sqrt(pow(vector[i],2)+pow(vector[i+1],2));
        if(temp>max){   
            fundamental_frequency=i;   
			max = temp;
        }   
    }   
   
    //since the array of complex has the format [real][complex]=>[absolute value]   
    //the maximum absolute value must be ajusted to half   
    fundamental_frequency=(long)floor((float)fundamental_frequency/2);  
	return fundamental_frequency;
}

void Schwingung::calculateStatistics()
{
	double sum = 0;
	double varianzSumme = 0;
	for (int i = 0; i < this->usedComplexElements; ++i)
	{
		sum += this->getRealPart(i);
	}
	this->Mittelert = sum / this->usedComplexElements;
	sum = 0;
	double temp;
	for (int i = 0; i < this->usedComplexElements; ++i)
	{
		temp = this->getRealPart(i) - this->Mittelert;
		sum += temp * temp;
	}
	this->Varianz = (1/(this->usedComplexElements - 1) * sum);
	this->Standardabweichung = std::sqrt(this->Varianz);
}

double Schwingung::getRealMittelwert()
{
	return this->Mittelert;
}

double Schwingung::getRealStandardabweichung()
{
	return this->Standardabweichung;
}

double Schwingung::getRealVarianz()
{
	return this->Varianz;
}