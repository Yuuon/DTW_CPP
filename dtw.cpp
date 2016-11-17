/*******************************************************************************
 *
 * Drone control through voice recognition -- PC to drone communication
 * Team GYTAM, feb. 2016
 *
 *
 ******************************************************************************/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for memcmp
#include <stdint.h> // for int16_t and int32_t
#include <math.h>
#include <iostream>
#include <limits>
#include "dtw.h"
using namespace std;
/**
* Dtw function that given two matrix of cep coefficient computes distance
* between those two signals.
*  @param n_ck      Dimension of unknow signal
*  @param n_cunk    Dimension of know signal
*  @param dim_mfcc  Size of nfcc decompostion base
*  @param c_k       Matrix of know signal
*  @param c_unk     Matrix of unknow signal
*  @return Distance between the two signals
*/

float distance(float * c_k, float* c_unk, int i, int j) {
	float d = 0;
	//for(int k = 0; k < 12; k++) {
    d += pow( (c_k[i]-c_unk[j]) ,2 );

	//}
	return d;
}

float dtw(int n_ck, int n_cunk, int dim_mfcc, float* c_k, float* c_unk) {

/* à compléter */
    int w0 = 1;
    int w1 = 2;
    int w2 = 1;
    double r = min(n_ck, n_cunk)|2 + 1;
    double table[n_ck][n_cunk];
    double d;
    table[0][0] = 0.0;
    for(int i = 1; i < n_cunk + 1; i++ ) {
        table[0][i] = numeric_limits<double>::infinity();
        
    }
    for(int i = 1; i < n_ck + 1; i++) {
    	table[i][0] = numeric_limits<double>::infinity();
    }
    for(int i = 1; i < n_ck + 1; i++ ) {
    	for(int j = 1; j< n_cunk + 1; j++) {
    		if(abs(i-j) <= r){
    			float d1 = table[i-1][j] + w0 * distance(c_k, c_unk, i-1, j-1);
    			float d2 = table[i-1][j-1] + w1 * distance(c_k, c_unk, i-1,j-1);
    			float d3 = table[i][j-1] + w2 * distance(c_k,c_unk,i-1,j-1);
    			table[i][j] = min(d1, min(d2,d3));
    		} else {
    			table[i][j] = numeric_limits<double>::infinity();
    		}
    	}
    }
    d = table[n_ck][n_cunk]/(n_ck+n_cunk);
    return d;
}


