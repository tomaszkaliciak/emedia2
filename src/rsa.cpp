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

// https://rosettacode.org/wiki/Modular_inverse#C.2B.2B
void RSA::calculatePrivateKey() {
    long a = e;
    long b = fi;
    long b0 = b, t, q;
    long x0 = 0, x1 = 1;

    if (b == 1) {
        d =1;
        return;
    }

    while (a > 1) {
        q = a / b;
        t = b, b = a % b, a = t;
        t = x0, x0 = x1 - q * x0, x1 = t;
    }

    if (x1 < 0) x1 += b0;
    d = x1;
}

// zwraca wartość (base^exp) % mod
// https://www.topcoder.com/community/data-science/data-science-tutorials/primality-testing-non-deterministic-algorithms/
unsigned long RSA::powerModulo(unsigned long a, unsigned long b, unsigned long c) {
    unsigned long result = 1;
    while (b) {
        if (b % 2) {
            result = (result * a) % c;
        }

        a = (a * a) % c;
        b /= 2;
    }
    return result;
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
