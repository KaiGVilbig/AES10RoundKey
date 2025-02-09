#include <iostream>
#include <iomanip>
#include "s-box.h"
#include "RCon.h"

const std::vector<uint8_t> key = { 0x39, 0xEF, 0x7c, 0x5E, 0x43, 0xB4, 0xF9, 0xCE, 0x5E, 0xF3, 0x37, 0x3C, 0x3B, 0xD2, 0x64, 0xB6 };
//const std::vector<uint8_t> key = { 0xaa, 0xaa, 0xaa, 0xaa, 0xbb, 0xbb, 0xbb, 0xbb, 0xcc, 0xcc, 0xcc, 0xcc, 0x01, 0x02, 0x03, 0x04 };

// Get s-box value
uint8_t getSBoxValue(uint8_t input) {
	return sBox[input];
}

void createKeys(std::vector<uint32_t>& words) {

	for (int i = 0; i < 16; i += 4) {
		uint32_t word = (key[i] << 24) | (key[i + 1] << 16) | (key[i + 2] << 8) | key[i + 3];
		words.push_back(word);
	}
}

uint32_t rotWord(uint32_t word) {
	return (word << 8) | (word >> (32 - 8));
}

uint32_t subWord(uint32_t word) {
	uint8_t b0 = (word >> 24) & 0xFF;
	uint8_t b1 = (word >> 16) & 0xFF;
	uint8_t b2 = (word >> 8) & 0xFF;
	uint8_t b3 = word & 0xFF;

	return (getSBoxValue(b0) << 24) | (getSBoxValue(b1) << 16) | (getSBoxValue(b2) << 8) | getSBoxValue(b3);
}

void generateRoundKeys(std::vector<uint32_t>& words) {
	for (int i = 1; i <= 10; i++) {
		uint32_t temp = words[(i - 1) * 4 + 3];
		uint32_t rot = rotWord(temp);
		uint32_t sub = subWord(rot);
		uint32_t w0 = sub ^ words[(i - 1) * 4];
		w0 = w0 ^ RCon[i - 1];
		words.push_back(w0);
		uint32_t w1 = words[(i - 1) * 4 + 1] ^ words[i * 4];
		words.push_back(w1);
		uint32_t w2 = words[(i - 1) * 4 + 2] ^ words[i * 4 + 1];
		words.push_back(w2);
		uint32_t w3 = words[(i - 1) * 4 + 3] ^ words[i * 4 + 2];
		words.push_back(w3);
	}
}

void getRoundKeys(std::vector<uint32_t>& words) {

	std::cout << "Below is the AES-128 key schedule for the key: " << std::hex << (int)words[0] <<
		std::hex << (int)words[1] << std::hex << (int)words[2] << std::hex << (int)words[3] << std::endl;
	for (int i = 4; i < 44; i+=4) {
		std::cout << "Round " << std::dec << (i - 1) / 4 + 1 << " key: " << std::hex << std::setfill('0') << std::setw(8) << (int)words[i] <<
			std::hex << std::setfill('0') << std::setw(8) << (int)words[i + 1] << std::hex << std::setfill('0') << std::setw(8) << (int)words[i + 2] << 
			std::hex << std::setfill('0') << std::setw(8) << (int)words[i + 3] << std::endl;
	}
}

int main() {
	std::vector<uint32_t> words;
	createKeys(words);
	generateRoundKeys(words);
	getRoundKeys(words);
}