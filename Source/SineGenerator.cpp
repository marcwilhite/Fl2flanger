/*
  ==============================================================================

    SineGenerator.cpp
    Created: 1 Feb 2014 11:48:52am
    Author:  Marc Wilhite

  ==============================================================================
*/

#include "SineGenerator.h"

SineGenerator::SineGenerator() :
twoPI(2*(2*acos(0.0)))
{
    kSampleRate = 44100;
    kFrequency = 0.0;
    kPhase = 0.0;
    updateIncrement();
}

void SineGenerator::setFrequency(double frequency)
{
    kFrequency = frequency;
    updateIncrement();
}

void SineGenerator::setSampleRate(double sampleRate)
{
    kSampleRate = sampleRate;
    updateIncrement();
}

void SineGenerator::updateIncrement()
{
    kPhaseIncrement = kFrequency * twoPI / kSampleRate;
}

void SineGenerator::reset()
{
    kPhase = 0.0;
}

float SineGenerator::tick()
{
    float wave = sin(kPhase);
    kPhase += kPhaseIncrement;
    while (kPhase >= twoPI) {
        kPhase -= twoPI;
    }
    return wave;
}