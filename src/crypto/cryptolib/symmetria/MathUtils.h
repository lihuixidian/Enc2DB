#ifndef MATHUTILS_H
#define MATHUTILS_H

class MathUtils {

    /**
     * Return a random long number in the range -range/2 to range/2
     */
    public:
    
    long  long randLong(long long range);
    /**
     * Return a random long number in the range 0 to max
     */
    long long randLongPos(long long max);

    long long gcd(long long a, long long b);
    long long* egcd(long long a, long long b);

    long long mod(long long a, long long modulo);

    long long modAdd(long long a, long  long b, long long modulo);

    long long modSubtract(long long a, long long b, long long modulo);

    long long modNegate(long long a, long long modulo);
    long long modMul(long long a, long long b, long long modulo);
    long long modDiv(long long a, long long b, long long modulo);
    long long modPow(long long  a, long long b, long long modulo);
    long long modInverse(long long a, long long modulo);

    // void testModulo();

    // void timeModulo();


};
#endif
