#include "SoundManager.h"
#include <iostream>
#include <math.h>
#include "SFML/System.hpp"

Mp3::Mp3() : handle(NULL), bufferSize(0), buffer(0) {
	int err = MPG123_OK;
	if((err = mpg123_init()) != MPG123_OK) {
		std::cerr << mpg123_plain_strerror(err) << std::endl;
		return;
	}

	handle = mpg123_new(NULL, &err);
	if(!handle) {
		std::cerr << "Unable to create mpg123 handle: " << mpg123_plain_strerror(err) << std::endl;
	}
}

Mp3::~Mp3() {
	stop();

	if(buffer) {
		delete buffer;
		buffer = NULL;
	}

	mpg123_close(handle);
	mpg123_delete(handle);
	mpg123_exit();
}

bool Mp3::openFromFile(const std::string& fileName, size_t fftChunkSize) {
	stop();
	if(buffer) {
		delete buffer;
	}

	if(handle) {
		mpg123_close(handle);
	}

	if(mpg123_open(handle, fileName.c_str()) != MPG123_OK) {
		std::cerr << mpg123_strerror(handle) << std::endl;
		return false;
	}

	long rate = 0;
	int channels = 0;
	int encoding = 0;
	if(mpg123_getformat(handle, &rate, &channels, &encoding) != MPG123_OK) {
		std::cerr << "Failed to get format information for " << fileName << std::endl;
		return false;
	}
	std::cout << rate << std::endl;

	bufferSize = mpg123_outblock(handle);
	std::cout << bufferSize << std::endl;
	buffer = new unsigned char[bufferSize];
	if(!buffer) {
		std::cerr << "Failed to reserve memory for " << fileName << std::endl;
		return false;
	}
	fftChunkSize = fftChunkSize;
	initialize(channels, rate);
	return true;
}

bool Mp3::onGetData(Chunk& data) {
	sf::Lock lock(soundMutex);

	if(handle) {
		size_t done;
		mpg123_read(handle, buffer, bufferSize, &done);

		data.samples = (short*) buffer;
		data.sampleCount = done/sizeof(short);
		std::copy(data.samples, data.samples + data.sampleCount, std::back_inserter(preprocSamples));

		return data.sampleCount > 0;
	}
	else {
		return false;
	}
}

void Mp3::onSeek(sf::Time timeOffset) {
	sf::Lock lock(soundMutex);

	if(handle) {
		mpg123_seek(handle, timeOffset.asSeconds(), 0);
	}
}

bool Mp3::fillBuffer(std::vector<double>& inputVector) {
	sf::Lock lock(soundMutex);;
	if(preprocSamples.size() >= fftChunkSize * 2) {
		dComplex* preFFTSamples = new dComplex[fftChunkSize];
		for(std::size_t i = 0; i < fftChunkSize * 2; i += 2) {
			preFFTSamples[i / 2] = dComplex(((preprocSamples[i] + preprocSamples[i + 1]) / (32767.0 * 2)), 0.0);
		}
		preprocSamples.erase(preprocSamples.begin(), preprocSamples.begin() + fftChunkSize * 2);
		dComplex* postFFTSamples = FFTSimple(preFFTSamples, fftChunkSize);
		for(size_t i = 0; i < fftChunkSize; i++) {
			inputVector.insert(inputVector.begin(), std::norm(postFFTSamples[i]));
		}
		delete preFFTSamples;
		delete postFFTSamples;
		return true;
	}
	return false;
}

void Mp3::FFTCalculate(dComplex* x, int N, int skip, dComplex* X, dComplex* D, dComplex* twiddles) const {
	dComplex* E = D + N/2;
	int k;

	if(N == 1)
	{
		X[0] = x[0];
		return;
	}

	FFTCalculate(x, N/2, skip * 2, E, X, twiddles);
	FFTCalculate(x + skip, N/2, skip * 2, D, X, twiddles);

	for(k = 0; k < N / 2; k++)
	{
		D[k] = twiddles[k  * skip] * D[k];
	}

	for(k = 0; k < N / 2; k++)
	{
		X[k] = E[k] + D[k];
		X[k + N / 2] = E[k] - D[k];
	}
}

dComplex* Mp3::FFTTwiddleFactors(int N) const {
	dComplex* twiddles = new dComplex[N / 2];
	int k;

	for(k = 0; k < N / 2; k++)
	{
		twiddles[k] = std::polar(1.0, -2.0 * PI * k / N);
	}

	return twiddles;
}

dComplex* Mp3::FFTSimple(dComplex* x, int N) const {
	dComplex* ret = new dComplex[N];
	dComplex* scratch = new dComplex[N];
	dComplex* twiddles = FFTTwiddleFactors(N);
	FFTCalculate(x, N, 1, ret, scratch, twiddles);
	delete twiddles;
	delete scratch;
	return ret;
}


void SoundManager::init() {
	gameSong.openFromFile("song.mp3", 1024);
	gameSong.play();

	lastBandBuffer.resize(64, 0.0);
	energyHistoryBuffer.resize(47, std::vector<double>(64, 0.0));
}

void SoundManager::update() {
	if(gameSong.fillBuffer(dCModBuffer)) {
		frameCalc = true;
		for(int i = 0; i < 64; i++)
		{
			double subInstantEnergy = 0;
			for(std::vector<double>::iterator itor = dCModBuffer.begin() + (i * 16); itor < dCModBuffer.begin() + ((i + 1) * 16); itor++)
			{
				subInstantEnergy += (*itor);
			}
			energyBandBuffer.insert(energyBandBuffer.begin(), (1.0 / 16.0) * subInstantEnergy);
		}
		for(int i = 0; i < 64; i++)
		{
			double energySum = 0;
			for(int j = 0; j < 47; j++)
			{
				energySum += energyHistoryBuffer[j][i];
			}
			meanBandBuffer.insert(meanBandBuffer.begin(), (1.0 / 47.0) * energySum);
		}
		frameBeat = false;
		for(int i = 0; i < 64; i++)
		{
			if(energyBandBuffer[i] > 5 * meanBandBuffer[i] && energyBandBuffer[i] > 0.01)
			{
				frameBandBeats[i] = false;
				frameBandRise[i] = true;
			}
			else if(energyBandBuffer[i] < lastBandBuffer[i] && frameBandRise[i] == true)
			{
				frameBeat = true;
				frameBandBeats[i] = true;
				frameBandRise[i] = false;
			}
			else
			{
				frameBandBeats[i] = false;
			}
		}
		lastBandBuffer = energyBandBuffer;

		energyHistoryBuffer.insert(energyHistoryBuffer.begin(), energyBandBuffer);
		energyHistoryBuffer.pop_back();

		dCModBuffer.clear();
		energyBandBuffer.clear();
		meanBandBuffer.clear();
	}
}

bool SoundManager::isFrameBeat() const{
	return frameBeat;
}

bool SoundManager::getBandBeat(int band) const {
	if(band >= 0 && band <= 63) {
		return frameBandBeats[band];
	}
	else {
		return false;
	}
}


