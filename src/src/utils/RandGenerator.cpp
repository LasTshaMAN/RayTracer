#include "RandGenerator.h"

#include <stdlib.h> 
#include <time.h>

const RandGenerator RandGenerator::instance = RandGenerator();

RandGenerator::RandGenerator() {
    srand(static_cast<unsigned>(time(nullptr)));
}

RandGenerator::~RandGenerator() {
}

float RandGenerator::getNumberFromZeroToOne() const {
    return static_cast<float>(rand()) / RAND_MAX;
}

const RandGenerator& RandGenerator::getRandGenerator() {
    return instance;
}