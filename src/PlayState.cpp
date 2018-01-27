#include "PlayState.h"
#include <iostream>
#include <math.h>
#include "SFML/Graphics.hpp"
#include "WindowManager.h"
#include "SoundManager.h"

static const double PI =  3.141592653589793238460;
static const double DEGRAG = 180/PI;
static const float FIELD_WIDTH = 1280.f;
static const float FIELD_HEIGHT = 720.f;
static const float PLAYER_RADIUS = 163.f;
static const float CIRCLE_RADIUS = 8.f;
static const float BEAT_VELOCITY = 163.f;
static const int BEAT_RESERVE = 512;
static const int BAND_COUNT = 64;

void PlayState::init() {
	stackSize = 0;
	beats.reserve(BEAT_RESERVE);

	player.shape.setRadius(CIRCLE_RADIUS);
	player.shape.setFillColor(sf::Color::Magenta);
}

void PlayState::update(GameManager& game, WindowManager& window, SoundManager& sound, float delta) {
	for(unsigned int i = 0; i < beats.size(); i++) {
		Beat& beat = beats.at(i);
		beat.position += beat.velocity * delta;

		if(sound.isFrameBeat()) {
			beat.shape.setFillColor(sf::Color::Cyan);
		}
		else {
			beat.shape.setFillColor(sf::Color::Blue);
		}

		if(collideBeat(beat)) {
			beats.erase(beats.begin() + i);
			continue;
		}

		if(cleanupBeat(beat)) {
			beats.erase(beats.begin() + i);
		}
	}

	for(int i = 0; i < BAND_COUNT; i++) {
		if(sound.getBandBeat(i)) {
			spawnBeat(i);
		}
	}

	float mouseAngle = atan2f(window.getMousePosition().y - FIELD_HEIGHT / 2.f, 
		window.getMousePosition().x - FIELD_WIDTH / 2.f);
	player.position.x = FIELD_WIDTH / 2.f + cosf(mouseAngle) * PLAYER_RADIUS;
	player.position.y = FIELD_HEIGHT / 2.f + sinf(mouseAngle) * PLAYER_RADIUS;
}

bool PlayState::collideBeat(const Beat& beat) const {
	if((beat.position.x - player.position.x) * (beat.position.x - player.position.x) +
		(beat.position.y - player.position.y) * (beat.position.y - player.position.y)
		<= CIRCLE_RADIUS * CIRCLE_RADIUS) {
			return true;
		}
	else {
		return false;
	}
}

bool PlayState::cleanupBeat(const Beat& beat) const {
	if(beat.position.x <= 0.f - CIRCLE_RADIUS
		|| beat.position.x >= FIELD_WIDTH + CIRCLE_RADIUS
		|| beat.position.y <= 0.f - CIRCLE_RADIUS 
		|| beat.position.y >= FIELD_HEIGHT + CIRCLE_RADIUS) {
		return true;
	}
	else {
		return false;
	}
}

void PlayState::spawnBeat(int i) {
	Beat newBeat;
	newBeat.band = i;
	newBeat.shape.setFillColor(sf::Color::Cyan);
	newBeat.shape.setRadius(8.f);
	newBeat.position.x = FIELD_WIDTH / 2.f;
	newBeat.position.y = FIELD_HEIGHT / 2.f;
	newBeat.velocity.x = cosf(((float) i / BAND_COUNT) * 2 * PI) * BEAT_VELOCITY;
	newBeat.velocity.y = sinf(((float) i / BAND_COUNT) * 2 * PI) * BEAT_VELOCITY;
	beats.push_back(newBeat);

	if(beats.capacity() != stackSize) {
		stackSize = beats.capacity();
	}
}

void PlayState::render(WindowManager& window) {
	player.shape.setPosition(player.position);
	window.draw(player.shape);

	for(unsigned int i = 0; i < beats.size(); i++) {
		beats.at(i).shape.setPosition(beats.at(i).position);
		window.draw(beats.at(i).shape);
	}
}

void PlayState::pause() {
}

void PlayState::resume() {
}

void PlayState::cleanup() {
	beats.clear();
}
