//
// Created by 甜甜 on 2020/12/13.
//

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include<ext/hash_map>
#include "str22struct.h"
using namespace __gnu_cxx;
//#include "split.h"
using namespace std;



string str22struct::getString(const int& a)
{
    char c[256]={0};
    snprintf(c,sizeof(c),"%d",a);
    return c;
}

string str22struct::getString(const long long & a)
{
    char c[256]={0};
    snprintf(c,sizeof(c),"%lld",a);
    return c;
}


string str22struct::getString(const string& s)
{
    return s;
}




string str22struct::ArraySymCipher_to_str(char **dest, struct ArraySymCipher_change *cipher)
{
    unsigned int size = 0;
    string str0;

    if (dest == NULL || cipher == NULL)
        return NULL;

    size =  550 * sizeof(int) + 2 *sizeof(char);
    // vector<int> des;
    // memcpy(&des, &cipher->idsPos[0], cipher->idsPos.size()+1);
    string vec2strPos;
    string vec2strNeg;
    for (int i = 0; i < cipher->idsPos.size(); i++)
    {
        //vec2strPos += "<";
        vec2strPos += to_string(cipher->idsPos[i])+"<";
        //vec2strPos += ">";
    };
    for (int i = 0; i < cipher->idsNeg.size(); i++)
    {
        //vec2strNeg+= "<";
        vec2strNeg += to_string(cipher->idsNeg[i])+"<";
        //vec2strNeg+= ">";
    };
//    cout<<"vec2strPos: "<<vec2strPos<<endl;
//    cout<<"vec2strNeg: "<<vec2strNeg<<endl;

    *dest = (char *)malloc(size);
    if (*dest == NULL)
        return NULL;

    memset(*dest, 0, size);

    sprintf(*dest,
            "%lld~"
            "%d!"
            "%ld@"
            "%lld*"
            "%d("
            "%ld)"
            "%lld+", cipher->value, cipher->sizePos, cipher->offsetPos,cipher->cardMultiplierPos, cipher->sizeNeg, cipher->offsetNeg,cipher->cardMultiplierNeg);
    if (!vec2strPos.empty()&&!vec2strNeg.empty())
    {
        str0 = vec2strPos+"#"+vec2strNeg+"$";
    }
    else if(vec2strPos.empty()&&!vec2strNeg.empty())
    {
        str0 = "NU#"+vec2strNeg+"$";
    }
    else if(!vec2strPos.empty()&&vec2strNeg.empty())
    {
        str0 = vec2strPos+"#"+"NU$";
    }

    string str1 = *dest+str0;
    //cout<<str1<<endl;
    return str1;
    //, cipher->idsPos, cipher->cardMultiplierPos, cipher->cardPos, cipher->sizeNeg, cipher->offsetNeg, cipher->idsNeg, cipher->cardMultiplierNeg, cipher->cardNeg
}
char * str22struct::strsep(char **stringp, const char *delim)
{
    //判断stringp是否为空,提高程序的容错性
    char *s;
    const char *spanp;
    int c, sc;
    char *tok;
    if ((s = *stringp)== NULL)
        return (NULL);
    for (tok = s;;) {
        c = *s++;
        spanp = delim;
        do {
            if ((sc =*spanp++) == c) {
                if (c == 0)
                    s = NULL;
                else
                    s[-1] = 0;
                *stringp = s;
                return (tok);
            }
        } while (sc != 0);
    }
}

void str22struct::pushToVec(std::vector<std::string>&obj,const char *param,std::string token)
{
    char *p = (char*)param;
    char *key_point;
    while(p)
    {
        while ( key_point = strsep(&p,token.c_str()))//关键字为c或d，它们连续出现了
        {
            //函数遇到token时，key_point指向token返回
            if (*key_point == 0)
                continue;
            else
                break;
        }
        if (key_point)
            obj.push_back(key_point);
    }
}

vector<string> str22struct::split(string s, char delim){
    vector<string> v;
    stringstream stringstream1(s);
    string tmp;
    while(getline(stringstream1, tmp, delim)){
        v.push_back(tmp);
    }
    return v;
}

int str22struct::ArraySymCipher_parse(string str, struct ArraySymCipher_change **cipher)
{
    if (str == "" || cipher == NULL)
        return -1;

    *cipher = (struct ArraySymCipher_change *)malloc(sizeof(struct ArraySymCipher_change));
    if (*cipher == NULL)
        return -2;

    memset(*cipher, 0, sizeof(struct ArraySymCipher_change));
    string test1(str);
    string token1("~!@*()+#$%^");
    vector<string> obj1;

    vector<string> obj2;
    vector<string> obj3;

    vector<string> obj4;
    vector<string> obj5;
    vector<string> obj6;

    pushToVec(obj1,test1.c_str(),token1);

    (*cipher)->value=std::stoll(obj1[0]);
    //cout<<"value___: "<<(*cipher)->value<<endl;
    (*cipher)->sizePos=std::stoi(obj1[1]);
    (*cipher)->offsetPos=std::stol(obj1[2]);
    (*cipher)->cardMultiplierPos=std::stol(obj1[3]);
    (*cipher)->sizeNeg=std::stoi(obj1[4]);
    (*cipher)->offsetNeg=std::stol(obj1[5]);
    (*cipher)->cardMultiplierNeg=std::stol(obj1[6]);
    if (obj1[7]!="NU")
    {
        string test2(obj1[7]);
        string token2("<");
        pushToVec(obj2,test2.c_str(),token2);
        for (int i = 0; i < obj2.size(); i++)
        {
            (*cipher)->idsPos.push_back(std::stoi(obj2[i]));
            //cout<<"obj2: "<<(*cipher)->idsPos[i]<<endl;
        };
    }else
    {
        (*cipher)->idsPos.clear();
    }

    if (obj1[8]!="NU")
    {
        string test3(obj1[8]);
        string token3("<");
        pushToVec(obj3,test3.c_str(),token3);
        for (int i = 0; i < obj3.size(); i++)
        {
            (*cipher)->idsNeg.push_back(std::stoi(obj3[i]));
            //cout<<"obj3: "<<(*cipher)->idsNeg[i]<<endl;
        };
    }else
    {
        (*cipher)->idsNeg.clear();
    }

    if (obj1[9]!="LL")
    {
        string test4(obj1[9]);
        string token4("?");
        pushToVec(obj4,test4.c_str(),token4);


        for (int i = 0; i < obj4.size(); i++)
        {
            vector<string> v = split(obj4[i], '_');
            if(v.size() == 2)
            {
                (*cipher)->cardPos[std::stoi(v[0])]=std::stoll(v[1]);
            }
            else
            {
                printf("Failed cardPos");
                return -1;
            }
        };
    }else
    {
        (*cipher)->cardPos.clear();
    }

    if (obj1[10]!="LL")
    {
        string test5(obj1[10]);
        string token5("?");
        pushToVec(obj5,test5.c_str(),token5);
        for (int i = 0; i < obj5.size(); i++)
        {
            vector<string> v = split(obj5[i], '_');
            if(v.size() == 2)
            {
                (*cipher)->cardNeg[std::stoi(v[0])]=std::stoll(v[1]);
            }
            else
            {
                printf("Failed cardNeg");
                return -1;
            }
        };
    }else
    {
        (*cipher)->cardNeg.clear();
    }

    return 0;
}

//int main(int argc, char *argv[])
//{
//
//    struct ArraySymCipher_change a;
//    struct ArraySymCipher_change *p = NULL;
//    char *str = NULL;
//    string res;
//    string dest;
//    string structTostr;
//    int res1;
//    str22struct str;
//
//    a.value = 6016718727187368292;
//    a.sizePos = 1;
//    a.offsetPos= 1;
//    a.idsPos={0};
//    a.cardMultiplierPos=1;
//    a.sizeNeg = 0;
//    a.offsetNeg= 0;
//    a.idsNeg={};
//    a.cardMultiplierNeg=0;
//    // a.cardPos[1]={2};
//    // a.cardPos[2]={9};
//    // a.cardNeg[1]={0};
//
//    string mapTostrPos = mapToString(a.cardPos);
//    string mapTostrNeg = mapToString(a.cardNeg);
//
//
//    res =ArraySymCipher_to_str(&str, &a);
//    //cout<<"res:"<<str<<endl;
//    structTostr=res+mapTostrPos+"%"+mapTostrNeg+"^";
//    //cipher->value, cipher->sizePos, cipher->offsetPos,cipher->cardMultiplierPos, cipher->sizeNeg, cipher->offsetNeg,cipher->cardMultiplierNeg,vec2strPos+"#"+vec2strNeg+"$",mapTostrPos+"%"+mapTostrNeg+"^"
//    cout<<"structTostr= "<<structTostr<<endl;
//
//
//    // if (res != 0)
//    // {
//    // 	printf("Failed to executeArraySymCipher_to_str");
//    // 	return -1;
//    // }
//    // const char *q = str.data();
//    // cout<<"q:"<<*q<<endl;
//    // char *h = const_cast<char*>(q);
//
//
//    res1=ArraySymCipher_parse(structTostr, &p);
//
//    cout<<"value： "<<p->value<<endl;
//    cout<<"sizepos： "<<p->sizePos<<endl;
//    cout<<"offsetPos："<<p->offsetPos<<endl;
//    cout<<"cardMultiplierPos："<<p->cardMultiplierPos<<endl;
//    cout<<"sizeNeg： "<<p->sizeNeg<<endl;
//    cout<<"offsetNeg："<<p->offsetNeg<<endl;
//    cout<<"cardMultiplierNeg："<<p->cardMultiplierNeg<<endl;
//    if((p->idsPos).size()!=0)
//    {
//        for(int i=0;i<(p->idsPos).size();i++)
//        {
//            cout<<"idsPOS："<<p->idsPos[i]<<endl;
//        }
//    }else{
//        cout<<"idsPOS为空"<<endl;
//    }
//
//    if((p->idsNeg).size()!=0)
//    {
//        for(int i=0;i<(p->idsNeg).size();i++)
//        {
//            cout<<"idsneg："<<p->idsNeg[i]<<endl;
//        }
//    }else
//    {
//        cout<<"idsneg为空"<<endl;
//    }
//
//
//
//    hash_map<int, long long> ::iterator iter1;
//    if ((p->cardPos).begin()!=(p->cardPos).end())
//    {
//        for(iter1=(p->cardPos).begin();iter1!=(p->cardPos).end();iter1++)
//        {
//            cout<<"cardPOS_key: "<<iter1->first<<" cardPOS_VALUE: "<<iter1->second<<endl;
//        }
//    }else
//    {
//        cout<<"cardPOS为空"<<endl;
//    }
//
//
//    hash_map<int, long long> ::iterator iter2;
//    if ((p->cardNeg).begin()!=(p->cardNeg).end())
//    {
//        for(iter2=(p->cardNeg).begin();iter2!=(p->cardNeg).end();iter2++)
//        {
//            cout<<"cardNEG_key: "<<iter2->first<<" cardNEG_VALUE: "<<iter2->second<<endl;
//        }
//    }else
//    {
//        cout<<"cardNEG为空"<<endl;
//    }
//
//
//
//    if (res1 != 0)
//    {









//        free(str);
//        str = NULL;
//    }
//
//    free(p);
//    p = NULL;
//
//    return 0;
//}