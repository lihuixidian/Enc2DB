#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include "time.h"
#include "MathUtils.h"
using namespace std;
#define N  999

    /**
     * Return a random long long  number in the range -range/2 to range/2
     */
    
    long long MathUtils:: randLong (long long  range){
        float random;
	    srand(time(NULL));//设置随机数种子，使每次产生的随机序列不同
		random= rand() % (N + 1) / (float)(N + 1);
        return  (long long ) (random * range - range / 2);

    }
    /**
     * Return a random long long  number in the range 0 to max
     */
     long long  MathUtils::randLongPos(long long  max) {
        float random;
	    srand(time(NULL));//设置随机数种子，使每次产生的随机序列不同
		random= rand() % (N + 1) / (float)(N + 1);
        return (long long ) (random * max);
    }

    long long  MathUtils:: gcd(long long  a, long long  b) {
    return b == 0 ? (a < 0 ? -a : a) : gcd(b, a % b);
    }
//////////////////////////////////////////////////////////////////////////////
    // public static long long  gcdBI(long long  a, long long  b) {
    //     return BigInteger.valueOf(a).gcd(BigInteger.valueOf(b)).long long Value();
    // }

    /**
     * This function performs the extended euclidean algorithm on two numbers a and b. The function
     * returns the gcd(a,b) as well as the numbers x and y such that ax + by = gcd(a,b). This
     * calculation is important in number theory and can be used for several things such as finding
     * modular inverses and solutions to linear Diophantine equations.
     */
    long long * MathUtils:: egcd(long long  a, long long  b) {
    if (b == 0)
    {
        //vector<long long > array;
        //long long array[3];
        long long  *array = new long long  [3];
        if (a<0)
        {
            array[0]=-a;
            array[1]=0L;
            array[2]=0L;

        }else
        {
            array[0]=a;
            array[1]=1L;
            array[2]=0L;


        }
//        if (a<0)
//        {
//            array.push_back(-a);
//            array.push_back(0L);
//            array.push_back(0L);
//
//        }else
//        {
//            array.push_back(a);
//            array.push_back(1L);
//            array.push_back(0L);
//
//        }
//        long long arr[array.size()];
//        for (int i=0;i<array.size();i++)
//        {
//            arr[i]=array[i];
//        }
        return array;
        
    }
    long long * v = egcd(b, a % b);
    long long  tmp = v[1] - v[2] * (a / b);
    v[1] = v[2];
    v[2] = tmp;
    return v;
}

    long long  MathUtils::mod(long long  a, long long  modulo) {
        long long  r = a % modulo;
        if (r < 0)
        {
            r += modulo;
        }
        return r;
    }
/////////////////////////////////////////////////////////////////////////
    // public static long long  modBI(long long  a, long long  modulo) {
    //     return BigInteger.valueOf(a).mod(BigInteger.valueOf(modulo)).long long Value();
    // }

    long long  MathUtils::modAdd(long long  a, long long b, long long  modulo) {
        //unsigned long long bignum=ULLONG_MAX+1;
        a = mod(a, modulo);
        b = mod(b, modulo);
        long long  r = a + b;
        if (r < 0)
            r += 2;
        return mod(r, modulo);
    }
/////////////////////////////////////////////////////////////////////////////
    // public static long long  modAddBI(long long  a, long long  b, long long  modulo) {
    //     return BigInteger.valueOf(a).add(BigInteger.valueOf(b)).mod(BigInteger.valueOf(modulo)).long long Value();
    // }

    long long  MathUtils::modSubtract(long long  a, long long  b, long long  modulo) {
        return modAdd(a, -b, modulo);
    }
/////////////////////////////////////////////////////////////////////////////////
    // public static long long  modSubtractBI(long long  a, long long  b, long long  modulo) {
    //     return BigInteger.valueOf(a).subtract(BigInteger.valueOf(b)).mod(BigInteger.valueOf(modulo)).long long Value();
    // }

    long long  MathUtils::modNegate(long long  a, long long  modulo) {
        return mod(-a, modulo);
    }

//////////////////////////////////////////////////////////////////////////////////////
    // public static long long  modNegateBI(long long  a, long long  modulo) {
    //     return BigInteger.valueOf(a).negate().mod(BigInteger.valueOf(modulo)).long long Value();
    // }

    /**
     * source: https://stackoverflow.com/questions/12168348/ways-to-do-modulo-multiplication-with-primitive-types
     */
    long long  MathUtils::modMul(long long  a, long long  b, long long  modulo) {

        if (a == 1)
            return b;
        if (b == 1)
            return a;

        a = mod(a, modulo);
        b = mod(b, modulo);

        if (a == 1)
            return b;
        if (b == 1)
            return a;

        long long  res = 0;
        long long  temp_b;

        while (a != 0) {
            if ((a & 1) == 1) {
                // Add b to res, n m, without overflow
                if (b >= modulo - res) // Equiv to if (res + b >= m), without overflow
                    res -= modulo;
                res += b;
            }
            a >>= 1;

            // Double b, n m
            temp_b = b;
            if (b >= modulo - b) // Equiv to if (2 * b >= m), without overflow
                temp_b -= modulo;
            b += temp_b;
        }
        return res;
    }

    long long  MathUtils::modDiv(long long  a, long long  b, long long  modulo) {
        return modMul(a, modInverse(b, modulo), modulo);
    }
////////////////////////////////////////////////////////////////////////////
    // public static long long  modMulBI(long long  a, long long  b, long long  modulo) {
    //     return BigInteger.valueOf(a).multiply(BigInteger.valueOf(b)).mod(BigInteger.valueOf(modulo)).long long Value();
    // }

    long long  MathUtils::modPow(long long   a, long long  b, long long  modulo) {
        //return modPowBI(a, b, modulo); // modPowBI faster
    if (modulo <= 0)
        
        throw "modulo is less than 0";

    bool invertResult;
    if ((invertResult = (b < 0)))
        b = modNegate(b, modulo);

    a = mod(a, modulo);
    if (a == 0)
        return 0;
    long long  res = 1;
    while (b > 0) {
        if (b % 2 == 1)
            res = modMul(res, a, modulo);
            a = modMul(a, a, modulo);
            b >>= 1;
    }

    if (invertResult) {
        if (gcd(res, modulo) != 1) {
            cout<<"Attempted to invert BigInteger with no inverse."<<endl;
            return 0;
        }
        res = modInverse(res, modulo);
    }

    return res;
    }
//////////////////////////////////////////////////////////////////////////////////////
    // public static long long  modPowBI(long long  a, long long  b, long long  modulo) {
    //     return modPowBI(BigInteger.valueOf(a), BigInteger.valueOf(b), BigInteger.valueOf(modulo)).long long Value();
    // }
////////////////////////////////////////////////////////////////////////////////////////
    // public static BigInteger modPowBI(BigInteger a, BigInteger b, BigInteger modulo) {
    //     BigInteger r = BigInteger.ZERO;
    //     try {
    //         r = a.modPow(b, modulo);
    //     } catch (ArithmeticException e) {
    //         cout<<("Attempted to invert BigInteger with no inverse.");
    //     }
    //     return r;
    // }

    /**
     * Returns the modular inverse of 'a' mod 'm' Make sure m > 0 and 'a' & 'm' are relatively
     * prime.
     */
    long long  MathUtils::modInverse(long long  a, long long  modulo) {
        a = modAdd(a, modulo, modulo);
        long long   *v = egcd(a, modulo);
        long long  x = v[1];
        delete[] v;
        return modAdd(x, modulo, modulo);
    }
////////////////////////////////////////////////////////////////////////////////////////////////
    // public static long long  modInverseBI(long long  n, long long  modulo) {
    //     return BigInteger.valueOf(n).modInverse(BigInteger.valueOf(modulo)).long long Value();
    // }


    void MathUtils::testModulo() {

        long long  iterations = 1000;

        long long  n = LLONG_MAX;
        long long  actual, expected;
        for (long long  i = 0; i >= 0 && i < iterations; i++) {

            long long  a = randLong(LLONG_MAX);
            long long  b = randLong(LLONG_MAX);

            // test mod
            actual = mod(a, n);
            // expected = modBI(a, n);
            // if (actual != expected)
            //     throw new AssertionError("Mod: Expected=" + expected + " Actual=" + actual);

            // test modAdd
            actual = modAdd(a, b, n);
            // expected = modAddBI(a, b, n);
            // if (actual != expected)
            //     throw new AssertionError("ModAdd: Expected=" + expected + " Actual=" + actual);

            // test modSub
            actual = modSubtract(a, b, n);
            // expected = modSubtractBI(a, b, n);
            // if (actual != expected)
            //     throw new AssertionError("ModSub: Expected=" + expected + " Actual=" + actual);

            // test modNegate
            actual = modNegate(a, n);
            // expected = modNegateBI(a, n);
            // if (actual != expected)
            //     throw new AssertionError("ModNegate: Expected=" + expected + " Actual=" + actual);

            // test modMul
            actual = modMul(a, b, n);
            // expected = modMulBI(a, b, n);
            // if (actual != expected)
            //     throw new AssertionError("ModMul: Expected=" + expected + " Actual=" + actual);

            // test modPow
            actual = modPow(a, b, n);
            // expected = modPowBI(a, b, n);
            // if (actual != expected)
            //     throw new AssertionError("ModPow: Expected=" + expected + " Actual=" + actual);

            // test gcd
            actual = gcd(a, n);
            // expected = gcdBI(a, n);
            // if (actual != expected)
            //     throw new AssertionError("Gcd: Expected=" + expected + " Actual=" + actual);

            // test modInverse
            // try {
            //     expected = modInverseBI(a, n);
            // } catch (ArithmeticException e) {
            //     continue;
            // }
            actual = modInverse(a, n);
            // if (actual != expected)
            //     throw new AssertionError("Inverse: Expected=" + expected + " Actual=" + actual);
        }

        cout<<"Test complete"<<endl;
    }

    void MathUtils::timeModulo() {
        int iterations = 10000;
        int warmup = 100;

        long long  n = LLONG_MAX;
        double t1 = 0;
        //long long  t2 = 0;
        double ta1 = 0;
        //long long  ta2 = 0;
        double ts1 = 0;
        //long long  ts2 = 0;
        double tm1 = 0;
        //long long  tm2 = 0;
        double tp1 = 0;
        //long long  tp2 = 0;
        double ti1 = 0;
        //long long  ti2 = 0;
        clock_t start;
        for (int i = 0; i < iterations; i++) {
            long long   a;
            while (true) {
                a = randLong(LLONG_MAX);
                modInverse(a, n);
                break;
                
            }
            long long  b = randLong(LLONG_MAX);

            // time mod
            start = clock();
            mod(a, n);
            if (i >= warmup)
                t1 += (double)(clock() - start);
            // start = System.nanoTime();
            // MathUtils.modBI(a, n);
            // if (i >= warmup)
            //     t2 += (System.nanoTime() - start);

            // time add
            start = clock();
            modAdd(a, b, n);
            if (i >= warmup)
                ta1 += (double)(clock() - start);
            //start = System.nanoTime();
            // MathUtils.modAddBI(a, b, n);
            // if (i >= warmup)
            //     ta2 += (System.nanoTime() - start);

            // time sub
            start = clock();
            modSubtract(a, b, n);
            if (i >= warmup)
                ts1 += (double)(clock() - start);
            //start = System.nanoTime();
            // MathUtils.modSubtractBI(a, b, n);
            // if (i >= warmup)
            //     ts2 += (System.nanoTime() - start);

            // time mul
            start = clock();
            modMul(a, b, n);
            if (i >= warmup)
                tm1 += (double)(clock() - start);
            //start = System.nanoTime();
            // MathUtils.modMulBI(a, b, n);
            // if (i >= warmup)
            //     tm2 += (System.nanoTime() - start);

            // time pow
            start = clock();
            modPow(a, b, n);
            if (i >= warmup)
                tp1 += (double)(clock() - start);
            //start = System.nanoTime();
            // MathUtils.modPowBI(a, b, n);
            // if (i >= warmup)
            //     tp2 += (System.nanoTime() - start);

            // time inv
            start = clock();
            modInverse(a, n);
            if (i >= warmup)
                ti1 += (double)(clock()- start);
            //start = System.nanoTime();
            // MathUtils.modInverseBI(a, n);
            // if (i >= warmup)
            //     ti2 += (System.nanoTime() - start);

        }

        cout<<"Mod"<<endl;
        cout<<"T1=" <<t1 / (iterations - warmup)<<endl;
        //cout<<("T2=" + t2 / (iterations - warmup));

        cout<<"Add"<<endl;
        cout<<"T1=" << ta1 / (iterations - warmup)<<endl;
        //cout<<("T2=" + ta2 / (iterations - warmup));

        cout<<"Sub"<<endl;
        cout<<"T1="<< ts1 / (iterations - warmup)<<endl;
        //cout<<("T2=" + ts2 / (iterations - warmup));

        cout<<"Mul"<<endl;
        cout<<"T1="<< tm1 / (iterations - warmup)<<endl;
        //cout<<("T2=" + tm2 / (iterations - warmup));

        cout<<"Pow"<<endl;
        cout<<"T1=" <<tp1 / (iterations - warmup)<<endl;
        //cout<<("T2=" + tp2 / (iterations - warmup));

        cout<<"Inv"<<endl;
        cout<<"T1=" <<ti1 / (iterations - warmup)<<endl;
        //cout<<("T2=" + ti2 / (iterations - warmup));
    }

//int main() {
//        MathUtils math;
//        math.testModulo();
//        math.timeModulo();
//    }