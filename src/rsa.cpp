#include "../inc/rsa.h"
#include <cmath>
#include <iostream>

RSA::RSA(unsigned long prime1, unsigned long prime2): p(prime1),q(prime2) {
    n = p * q;
    fi = (p-1) * (q-1);
    calculatePublicKey();

    calculatePrivateKey();
}

unsigned long gcd(unsigned long a, unsigned long b) {
    unsigned long tmp;
    while (b != 0)  {
      tmp = b;
      b = a % b;
      a = tmp;
    }
    return a;
}


void RSA::calculatePublicKey() {
    unsigned long a = fi;
    unsigned long b = n/2;
    while (gcd(a,b) > 1 && b < n) {
            ++b;
        }
    e = b;
}
/* Rozszerzony Algorytm Euklidesa.
https://en.wikibooks.org/wiki/Algorithm_Implementation/Mathematics/Extended_Euclidean_algorithm
*/

void RSA::calculatePrivateKey() {
    unsigned long ee = e;
    unsigned long fii = fi;
    unsigned long w = 0;
    unsigned long x = 0;
    unsigned long y = 0;
    unsigned long x1 = 0;
    unsigned long x2 = 1;
    unsigned long y1 = 1;
    unsigned long temp = fii;
    unsigned long temp1;
    unsigned long temp2;
    while (ee > 0) {
        temp1 = temp/ee;
        temp2 = temp - temp1 * ee;
        temp = ee;
        ee = temp2;

        x = x2- temp1* x1;
        y = w - temp1 * y1;

        x2 = x1;
        x1 = x;
        w = y1;
        y1 = y;
    }
    if (temp == 1) {
        d = w + fi;
    }
    else d = 0;
}

// zwraca wartość (base^exp) % mod
unsigned long RSA::powerModulo(unsigned long a, unsigned long b, unsigned long c) {
    unsigned long n, *pows, *indexes, indexCounter = 0, searchBit = 1, partialMul = 1;
    n = floor(log2(b)) + 1;
    pows = new unsigned long[n];
    indexes = new unsigned long[n];
    pows[0] = a % c;
    for (int i = 1; i < n; i++) {
        pows[i] = (pows[i - 1] * pows[i - 1]) % c;
    }


    for (int i = 0; i < n; i++) {
        int index = b & searchBit;
        if (index != 0) {
            indexes[indexCounter] = floor(log2(index));
            indexCounter++;
        }
        searchBit = searchBit << 1;
    }

    for (int i = 0; i < indexCounter; i++) {
        partialMul = (partialMul * pows[indexes[i]]) % c;
    }
    delete[] pows;
    delete[] indexes;
    return partialMul % c;
}

long RSA::encode(long msg) {
    long hash;
    if(msg < 0) {
        msg *= -1;
        hash = powerModulo(msg,e,n);
        hash *= -1;
    }
    else {
        hash = powerModulo(msg,e,n);
    }
    return hash;
}

long RSA::decode(long hash) {
    long msg;
    if(hash < 0) {
        hash *= -1;
        msg = powerModulo(hash,d,n);
        msg *= -1;
    }
    else {
        msg = powerModulo(hash,d,n);
    }
    return msg;
}
