#pragma once
#include "common.h"
#include <regex>

class VigenereCipher {
public:
	/*
	* Encrypt the text using the generated key
	*/
	void encrypt(char* temporary, int textLength, char matrix[][SIZE], string key, string word);

	/*
	* Decrypt the text using the generated key
	*/
	void decrypt(char* temporary, int textLength, char matrix[][SIZE], string key, string word);

	/*
	* Construct the key needed for the encryption/decryption process
	*/
	string constructKey(int keyLength, string keyword);
};