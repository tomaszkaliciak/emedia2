#ifndef RSA_H
#define RSA_H

class RSA {
private:
    unsigned long p;
    unsigned long q;
    unsigned long n;
    unsigned long fi;
    unsigned long e;
    unsigned long d;
    unsigned long calculatePublicKey();
    unsigned long calculatePrivateKey();
    unsigned long powerModulo(long a, long b, long c);
public:
    RSA(unsigned long p, unsigned long q);
    unsigned long getPublicKey();
    long encode(long msg);
    long decode(long hash);
};

#endif // RSA_H
