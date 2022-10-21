
#include <memory>

#include <map>
#include <unordered_map>
#include <vector>
#include <string>
#include "MathUtils.h"
#include "Symcipher.h"
#include "ArraySymCipher.h"
#include "str22struct.h"

using namespace std;
// struct ArraySymCipher
//{
//    long long  value;
//    int sizePos;
//    long long  offsetPos;
//    vector<int> idsPos;
//    //int idsPos[256];
//    long long cardMultiplierPos;
//    unordered_map<int, long long > cardPos;
//    int sizeNeg;
//    long long offsetNeg;
//    vector<int> idsNeg;
//    //int idsNeg[256];
//    long long cardMultiplierNeg;
//    unordered_map<int, long long > cardNeg;
//};

//新增运算时结构体转对象
ArraySymCipher::ArraySymCipher(string cipher)
{
    str22struct::ArraySymCipher_change *p = NULL;
    str.ArraySymCipher_parse(cipher, &p);
    // SymCipher(p->value\)
    this->value = p->value;
    this->sizePos = p->sizePos;
    this->offsetPos = p->offsetPos;
    this->cardMultiplierPos = p->cardMultiplierPos;
    this->sizeNeg = p->sizeNeg;
    this->offsetNeg = p->offsetNeg;
    this->cardMultiplierNeg = p->cardMultiplierNeg;
    this->idsPos.assign(p->idsPos.begin(), p->idsPos.end());
    this->idsNeg.assign(p->idsNeg.begin(), p->idsNeg.end());
    this->cardPos = p->cardPos;
    this->cardNeg = p->cardNeg;
    delete p;
}
long long ArraySymCipher::getValue()
{
    return value;
}
int ArraySymCipher::getsizePos()
{
    return this->sizePos;
}
long long ArraySymCipher::getoffsetPos()
{
    return this->offsetPos;
}
long long ArraySymCipher::getcardMultiplierPos()
{
    return this->cardMultiplierPos;
}
int ArraySymCipher::getsizeNeg()
{
    return this->sizeNeg;
}
long long ArraySymCipher::getoffsetNeg()
{
    return this->offsetNeg;
}
long long ArraySymCipher::getcardMultiplierNeg()
{
    return this->cardMultiplierNeg;
}
vector<int> &ArraySymCipher::getidsPos()
{
    return this->idsPos;
}
vector<int> &ArraySymCipher::getidsNeg()
{
    return this->idsNeg;
}
unordered_map<int, long long> ArraySymCipher::getcardPos()
{
    return this->cardPos;
}
unordered_map<int, long long> ArraySymCipher::getcardNeg()
{
    return this->cardNeg;
}

ArraySymCipher::ArraySymCipher(long long value, long long id)
{
    SymCipher::setValue(value);
    sizePos = 1;
    offsetPos = id;
    idsPos.push_back(0);
    cardMultiplierPos = 1;
}

void ArraySymCipher::addIds(ArraySymCipher other, bool isPos)
{
    int thisSize;
    long long thisOffset;
    // int *thisIds;
    vector<int> thisIds;
    long long thisCardMultiplier;
    unordered_map<int, long long> thisCard;

    int otherSize;
    long long otherOffset;
    // int *otherIds;
    vector<int> otherIds;
    long long otherCardMultiplier;
    unordered_map<int, long long> otherCard;

    // choose positive or negative items
    if (isPos)
    {
        thisSize = sizePos;
        thisOffset = offsetPos;
        thisIds = idsPos;
        thisCardMultiplier = cardMultiplierPos;
        thisCard = cardPos;

        otherSize = other.sizePos;
        otherOffset = other.offsetPos;
        otherIds = other.idsPos;
        otherCardMultiplier = other.cardMultiplierPos;
        otherCard = other.cardPos;
    }
    else
    {
        thisSize = sizeNeg;
        thisOffset = offsetNeg;
        thisIds = idsNeg;
        thisCardMultiplier = cardMultiplierNeg;
        thisCard = cardNeg;

        otherSize = other.sizeNeg;
        otherOffset = other.offsetNeg;
        otherIds = other.idsNeg;
        otherCardMultiplier = other.cardMultiplierNeg;
        otherCard = other.cardNeg;
    }

    if (otherSize == 0)
        return;

    int newSize;
    long long newOffset = 0;
    // int *newIds;
    vector<int> newIds;
    long long newCardMultiplier;
    unordered_map<int, long long> newCard;

    if (thisSize == 0)
    {
        newSize = otherSize;
        newOffset = otherOffset;
        newIds = otherIds;
        newCardMultiplier = otherCardMultiplier;
        newCard = otherCard;
    }
    else
    {
        // upper bound on new size since ids can cancel out, i.e., in case cardinality
        // sums up to 0.
        int upperSize = thisSize + otherSize;
        for (int i = 0; i < upperSize; i++)
        {
            newIds.push_back(0);
        }
        // newIds = new int[upperSize]();

        // unordered_map<int,long long> newCard;

        // keep the cardinality multiplier of the long longest ciphertext
        newCardMultiplier = thisCardMultiplier;
        if (thisSize < otherSize)
            newCardMultiplier = otherCardMultiplier;

        int thisIndex = 0;
        int otherIndex = 0;
        int newIndex = 0;
        long long currentOffset = 0;

        while (thisIndex < thisSize || otherIndex < otherSize)
        {

            long long thisId = -1;
            long long otherId = -1;
            if (thisIndex < thisSize)
                thisId = thisOffset + thisIds[thisIndex]; // thisIDS出错  thisIDS[1]应该是1不是0
            if (otherIndex < otherSize)
                otherId = otherOffset + otherIds[otherIndex];

            if (otherIndex >= otherSize || (thisIndex < thisSize && thisId < otherId))
            {
                // set the id
                newIds[newIndex] = (int)(thisId - currentOffset); // thisID应该是2不是1
                //##########################newIDS{1,0}
                if (newIndex == 0)
                {
                    newOffset = newIds[newIndex];
                    newIds[newIndex] = 0;
                }

                // update offsets
                thisOffset = thisId;
                currentOffset = thisId;

                // set the cardinality
                long long card = thisCardMultiplier;
                if (!thisCard.empty() && thisCard.find(thisIndex) != thisCard.end())
                    card = thisCard[thisIndex];
                if (card != newCardMultiplier)
                    newCard[newIndex] = card;

                // update indices
                thisIndex++;
                newIndex++;
            }
            else if (thisIndex >= thisSize || otherId < thisId)
            {
                // set the id
                newIds[newIndex] = (int)(otherId - currentOffset); //输出一下newsid
                if (newIndex == 0)
                {
                    newOffset = newIds[newIndex];
                    newIds[newIndex] = 0;
                }

                // update offsets
                otherOffset = otherId;
                currentOffset = otherId;

                // set the cardinality
                long long card = otherCardMultiplier;
                if (otherCard.empty() == false && otherCard.find(otherIndex) != otherCard.end())
                    card = otherCard[otherIndex];
                if (card != newCardMultiplier)
                    newCard[newIndex] = card;

                // update indices
                otherIndex++;
                newIndex++;
            }
            else
            {
                // set the id
                newIds[newIndex] = (int)(thisId - currentOffset);
                if (newIndex == 0)
                {
                    newOffset = newIds[newIndex];
                    newIds[newIndex] = 0;
                }

                // update offsets
                thisOffset = thisId;
                otherOffset = thisId;
                currentOffset = thisId;

                // set the cardinality
                long long card1 = thisCardMultiplier;
                if (thisCard.empty() == false && thisCard.find(thisIndex) != thisCard.end())
                    card1 = thisCard[thisIndex];

                long long card2 = otherCardMultiplier;
                if (otherCard.empty() == false && otherCard.find(otherIndex) != otherCard.end())
                    card2 = otherCard[otherIndex];

                long long card = card1 + card2;
                if (card != newCardMultiplier)
                    newCard[newIndex] = card;

                // update indices
                thisIndex++;
                otherIndex++;
                newIndex++;
            }
        }
        newSize = newIndex;
    }

    if (isPos)
    {
        sizePos = newSize;
        offsetPos = newOffset;
        idsPos = newIds;
        cardMultiplierPos = newCardMultiplier;
        cardPos = newCard;
    }
    else
    {
        sizeNeg = newSize;
        offsetNeg = newOffset;
        idsNeg = newIds;
        cardMultiplierNeg = newCardMultiplier;
        cardNeg = newCard;
    }
    //        if (idsPos.size()!=0)
    //        {
    //            for (int a = 0;a<idsPos.size();a++)
    //            {
    //                cout<<"idPOSarray: "<<idsPos[a]<<endl;
    //            }
    //        }
    //        if (idsPos.size()==0)
    //        {
    //                cout<<"idPOSarray is null"<<endl;
    //
    //        }
    //        cout<<"sizepos: "<<sizePos<<endl;
    //        cout<<"offsetPOS: "<<offsetPos<<endl;
    //        cout<<"cardmultiplier: "<<cardMultiplierPos<<endl;
    //        unordered_map<int, long long> ::iterator iter1;
    //        for(iter1=cardPos.begin();iter1!=cardPos.end();iter1++)
    //        {
    //            cout<<"cardPOS_key: "<<iter1->first<<" cardPOS_VALUE: "<<iter1->second<<endl;
    //        }
    //        if (idsNeg.size()!=0)
    //        {
    //            for (int b = 0;b<idsNeg.size();b++)
    //            {
    //                cout<<"idsNegarray: "<<idsNeg[b]<<endl;
    //            }
    //        }
    //        if (idsNeg.size()==0)
    //        {
    //            cout<<"idNEGarray is null"<<endl;
    //        }
    //        cout<<"sizeNEG: "<<sizeNeg<<endl;
    //        cout<<"offsetNEG: "<<offsetNeg<<endl;
    //        cout<<"cardmultiplierNEG: "<<cardMultiplierNeg<<endl;
    //        unordered_map<int, long long> ::iterator iter2;
    //        for(iter2=cardNeg.begin();iter2!=cardNeg.end();iter2++)
    //        {
    //            cout<<"cardNEG_key: "<<iter2->first<<" cardNEG_VALUE: "<<iter2->second<<endl;
    //        }
    //        //            for (int *p=idsPos;p<=&idsPos[sizeof(idsPos)/sizeof(idsPos[0])-1];p++)
    ////            {
    ////                cout<<"idPOSarray: "<<*p<<endl;
    ////            }
}

void ArraySymCipher::addIds(ArraySymCipher other)
{
    addIds(other, true);
    addIds(other, false);
}

void ArraySymCipher::multiplyIds(long long multiplier, long long modulo)
{
    if (multiplier == 0)
    {
        sizePos = 0;
        sizeNeg = 0;
        offsetPos = 0L;
        offsetNeg = 0l;
        ////////////////////////////////////////////////////////

        idsPos.clear();
        idsNeg.clear();

        // idsPos = nullptr;
        // idsNeg = nullptr;
        ///////////////////////////////////////////////////////
        cardMultiplierPos = 1L;
        cardMultiplierNeg = 1L;
        cardPos.clear();
        cardNeg.clear();
        return;
    }

    // swap
    if (multiplier < 0)
    {
        multiplier = -multiplier;
        // swap sizes
        int size = sizePos;
        sizePos = sizeNeg;
        sizeNeg = size;

        // swap offsets
        long long offset = offsetPos;
        offsetPos = offsetNeg;
        offsetNeg = offset;

        // swap ids
        vector<int> ids = idsPos;
        idsPos = idsNeg;
        idsNeg = ids;

        // swap cardMultiplier
        long long cm = cardMultiplierPos;
        cardMultiplierPos = cardMultiplierNeg;
        cardMultiplierNeg = cm;

        // swap cardinalities
        unordered_map<int, long long> card = cardPos;
        cardPos = cardNeg;
        cardNeg = card;
    }

    if (multiplier == 1)
        return;

    cardMultiplierPos = math.modMul(cardMultiplierPos, multiplier, modulo);
    unordered_map<int, long long>::iterator iter;
    iter = cardPos.begin();
    if (!cardPos.empty())
        while (iter != cardPos.end())
        {
            cardPos[iter->first] = math.modMul(iter->second, multiplier, modulo);
            iter++;
        }

    cardMultiplierNeg = math.modMul(cardMultiplierNeg, multiplier, modulo);
    unordered_map<int, long long>::iterator iter2;
    iter2 = cardNeg.begin();
    if (!cardNeg.empty())
        while (iter2 != cardNeg.end())
        {
            cardNeg[iter2->first] = math.modMul(iter2->second, multiplier, modulo);
            iter2++;
        }

    // for (int a = 0; a < idsPos.size(); a++) {
    //     // " << idsPos[a] << endl;
    // }
    // //            for (int *p=idsPos;p<=&idsPos[sizeof(idsPos)/sizeof(idsPos[0])-1];p++)
    // //            {
    // //                cout<<"idPOSarray: "<<*p<<endl;
    // //            }
    // // " << sizePos << endl;
    // // " << offsetPos << endl;
    // // " << cardMultiplierPos << endl;
    // unordered_map<int, long long>::iterator itera;
    // for (itera = cardPos.begin(); itera != cardPos.end(); itera++) {
    //     // " << itera->first << " cardPOS_VALUE: " << itera->second << endl;
    // }
    // for (int b = 0; b < idsNeg.size(); b++) {
    //     // " << idsNeg[b] << endl;
    // }
    // // " << sizeNeg << endl;
    // // " << offsetNeg << endl;
    // // " << cardMultiplierNeg << endl;
    // unordered_map<int, long long>::iterator iterb;
    // for (iterb = cardNeg.begin(); iterb != cardNeg.end(); iterb++) {
    //     // " << iterb->first << " cardNEG_VALUE: " << iterb->second << endl;
    // }
}

// sizePos + sizeNeg
/**
 * Return the number of ids currently stored.
 */
int ArraySymCipher::getSize()
{
    return this->sizePos + this->sizeNeg;
}

//
//@Override
// for (int i=0;i<upperSize;i++)
//{
// newIds.push_back(0);
//}
vector<vector<long long>> ArraySymCipher::getIds()
{ ////////接口已经改变，返回的是ids2

    int size = getSize(); //可能有问题，怎么可能是6643153
    // ids和card容量是一样的
    vector<long long> ids;
    vector<long long> card;
    vector<vector<long long>> combination;
    for (int i = 0; i < size; i++)
    {
        ids.push_back(0);
    }
    for (int j = 0; j < size; j++)
    {
        card.push_back(0);
    }
    long long currentOffset = offsetPos;

    for (int i = 0; i < sizePos; i++)
    {
        long long id = currentOffset + idsPos[i];
        currentOffset = id;
        ids[i] = id;

        long long c = this->cardMultiplierPos;
        if (!this->cardPos.empty() && this->cardPos.find(i) != this->cardPos.end())
            c = this->cardPos[i];
        card[i] = c;
    }

    currentOffset = offsetNeg;
    for (int i = 0; i < sizeNeg; i++)
    {
        long long id = currentOffset + idsNeg[i]; //可能有问题，报错Signal: SIGSEGV (Segmentation fault)

        currentOffset = id;
        ids[i + sizePos] = id;

        long long c = this->cardMultiplierNeg;
        if (!this->cardNeg.empty() && this->cardNeg.find(i) != this->cardNeg.end())
            c = this->cardNeg[i];
        card[i + sizePos] = -c;
    }
    combination.push_back(ids);
    combination.push_back(card);
    return combination;
}

void ArraySymCipher::add(SymCipher &other, long long modulo)
{
    //        long long ov = 0;
    //        ov = other.getValue();
    //        //" << ov << endl;
    //        char* p = new char[sizeof(long long)];
    //        p = (char*) ov;
    SymCipher::addValue(other.getValue(), modulo);
    addIds((ArraySymCipher &)other);
}

void ArraySymCipher::sub(SymCipher &other, long long modulo)
{
    SymCipher::subValue(other.getValue(), modulo);
    ArraySymCipher &o = (ArraySymCipher &)other;
    o.multiplyIds(-1, modulo);
    addIds(o);
}

void ArraySymCipher::multiply(long long m, long long modulo)
{
    this->multiplyValue(m, modulo);
    this->multiplyIds(m, modulo);
}

void ArraySymCipher::multiply(SymCipher &other, long long modulo)
{
    this->multiplyValue(other.getValue(), modulo);
    this->addIds((ArraySymCipher &)other);
}

void ArraySymCipher::pow(long long m, long long modulo)
{
    this->raiseValue(m, modulo);
    this->multiplyIds(m, modulo);
}
