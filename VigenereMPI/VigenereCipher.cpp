#include "vigenere_cipher.h"

void VigenereCipher::encrypt(char* temporary, int textLength, char matrix[][SIZE], string key, string word) {
	for (int iterator = 0; iterator < textLength; iterator++) {
		// the row is given by the letter from the key and the column is given by the letter from the word
		temporary[iterator] = matrix[key[iterator] - 'A'][word[iterator] - 'A'];
	}
	temporary[textLength] = '\0';
}

void VigenereCipher::decrypt(char* temporary, int textLength, char matrix[][SIZE], string key, string word) {
	int column;
	for (int iterator = 0; iterator < textLength; iterator++) {
		bool found = false;
		/* the row is given by the key's letter and the word's letter is searched and the column
			will be the corresponding letter
		*/
		for (int iterator_letter = 0; iterator_letter < SIZE && !found; iterator_letter++) {
			if (matrix[key[iterator] - 'A'][iterator_letter] == word[iterator]) {
				column = iterator_letter;
				found = true;
			}
		}
		temporary[iterator] = 'A' + column;
	}
	temporary[textLength] = '\0';
}

string VigenereCipher::constructKey(int keyLength, string keyword) {
	string key;

	int quotient = keyLength / keyword.length();
	int reminder = keyLength % keyword.length();

	// repeat the key
	for (int iterator = 0; iterator < quotient; iterator++) {
		key.append(keyword);
	}

	// add the first characters in order to complete the key
	key.append(keyword.substr(0, reminder));
	transform(key.begin(), key.end(), key.begin(), ::toupper);
	return key;
}