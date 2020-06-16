#define _CRT_SECURE_NO_WARNINGS
#include "projekt_header.h"


int main() {

	char* polaznici = "polaznici.bin";
	unsigned int brojPolaznika = 1;

	kreiranje(polaznici, &brojPolaznika);
	izbornik(polaznici, &brojPolaznika);

	return 0;

}