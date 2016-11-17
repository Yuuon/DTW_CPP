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



int main(){
/*
 * 9 fichier audio + 1 'ref.wav'
 */
vector<string> vocabulaire;
vocabulaire.push_back("avance");
vocabulaire.push_back("recule");
vocabulaire.push_back("decollage");
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
char *ref; 
string fichier; 
float table [9] ;
int n_ck, n_cunk, dim_mfcc;
float * c_k;
float * c_unk;
/*lecture des fichiers audio*/
for(int mot = 1; mot < nbmots; mot++){
	fichier = chemin + vocabulaire[mot] + ".wav";
	nomFichier = fichier.c_str();
	char * nomF = strdup(nomFichier);
	char * mfcName = (char *)malloc(256 * sizeof(char));
	strcpy(mfcName, nomF);
	nameWavToMfc(nomF, mfcName);
	FILE ** audioFile =(FILE**) malloc(sizeof(FILE));
	wavfile * pHeader =(wavfile*) malloc(sizeof(wavfile));
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
	float ** X_mfcc = (float **)malloc(sizeof(float));
	int * length_xmfcc = (int *) malloc(sizeof(int));
	computeMFCC(X_mfcc, length_xmfcc, *xFiltre,
			pHeader->totallength, pHeader->frequency, 512, 256, 12, 26);



	/*
	 * TODO:
	 * il faut faire une lecteure sur le fichier ouvert par WavRead
	 * et le mettre dans un tableau de int16
	 * i.e.: while(fread(&buf,sizeof(int16_t),1,p_wav)>0)
	 *       x[i++] = buf;
	 * Pour computeMFCC, il s'agit de :
	 * frame_length : la taille de la fenetre de calcul des parametres.
	 * Par exemple 512 échantillons.
	 *
	 * frame_step : correspond au décallage déchantillons
	 * pour prendre la fenetre suivante.
	 * Si on veut prendre la moitié, prendre 256.
	 *
	 * dim_mfcc : en général 13. C'est un max pour de la parole.
	 *  ************** on prends 12 comme MATLAB **************
	 *
	 *  num_filter : Prende 26 par exemple.
	 *
	 *  ************* s'il y a probleme executer(compiler OK),
	 *                essayer initialiser les pointeur ********
	 */

	dtw(n_ck, n_cunk, dim_mfcc, c_k, c_unk);
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
