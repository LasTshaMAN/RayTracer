#ifndef __RANDGENERATOR__
#define __RANDGENERATOR__

class RandGenerator {
public:
    float getNumberFromZeroToOne() const;

public:
    static const RandGenerator& getRandGenerator();
private:
    RandGenerator();
    ~RandGenerator();

    static const RandGenerator instance;
};

#endif