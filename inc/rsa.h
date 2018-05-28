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
    void calculatePublicKey();
    void calculatePrivateKey();
    unsigned long powerModulo(unsigned long a, unsigned long b, unsigned long c);
public:
    RSA(unsigned long p, unsigned long q);
    unsigned long getPublicKey();
    long encode(long msg);
    long decode(long hash);
};

#endif // RSA_H
