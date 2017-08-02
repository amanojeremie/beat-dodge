#include "PlayState.h"
#include <iostream>
#include <math.h>
#include <SFML/Graphics.hpp>
#include "WindowManager.h"
#include "SoundManager.h"

void PlayState::init() {
	beats.reserve(512);

	player.shape.setRadius(8.f);
	player.shape.setFillColor(sf::Color::Magenta);
}

void PlayState::update(GameManager& game, WindowManager& window, SoundManager& sound, float delta) {
	for(std::vector<Beat>::iterator iter = beats.begin(); iter < beats.end(); iter++) {
		iter->position += iter->velocity * delta;
		if(sound.isFrameBeat()) {
			iter->shape.setFillColor(sf::Color::Cyan);
		}
		else {
			iter->shape.setFillColor(sf::Color::Blue);
		}
		if(collideBeat(iter)) {
			beats.erase(iter);
			continue;
		}
		cleanupBeat(iter);
	}

	for(int i = 0; i < 64; i++) {
		if(sound.getBandBeat(i)) {
			spawnBeat(i);
		}
	}

	float mouseAngle = atan2f(window.getMousePosition().y - 360.f, window.getMousePosition().x - 640.f);
	player.position.x = 640.f + cosf(mouseAngle) * 163.f;
	player.position.y = 360.f + sinf(mouseAngle) * 163.f;
}

bool PlayState::collideBeat(std::vector<Beat>::iterator beat) {
	if((beat->position.x - player.position.x) * (beat->position.x - player.position.x) +
		(beat->position.y - player.position.y) * (beat->position.y - player.position.y)
		<= 64.f) {
			return true;
		}
	else {
		return false;
	}
}

void PlayState::cleanupBeat(std::vector<Beat>::iterator beat) {
	if(beat->position.x <= -8.f || beat->position.x >= 1288.f || beat->position.y <= -8.f || beat->position.y >= 728.f) {
		beats.erase(beat);
	}
}

void PlayState::spawnBeat(int i) {
	Beat newBeat;
	newBeat.band = i;
	newBeat.shape.setFillColor(sf::Color::Cyan);
	newBeat.shape.setRadius(8.f);
	newBeat.position.x = 1280.f / 2.f;
	newBeat.position.y = 720.f / 2.f;
	newBeat.velocity.x = cosf((i / 64.f) * 2 * PI) * 163.f;
	newBeat.velocity.y = sinf((i / 64.f) * 2 * PI) * 163.f;
	beats.push_back(newBeat);
}

void PlayState::render(WindowManager& window) {
	player.shape.setPosition(player.position);
	window.draw(player.shape);

	for(std::vector<Beat>::iterator iter = beats.begin(); iter < beats.end(); iter++) {
		(*iter).shape.setPosition((*iter).position);
		window.draw((*iter).shape);
	}
}

void PlayState::pause() {
}

void PlayState::resume() {
}

void PlayState::cleanup() {
	beats.clear();
}
