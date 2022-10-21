

#include "InterfaceWarpper.h"

text *callFunction4p(text *t1, text *t2, text *t3, text *t4, func4p_t func, OpType type)
{

    size_t s1 = VARSIZE(t1) - VARHDRSZ;
    size_t s2 = VARSIZE(t2) - VARHDRSZ;
    size_t s3 = VARSIZE(t3) - VARHDRSZ;
    size_t s4 = VARSIZE(t4) - VARHDRSZ;

    char *dat1 = palloc0(s1 + 1);
    char *dat2 = palloc0(s2 + 1);
    char *dat3 = palloc0(s3 + 1);
    char *dat4 = palloc0(s4 + 1);
    memcpy(dat1, t1->vl_dat, s1);
    memcpy(dat2, t2->vl_dat, s2);
    memcpy(dat3, t3->vl_dat, s3);
    memcpy(dat4, t4->vl_dat, s4);

    bool isLeftFloat = dat1[s1 - 1] == '2';
    bool isRightFloat = dat2[s2 - 1] == '2';
    // 去除标志位
    dat1[s1 - 1] = 0;
    dat2[s2 - 1] = 0;

    if (isLeftFloat || isRightFloat)
    {
        if (!(isLeftFloat && isRightFloat))
        {
            if (!isLeftFloat)
            {
                //TODO: 怎么乘倍数？
            }
        }
    }

    char *buffer = saheAdd(dat1, dat2, dat3, dat4);
    size_t buffSize = strlen(buffer);
    text *destination = (text *)palloc0(VARHDRSZ + buffSize + 1);
    SET_VARSIZE(destination, VARHDRSZ + buffSize + 1);

    char *pbuf = VARDATA(destination);
    memcpy(pbuf, buffer, buffSize);
    free(buffer);
    // 没有解决加法下放缩的问题，只能先设置为1了
    destination->vl_dat[buffSize] = '1';

    PG_RETURN_TEXT_P(destination);
}
