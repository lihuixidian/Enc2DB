

#include <libpq-fe.h>
#include <iostream>
#include <stdlib.h>
#include <functional>
#include <unistd.h>
#include <chrono>
#include <vector>
#include <string>

using namespace std;

// 如果要连接需要提供这三个函数

bool CREATE_SQL = 1;
bool INSERT_SQL = 1;
bool SELECT_SQL = 1;
bool UPDATE_SQL = 0;
bool DROP_SQL = 0;
bool TEST_SQL = 0;

bool SHOW_RES = 0;

int g_times = 1;

using namespace std;

PGconn *gconn = nullptr;

void drop_sql(PGconn *conn);
PGconn *connect();
void processResult(PGresult *res, bool isShow);

void exit_nicely(PGconn *conn)
{
    // drop_sql(conn);
    PQfinish(conn);
    abort();
}

void printPGresult(PGresult *res)
{
    std::cout << PQntuples(res) << "tuples, " << PQnfields(res) << " fields"
              << std::endl;
    // print column name
    for (int i = 0; i < PQnfields(res); i++)
    {
        std::cout << PQfname(res, i) << "\t";
    }
    std::cout << std::endl;
    // print column values
    for (int i = 0; i < PQntuples(res); i++)
    {
        for (int j = 0; j < PQnfields(res); j++)
        {
            std::cout << PQgetvalue(res, i, j) << "\t";
        }
        std::cout << std::endl;
    }
}

void create_sql(PGconn *conn)
{
    PGresult *res;
    char buf[128];
    sprintf(buf, "create table if not exists stu (id int , fl float, name VARCHAR(30), t timestamp );");
    res = PQexec(conn, buf); //为查询语句声明一个游标
    processResult(res, SHOW_RES);
}

void insert_sql(PGconn *conn, int times)
{
    PGresult *res;
    char buf[1024];
    while (times--)
    {
        sprintf(buf, "INSERT into stu VALUES (%d, %lf, '%s', now());",
                times , times * 3.1415926, "helloWWWWorld");

        res = PQexec(conn, buf); //为查询语句声明一个游标
        processResult(res, SHOW_RES);
    }
}

void select_sql(PGconn *conn, int ptime)
{
    PGresult *res;
    char buf[1024];
    int times = ptime;
    while (times-- >= 1)
    {
        // //sleep(0.5);
        // he and me
        sprintf(buf, "select id + %d, fl * %d from stu ",
                times, times);

        res = PQexec(conn, buf); //为查询语句声明一个游标
        processResult(res, SHOW_RES);

        sprintf(buf, "select t from stu where id > %d and fl < %d",
                ptime / 2, ptime * 6);

        res = PQexec(conn, buf); //为查询语句声明一个游标
        processResult(res, SHOW_RES);
    }
    
    // cout << buf << endl;
}

void processResult(PGresult *res, bool isShow)
{
    int ret = PQresultStatus(res);
    if (PQresultStatus(res) != PGRES_TUPLES_OK && PQresultStatus(res) != PGRES_COMMAND_OK) //判断游标生成是否成功
    {
        fprintf(stderr, "DECLARE CURSOR failed: %s\n", PQerrorMessage(gconn));
        PQclear(res);
        exit_nicely(gconn);
    }
    if (isShow)
        printPGresult(res);
    PQclear(res);
}

void test_sql(PGconn *conn, int times)
{
    PGresult *res;
    char buf[1024];

    vector<string> select_sqls = {
        // 聚集函数
        // "select min(id) from stu",
        // "select max(id) from stu",
        // "select sum(id) from stu"

        "SELECT COUNT( (s_i_id))                    \
                        FROM order_line1, stock1    \
                       WHERE ol_w_id = 1            \
                         AND ol_d_id = 4            \
                         AND ol_o_id < 3011         \
                         AND ol_o_id >= 2991        \
                         AND s_w_id= 1              \
                         AND s_i_id=ol_i_id         \
                         AND s_quantity < 16;"};
    for (auto &sql : select_sqls)
    {
        res = PQexec(conn, sql.c_str()); //为查询语句声明一个游标
        processResult(res, 1);
    }
}

void update_sql(PGconn *conn, int times)
{
    PGresult *res;
    char buf[1024];

    sprintf(buf, "update stu set id = id + id, fl = fl* fl;");

    res = PQexec(conn, buf); //为查询语句声明一个游标
    processResult(res, SHOW_RES);
}

void drop_sql(PGconn *conn)
{
    char buf[1024];
    PGresult *res;
    // he
    sprintf(buf, "drop table stu;");
    if (PQstatus(conn) != CONNECTION_OK)
    {
        PQfinish(conn);
        conn = connect();
    }

    res = PQexec(conn, buf); //为查询语句声明一个游标
    PQclear(res);
}

PGconn *connect()
{
    const char *conninfo;
    PGconn *conn;
    // conninfo = "host=127.0.0.1 port=5432 user=postgres";
    conninfo = "host=127.0.0.1 port=5432 user=zheng dbname=encrypt";
    conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Connection to database failed: %s",
                PQerrorMessage(conn));
        sleep(1);
        exit_nicely(conn); //关闭连接并执行清理操作
    }
    gconn = conn;
    char buf[] = "select pg_backend_pid();";

    auto res = (PQexec(conn, buf));
    processResult(res, SHOW_RES);
    if(SHOW_RES) {
        getchar();
    }

    return conn;
}

class M
{
public:
    M() : _conn(connect())
    {
    }
    ~M()
    {
        // drop_sql(_conn);
        PQfinish(_conn);
    }
    void run(int times)
    {
        // PQexec(_conn, "BEGIN");  //为查询语句声明一个游标
        
        if (CREATE_SQL)
        {
            auto start = std::chrono::system_clock::now();
            create_sql(_conn);
            auto end = std::chrono::system_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start);
            cout << "create done...running for " << elapsed.count() << " seconds " << endl;
        }
        if (INSERT_SQL)
        {
            auto start = std::chrono::system_clock::now();
            insert_sql(_conn, times);
            auto end = std::chrono::system_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start);
            cout << "insert done...running for " << elapsed.count() << " seconds " << endl;
        }
        // PQexec(_conn, "COMMIT"); //为查询语句声明一个游标
        // PQexec(_conn, "BEGIN");
        if (SELECT_SQL)
        {
            select_sql(_conn, 1);  // for enclave init 
            auto start = std::chrono::system_clock::now();
            select_sql(_conn, times);
            auto end = std::chrono::system_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start);
            cout << "select done...running for " << elapsed.count() << " seconds " << endl;
        }
        // PQexec(_conn, "COMMIT");
        if (TEST_SQL)
        {
            test_sql(_conn, times);
            cout << "test done\n";
        }

        if (UPDATE_SQL)
        {
            auto start = std::chrono::system_clock::now();
            update_sql(_conn, times);
            auto end = std::chrono::system_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start);
            cout << "update done...running for " << elapsed.count() << " seconds " << endl;
        }

        if(DROP_SQL) {
            drop_sql(_conn);
        }
        // PQexec(_conn, "COMMIT"); //为查询语句声明一个游标
    }

private:
    PGconn *_conn;
};

int main(int argc, char **argv)
{
    if (argc < 7)
    {
        printf("usage: ./sqltest [running times] [iscreate] [isinsert] [isselect] [isupdate] [is show res] -- 1 for yes 0 for no\n");
        exit(0);
    }
    CREATE_SQL = "1" == string(argv[2]);

    DROP_SQL = "-1" == string(argv[2]);

    INSERT_SQL = "1" == string(argv[3]);
    SELECT_SQL = "1" == string(argv[4]);
    UPDATE_SQL = "1" == string(argv[5]);
    


    SHOW_RES = "1" == string(argv[6]);
    int times = atoi(argv[1]);
    M m;

    m.run(times);
    // drop_sql(conn);
}