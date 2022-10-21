## Existing Problems

----

1. [O] SELECT pg_backend_pid();  This statement adds AES encryption by default, but returns pid results without encryption.

   Added to the filter

2. [X] pow(col, 0); The resulting ciphertext returned by it cannot be decrypted

3. [X] insert into stu2(id, fl, name) select id, fl, name from stu;  Does not support

4. [X] update problem

5. [O] ~~max min Intermediate state can only be ORE or AES, pure software can not solve, because the final state is ORE can not decrypt, AES can only use ORE~~

   Passing ORE, AES two ciphertexts, the return result is AES form

6. [X] SELECT COUNT(DISTINCT (s_i_id)) No translation DISTINCT

7. [O] UDFs are used for full table scans without being considered by the optimizer, and should actually be executed last

   It is now found that the cost of UDFs can be directly defined

8. [O] A floating-point number, represented as a long on the server side, so the server does not know whether it is a floating-point number or not, and can not determine whether to process the result of the operation

   The ciphertext of SMHE is appended with one bit at the end to indicate whether it is a floating point number.

   [X] select * from test where fl = 20;  float fails when used as a where condition

9. [X] select 1/3 from test;  Segmentation fault

10. In [X] homomorphic addition, if one of the float operands is scaled and the other is not, then the non-scaled operands need to be scaled, which requires communication with the remote end

11. [X] update set id = id + id,fl = fl * fl, float column result error, int column does not, unknown reason

12. [X] select id + id + id from test;  Two consecutive operations cause translation errors

## TODO:

------

1. [X] Add Log module

   The original log code needs to be replaced, and the log information inside the Enclave needs to be printed through ocall

2.  [X] Add remote attestation module
