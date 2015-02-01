/*
  ==============================================================================

    ParamSmoother.h
    Created: 12 Jan 2014 7:31:10pm
    Author:  Marc Wilhite

  ==============================================================================
*/

#ifndef PARAMSMOOTHER_H_INCLUDED
#define PARAMSMOOTHER_H_INCLUDED


class ParamSmooth
{
public:
    ParamSmooth() { a = 0.99; b = 1. - a; z = 0.; };
    ~ParamSmooth() {};
    inline double Process(double in) { z = (in * b) + (z * a); return z; }
    void setSmoothAmount(float smooth) { a = smooth; }
private:
    double a, b, z;
};




#endif  // PARAMSMOOTHER_H_INCLUDED
