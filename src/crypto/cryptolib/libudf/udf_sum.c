// #include "stdio.h"
// #include "Interface.h"
// #include <string.h>
// #include "postgres.h"
// #include "fmgr.h"
// #include "utils/geo_decls.h"

// #ifdef PG_MODULE_MAGIC
// PG_MODULE_MAGIC;
// #endif

/*
typedef struct
{
    char *sum;
} state;

PG_FUNCTION_INFO_V1(sum_sahe);

Datum sum_sahe(PG_FUNCTION_ARGS)
{
    state *my_state = (state *)PG_GETARG_POINTER(0);
    text *arg1 = PG_GETARG_TEXT_P(1);
    //text  *arg2 = PG_GETARG_TEXT_P(1);
    my_state->sum = SAHE_add_udf(my_state->sum, arg1->vl_dat);
    text *destination = (text *)palloc(VARHDRSZ + strlen(my_state->sum));
    SET_VARSIZE(destination, VARHDRSZ + strlen(my_state->sum));
    memcpy(destination->vl_dat, my_state->sum, strlen(my_state->sum));
    PG_RETURN_POINTER(my_state);
}
*/

/*
PG_FUNCTION_INFO_V1(sum_sahe);

Datum sum_sahe(PG_FUNCTION_ARGS)
{

    // state value can safely be modified in-place in transition functions
    text *sum = PG_GETARG_TEXT_P(0);
    text *arg = PG_GETARG_TEXT_P(1);

    char *buffer = SAHE_add_udf(sum->vl_dat, arg->vl_dat);
    // if (sum != NULL)
    // {
    //     if (strlen(sum->vl_dat) == 0)
    //     {
    //         sum = (text *)palloc(VARHDRSZ + strlen(buffer));
    //     }
    // }
    // SET_VARSIZE(sum, VARHDRSZ + strlen(buffer));
    // memcpy(sum->vl_dat, buffer, strlen(buffer));
    text *destination = (text *) palloc(VARHDRSZ + strlen(buffer));
    SET_VARSIZE(destination, VARHDRSZ + strlen(buffer));
    memcpy(destination->vl_dat, buffer, strlen(buffer));

    PG_RETURN_TEXT_P(destination);
}
*/

// 已经成功的尝试

/*
PG_FUNCTION_INFO_V1(sum_sahe);

Datum sum_sahe(PG_FUNCTION_ARGS) {

	// // state value can safely be modified in-place in transition functions
	// text * sum = PG_GETARG_TEXT_P(0);
	// // float8 * internal_values = (float8 *) ARR_DATA_PTR(internal);
    // text * plus_val     = PG_GETARG_TEXT_P(1);

    // char * internal_sum = SAHE_add_udf(sum->vl_dat, plus_val->vl_dat);

    
    // memcpy(sum->vl_dat, internal_sum, strlen(internal_sum));

	// PG_RETURN_TEXT_P(sum);
    // const char * x = "383431343438353432393834363437383231397e31213140312a30283029302b303c234e55244c4c254c4c5e";

    text  * sum = PG_GETARG_TEXT_P(0);


    // if (strlen(sum->vl_dat) == 0)
    // {
    //     // sum = new char[strlen(x) + 1];
    //     // strcpy(sum->vl_dat, x);
    //     memcpy(sum->vl_dat, x, strlen(x));
    //     // sum->vl_dat = x;
    // }
    

    text  * plus_val = PG_GETARG_TEXT_P(1);
    char  * interval_sum = SAHE_add_udf(sum->vl_dat,plus_val->vl_dat); 
    
    sum = (text *) palloc(VARHDRSZ + strlen(interval_sum));
    SET_VARSIZE(sum, VARHDRSZ + strlen(interval_sum));
    memcpy(sum->vl_dat, interval_sum, strlen(interval_sum));

    // text *destination = (text *) palloc(VARHDRSZ + strlen(buffer));
    // SET_VARSIZE(destination, VARHDRSZ + strlen(buffer));
    // memcpy(destination->vl_dat, buffer, strlen(buffer));

    PG_RETURN_TEXT_P(sum);



}
*/

/*
PG_FUNCTION_INFO_V1(sum_sahe_sfunc);

// 其实是把该列所有值进行了一次运算
Datum sum_sahe_sfunc(PG_FUNCTION_ARGS) {

	// state value can safely be modified in-place in transition functions
	ArrayType * internal = PG_GETARG_ARRAYTYPE_P(0);
//	float8 * internal_values = check_float8_array(internal, "entropy_accum", 2);
	text * internal_values = (text *) ARR_DATA_PTR(internal);
	text * args = PG_GETARG_TEXT_P(1);
	char * internal_sum = SAHE_add_udf(internal_values[0].vl_dat, args->vl_dat);
    
    internal_values[0] = cstring_to_text(internal_sum);


	PG_RETURN_ARRAYTYPE_P(internal);
}

PG_FUNCTION_INFO_V1(sum_sahe_final);

Datum sum_sahe_final(PG_FUNCTION_ARGS) {

	text * internal_values = (text *) ARR_DATA_PTR(PG_GETARG_ARRAYTYPE_P(0));
	PG_RETURN_TEXT_P(internal_values[0]);
}
*/

