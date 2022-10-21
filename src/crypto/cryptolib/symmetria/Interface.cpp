//
// Created by 甜甜 on 2020/12/14.
//
////UDF需要调用的c++相关接口函数
///加解密测试
#include "iostream"
#include <vector>
#include <string>
#include "MathUtils.h"
#include "Symcipher.h"
#include "ArraySymCipher.h"
#include "str22struct.h"
#include "SymPHE.h"
#include "SymAHE.h"
#include "SymMHE.h"
//#include "Interface.h"
#include "t.h"
//#include <sstream>
#include <iomanip>
#include <cstring>
using namespace __gnu_cxx;
using namespace std;

#ifdef  __cplusplus
extern "C" {
#endif
    #include "Interface.h"
#ifdef __cplusplus
}
#endif



//#ifdef  __cplusplus
//extern "C" {
//#endif
char *SAHE_add_udf(char *cipher1,char *cipher2){ // post


    size_t bufsz2 = sizeof(cipher1) + 1;
    size_t hexsz2 = 2 * (strlen(cipher1) + 1);
    char* nc2 = new char[hexsz2];
    t change;
    str22struct str22;
    change.hex2buf((uint8_t*)nc2, &hexsz2, cipher1, hexsz2);
    //cout<<"cipher1_parse: "<<nc2<<endl;

    size_t bufsz3 = sizeof(cipher2) + 1;
    size_t hexsz3 = 2 * (strlen(cipher2) + 1);
    char* nc3 = new char[hexsz3];
    change.hex2buf((uint8_t*)nc3, &hexsz3, cipher2, hexsz3);
    //cout<<"cipher2_parse: "<<nc3<<endl;


    char *str = NULL;
    string res;
    string structTostr;


    SymCipher* a=new ArraySymCipher(nc2);
    SymCipher* b=new ArraySymCipher(nc3);

    SymAHE symAHE =SymAHE();
    //SymCipher &c_ahe3 = symAHE.encrypt(1001);

    symAHE.add(*a,*b);
    str22struct:: ArraySymCipher_change p ;

    p.value=a->getValue();
    p.sizePos=a->getsizePos();
    p.offsetPos=a->getoffsetPos();
    p.cardMultiplierPos=a->getcardMultiplierPos();
    p.sizeNeg=a->getsizeNeg();
    p.offsetNeg=a->getoffsetNeg();
    p.cardMultiplierNeg=a->getcardMultiplierNeg();
    p.idsPos.assign(a->getidsPos().begin(),a->getidsPos().end());
    p.idsNeg.assign(a->getidsNeg().begin(),a->getidsNeg().end());
    p.cardPos=a->getcardPos();
    p.cardNeg=a->getcardNeg();

    string mapTostrPos = str22.mapToString(p.cardPos);
    string mapTostrNeg = str22.mapToString(p.cardNeg);

    res =str22.ArraySymCipher_to_str(&str, &p);
    structTostr=res+mapTostrPos+"%"+mapTostrNeg+"^";
    //char *ch=(char*)structTostr.data();

    size_t bufsz1 = sizeof(structTostr) + 1;
    size_t hexsz1 = 2 * (strlen(structTostr.data()) + 1);
    char* h = new char[hexsz1 * 2];
    change.buf2hex((uint8_t*)structTostr.data(), strlen(structTostr.data()), h, &hexsz1);

    delete a;
    delete b;
    delete []nc2;
    delete []nc3;


    return h;

}


char* SAHE_addplaintext_udf(char * cipher1,int m ) { // post
//
    size_t bufsz2 = sizeof(cipher1) + 1;
    size_t hexsz2 = 2 * (strlen(cipher1) + 1);
    char *nc2 = new char[hexsz2];
    t change;
    str22struct str22;
    change.hex2buf((uint8_t *) nc2, &hexsz2, cipher1, hexsz2);
    //cout << "cipher1_parse: " << nc2 << endl;

    char *str = NULL;
    string res;
    string structTostr;

    SymCipher *a = new ArraySymCipher(nc2);

    SymAHE symAHE = SymAHE();

    symAHE.addPlaintext(*a, m);
    str22struct::ArraySymCipher_change p;

    p.value = a->getValue();
    p.sizePos = a->getsizePos();
    p.offsetPos = a->getoffsetPos();
    p.cardMultiplierPos = a->getcardMultiplierPos();
    p.sizeNeg = a->getsizeNeg();
    p.offsetNeg = a->getoffsetNeg();
    p.cardMultiplierNeg = a->getcardMultiplierNeg();
    p.idsPos.assign(a->getidsPos().begin(), a->getidsPos().end());
    p.idsNeg.assign(a->getidsNeg().begin(), a->getidsNeg().end());
    p.cardPos = a->getcardPos();
    p.cardNeg = a->getcardNeg();

    string mapTostrPos = str22.mapToString(p.cardPos);
    string mapTostrNeg = str22.mapToString(p.cardNeg);

    res = str22.ArraySymCipher_to_str(&str, &p);
    structTostr = res + mapTostrPos + "%" + mapTostrNeg + "^";
    //char *ch=(char*)structTostr.data();

    size_t bufsz1 = sizeof(structTostr) + 1;
    size_t hexsz1 = 2 * (strlen(structTostr.data()) + 1);
    char *h = new char[hexsz1 * 2];
    change.buf2hex((uint8_t *) structTostr.data(), strlen(structTostr.data()), h, &hexsz1);

    delete a;
    delete []nc2;
    return h;

}

char* SAHE_substract_udf(char * cipher1,char * cipher2){ // post


    size_t bufsz2 = sizeof(cipher1) + 1;
    size_t hexsz2 = 2 * (strlen(cipher1) + 1);
    char* nc2 = new char[hexsz2];
    t change;
    str22struct str22;
    change.hex2buf((uint8_t*)nc2, &hexsz2, cipher1, hexsz2);
    //cout<<"cipher1_parse: "<<nc2<<endl;

    size_t bufsz3 = sizeof(cipher2) + 1;
    size_t hexsz3 = 2 * (strlen(cipher2) + 1);
    char* nc3 = new char[hexsz3];
    change.hex2buf((uint8_t*)nc3, &hexsz3, cipher2, hexsz3);
    //cout<<"cipher2_parse: "<<nc3<<endl;


    char *str = NULL;
    string res;
    string structTostr;


    SymCipher* a=new ArraySymCipher(nc2);
    SymCipher* b=new ArraySymCipher(nc3);

    SymAHE symAHE =SymAHE();
    //SymCipher &c_ahe3 = symAHE.encrypt(1001);

    symAHE.subtract(*a, *b);
    str22struct:: ArraySymCipher_change p ;

    p.value=a->getValue();
    p.sizePos=a->getsizePos();
    p.offsetPos=a->getoffsetPos();
    p.cardMultiplierPos=a->getcardMultiplierPos();
    p.sizeNeg=a->getsizeNeg();
    p.offsetNeg=a->getoffsetNeg();
    p.cardMultiplierNeg=a->getcardMultiplierNeg();
    p.idsPos.assign(a->getidsPos().begin(),a->getidsPos().end());
    p.idsNeg.assign(a->getidsNeg().begin(),a->getidsNeg().end());
    p.cardPos=a->getcardPos();
    p.cardNeg=a->getcardNeg();

    string mapTostrPos = str22.mapToString(p.cardPos);
    string mapTostrNeg = str22.mapToString(p.cardNeg);

    res =str22.ArraySymCipher_to_str(&str, &p);
    structTostr=res+mapTostrPos+"%"+mapTostrNeg+"^";
    //char *ch=(char*)structTostr.data();

    size_t bufsz1 = sizeof(structTostr) + 1;
    size_t hexsz1 = 2 * (strlen(structTostr.data()) + 1);
    char* h = new char[hexsz1 * 2];
    change.buf2hex((uint8_t*)structTostr.data(), strlen(structTostr.data()), h, &hexsz1);


    delete a;
    delete b;
    delete []nc2;
    delete []nc3;
    return h;


}
char* SAHE_multiply_udf(char * cipher1,int m ){ // post

    size_t bufsz2 = sizeof(cipher1) + 1;
    size_t hexsz2 = 2 * (strlen(cipher1) + 1);
    char *nc2 = new char[hexsz2];
    t change;
    str22struct str22;
    change.hex2buf((uint8_t *) nc2, &hexsz2, cipher1, hexsz2);
    //cout << "cipher1_parse: " << nc2 << endl;

    char *str = NULL;
    string res;
    string structTostr;

    SymCipher *a = new ArraySymCipher(nc2);

    SymAHE symAHE = SymAHE();

    symAHE.multiply(*a, m);
    str22struct::ArraySymCipher_change p;

    p.value = a->getValue();
    p.sizePos = a->getsizePos();
    p.offsetPos = a->getoffsetPos();
    p.cardMultiplierPos = a->getcardMultiplierPos();
    p.sizeNeg = a->getsizeNeg();
    p.offsetNeg = a->getoffsetNeg();
    p.cardMultiplierNeg = a->getcardMultiplierNeg();
    p.idsPos.assign(a->getidsPos().begin(), a->getidsPos().end());
    p.idsNeg.assign(a->getidsNeg().begin(), a->getidsNeg().end());
    p.cardPos = a->getcardPos();
    p.cardNeg = a->getcardNeg();

    string mapTostrPos = str22.mapToString(p.cardPos);
    string mapTostrNeg = str22.mapToString(p.cardNeg);

    res = str22.ArraySymCipher_to_str(&str, &p);
    structTostr = res + mapTostrPos + "%" + mapTostrNeg + "^";
    //char *ch=(char*)structTostr.data();

    size_t bufsz1 = sizeof(structTostr) + 1;
    size_t hexsz1 = 2 * (strlen(structTostr.data()) + 1);
    char *h = new char[hexsz1 * 2];
    change.buf2hex((uint8_t *) structTostr.data(), strlen(structTostr.data()), h, &hexsz1);

    delete a;
    delete []nc2;
    return h;


}
char* SAHE_neggate_udf(char *cipher1){ // post
//
    size_t bufsz2 = sizeof(cipher1) + 1;
    size_t hexsz2 = 2 * (strlen(cipher1) + 1);
    char* nc2 = new char[hexsz2];
    t change;
    str22struct str22;
    change.hex2buf((uint8_t*)nc2, &hexsz2, cipher1, hexsz2);
    //cout<<"cipher1_parse: "<<nc2<<endl;

    char *str = NULL;
    string res;
    string structTostr;


    SymCipher* a=new ArraySymCipher(nc2);

    SymAHE symAHE =SymAHE();
    //SymCipher &c_ahe3 = symAHE.encrypt(1001);

    symAHE.neggate(*a);
    str22struct:: ArraySymCipher_change p ;

    p.value=a->getValue();
    p.sizePos=a->getsizePos();
    p.offsetPos=a->getoffsetPos();
    p.cardMultiplierPos=a->getcardMultiplierPos();
    p.sizeNeg=a->getsizeNeg();
    p.offsetNeg=a->getoffsetNeg();
    p.cardMultiplierNeg=a->getcardMultiplierNeg();
    p.idsPos.assign(a->getidsPos().begin(),a->getidsPos().end());
    p.idsNeg.assign(a->getidsNeg().begin(),a->getidsNeg().end());
    p.cardPos=a->getcardPos();
    p.cardNeg=a->getcardNeg();

    string mapTostrPos = str22.mapToString(p.cardPos);
    string mapTostrNeg = str22.mapToString(p.cardNeg);

    res =str22.ArraySymCipher_to_str(&str, &p);
    structTostr=res+mapTostrPos+"%"+mapTostrNeg+"^";
    //char *ch=(char*)structTostr.data();

    size_t bufsz1 = sizeof(structTostr) + 1;
    size_t hexsz1 = 2 * (strlen(structTostr.data()) + 1);
    char* h = new char[hexsz1 * 2];
    change.buf2hex((uint8_t*)structTostr.data(), strlen(structTostr.data()), h, &hexsz1);

    delete a;
    delete []nc2;
    return h;

}

char* SMHE_MULTIPLY_udf(char *cipher1,char *cipher2){ // post
    size_t bufsz2 = sizeof(cipher1) + 1;
    size_t hexsz2 = 2 * (strlen(cipher1) + 1);
    char* nc2 = new char[hexsz2];
    t change;
    str22struct str22;
    change.hex2buf((uint8_t*)nc2, &hexsz2, cipher1, hexsz2);
    //cout<<"cipher1_parse: "<<nc2<<endl;

    size_t bufsz3 = sizeof(cipher2) + 1;
    size_t hexsz3 = 2 * (strlen(cipher2) + 1);
    char* nc3 = new char[hexsz3];
    change.hex2buf((uint8_t*)nc3, &hexsz3, cipher2, hexsz3);
    //cout<<"cipher2_parse: "<<nc3<<endl;


    char *str = NULL;
    string res;
    string structTostr;


    SymCipher* a=new ArraySymCipher(nc2);
    SymCipher* b=new ArraySymCipher(nc3);

    SymMHE symMHE =SymMHE();

    symMHE.multiply(*a,*b);
    str22struct:: ArraySymCipher_change p ;

    p.value=a->getValue();
    p.sizePos=a->getsizePos();
    p.offsetPos=a->getoffsetPos();
    p.cardMultiplierPos=a->getcardMultiplierPos();
    p.sizeNeg=a->getsizeNeg();
    p.offsetNeg=a->getoffsetNeg();
    p.cardMultiplierNeg=a->getcardMultiplierNeg();
    p.idsPos.assign(a->getidsPos().begin(),a->getidsPos().end());
    p.idsNeg.assign(a->getidsNeg().begin(),a->getidsNeg().end());
    p.cardPos=a->getcardPos();
    p.cardNeg=a->getcardNeg();

    string mapTostrPos = str22.mapToString(p.cardPos);
    string mapTostrNeg = str22.mapToString(p.cardNeg);

    res =str22.ArraySymCipher_to_str(&str, &p);
    structTostr=res+mapTostrPos+"%"+mapTostrNeg+"^";
    //char *ch=(char*)structTostr.data();

    size_t bufsz1 = sizeof(structTostr) + 1;
    size_t hexsz1 = 2 * (strlen(structTostr.data()) + 1);
    char* h = new char[hexsz1 * 2];
    change.buf2hex((uint8_t*)structTostr.data(), strlen(structTostr.data()), h, &hexsz1);

    delete a;
    delete b;
    delete []nc2;
    delete []nc3;

    return h;
}
//    SymMHE symMHE =SymMHE();
//
//    symMHE.multiplyPlaintext(a, m);
char* SMHE_MULTIPLYPLAIN_udf(char * cipher1,int m ){ // post
//
    size_t bufsz2 = sizeof(cipher1) + 1;
    size_t hexsz2 = 2 * (strlen(cipher1) + 1);
    char *nc2 = new char[hexsz2];
    t change;
    str22struct str22;
    change.hex2buf((uint8_t *) nc2, &hexsz2, cipher1, hexsz2);
    //cout << "cipher1_parse: " << nc2 << endl;

    char *str = NULL;
    string res;
    string structTostr;

    SymCipher *a = new ArraySymCipher(nc2);

    SymMHE symMHE =SymMHE();

    symMHE.multiplyPlaintext(*a, m);
    str22struct::ArraySymCipher_change p;

    p.value = a->getValue();
    p.sizePos = a->getsizePos();
    p.offsetPos = a->getoffsetPos();
    p.cardMultiplierPos = a->getcardMultiplierPos();
    p.sizeNeg = a->getsizeNeg();
    p.offsetNeg = a->getoffsetNeg();
    p.cardMultiplierNeg = a->getcardMultiplierNeg();
    p.idsPos.assign(a->getidsPos().begin(), a->getidsPos().end());
    p.idsNeg.assign(a->getidsNeg().begin(), a->getidsNeg().end());
    p.cardPos = a->getcardPos();
    p.cardNeg = a->getcardNeg();

    string mapTostrPos = str22.mapToString(p.cardPos);
    string mapTostrNeg = str22.mapToString(p.cardNeg);

    res = str22.ArraySymCipher_to_str(&str, &p);
    structTostr = res + mapTostrPos + "%" + mapTostrNeg + "^";
    //char *ch=(char*)structTostr.data();

    size_t bufsz1 = sizeof(structTostr) + 1;
    size_t hexsz1 = 2 * (strlen(structTostr.data()) + 1);
    char *h = new char[hexsz1 * 2];
    change.buf2hex((uint8_t *) structTostr.data(), strlen(structTostr.data()), h, &hexsz1);

    delete a;
    delete []nc2;
    return h;

}

char* SMHE_DIVIDE_udf(char *cipher1,char *cipher2){ // post

    size_t bufsz2 = sizeof(cipher1) + 1;
    size_t hexsz2 = 2 * (strlen(cipher1) + 1);
    char* nc2 = new char[hexsz2];
    t change;
    str22struct str22;
    change.hex2buf((uint8_t*)nc2, &hexsz2, cipher1, hexsz2);
    //<<"cipher1_parse: "<<nc2<<endl;

    size_t bufsz3 = sizeof(cipher2) + 1;
    size_t hexsz3 = 2 * (strlen(cipher2) + 1);
    char* nc3 = new char[hexsz3];
    change.hex2buf((uint8_t*)nc3, &hexsz3, cipher2, hexsz3);
    //cout<<"cipher2_parse: "<<nc3<<endl;


    char *str = NULL;
    string res;
    string structTostr;


    SymCipher* a=new ArraySymCipher(nc2);
    SymCipher* b=new ArraySymCipher(nc3);

    SymMHE symMHE =SymMHE();

    symMHE.divide(*a, *b);
    str22struct:: ArraySymCipher_change p ;

    p.value=a->getValue();
    p.sizePos=a->getsizePos();
    p.offsetPos=a->getoffsetPos();
    p.cardMultiplierPos=a->getcardMultiplierPos();
    p.sizeNeg=a->getsizeNeg();
    p.offsetNeg=a->getoffsetNeg();
    p.cardMultiplierNeg=a->getcardMultiplierNeg();
    p.idsPos.assign(a->getidsPos().begin(),a->getidsPos().end());
    p.idsNeg.assign(a->getidsNeg().begin(),a->getidsNeg().end());
    p.cardPos=a->getcardPos();
    p.cardNeg=a->getcardNeg();

    string mapTostrPos = str22.mapToString(p.cardPos);
    string mapTostrNeg = str22.mapToString(p.cardNeg);

    res =str22.ArraySymCipher_to_str(&str, &p);
    structTostr=res+mapTostrPos+"%"+mapTostrNeg+"^";
    //char *ch=(char*)structTostr.data();

    size_t bufsz1 = sizeof(structTostr) + 1;
    size_t hexsz1 = 2 * (strlen(structTostr.data()) + 1);
    char* h = new char[hexsz1 * 2];
    change.buf2hex((uint8_t*)structTostr.data(), strlen(structTostr.data()), h, &hexsz1);


    delete a;
    delete b;
    delete []nc2;
    delete []nc3;
    return h;


}
//    SymMHE symMHE =SymMHE();
//
//    symMHE.pow(a, m);
char* SMHE_POW_udf(char *cipher1,int m ){ // post

    size_t bufsz2 = sizeof(cipher1) + 1;
    size_t hexsz2 = 2 * (strlen(cipher1) + 1);
    char *nc2 = new char[hexsz2];
    t change;
    str22struct str22;
    change.hex2buf((uint8_t *) nc2, &hexsz2, cipher1, hexsz2);
    //cout << "cipher1_parse: " << nc2 << endl;

    char *str = NULL;
    string res;
    string structTostr;

    SymCipher *a = new ArraySymCipher(nc2);

    SymMHE symMHE =SymMHE();

    symMHE.pow(*a, m);
    str22struct::ArraySymCipher_change p;

    p.value = a->getValue();
    p.sizePos = a->getsizePos();
    p.offsetPos = a->getoffsetPos();
    p.cardMultiplierPos = a->getcardMultiplierPos();
    p.sizeNeg = a->getsizeNeg();
    p.offsetNeg = a->getoffsetNeg();
    p.cardMultiplierNeg = a->getcardMultiplierNeg();
    p.idsPos.assign(a->getidsPos().begin(), a->getidsPos().end());
    p.idsNeg.assign(a->getidsNeg().begin(), a->getidsNeg().end());
    p.cardPos = a->getcardPos();
    p.cardNeg = a->getcardNeg();

    string mapTostrPos = str22.mapToString(p.cardPos);
    string mapTostrNeg = str22.mapToString(p.cardNeg);

    res = str22.ArraySymCipher_to_str(&str, &p);
    structTostr = res + mapTostrPos + "%" + mapTostrNeg + "^";
    //char *ch=(char*)structTostr.data();

    size_t bufsz1 = sizeof(structTostr) + 1;
    size_t hexsz1 = 2 * (strlen(structTostr.data()) + 1);
    char *h = new char[hexsz1 * 2];
    change.buf2hex((uint8_t *) structTostr.data(), strlen(structTostr.data()), h, &hexsz1);

    delete a;
    delete []nc2;
    return h;

}
//    SymMHE symMHE =SymMHE();
//
//    symMHE.inverse(a);
char* SMHE_INVERSE_udf(char* cipher1){ // post
    size_t bufsz2 = sizeof(cipher1) + 1;
    size_t hexsz2 = 2 * (strlen(cipher1) + 1);
    char* nc2 = new char[hexsz2];
    t change;
    str22struct str22;
    change.hex2buf((uint8_t*)nc2, &hexsz2, cipher1, hexsz2);
    //cout<<"cipher1_parse: "<<nc2<<endl;

    char *str = NULL;
    string res;
    string structTostr;


    SymCipher* a=new ArraySymCipher(nc2);

    SymMHE symMHE =SymMHE();
    symMHE.inverse(*a);
    str22struct:: ArraySymCipher_change p ;

    p.value=a->getValue();
    p.sizePos=a->getsizePos();
    p.offsetPos=a->getoffsetPos();
    p.cardMultiplierPos=a->getcardMultiplierPos();
    p.sizeNeg=a->getsizeNeg();
    p.offsetNeg=a->getoffsetNeg();
    p.cardMultiplierNeg=a->getcardMultiplierNeg();
    p.idsPos.assign(a->getidsPos().begin(),a->getidsPos().end());
    p.idsNeg.assign(a->getidsNeg().begin(),a->getidsNeg().end());
    p.cardPos=a->getcardPos();
    p.cardNeg=a->getcardNeg();

    string mapTostrPos = str22.mapToString(p.cardPos);
    string mapTostrNeg = str22.mapToString(p.cardNeg);

    res =str22.ArraySymCipher_to_str(&str, &p);
    structTostr=res+mapTostrPos+"%"+mapTostrNeg+"^";
    //char *ch=(char*)structTostr.data();

    size_t bufsz1 = sizeof(structTostr) + 1;
    size_t hexsz1 = 2 * (strlen(structTostr.data()) + 1);
    char* h = new char[hexsz1 * 2];
    change.buf2hex((uint8_t*)structTostr.data(), strlen(structTostr.data()), h, &hexsz1);


    delete a;
    delete []nc2;
    return h;

}
//  int main()
// {


//     //Interface inter;
//     long long m1=10;
//     long long m2=-10;
//     cout<<"test_DET："<<endl;
//     encryption encrypt;
//     char* m_char1="-12345";

//     size_t len1;
//     size_t len2;
//     char* cipher =encrypt.DET_encrypt(m_char1,&len1);
//     char* m_char2=encrypt.DET_decrypt(cipher,&len2);
// //    char* cipher =encrypt.DET_encrypt(m_char1, len1);
// //    char* m_char2=encrypt.DET_decrypt(cipher, len2);
//     cout<<"DET_DONE"<<endl;
//     cout<<"test_SAHE："<<endl;
//     char  cipher1[]="363031363731383732373138373336383239327e31213140312a30283029302b303c234e55244c4c254c4c5e";//6016718727187368292~1!1@1*0(0)0+0<#NU$LL%LL^
//     char cipher2[]="373033363131343739333030363738383338327e31213240312a30283029302b303c234e55244c4c254c4c5e";//7036114793006788382~1!2@1*0(0)0+0<#NU$LL%LL^
//     //inter.SAHE_add_udf(cipher1,cipher2);
//     SymAHE symAHE=SymAHE();
//     char *a=symAHE.encrypt(m1);
//     char *b=symAHE.encrypt(m2);
//     char* result=SAHE_add_udf(a,b);
//     char* result_plain=SAHE_addplaintext_udf(a,m2);
//     long long result_de=symAHE.decrypt(result);
//     long long result__plain=symAHE.decrypt(result_plain);
//     long long m_1=symAHE.decrypt(a);
//     //char *b=symAHE.encrypt(m2);s
//     long long m_2=symAHE.decrypt(b);
//     cout<<"SAHE_DONE"<<endl;
//     cout<<"test_SMHE："<<endl;
//     SymMHE symMHE=SymMHE();
//     char *c=symMHE.encrypt(m1);
//     char *d=symMHE.encrypt(m2);
//     char * multi=SMHE_MULTIPLY_udf(c,d);
//     long long multi_de=symMHE.decrypt(multi);
//     long long m_3=symMHE.decrypt(c);
//     long long m_4=symMHE.decrypt(d);
//     cout<<"SMHE_DONE"<<endl;

//     //SAHE_add_udf(cipher1,cipher2);

// }







// extern "C" void* getAS(long, long) {
//     auto tmp = new ArraySymCipher();
//     return tmp;
// }
