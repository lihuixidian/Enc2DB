

// #include "KeyManager.h"
// #include "Singleton.h"
// #include "SymAHE.h"
// #include "SymMHE.h"



// extern "C"
// {
//     void *CurrentMemoryContext;
//     void *pg_detoast_datum;
//     void *MemoryContextAlloc;

//     void elog_start(const char *filename, int lineno, const char *funcname)
//     {
//     }
//     int errmsg_internal(const char *fmt, ...)
//     {
//         fprintf(stderr, "ERROR: %s\n", fmt);
//         return 0; /* return value does not matter */
//     }
//     int errcode(int sqlerrcode)
//     {
//         return 0; /* return value does not matter */
//     }

//     void elog_finish(int elevel, const char *fmt, ...)
//     {
//         fprintf(stderr, "ERROR: %s\n", fmt);
//         //  exit(1);
//     }

//     bool errstart(int elevel, const char *filename, int lineno,
//                   const char *funcname, const char *domain)
//     {
//         return true;
//     }

//     void errfinish(int dummy, ...)
//     {
//         // proc_exit(1);
//     }

//     int errhint(const char *fmt, ...)
//     {
//         fprintf(stderr, "HINT: %s\n", fmt);
//         return 0; /* return value does not matter */
//     }

//     int errdetail(const char *fmt, ...)
//     {
//         fprintf(stderr, "DETAIL: %s\n", fmt);
//         return 0; /* return value does not matter */
//     }

//     int errmsg(const char *fmt, ...)
//     {
//         fprintf(stderr, "ERR: %s\n", fmt);
//         return 0; /* return value does not matter */
//     }

// #include "udfDispatch.h"
// }

// int main() {
//     int n = 10;
//     SymAHE ahe;
//     SymMHE mhe;
//     for (int i = 1, j = n; i <= n; i++, j--)
//     {
//         long long Mres = i * j;
//         long long Ares = i + j;

//         char *Acipher1 =  ahe.encrypt(i);
//         char *Acipher2 = ahe.encrypt(j);

//         char *caRes = saheAdd(Acipher1,Acipher2);
//         long long aRes = ahe.decrypt(caRes);

//         assert(aRes == Ares);

//         char *Mcipher1 = mhe.encrypt(i);
//         char *Mcipher2 = mhe.encrypt(j);

//         char *cmRes = smheMul(Mcipher1, Mcipher2);
//         long long mRes = mhe.decrypt(cmRes);

//         assert(mRes == Mres);

//         delete[] Acipher1;
//         delete[] Acipher2;
//         delete[] caRes;

//         delete[] cmRes;
//         delete[] Mcipher1;
//         delete[] Mcipher2;
//     }
// }