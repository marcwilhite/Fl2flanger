/*
  ==============================================================================

    SineGenerator.h
    Created: 1 Feb 2014 11:48:52am
    Author:  Marc Wilhite

  ==============================================================================
*/

#ifndef SINEGENERATOR_H_INCLUDED
#define SINEGENERATOR_H_INCLUDED

#include <math.h>

class SineGenerator
{
public:
    SineGenerator();
    
    void setFrequency(double frequency);
    void setSampleRate(double sampleRate);
    void reset();
    float tick();
    
private:
    const double twoPI;
    double kFrequency;
    double kPhase;
    double kSampleRate;
    double kPhaseIncrement;
    void updateIncrement();
};

#endif  // SINEGENERATOR_H_INCLUDED
