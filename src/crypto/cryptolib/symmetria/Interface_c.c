//
// Created by 甜甜 on 2020/12/17.

//SAHE,SMHE UDF
//udf名字：sahe_add（密文同态加）sahe_addplain（密文+明文）sahe_substract（密文相减）sahe_multiply（密文*明文）sahe_negate（密文取负）
//udf名字：smhe_multiply（密文同态乘）smhe_multiplyplain（密文*明文）smhe_divide（密文除以密文） smhe_pow（密文的明文次幂）smhe_inverse（密文的乘法逆）

//
// Created by 甜甜 on 2020/12/17.
//
#include "stdio.h"
#include "Interface.h"
#include <string.h>
#include "postgres.h"
#include "fmgr.h"
#include "utils/geo_decls.h"
//
//
//
//
////#include "postgres.h"
////#include <string.h>
////#include "fmgr.h"
////#include "geo_decls.h"
//
#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif


PG_FUNCTION_INFO_V1(sahe_add);

Datum sahe_add(PG_FUNCTION_ARGS)
{
   text  *arg1 = PG_GETARG_TEXT_P(0);
   text  *arg2 = PG_GETARG_TEXT_P(1);
   char *buffer=SAHE_add_udf(arg1->vl_dat,arg2->vl_dat); /* our source data */
   text *destination = (text *) palloc(VARHDRSZ + strlen(buffer));
   SET_VARSIZE(destination, VARHDRSZ + strlen(buffer));
   memcpy(destination->vl_dat, buffer, strlen(buffer));

   PG_RETURN_TEXT_P(destination);
}

////Supposing that the above code has been prepared in file funcs.c and compiled into a shared object, we could define the functions to PostgreSQL with commands like this:
////CREATE FUNCTION sahe_add(text, text) RETURNS text
////    AS 'DIRECTORY/Interface_c', 'sahe_add'
////    LANGUAGE C STRICT;
//
//

PG_FUNCTION_INFO_V1(sahe_addplain);

Datum
sahe_addplain(PG_FUNCTION_ARGS)
{
   text  *arg1 = PG_GETARG_TEXT_P(0);
   int32  *m1 = PG_GETARG_INT32(1);
   char *buffer=SAHE_addplaintext_udf(arg1->vl_dat,m1); /* our source data */
   text *destination = (text *) palloc(VARHDRSZ + strlen(buffer));
   SET_VARSIZE(destination, VARHDRSZ + strlen(buffer));
   memcpy(destination->vl_dat, buffer, strlen(buffer));

   PG_RETURN_TEXT_P(destination);
}

PG_FUNCTION_INFO_V1(sahe_substract);

Datum
sahe_substract(PG_FUNCTION_ARGS)
{
   text  *arg1 = PG_GETARG_TEXT_P(0);
   text  *arg2 = PG_GETARG_TEXT_P(1);
   char *buffer=SAHE_substract_udf(arg1->vl_dat,arg2->vl_dat); /* our source data */
   text *destination = (text *) palloc(VARHDRSZ + strlen(buffer));
   SET_VARSIZE(destination, VARHDRSZ + strlen(buffer));
   memcpy(destination->vl_dat, buffer, strlen(buffer));

   PG_RETURN_TEXT_P(destination);

}

PG_FUNCTION_INFO_V1(sahe_multiply);

Datum
sahe_multiply(PG_FUNCTION_ARGS)
{
   text  *arg1 = PG_GETARG_TEXT_P(0);
   int32  *m1 = PG_GETARG_INT32(1);
   char *buffer=SAHE_multiply_udf(arg1->vl_dat,m1); /* our source data */
   text *destination = (text *) palloc(VARHDRSZ + strlen(buffer));
   SET_VARSIZE(destination, VARHDRSZ + strlen(buffer));
   memcpy(destination->vl_dat, buffer, strlen(buffer));

   PG_RETURN_TEXT_P(destination);

}
PG_FUNCTION_INFO_V1(sahe_negate);

Datum
sahe_negate(PG_FUNCTION_ARGS)
{
   text  *arg1 = PG_GETARG_TEXT_P(0);
   char *buffer=SAHE_neggate_udf(arg1->vl_dat); /* our source data */
   text *destination = (text *) palloc(VARHDRSZ + strlen(buffer));
   SET_VARSIZE(destination, VARHDRSZ + strlen(buffer));
   memcpy(destination->vl_dat, buffer, strlen(buffer));
   PG_RETURN_TEXT_P(destination);
}

PG_FUNCTION_INFO_V1(smhe_multiply);

Datum
smhe_multiply(PG_FUNCTION_ARGS)
{
   text  *arg1 = PG_GETARG_TEXT_P(0);
   text  *arg2 = PG_GETARG_TEXT_P(1);
   char *buffer=SMHE_MULTIPLY_udf(arg1->vl_dat,arg2->vl_dat); /* our source data */
   text *destination = (text *) palloc(VARHDRSZ + strlen(buffer));
   SET_VARSIZE(destination, VARHDRSZ + strlen(buffer));
   memcpy(destination->vl_dat, buffer, strlen(buffer));

   PG_RETURN_TEXT_P(destination);
}

PG_FUNCTION_INFO_V1(smhe_multiplyplain);

Datum
smhe_multiplyplain(PG_FUNCTION_ARGS)
{
   text  *arg1 = PG_GETARG_TEXT_P(0);
   int32  *m1 = PG_GETARG_INT32(1);
   char *buffer=SMHE_MULTIPLYPLAIN_udf(arg1->vl_dat,m1); /* our source data */
   text *destination = (text *) palloc(VARHDRSZ + strlen(buffer));
   SET_VARSIZE(destination, VARHDRSZ + strlen(buffer));
   memcpy(destination->vl_dat, buffer, strlen(buffer));

   PG_RETURN_TEXT_P(destination);
}
//
PG_FUNCTION_INFO_V1(smhe_divide);

Datum
smhe_divide(PG_FUNCTION_ARGS)
{
   text  *arg1 = PG_GETARG_TEXT_P(0);
   text  *arg2 = PG_GETARG_TEXT_P(1);
   char *buffer=SMHE_DIVIDE_udf(arg1->vl_dat,arg2->vl_dat); /* our source data */
   text *destination = (text *) palloc(VARHDRSZ + strlen(buffer));
   SET_VARSIZE(destination, VARHDRSZ + strlen(buffer));
   memcpy(destination->vl_dat, buffer, strlen(buffer));
   PG_RETURN_TEXT_P(destination);

}

PG_FUNCTION_INFO_V1(smhe_pow);

Datum
smhe_pow(PG_FUNCTION_ARGS)
{
   text  *arg1 = PG_GETARG_TEXT_P(0);
   int32  *m1 = PG_GETARG_INT32(1);
   char *buffer=SMHE_POW_udf(arg1->vl_dat,m1); /* our source data */
   text *destination = (text *) palloc(VARHDRSZ + strlen(buffer));
   SET_VARSIZE(destination, VARHDRSZ + strlen(buffer));
   memcpy(destination->vl_dat, buffer, strlen(buffer));
   PG_RETURN_TEXT_P(destination);
}

PG_FUNCTION_INFO_V1(smhe_inverse);

Datum
smhe_inverse(PG_FUNCTION_ARGS)
{
   text  *arg1 = PG_GETARG_TEXT_P(0);
   char *buffer=SMHE_INVERSE_udf(arg1->vl_dat); /* our source data */
   text *destination = (text *) palloc(VARHDRSZ + strlen(buffer));
   SET_VARSIZE(destination, VARHDRSZ + strlen(buffer));
   memcpy(destination->vl_dat, buffer, strlen(buffer));
   PG_RETURN_TEXT_P(destination);
}


//int main()
//{
//    char  *cipher1="363031363731383732373138373336383239327e31213140312a30283029302b303c234e55244c4c254c4c5e";//6016718727187368292~1!1@1*0(0)0+0<#NU$LL%LL^
//    char  *cipher2="373033363131343739333030363738383338327e31213240312a30283029302b303c234e55244c4c254c4c5e";//7036114793006788382~1!2@1*0(0)0+0<#NU$LL%LL^
//    int m1=2456454;
//    char *a1 = SAHE_add_udf(cipher1,cipher2);//"333832393436313438333333393338303836377e32213140312a30283029302b303c313c234e55244c4c254c4c5e"
// //    delete []a1;
//    char *a2=SAHE_addplaintext_udf(cipher1,m1 );//"363031363731383732373138393832343734367e31213140312a30283029302b303c234e55244c4c254c4c5e"
//    char *a3 = SAHE_substract_udf(cipher1,cipher2);//"383230333937353937313033353335353731377e31213140312a31283229312b303c23303c244c4c254c4c5e"
//    char *a4 = SAHE_multiply_udf(cipher1,m1 );//"333137383030313139343930333439373137327e31213140323435363435342a30283029302b303c234e55244c4c254c4c5e"
//    char *a5 = SAHE_neggate_udf(cipher1);//"333230363635333330393636373430373531357e30213040302a31283129312b4e5523303c244c4c254c4c5e"
//    char  *cipher3="343137363032383532393735363036383935357e31213140312a30283029302b303c234e55244c4c254c4c5e";
//    char  *cipher4="383331303532373239373136323934383732337e31213240312a30283029302b303c234e55244c4c254c4c5e";
//    //4176028529756068955~1!1@1*0(0)0+0<#NU$LL%LL^
//    //8310527297162948723~1!2@1*0(0)0+0<#NU$LL%LL^
//    char *a6 = SMHE_MULTIPLY_udf(cipher3,cipher4);//"3336313438323737383931353331343233377e32213140312a30283029302b303c313c234e55244c4c254c4c5e"
//    char *a7 = SMHE_MULTIPLYPLAIN_udf(cipher3,m1 );//"3638373231323332383937373435303334327e31213140312a30283029302b303c234e55244c4c254c4c5e"
//    char *a8 = SMHE_DIVIDE_udf(cipher3,cipher4);//"373836353832373430313732323532393839367e31213140312a31283229312b303c23303c244c4c254c4c5e"
//    char *a9 = SMHE_POW_udf(cipher3,m1 );//"383333333536393832383432393239303634377e31213140323435363435342a30283029302b303c234e55244c4c254c4c5e"
//    char *a10 = SMHE_INVERSE_udf(cipher3);//"313835363731323736333331313636313839317e30213040302a31283129312b4e5523303c244c4c254c4c5e"

// }
