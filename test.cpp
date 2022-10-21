#include <string>
#include <vector>

using namespace std;

#ifdef __cplusplus
extern "C"
{
#endif

// #include "postgres.h"
// #include "encryptsql/encryptstmt.h"
// #include "encryptsql/fieldmap.h"
// #include "utils/memutils.h"
// #include "encryptsql/node2str.h"
// #include "encryptsql/parser.h"
#include <string.h>
#include "encryptsql.h"

#ifdef __cplusplus
}
#endif

vector<string> select_sqls = {
    "SELECT l_returnflag,  \
    CASE WHEN l_linestatus = 1 THEN 'one' WHEN l_quantity = 2 THEN 'two' ELSE 'other' END \
        FROM lineitem;",
    "select       l_returnflag,      l_linestatus,      sum(l_quantity) as sum_qty,      sum(l_extendedprice) as "
    "sum_base_price,      sum(l_extendedprice * (1 - l_discount)) as sum_disc_price,      sum(l_extendedprice * (1 - "
    "l_discount) * (1 + l_tax)) as sum_charge,      avg(l_quantity) as avg_qty,      avg(l_extendedprice) as "
    "avg_price,      avg(l_discount) as avg_disc,      count(*) as count_order    from      lineitem    where      "
    "l_extendedprice > 100    group by      l_returnflag,      l_linestatus    order by      l_returnflag,      "
    "l_linestatus;",
    "select      l_orderkey,      sum(l_extendedprice * (1 - l_discount)) as revenue,      o_orderdate,      "
    "o_shippriority    from      customer,      orders,      lineitem    where      c_mktsegment = 'BUILDING'      and "
    "c_custkey = o_custkey      and l_orderkey = o_orderkey      and o_totalprice > 10      and l_extendedprice > 10   "
    " group by      l_orderkey,      o_orderdate,      o_shippriority    order by      revenue desc,      o_orderdate;",
    "select      o_orderpriority,      count(*) as order_count    from      orders    where      o_totalprice > 100    "
    "  and exists (        select          *        from          lineitem        where          l_orderkey = "
    "o_orderkey          and l_extendedprice > 100      )    group by      o_orderpriority    order by      "
    "o_orderpriority;",
    "select      n_name,      sum(l_extendedprice * (1 - l_discount)) as revenue    from      customer,      orders,   "
    "   lineitem,      supplier,      nation,      region    where      c_custkey = o_custkey      and l_orderkey = "
    "o_orderkey      and l_suppkey = s_suppkey      and c_nationkey = s_nationkey      and s_nationkey = n_nationkey   "
    "   and n_regionkey = r_regionkey      and r_name = 'ASIA'      and o_orderdate >= '1994-01-01'      and "
    "o_orderdate < '1995-01-01'      and c_acctbal > 10      and s_acctbal > 20    group by      n_name    order by    "
    "  revenue desc;",
    "select      sum(l_extendedprice * l_discount) as revenue    from      lineitem    where                "
    "l_extendedprice > 100;",
    "select      supp_nation,      cust_nation,      l_year,      sum(volume) as revenue    from      (        select  "
    "        n1.n_name as supp_nation,          n2.n_name as cust_nation,          DATE_PART('YEAR',l_shipdate) as "
    "l_year,          l_extendedprice * (1 - l_discount) as volume        from          supplier,          lineitem,   "
    "       orders,          customer,          nation n1,          nation n2        where          s_suppkey = "
    "l_suppkey          and o_orderkey = l_orderkey          and c_custkey = o_custkey          and s_nationkey = "
    "n1.n_nationkey          and c_nationkey = n2.n_nationkey          and (            (n1.n_name = 'FRANCE' and "
    "n2.n_name = 'GERMANY')            or (n1.n_name = 'GERMANY' and n2.n_name = 'FRANCE')          )          and "
    "l_shipdate between '1995-01-01' and '1996-12-31'          and o_totalprice > 100          and c_acctbal > 10      "
    ") as shipping    group by      supp_nation,      cust_nation,      l_year    order by      supp_nation,      "
    "cust_nation,      l_year;",
    "select      o_year,      sum(case        when nation = 'BRAZIL' then volume        else 0      end) / sum(volume) "
    "as mkt_share    from      (        select          DATE_PART('YEAR',o_orderdate) as o_year,          "
    "l_extendedprice * (1 - l_discount) as volume,          n2.n_name as nation        from          part,          "
    "supplier,          lineitem,          orders,          customer,          nation n1,          nation n2,          "
    "region        where          p_partkey = l_partkey          and s_suppkey = l_suppkey          and l_orderkey = "
    "o_orderkey          and o_custkey = c_custkey          and c_nationkey = n1.n_nationkey          and "
    "n1.n_regionkey = r_regionkey          and r_name = 'AMERICA'          and s_nationkey = n2.n_nationkey          "
    "and o_orderdate between '1995-01-01' and '1996-12-31'          and p_type = 'ECONOMY ANODIZED STEEL'          and "
    "s_acctbal > 10          and l_extendedprice > 100      ) as all_nations    group by      o_year    order by      "
    "o_year; ",
    "select      n_name,      o_year,      sum(amount) as sum_profit    from      (        select          n_name,     "
    "     DATE_PART('YEAR',o_orderdate) as o_year,          l_extendedprice * (1 - l_discount) - ps_supplycost * "
    "l_quantity as amount        from          part,          supplier,          lineitem,          partsupp,          "
    "orders,          nation        where          s_suppkey = l_suppkey          and ps_suppkey = l_suppkey          "
    "and ps_partkey = l_partkey          and p_partkey = l_partkey          and o_orderkey = l_orderkey          and "
    "s_nationkey = n_nationkey          and p_name like '%green%'          and s_acctbal > 10          and "
    "ps_supplycost > 100      ) as profit    group by      n_name,      o_year    order by      n_name,      o_year "
    "desc; ",
    "select      c_custkey,      c_name,      sum(l_extendedprice * (1 - l_discount)) as revenue,      c_acctbal,      "
    "n_name,      c_address,      c_phone,      c_comment    from      customer,      orders,      lineitem,      "
    "nation    where      c_custkey = o_custkey      and l_orderkey = o_orderkey      and o_orderdate >= '1993-10-01'  "
    "    and o_orderdate < '1994-01-01'      and c_nationkey = n_nationkey      and c_acctbal > 10      and "
    "l_extendedprice > 100    group by      c_custkey,      c_name,      c_acctbal,      c_phone,      n_name,      "
    "c_address,      c_comment    order by      revenue desc;",
    "select      ps_partkey,      sum(ps_supplycost * ps_availqty) as value    from      partsupp,      supplier,      "
    "nation    where      ps_suppkey = s_suppkey      and s_nationkey = n_nationkey      and n_name = 'GERMANY'      "
    "and ps_supplycost > 20      and s_acctbal > 10    group by      ps_partkey having        sum(ps_supplycost * "
    "ps_availqty) > (          select            sum(ps_supplycost * ps_availqty) * 0.0001000000          from         "
    "   partsupp,            supplier,            nation          where            ps_suppkey = s_suppkey            "
    "and s_nationkey = n_nationkey            and n_name = 'GERMANY'        )    order by      value desc;",
};
vector<string> create_sqls = {

    "CREATE TABLE SD.REGION(R_REGIONKEY INT NOT NULL,R_NAME CHAR(25) NOT NULL,R_COMMENT VARCHAR(152));",
    "CREATE TABLE REGION(R_REGIONKEY INT NOT NULL,R_NAME CHAR(25) NOT NULL,R_COMMENT VARCHAR(152));",

    "CREATE TABLE NATION(           \
            N_NATIONKEY INT NOT NULL,   \
            N_NAME CHAR(25) NOT NULL,   \
            N_REGIONKEY INT NOT NULL,   \
            N_COMMENT VARCHAR(152));",

    /*
    CREATE TABLE SUPPLIER(
            S_SUPPKEY BIGINT NOT NULL,
            S_NAME CHAR(25) NOT NULL,
            S_ADDRESS VARCHAR(40) NOT NULL,
            S_NATIONKEY INT NOT NULL,
            S_PHONE CHAR(15) NOT NULL,
            S_ACCTBAL DECIMAL(15, 2) NOT NULL,
            S_COMMENT VARCHAR(101) NOT NULL);
    */

    "CREATE TABLE SUPPLIER(                 \
            S_SUPPKEY BIGINT NOT NULL,          \
            S_NAME CHAR(25) NOT NULL,           \
            S_ADDRESS VARCHAR(40) NOT NULL,     \
            S_NATIONKEY INT NOT NULL,           \
            S_PHONE CHAR(15) NOT NULL,          \
            S_ACCTBAL DECIMAL(15, 2) NOT NULL,  \
            S_COMMENT VARCHAR(101) NOT NULL);",

    /*
    CREATE TABLE CUSTOMER(
            C_CUSTKEY BIGINT NOT NULL,
            C_MKTSEGMENT CHAR(10) NOT NULL,
            C_NATIONKEY INT NOT NULL,
            C_NAME VARCHAR(25) NOT NULL,
            C_ADDRESS VARCHAR(40) NOT NULL,
            C_PHONE CHAR(15) NOT NULL,
            C_ACCTBAL DECIMAL(15, 2) NOT NULL,
            C_COMMENT VARCHAR(117) NOT NULL);

INSERT INTO customer VALUES (1, 'BUILDING', 8, 'Customer#000000001', 'KwX3hMHjZ6', '937-241-3198', 3560, 'ironic excuses
detect slyly silent requests. requests according to the exc'),  (2, 'AOUSU', 5, 'Customer#000000002', 'OZUXOCH','9327-2141-13198', 7560, 'ajisd jaiosjdioz hu ia byq');


    create table stu(id int not null primary key);
    insert into stu values(258),(299),(301),(4747);
    select * from stu;

            select C_CUSTKEY, C_COMMENT from customer;
            select C_CUSTKEY, C_COMMENT, C_ADDRESS, C_PHONE from customer;

    */

    "CREATE TABLE CUSTOMER(                 \
            C_CUSTKEY BIGINT NOT NULL,          \
            C_MKTSEGMENT CHAR(10) NOT NULL,     \
            C_NATIONKEY INT NOT NULL,           \
            C_NAME VARCHAR(25) NOT NULL,        \
            C_ADDRESS VARCHAR(40) NOT NULL,     \
            C_PHONE CHAR(15) NOT NULL,          \
            C_ACCTBAL DECIMAL(15, 2) NOT NULL,  \
            C_COMMENT VARCHAR(117) NOT NULL);",

    "CREATE TABLE PART(                     \
            P_PARTKEY BIGINT NOT NULL,          \
            P_NAME VARCHAR(55) NOT NULL,        \
            P_MFGR CHAR(25) NOT NULL,           \
            P_BRAND CHAR(10) NOT NULL,          \
            P_TYPE VARCHAR(25) NOT NULL,        \
            P_SIZE BIGINT NOT NULL,             \
            P_CONTAINER CHAR(10) NOT NULL,      \
            P_RETAILPRICE DECIMAL(15, 2) NOT NULL,  \
            P_COMMENT VARCHAR(23) NOT NULL);",

    "CREATE TABLE PARTSUPP(                     \
            PS_PARTKEY BIGINT NOT NULL,             \
            PS_SUPPKEY BIGINT NOT NULL,             \
            PS_AVAILQTY BIGINT NOT NULL,            \
            PS_SUPPLYCOST DECIMAL(15, 2) NOT NULL,  \
            PS_COMMENT VARCHAR(199) NOT NULL);",

    "CREATE TABLE ORDERS(                       \
            O_ORDERKEY BIGINT NOT NULL,             \
            O_CUSTKEY BIGINT NOT NULL,              \
            O_ORDERSTATUS CHAR(1) NOT NULL,         \
            O_TOTALPRICE DECIMAL(15, 2) NOT NULL,   \
            O_ORDERDATE DATE NOT NULL,              \
            O_ORDERPRIORITY CHAR(15) NOT NULL,      \
            O_CLERK CHAR(15) NOT NULL,              \
            O_SHIPPRIORITY BIGINT NOT NULL,         \
            O_COMMENT VARCHAR(79) NOT NULL);",

    "CREATE TABLE LINEITEM(                         \
            L_ORDERKEY BIGINT NOT NULL,                 \
            L_PARTKEY BIGINT NOT NULL,                  \
            L_SUPPKEY BIGINT NOT NULL,                  \
            L_LINENUMBER BIGINT NOT NULL,               \
            L_QUANTITY DECIMAL(15, 2) NOT NULL,         \
            L_EXTENDEDPRICE DECIMAL(15, 2) NOT NULL,    \
            L_DISCOUNT DECIMAL(15, 2) NOT NULL,         \
            L_TAX DECIMAL(15, 2) NOT NULL,              \
            L_RETURNFLAG CHAR(1) NOT NULL,              \
            L_LINESTATUS CHAR(1) NOT NULL,              \
            L_SHIPDATE DATE NOT NULL,                   \
            L_COMMITDATE DATE NOT NULL,                 \
            L_RECEIPTDATE DATE NOT NULL,                \
            L_SHIPINSTRUCT CHAR(25) NOT NULL,           \
            L_SHIPMODE CHAR(10) NOT NULL,               \
            L_COMMENT VARCHAR(44) NOT NULL);"};
vector<string> insert_sqls = {
    "INSERT INTO customer VALUES (1, 'BUILDING', 8, 'Customer#000000001', 'KwX3hMHjZ6', '937-241-3198', 3560, 'ironic excuses detect slyly silent requests. requests according to the exc'), \
    (2, 'AOUSU', 5, 'Customer#000000002', 'OZUXOCH', '9327-2141-13198', 7560, 'ajisd jaiosjdioz hu ia byq');",
    "INSERT INTO customer (N_NATIONKEY, N_NAME) VALUES (1, 2), (4,5)"};
vector<string> delete_sqls = {"DELETE FROM customer WHERE L_ORDERKEY = 2;"};
vector<string> update_sqls = {"update customer c set C_CUSTKEY = line.L_ORDERKEY from LINEITEM line where c.C_CUSTKEY "
                              "= line.L_ORDERKEY and c.S_NAME = 'a' "};

vector<string> select_sql = {
    "select      supp_nation,      cust_nation,      l_year,      sum(volume) as revenue    from      (        select  "
    "        n1.n_name as supp_nation,          n2.n_name as cust_nation,          DATE_PART('YEAR',l_shipdate) as "
    "l_year,          l_extendedprice * (1 - l_discount) as volume        from          supplier,          lineitem,   "
    "       orders,          customer,          nation n1,          nation n2        where          s_suppkey = "
    "l_suppkey          and o_orderkey = l_orderkey          and c_custkey = o_custkey          and s_nationkey = "
    "n1.n_nationkey          and c_nationkey = n2.n_nationkey          and (            (n1.n_name = 'FRANCE' and "
    "n2.n_name = 'GERMANY')            or (n1.n_name = 'GERMANY' and n2.n_name = 'FRANCE')          )          and "
    "l_shipdate between '1995-01-01' and '1996-12-31'          and o_totalprice > 100          and c_acctbal > 10      "
    ") as shipping    group by      supp_nation,      cust_nation,      l_year    order by      supp_nation,      "
    "cust_nation,      l_year;"};
auto sqls = select_sql;

const char *encryptOneSql(const char *sql) __attribute__((weak));

void testBufConversion();

// int main()
// {
//     const char* ans = "";
//     //     mapperInit("./map.json");
//     int i = 0;
//     testBufConversion();

//     for (auto sql : sqls) {
//         break;
//         i++;
//         // const char* sql= sqls[i];
//
//         //
//         char* c_sql = (char*)malloc(sql.size() + 1);
//         strcpy(c_sql, sql.c_str());
//         //     List *raw_parsetree_list = raw_parser(c_sql);
//         //     Node *tree = (Node *)linitial(raw_parsetree_list);
//         //     tree = encryptStmt((Node *)tree);
//         // char* ans = parseTree2str((Node*)linitial(raw_parsetree_list));
//         printf("good\n");
//         if (encryptOneSql)
//             ans = encryptOneSql(c_sql);
//         else {
//             // encrypt \n";
//         }

//         printf("%s\n", ans);
//         // << " is done\n\n";
//     }
//     //     mapperDestory();
// }

//////////////////////////////
/// Speed test
/////////////////////////////

//
// #include <string>
// #include <sys/time.h>
// #include <sstream>
// #include <stdio.h>
// using namespace std;
// #define OUT_IN_REPEATE_NUM 10000
// #define IN_REPEATE_NUM 60

// string s1 = "abcedfg";
// string s2 = "hijklmn";
// string s3 = "opqrst";

// void test1() {
//         string s;
//         for (size_t i = 0; i < IN_REPEATE_NUM; i++)
//         {
//                 char *tmp = (char*)malloc(s1.size() + s2.size() + s3.size());
//                 strcpy(tmp, s1.c_str());
//                 strcpy(tmp, s2.c_str());
//                 strcpy(tmp, s3.c_str());
//         }
// }

// void test2()
// {
//         string s;
//         for (size_t i = 0; i < IN_REPEATE_NUM; i++)
//         {
//                 s += s1;
//                 s += s2;
//                 s += s3;
//         }
// }

// int main()
// {
//         string ss, plus, append, sprintf;
//         struct timeval sTime, eTime;
//         gettimeofday(&sTime, NULL);
//         for (int i = 0; i < OUT_IN_REPEATE_NUM; i++)
//         {
//                 sprintf = "";
//                 test1();
//         }
//         gettimeofday(&eTime, NULL);
//         long SprintfTime = (eTime.tv_sec - sTime.tv_sec) * 1000000 + (eTime.tv_usec - sTime.tv_usec); //exeTime
//         单位是微秒 gettimeofday(&sTime, NULL); for (int i = 0; i < OUT_IN_REPEATE_NUM; i++)
//         {
//                 append = "";
//                 test2();
//         }
//         gettimeofday(&eTime, NULL);
//         long AppendTime = (eTime.tv_sec - sTime.tv_sec) * 1000000 + (eTime.tv_usec - sTime.tv_usec); //exeTime
//         单位是微秒

//         // << endl
//              << AppendTime << endl;
// }