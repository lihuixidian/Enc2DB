-- 登录数据库,切换postgres用户,输入创建用户自定函数语句.用下面的命令为PostgreSQL定义这些函数:
-- 创建的函数名为sahe_add(text, text, text),该函数输入两个text类型的数据,返回类型为text的计算结果.
-- 其余函数#sahe_addplain,sahe_substract,sahe_multiply,sahe_negate,smhe_multiply,smhe_multiplyplain,
-- smhe_divide,smhe_pow,smhe_inverse,以此类推.
--  $libdir的实际目录位置可以用pg_config --pkglibdir命令找到。

CREATE OR REPLACE FUNCTION sahe_add(text, text, text, text) RETURNS text
    AS '$libdir/libudf', 'sahe_add'
COST 500
LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION sahe_addplain(text, INTEGER) RETURNS text
    AS '$libdir/libudf', 'sahe_addplain'
COST 500
LANGUAGE C STRICT;


CREATE OR REPLACE FUNCTION sahe_substract(text, text, text, text) RETURNS text
    AS '$libdir/libudf', 'sahe_substract'
COST 500
LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION sahe_multiply(text, INTEGER) RETURNS text
    AS '$libdir/libudf', 'sahe_multiply'
COST 500
LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION sahe_negate(text) RETURNS text
    AS '$libdir/libudf', 'sahe_negate'
COST 500
LANGUAGE C STRICT;


CREATE OR REPLACE FUNCTION smhe_multiply(text, text, text, text) RETURNS text
    AS '$libdir/libudf', 'smhe_multiply'
COST 500
LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION smhe_multiplyplain(text, INTEGER) RETURNS text
    AS '$libdir/libudf', 'smhe_multiplyplain'
COST 500
LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION smhe_divide(text, text, text, text) RETURNS text
AS '$libdir/libudf', 'smhe_divide'
COST 500
LANGUAGE C STRICT;


CREATE OR REPLACE FUNCTION smhe_pow(text, text, INTEGER) RETURNS text
    AS '$libdir/libudf', 'smhe_pow'
COST 500
LANGUAGE C STRICT;

-- CREATE FUNCTION smhe_inverse(text) RETURNS text
--     AS '$libdir/libudf', 'smhe_inverse'
-- COST 500
-- LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION udf_ore_lt(text, text, text, text) RETURNS BOOLEAN
    AS '$libdir/libudf', 'udf_ore_lt'
COST 500
LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION udf_ore_le(text, text, text, text) RETURNS BOOLEAN
    AS '$libdir/libudf', 'udf_ore_le'
COST 500
LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION udf_ore_gt(text, text, text, text) RETURNS BOOLEAN
    IMMUTABLE
    AS '$libdir/libudf', 'udf_ore_gt'
COST 500
LANGUAGE C STRICT;


CREATE OR REPLACE FUNCTION udf_ore_ge(text, text, text, text) RETURNS BOOLEAN
    AS '$libdir/libudf', 'udf_ore_ge'
COST 500
LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION udf_ore_ee(text, text, text, text) RETURNS BOOLEAN
    AS '$libdir/libudf', 'udf_ore_ee'
COST 500
LANGUAGE C STRICT;


-- 创建聚集函数
DROP AGGREGATE IF EXISTS sahe_sum(text, text);
DROP AGGREGATE IF EXISTS ore_max(text, text);
DROP AGGREGATE IF EXISTS ore_min(text, text);


CREATE OR REPLACE FUNCTION sum_sahe_sfunc(text, text, text) RETURNS text
AS '$libdir/libudf', 'sum_sahe_sfunc'
COST 500
LANGUAGE C IMMUTABLE STRICT;


CREATE AGGREGATE sahe_sum(text, text)
(
sfunc = sum_sahe_sfunc,
stype = text,
initcond = ''
);

-- 创建MAX, MIN函数ore_max,ore_min
CREATE OR REPLACE FUNCTION ore_max_sfunc(text, text, text) RETURNS text
AS '$libdir/libudf', 'ore_max_sfunc'
COST 500
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION ore_max_final(text) RETURNS text
AS '$libdir/libudf', 'ore_max_final'
COST 500
LANGUAGE C IMMUTABLE STRICT;



CREATE AGGREGATE ore_max(text,text)
(
sfunc = ore_max_sfunc,
finalfunc = ore_max_final,
stype = text,
initcond = ''
);

CREATE OR REPLACE FUNCTION ore_min_sfunc(text, text, text) RETURNS text
AS '$libdir/libudf', 'ore_min_sfunc'
COST 500
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION ore_min_final(text) RETURNS text
AS '$libdir/libudf', 'ore_min_final'
COST 500
LANGUAGE C IMMUTABLE STRICT;


CREATE AGGREGATE ore_min(text,text)
(
sfunc = ore_min_sfunc,
finalfunc = ore_min_final,
stype = text,
initcond = ''
);