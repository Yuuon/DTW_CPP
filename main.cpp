#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <iostream>
#include <limits>
#include <vector>
#include "dtw.h"
#include "FFTReal.h"
#include "libmfccOptim.h"
#include "WavToMfcc.h"

using namespace std;

void parametration(char * nomF, float ** X_mfcc, int * length_xmfcc) {
	FILE * fp = fopen(nomF,"r");
	fseek(fp, 0, SEEK_END);
	int sz = ftell(fp);
	FILE ** audioFile =(FILE**) malloc(sizeof(sz));
	wavfile * pHeader =(wavfile*) malloc(sizeof(44));
	wavRead(audioFile, nomF, pHeader);
	int i = 0;
	char *buffer = (char *)malloc(sizeof(int16_t));
	int16_t *matrice = (int16_t *)malloc(sizeof(pHeader->totallength));
	int16_t buf;
	while(fread(buffer,sizeof(int16_t),1,*audioFile)>0) {
		buf = strtoul(buffer,NULL,0);
		matrice[i++] = buf;
	}
	int16_t ** xFiltre = (int16_t **)malloc(sizeof(float));
	int * newLength = (int *) malloc(sizeof(int));
	float threshold = 0.7;
	removeSilence(matrice, pHeader->frequency, xFiltre, newLength, threshold);
	computeMFCC(X_mfcc, length_xmfcc, *xFiltre,
				pHeader->totallength, pHeader->frequency, 512, 256, 12, 26);
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
vocabulaire.push_back("tournedroite");
vocabulaire.push_back("tournegauche");
vocabulaire.push_back("faisunflip");
vocabulaire.push_back("arretetoi");
vocabulaire.push_back("etatdurgence");


string chemin = "";

int nbmots = vocabulaire.size();
const char *nomFichier;
string fichier; 
float table[9] ;
int n_ck, n_cunk, dim_mfcc;
float * c_k;
float * c_unk;
float ** X_mfcc_ref = (float **)malloc(sizeof(float));
int * length_xmfcc_ref = (int *) malloc(sizeof(int));
fichier = chemin + "ref.wav";
char * nomF_ref = strdup(fichier.c_str());
dim_mfcc = 12;

parametration(nomF_ref,X_mfcc_ref,length_xmfcc_ref);
n_cunk = *length_xmfcc_ref;
c_unk = *X_mfcc_ref;

for(int mot = 0; mot < nbmots; mot++){
	fichier = chemin + vocabulaire[mot] + ".wav";
	nomFichier = fichier.c_str();
	char * nomF = strdup(nomFichier);
	//char * mfcName = (char *)malloc(256 * sizeof(char));
	//strcpy(mfcName, nomF);
	//nameWavToMfc(nomF, mfcName);
	float ** X_mfcc = (float **)malloc(sizeof(float));
	int * length_xmfcc = (int *) malloc(sizeof(int));
	parametration(nomF,X_mfcc,length_xmfcc);
	n_ck = *length_xmfcc;
	c_k = *X_mfcc;

	table[mot] = dtw(n_ck, n_cunk, dim_mfcc, c_k, c_unk);

	printf("%f  ",table[mot]);



}





/*Compute MFCC of a signal*/
//computeMFCC(float **X_mfcc, int *length_xmfcc, int16_t *x, int Nx, int frequency, int frame_length, int frame_step, int dim_mfcc, int num_filter);

/*Remove silence of the signal at the start and the end*/
//removeSilence(int16_t *x, int Nx, int16_t **xFiltered, int *newLength, float threshold);

/*Read a wave file.*/
//wavRead(FILE **p_wav, char *filename, wavfile *p_header); 

/*il faut recuperer le fichier de l'app et le comparer avec les fichiers que lui reconnait*/
//dtw(int n_ck, int n_cunk, int dim_mfcc, float * c_k, float * c_unk);

}
