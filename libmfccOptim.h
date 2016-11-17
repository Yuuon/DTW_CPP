/*
* libmfcc.h - Header for libMFCC
* Copyright (c) 2010 Jeremy Sawruk
*
* This code is released under the MIT License.
* For conditions of distribution and use, see the license in LICENSE
*/

#pragma once

#define PI 3.1415926535898f

// Returns the specified (mth) MFCC
float * GetCoefficient(float* spectralData, unsigned int samplingRate, unsigned int NumFilters, unsigned int binSize, unsigned int m);

// Compute the normalization factor (For internal computation only - not to be called directly)
float * NormalizationFactor(int NumFilters, int m);

// Compute the filter parameter for the specified frequency and filter bands (For internal computation only - not the be called directly)
float GetFilterParameter(unsigned int samplingRate, unsigned int binSize, unsigned int frequencyBand, unsigned int filterBand);

// Compute the band-dependent magnitude factor for the given filter band (For internal computation only - not the be called directly)
float GetMagnitudeFactor(unsigned int filterBand);

// Compute the center frequency (fc) of the specified filter band (l) (For internal computation only - not the be called directly)
float GetCenterFrequency(unsigned int filterBand);
