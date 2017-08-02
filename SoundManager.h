#ifndef SOUNDMANAGER_H_INCLUDED
#define SOUNDMANAGER_H_INCLUDED

#ifndef PI
#define PI 3.141592653589793238460
#endif

#include <SFML/Audio.hpp>
#include <complex>
#include <vector>
#include "mpg123.h"

typedef std::complex<double> dComplex;

class Mp3 : public sf::SoundStream {
private:
	mpg123_handle* handle;
	size_t bufferSize;
	unsigned char* buffer;
	std::vector<sf::Int16> preprocSamples;
	size_t fftChunkSize;
	sf::Mutex soundMutex;

	dComplex* FFTSimple(dComplex* x, int N);
	dComplex* FFTTwiddleFactors(int N);
	void FFTCalculate(dComplex* x, int N, int skip, dComplex* X, dComplex* D, dComplex* twiddles);
protected:
	bool onGetData(Chunk& data);
	void onSeek(sf::Time timeOffset);
public:
	Mp3();
	~Mp3();

	bool openFromFile(const std::string& fileName, size_t fftChunkSize);
	bool fillBuffer(std::vector<double>& inputManager);
};

class SoundManager {
private:
	Mp3 gameSong;
	std::vector<double> dCModBuffer;
	std::vector<double> lastBandBuffer;
	std::vector<double> energyBandBuffer;
	std::vector<double> meanBandBuffer;
	std::vector< std::vector<double> > energyHistoryBuffer;
	bool frameCalc;
	bool frameBeat;
	bool frameBandRise[64];
	bool frameBandBeats[64];
	double waveForm[1024];
public:
	void init();
	void update();
	void cleanup();
	bool isFrameBeat();
	bool getBandBeat(int band);
};

#endif // SOUNDMANAGER_H_INCLUDED
