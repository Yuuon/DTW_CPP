#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <fstream>
#include <limits>
#include <vector>
#include "dtw.h"
#include "FFTReal.h"
#include "libmfccOptim.h"
#include "WavToMfcc.h"

using namespace std;

void parametration(char * nomF, float ** X_mfcc, int * length_xmfcc) {
	FILE * fp = fopen(nomF,"rb");
	//fseek(fp, 0, SEEK_END);
	//int sz = ftell(fp);
	//FILE ** audioFile =(FILE**) malloc(sizeof(sz));
	wavfile * pHeader =(wavfile*) malloc(sizeof(wavfile));
	//char *buffer = (char *)malloc(sizeof(char)*2);
	//int16_t *matrice = (int16_t *)malloc(sizeof(pHeader->bytes_in_data));
	int16_t *matrice = new int16_t[pHeader->bytes_in_data];

	int16_t ** xFiltre = (int16_t **)malloc(sizeof(int16_t));
	int * newLength = (int *) malloc(sizeof(int));
	float threshold = 0.7;
	//wavRead(audioFile, nomF, pHeader);



	if (fp == NULL) {
			fprintf(stderr, "Can't open input file %s\n", nomF);
			exit(1);
		}

		// read header
		if (fread(pHeader, sizeof(wavfile), 1, fp) < 1) {
			fprintf(stderr, "Can't read input file header %s\n", nomF);
			exit(1);
		}

		if (fread(matrice, sizeof(pHeader->bytes_in_data), 1, fp) < 1) {
			fprintf(stderr, "Can't read signal matrice %s\n", nomF);
			exit(1);
		}

		// if wav file isn't the same endianness than the current environment
		// we quit
		if (is_big_endian()) {
			if (memcmp((pHeader)->id, "RIFX", 4) != 0) {
				fprintf(stderr, "ERROR: %s is not a big endian wav file\n", nomF);
				exit(1);
			}
		}
		else {
			if (memcmp((pHeader)->id, "RIFF", 4) != 0) {
				fprintf(stderr, "ERROR: %s is not a little endian wav file\n", nomF);
				exit(1);
			}
		}

		if (memcmp((pHeader)->wavefmt, "WAVEfmt ", 8) != 0
			|| memcmp((pHeader)->data, "data", 4) != 0
			) {
			fprintf(stderr, "ERROR: Not wav format\n");
			exit(1);
		}
		if ((pHeader)->format != 16) {
			fprintf(stderr, "\nERROR: not 16 bit wav format.");
			exit(1);
		}
		if (memcmp((pHeader)->data, "data", 4) != 0) {
			fprintf(stderr, "ERROR: Prrroblem?\n");
			exit(1);
		}


	removeSilence(matrice, (pHeader->bytes_in_data)/8, xFiltre, newLength, threshold);

	computeMFCC(X_mfcc, length_xmfcc, *xFiltre,
				*newLength, pHeader->frequency, 512, 256, 12, 26);

	//free(*audioFile);
	free(pHeader);
	//free(buffer);
	free(matrice);
	free(*xFiltre);
	free(newLength);
	fclose(fp);
}

int main(){
/*
 * 9 fichier audio + 1 'ref.wav'
 */
vector<string> vocabulaire;
vocabulaire.push_back("avance");
vocabulaire.push_back("recule");
vocabulaire.push_back("droite");
vocabulaire.push_back("gauche");
vocabulaire.push_back("tourne_droite");
vocabulaire.push_back("tourne_gauche");
vocabulaire.push_back("fais_flip");
vocabulaire.push_back("arrete_toi");
vocabulaire.push_back("etat_urgence");


string chemin = "";

int nbmots = vocabulaire.size();
string fichier; 
float table[9] ;

int dim_mfcc = 12;


float ** X_mfcc_ref = (float **)malloc(sizeof(float));
int * length_xmfcc_ref = (int *) malloc(sizeof(int));
fichier = chemin + "ref.wav";
char * nomF_ref = strdup(fichier.c_str());

parametration(nomF_ref,X_mfcc_ref,length_xmfcc_ref);




for(int mot = 0; mot < nbmots; mot++){
	fichier = chemin + vocabulaire[mot] + ".wav";
	char * nomF = strdup(fichier.c_str());
	//char * mfcName = (char *)malloc(256 * sizeof(char));
	//strcpy(mfcName, nomF);
	//nameWavToMfc(nomF, mfcName);
	float ** X_mfcc = (float **)malloc(sizeof(float));
	int * length_xmfcc = (int *) malloc(sizeof(int));

	parametration(nomF,X_mfcc,length_xmfcc);


	table[mot] = dtw(*length_xmfcc, *length_xmfcc_ref, dim_mfcc, *X_mfcc, *X_mfcc_ref);

	printf("%f  ",table[mot]);

	free(X_mfcc);
	free(length_xmfcc);


}


free(X_mfcc_ref);
free(length_xmfcc_ref);
free(nomF_ref);


/*Compute MFCC of a signal*/
//computeMFCC(float **X_mfcc, int *length_xmfcc, int16_t *x, int Nx, int frequency, int frame_length, int frame_step, int dim_mfcc, int num_filter);

/*Remove silence of the signal at the start and the end*/
//removeSilence(int16_t *x, int Nx, int16_t **xFiltered, int *newLength, float threshold);

/*Read a wave file.*/
//wavRead(FILE **p_wav, char *filename, wavfile *p_header); 

/*il faut recuperer le fichier de l'app et le comparer avec les fichiers que lui reconnait*/
//dtw(int n_ck, int n_cunk, int dim_mfcc, float * c_k, float * c_unk);

}
