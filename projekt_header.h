#ifndef projekt_header_h
#define projekt_header_h

typedef struct polaznik {
	int ID;
	char ime[20];
	char prezime[20];
	char OIB[12];
	char polozeniPropisi[3];
	char polozenaPrvaPomoc[3];
	char placenVozacki[3];
	char brojSati[4];
	char sljedeciRed;
}POLAZNIK;


void kreiranje(char*, unsigned int*);
void unosPolaznika(char*, unsigned int*);
void izbornik(char*, unsigned int*);
void pretrazivanjePrezime(char* ,unsigned int*);
void brisanje(char*, unsigned int*);
void odabir(char*, unsigned int*);
void sortiranje(char*, unsigned int*);
void sortiranjeVece(char*, unsigned int*);
void popis(char*, unsigned int*);
void selekcija(char*, unsigned int*);
void propisi(char*, unsigned int*);
void prvaPomoc(char*, unsigned int*);
void placenVozacki(char*, unsigned int*);
void brisanjeDatoteke(char*, unsigned int*);

#endif