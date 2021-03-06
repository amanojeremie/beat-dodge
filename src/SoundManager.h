#ifndef SOUNDMANAGER_H_INCLUDED
#define SOUNDMANAGER_H_INCLUDED

#include <complex>
#include <vector>
#include "SFML/Audio.hpp"
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

		dComplex* FFTSimple(dComplex* x, int N) const;
		dComplex* FFTTwiddleFactors(int N) const;
		void FFTCalculate(dComplex* x, int N, int skip, dComplex* X, dComplex* D, dComplex* twiddles) const;
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
		std::vector<bool> frameBandRise;
		std::vector<bool> frameBandBeats;
	public:
		void init(const std::string& fileName);
		void update();
		void cleanup();
		bool isFrameBeat() const;
		bool getBandBeat(int band) const;
};

#endif // SOUNDMANAGER_H_INCLUDED
