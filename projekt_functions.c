#define _CRT_SECURE_NO_WARNINGS
#include "projekt_header.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>


void kreiranje(char* naziv, unsigned int* broj) {

	FILE* fp = fopen(naziv, "rb");

	if (fp == NULL) {
		perror("Datoteka ne postoji");

		fp = fopen(naziv, "wb");

		if (fp == NULL) {
			perror("\nGreska prilikom kreiranja datoteke");
			exit(EXIT_FAILURE);
		}
		else {
			fwrite(broj, sizeof(unsigned int), 1, fp);
			fclose(fp);
			printf("\nDatoteke kreirana.");
		}
	}
	else {
		fread(broj, sizeof(unsigned int), 1, fp);
		printf("\nDatoteka postoji na disku.");
		fclose(fp);
	}
}

void unosPolaznika(char* naziv, unsigned int* broj) {

	FILE* fp = NULL;
	char string[] = { 'd', 'a', '\0' };
	char string1[] = { 'n', 'e', '\0' };
	fp = fopen(naziv, "rb+");

	if (fp == NULL) {
		perror("Nemoguæe dodavanje novog polaznika u datoteku");
		return;
	}
	else {
		POLAZNIK novi = { 0 };
		novi.ID = (*broj)++;
		printf("\nUnesite ime:\n");
		scanf("%19s", novi.ime);
		printf("Unesite prezime:\n");
		scanf("%19s", novi.prezime);
		printf("Unesite OIB osobe:\n");
		scanf("%11s", novi.OIB);
		printf("Polozeni propisi[da/ne]:\n");
		do {
			scanf("%3s", novi.polozeniPropisi);
		} while ((strcmp(novi.polozeniPropisi, string) != 0) && (strcmp(novi.polozeniPropisi, string1) != 0));

		printf("Polozena prva pomoc[da/ne]:\n");
		do {
			scanf("%3s", novi.polozenaPrvaPomoc);
		} while ((strcmp(novi.polozenaPrvaPomoc, string) != 0) && (strcmp(novi.polozenaPrvaPomoc, string1) != 0));

		printf("Placen vozacki[da,ne]:\n");
		do {
			scanf("%3s", novi.placenVozacki);
		} while ((strcmp(novi.placenVozacki, string) != 0) && (strcmp(novi.placenVozacki, string1) != 0));

		printf("Broj odvozenih sati:\n");
		scanf("%3s", novi.brojSati);


		fseek(fp, sizeof(unsigned int) + ((*broj - 1) * sizeof(POLAZNIK)), SEEK_SET);
		fwrite(&novi, sizeof(POLAZNIK), 1, fp);
		rewind(fp);
		fwrite(broj, sizeof(unsigned int), 1, fp);
		fclose(fp);
	}


}

void pretrazivanjePrezime(char* naziv, unsigned int* broj) {
	FILE* fp = NULL;
	fp = fopen(naziv, "rb");
	 unsigned int i,brojac = 0, indeks = 0;
	 char prez[20] = { '\0' };

	if (naziv == NULL) {
		printf("Nemoguce otvaranje datoteke za citanje\n");
		return;
	}
	else {
		POLAZNIK* polaznici = NULL;

		fread(broj, sizeof(unsigned int), 1, fp);

		if (*broj == 1) {
			printf("Nije ni jedan polaznik unesen u datoteku\n");
			fclose(fp);
			return;
		}
		else {
			polaznici = (POLAZNIK*)calloc(*broj, sizeof(POLAZNIK));

			if (polaznici == NULL) {
				printf("Neuspjesno zauzimanje memorije\n");
			}

			else {
				fread(polaznici, sizeof(POLAZNIK), *broj, fp);
				fclose(fp);
				printf("Unesite prezime osobe koje zelite pronaci:\n");
				scanf("%19s", &prez);
				for (i = 0; i < *broj; i++) {
					if (strcmp((polaznici + i)->prezime, prez) == 0) {
						indeks = i;
						brojac++;
						printf("%d\t", (polaznici + indeks)->ID);
						printf("%s  ", (polaznici + indeks)->ime);
						printf("%s  ", (polaznici + indeks)->prezime);
						printf("%s  ", (polaznici + indeks)->OIB);
						printf("%s  ", (polaznici + indeks)->polozeniPropisi);
						printf("%s  ", (polaznici + indeks)->polozenaPrvaPomoc);
						printf("%s  ", (polaznici + indeks)->placenVozacki);
						printf("%s  \n", (polaznici + indeks)->brojSati);
					}
				}
				if (brojac == 0) {
					printf("Osoba s prezimenom %s ne nalazi se u popisu polaznika", prez);
				}
				free(polaznici);

					
			}
			
		}
	}
}

void brisanje(char* naziv, unsigned int* broj) {
	FILE* fp = NULL;
	fp = fopen(naziv, "rb");
	unsigned int i, brojac = 0;
	char oib[12] = {'\0'};

	if (fp == NULL) {
		printf("Nemoguce otvaranje datoteke za citanje\n");
		return;
	}
	else {
		POLAZNIK* polaznici = NULL;

		fread(broj, sizeof(unsigned int), 1, fp);

		if (*broj == 1) {
			printf("Nije ni jedan polaznik unesen u datoteku\n");
			fclose(fp);
			return;
		}
		else {
			polaznici = (POLAZNIK*)calloc(*broj, sizeof(POLAZNIK));

			if (polaznici == NULL) {
				printf("Neuspjesno zauzimanje memorije\n");
			}

			else {
				fread(polaznici, sizeof(POLAZNIK), *broj, fp);
				fclose(fp);
				printf("Unesite OIB osobe koju zelite obrisati:\n");
				scanf("%11s", &oib);
				for (i = 0; i < *broj; i++) {
					if (strcmp((polaznici + i)->OIB, oib) == 0) {
						brojac++;
						(*broj)--;
						break;
					}
				}
				if (brojac == 0) {
					printf("Osoba s OIB - om %s ne nalazi se u popisu polaznika", oib);
					free(polaznici);
					return;
				}

				fp = fopen(naziv, "wb");
				if (fp == NULL) {
					printf("Nemoguce otvaranje datoteke za pisanje\n");
					free(polaznici);
					return;
				}
				else {
					fwrite(broj, sizeof(unsigned int), 1, fp);
					brojac = 0;
					for (i = 0; i < (*broj) + 1; i++) {
						if (strcmp((polaznici + i)->OIB, oib) == 0) {
							brojac++;
							continue;
						}
						else {
							if (brojac == 0) {
								fwrite((polaznici + i), sizeof(POLAZNIK), 1, fp);
							}
							else {
								(polaznici + i)->ID = (polaznici + i)->ID--;
								fwrite((polaznici + i), sizeof(POLAZNIK), 1, fp);
							}
						}
					}
					fclose(fp);
					free(polaznici);
				}
			}
		}
	}
}


void odabir(char* naziv,unsigned int* broj) {
	int izbor;

	printf("\n1-od najmanjeg broja sati do najveceg:");
	printf("\n2-od najveceg broja sati do najmanjeg\n");
	do {
		scanf("%d", &izbor);
		if (izbor < 1 || izbor > 2)
		{
			printf("\nPogresan unos,ponovite");
		}
	} while (izbor < 1 || izbor > 2);

	if (izbor == 1) {
		sortiranje(naziv, broj);
	}
	else {
		sortiranjeVece(naziv, broj);
	}
	return;
}

void sortiranje(char* naziv, unsigned int* broj){

	
	FILE* fp = NULL;
	POLAZNIK* polaznici = NULL;
	POLAZNIK* privremeni = NULL;
	unsigned int i, j = 0, prvi, drugi;

	fp = fopen(naziv, "rb");

	if (fp == NULL) {
		printf("\nNeuspjelo otvaranje datoteke za citanje");
		return;
	}
	else {
		fread(broj, sizeof(unsigned int), 1, fp);
		if (*broj == 1) {
			printf("\nNemoguce sortiranje, nema unesenih polaznika");
			return;
		}

		polaznici = (POLAZNIK*)calloc(*broj, sizeof(POLAZNIK));

		if (polaznici == NULL) {
			printf("\nNemogucu zauzimanje memorije.");
			return;
		}

		privremeni = (POLAZNIK*)calloc(*broj, sizeof(POLAZNIK));
		if (privremeni == NULL) {
			printf("\nNemogucu zauzimanje memorije.");
			return;
		}
		fread(polaznici, sizeof(POLAZNIK), *broj, fp);
		fclose(fp);

		for (i = 0; i <= *broj - 1; i++) {
			for (j = i + 1; j <= (*broj) - 1; j++) {
				prvi = atoi((polaznici + i)->brojSati);
				drugi = atoi((polaznici + j)->brojSati);
				if (prvi > drugi) {
					memcpy(privremeni, (polaznici + i), sizeof(POLAZNIK));
					memcpy((polaznici + i), (polaznici + j), sizeof(POLAZNIK));
					memcpy((polaznici + j), privremeni, sizeof(POLAZNIK));

				}
			}
		}
		printf("\nSortirano.\n");

		for (i = 1; i <= *broj - 1; i++) {
			printf("%d\t %s    %s    %s    %s    %s    %s    %s\n", (polaznici + i)->ID, (polaznici + i)->ime, (polaznici + i)->prezime, (polaznici + i)->OIB, (polaznici + i)->polozeniPropisi, (polaznici + i)->polozenaPrvaPomoc, (polaznici + i)->placenVozacki, (polaznici + i)->brojSati);
		}

		free(polaznici);
		free(privremeni);
	}
	return;

}

void sortiranjeVece(char* naziv, unsigned int* broj){


	FILE* fp = NULL;
	POLAZNIK* polaznici = NULL;
	POLAZNIK* privremeni = NULL;
	unsigned int i, j = 0, prvi, drugi;

	fp = fopen(naziv, "rb");

	if (fp == NULL) {
		printf("\nNeuspjelo otvaranje datoteke za citanje");
		return;
	}
	else {
		fread(broj, sizeof(int), 1, fp);
		if (*broj == 1) {
			printf("\nNemoguce sortiranje, nema unesenih polaznika");
			return;
		}

		polaznici = (POLAZNIK*)calloc(*broj, sizeof(POLAZNIK));

		if (polaznici == NULL) {
			printf("\nNemogucu zauzimanje memorije.");
			return;
		}

		privremeni = (POLAZNIK*)calloc(*broj, sizeof(POLAZNIK));
		if (privremeni == NULL) {
			printf("\nNemogucu zauzimanje memorije.");
			return;
		}
		fread(polaznici, sizeof(POLAZNIK), *broj, fp);
		fclose(fp);

		for (i = 1; i <= *broj - 1; i++) {
			for (j = i + 1; j <= (*broj); j++) {
				prvi = atoi((polaznici + i)->brojSati);
				drugi = atoi((polaznici + j)->brojSati);
				if (prvi < drugi) {
					memcpy(privremeni, (polaznici + i), sizeof(POLAZNIK));
					memcpy((polaznici + i), (polaznici + j), sizeof(POLAZNIK));
					memcpy((polaznici + j), privremeni, sizeof(POLAZNIK));

				}
			}
		}
		printf("\nSortirano.\n");

		for (i = 1; i <= *broj - 1; i++) {
			printf("%d\t %s    %s    %s    %s    %s    %s    %s\n", (polaznici + i)->ID, (polaznici + i)->ime, (polaznici + i)->prezime, (polaznici + i)->OIB, (polaznici + i)->polozeniPropisi, (polaznici + i)->polozenaPrvaPomoc, (polaznici + i)->placenVozacki, (polaznici + i)->brojSati);
		}

		free(polaznici);
		free(privremeni);
	}
	return;

}

void popis(char* naziv, unsigned int* broj) {

	FILE* fp = NULL;
	fp = fopen(naziv, "rb");
	unsigned int i;

	if (naziv == NULL) {
		printf("Nemoguce otvaranje datoteke za citanje\n");
		return;
	}
	else {
		POLAZNIK* polaznici = NULL;

		fread(broj, sizeof(int), 1, fp);

		if (*broj == 1) {
			printf("Nije ni jedan polaznik unesen u datoteku\n");
			fclose(fp);
			return;
		}
		else {
			polaznici = (POLAZNIK*)calloc(*broj, sizeof(POLAZNIK));

			if (polaznici == NULL) {
				printf("Neuspjesno zauzimanje memorije\n");
			}

			else {
				fread(polaznici, sizeof(POLAZNIK), *broj, fp);
				fclose(fp);
				for (i = 0; i < *broj; i++) {
					if (i > 0) {
						printf("%d\t", (polaznici + i)->ID);
						printf("%s  ", (polaznici + i)->ime);
						printf("%s  ", (polaznici + i)->prezime);
						printf("%s  ", (polaznici + i)->OIB);
						printf("%s  ", (polaznici + i)->polozeniPropisi);
						printf("%s  ", (polaznici + i)->polozenaPrvaPomoc);
						printf("%s  ", (polaznici + i)->placenVozacki);
						printf("%s  \n", (polaznici + i)->brojSati);
					}
				}
				
				free(polaznici);


			}

		}
	}
}

void selekcija(char* naziv, unsigned int* broj) {

	int opcija;


	printf("\n1-polozeni propisi:");
	printf("\n2-polozena prva pomoc");
	printf("\n3-placen vozacki\n");
	do {
		scanf("%d", &opcija);
		if (opcija < 1 || opcija > 3)
		{
			printf("\nPogresan unos,ponovite\n");
		}
	} while (opcija < 1 || opcija > 3);

	if (opcija == 1) {
		propisi(naziv, broj);
	}
	if(opcija == 2) {
		prvaPomoc(naziv, broj);
	}
	if(opcija == 3){
		placenVozacki(naziv, broj);
	}
	return;


}


void propisi(char *naziv, unsigned int* broj) {

	char potvrda[] = { 'd', 'a', '\0' };
	FILE* fp = NULL;
	fp = fopen(naziv, "rb");
	unsigned int i;
	int brojac = 0, indeks = 0;

	if (naziv == NULL) {
		printf("Nemoguce otvaranje datoteke za citanje\n");
		return;
	}
	else {
		POLAZNIK* polaznici = NULL;

		fread(broj, sizeof(int), 1, fp);

		if (*broj == 1) {
			printf("Nije ni jedan polaznik unesen u datoteku\n");
			fclose(fp);
			return;
		}
		else {
			polaznici = (POLAZNIK*)calloc(*broj, sizeof(POLAZNIK));

			if (polaznici == NULL) {
				printf("Neuspjesno zauzimanje memorije\n");
			}

			else {
				fread(polaznici, sizeof(POLAZNIK), *broj, fp);
				fclose(fp);
				system("cls");
				for (i = 0; i < *broj; i++) {
					if (strcmp((polaznici + i)->polozeniPropisi, potvrda) == 0) {
						indeks = i;
						brojac++;
						printf("%d\t", (polaznici + indeks)->ID);
						printf("%s  ", (polaznici + indeks)->ime);
						printf("%s  ", (polaznici + indeks)->prezime);
						printf("%s  ", (polaznici + indeks)->OIB);
						printf("%s  ", (polaznici + indeks)->polozeniPropisi);
						printf("%s  ", (polaznici + indeks)->polozenaPrvaPomoc);
						printf("%s  ", (polaznici + indeks)->placenVozacki);
						printf("%s  \n", (polaznici + indeks)->brojSati);
					}
				}
				if (brojac == 0) {
					printf("\nNe postoji osoba s polozenim propisima u popisu");
				}
				free(polaznici);


			}

		}
	}

}


void prvaPomoc(char* naziv, unsigned int* broj) {

	char potvrda[] = { 'd', 'a', '\0' };
	FILE* fp = NULL;
	fp = fopen(naziv, "rb");
	unsigned int i;
	int brojac = 0, indeks = 0;

	if (naziv == NULL) {
		printf("Nemoguce otvaranje datoteke za citanje\n");
		return;
	}
	else {
		POLAZNIK* polaznici = NULL;

		fread(broj, sizeof(unsigned int), 1, fp);

		if (*broj == 1) {
			printf("Nije ni jedan polaznik unesen u datoteku\n");
			fclose(fp);
			return;
		}
		else {
			polaznici = (POLAZNIK*)calloc(*broj, sizeof(POLAZNIK));

			if (polaznici == NULL) {
				printf("Neuspjesno zauzimanje memorije\n");
			}

			else {
				fread(polaznici, sizeof(POLAZNIK), *broj, fp);
				fclose(fp);
				system("cls");
				for (i = 0; i < *broj; i++) {
					if (strcmp((polaznici + i)->polozenaPrvaPomoc, potvrda) == 0) {
						indeks = i;
						brojac++;
						printf("%d\t", (polaznici + indeks)->ID);
						printf("%s  ", (polaznici + indeks)->ime);
						printf("%s  ", (polaznici + indeks)->prezime);
						printf("%s  ", (polaznici + indeks)->OIB);
						printf("%s  ", (polaznici + indeks)->polozeniPropisi);
						printf("%s  ", (polaznici + indeks)->polozenaPrvaPomoc);
						printf("%s  ", (polaznici + indeks)->placenVozacki);
						printf("%s  \n", (polaznici + indeks)->brojSati);
					}
				}
				if (brojac == 0) {
					printf("\nNe postoji osoba s polozenom prvom pomoci u popisu");
				}
				free(polaznici);


			}

		}
	}


}


void placenVozacki(char* naziv, unsigned int* broj) {

	char potvrda[] = { 'd', 'a', '\0' };
	FILE* fp = NULL;
	fp = fopen(naziv, "rb");
	unsigned int i;
	int brojac = 0, indeks = 0;

	if (naziv == NULL) {
		printf("Nemoguce otvaranje datoteke za citanje\n");
		return;
	}
	else {
		POLAZNIK* polaznici = NULL;
		fread(broj, sizeof(unsigned int), 1, fp);

		if (*broj == 1) {
			printf("Nije ni jedan polaznik unesen u datoteku\n");
			fclose(fp);
			return;
		}
		else {
			polaznici = (POLAZNIK*)calloc(*broj, sizeof(POLAZNIK));

			if (polaznici == NULL) {
				printf("Neuspjesno zauzimanje memorije\n");
			}

			else {
				fread(polaznici, sizeof(POLAZNIK), *broj, fp);
				fclose(fp);
				system("cls");
				for (i = 0; i < *broj; i++) {
					if (strcmp((polaznici + i)->placenVozacki, potvrda) == 0) {
						indeks = i;
						brojac++;
						printf("%d\t", (polaznici + indeks)->ID);
						printf("%s  ", (polaznici + indeks)->ime);
						printf("%s  ", (polaznici + indeks)->prezime);
						printf("%s  ", (polaznici + indeks)->OIB);
						printf("%s  ", (polaznici + indeks)->polozeniPropisi);
						printf("%s  ", (polaznici + indeks)->polozenaPrvaPomoc);
						printf("%s  ", (polaznici + indeks)->placenVozacki);
						printf("%s  \n", (polaznici + indeks)->brojSati);
					}
				}
				if (brojac == 0) {
					printf("\nNe postoji osoba s placenim vozackim u popisu");
				}
				free(polaznici);


			}

		}
	}
}

void brisanjeDatoteke(char* naziv, unsigned int* broj) {
	remove(naziv);
	printf("\nDatoteka obrisana. Stvaranje nove.\n");
	(*broj) = 1;
	kreiranje(naziv, broj);
}

void izbornik(char* naziv, unsigned int* broj) {

	char opcija;

	do {
		printf("\n**************IZBORNIK**************");
		printf("\nEvidencija polaznika autoskole.\n");
		printf("1 - unos polaznika\n");
		printf("2 - pretrazivanje po prezimenu\n");
		printf("3 - brisanje polaznika(preko OIB-a):\n");
		printf("4 - sortiranje po broju odvozenih sati\n");
		printf("5 - pregled liste polaznika\n");
		printf("6 - odabir selekcije polaznika\n");
		printf("7 - brisanje datoteke\n");
		printf("0 - prekid programa\n");
		opcija = _getch();
		switch (opcija) {
		case '1':
			system("cls");
			unosPolaznika(naziv, broj);
			break;
		case '2':
			system("cls");
			pretrazivanjePrezime(naziv, broj);
			break;
		case '3':
			system("cls");
			brisanje(naziv, broj);
			break;
		case '4':
			system("cls");
			odabir(naziv, broj);
			break;
		case '5':
			system("cls");
			popis(naziv, broj);
			break;
		case '6':
			system("cls");
			selekcija(naziv, broj);
			break;
		case '7':
			system("cls");
			brisanjeDatoteke(naziv, broj); 
			break;
		case '0':
			return;
		default:
			system("cls");
			printf("\nPogresan unos\n");
		}
	} while (opcija != 0);

}