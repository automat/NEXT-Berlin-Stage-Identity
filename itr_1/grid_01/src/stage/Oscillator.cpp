#include "stage/Oscillator.h"
#include "Config.h"


Oscillator::Oscillator(){
    mPerlin = Perlin(OSCILLATOR_PERLIN_NUM_OCTAVES, OSCILLATOR_PERLIN_SEED);
}

float Oscillator::getValue(float x, float y, float t){
    return mPerlin.fBm(x, y, t);
    //return srdnoise2(x, y, t, 0, 0);
}

float Oscillator::getValue(float x, float y, float offsetX, float offsetY, float t){
    return mPerlin.fBm(x + offsetX, y + offsetY, t);
}