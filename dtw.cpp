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

float distance(float * c_k, float* c_unk, int i, int j, int dim_mfcc) {
	float d = 0;
	for(int k = 0; k < dim_mfcc; k++) {
		if(k == 0) {
			d += pow( (c_k[i]-c_unk[j]) ,2 );
		} else {
			d += pow( (c_k[k * i]-c_unk[k * j]) ,2 );
		}
	}
	return d;
}

float dtw(int n_ck, int n_cunk, int dim_mfcc, float* c_k, float* c_unk) {

/* à compléter */
    int w0 = 1;
    int w1 = 2;
    int w2 = 1;
    int r = (min(n_ck, n_cunk)/2) + 1;
    int a = n_ck;
    int b = n_cunk;
    float table[a+1][b+1];
    float d;
    table[0][0] = 0.0;
    for(int i = 1; i < b + 1; i++ ) {
        table[0][i] = numeric_limits<float>::infinity();
        
    }
    for(int i = 1; i < a + 1; i++) {
    	table[i][0] = numeric_limits<float>::infinity();
    }
    float d1,d2,d3;
    for(int i = 1; i < a + 1; i++ ) {
    	for(int j = 1; j< b + 1; j++) {
    		if(abs(i-j) <= r){
    			d1 = table[i-1][j] + w0 * distance(c_k, c_unk, i-1, j-1, dim_mfcc);
    			d2 = table[i-1][j-1] + w1 * distance(c_k, c_unk, i-1,j-1, dim_mfcc);
    			d3 = table[i][j-1] + w2 * distance(c_k, c_unk,i-1,j-1, dim_mfcc);
    			table[i][j] = min(d1, min(d2,d3));
    		} else {
    			table[i][j] = numeric_limits<float>::infinity();
    		}
    	}
    }
    d = table[a][b]/(a + b);
    return d;
}


