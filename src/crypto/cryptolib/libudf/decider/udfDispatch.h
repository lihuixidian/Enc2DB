

#pragma once


char *saheAdd(char *cipher1, char *cipher2, char *aescipher1, char *aescipher2);
char *saheAddp(char *cipher1, int m);
char *saheSub(char *cipher1, char *cipher2, char *aescipher1, char *aescipher2);
char *saheMul(char *cipher1, int m);
char *saheNeg(char *cipher1);
char *smheMul(char *cipher1, char *cipher2, char *aescipher1, char *aescipher2);
char *smheMulp(char *cipher1, int m);
char *smheDiv(char *cipher1, char *cipher2, char *aescipher1, char *aescipher2);
char *smhePow(char *cipher1, char *aescipher1, int m);
char *smheInv(char *cipher1);
char *saheAddp(char *cipher1, int m);
int oreCompare(char *cipher1, char *cipher2, char *aescipher1, char *aescipher2, int kind);