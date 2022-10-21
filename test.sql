
SELECT COUNT(DISTINCT (s_i_id))
                        FROM order_line1, stock1
                       WHERE ol_w_id = 1
                         AND ol_d_id = 4
                         AND ol_o_id < 3011
                         AND ol_o_id >= 2991
                         AND s_w_id= 1
                         AND s_i_id=ol_i_id
                         AND s_quantity < 16;

drop table test;
create table test(id int , fl float, name VARCHAR(30), t timestamp );

-- Float
INSERT into test VALUES (1,10.0, 'lz1', now());
INSERT into test VALUES (2,20, 'lz1', now());

update test set id = id + id, fl = fl*fl;
UPDATE test SET 
    id = id + 10;

-- test smhe update
UPDATE test SET 
    fl = fl * 20;


select fl  from test; -- show be 10
select id+2,fl * 10.1 from test; -- show be 101

INSERT into test VALUES (2, 2.333, 'lz2');
INSERT into test VALUES (3, -10.0, 'lz2');
INSERT into test VALUES (4, 1.0, 'lz2');
INSERT into test VALUES (8,10.0, 'lz1', now());
INSERT into test VALUES (7, 2.333, 'lz2');
INSERT into test VALUES (6, -10.0, 'lz2');
INSERT into test VALUES (5, 1.0, 'lz2');
select min(id) from test where id < 5;


-- AES Length
INSERT into test VALUES (1,0, 'lz1');
INSERT into test VALUES (2, 0,'lz2');
INSERT into test VALUES (18, 0,'lz3lz3lz3lz3lz3lz3');  
INSERT into test VALUES (37, 0,'lz3lz3lz3lz3lz3lz3lz3lz3lz3lz3lz3lz3l');  


-- insert into select
create table test2(id int , fl numeric, name VARCHAR(30), t timestamp );
insert into test2(id, fl, name) select id, fl, name from test; 

-- star query
select * from test;

-- he query
select id + 1  from test;
select id - 1  from test;
select -id from test;


-- me query
select id * 22  from test;
select id / 12  from test;
select -id from test;


-- update query

-- ! Not Support, because other field like AES can not be updated by this SQL 
update test set id = id + 13 where id = 2;
会被翻译成：
update E(test) set E_SAHE(id) = sahe_add(E_SAHE(id), E_SAHE(13)), E_AES(id) = sahe_add(E_SAHE(id), E_SAHE(13))  where id = 2;

create table IF NOT EXISTS test (
	o_id serial not null, 
	o_d_id float not null, 
	o_c_id int UNIQUE,
	o_entry_d timestamp,
	o_s varchar(48),
	PRIMARY KEY(o_id, o_d_id, o_c_id) 
	);


INSERT 
    into test 
VALUES 
    (1,10.0, 555, now(), 'dddd');


SELECT o_id, o_c_id*o_d_id + 1, o_entry_d as time
FROM 
    (SELECT * from test) as t1
where 
    o_c_id > 4;



UPDATE
    test
SET 
    o_c_id = o_c_id + 2 
WHERE
    o_d_id = 10;



UPDATE 
    table_aa7faec7
SET 
    column_8c0f2543=sahe_add(column_8c0f2543, '3676062...L%LL^', CI_ADD), 
    column_ad48b696=sahe_add(column_8c0f2543, '3676062...L%LL^', CI_MUL), 
    column_ceaf67a0=sahe_add(column_8c0f2543, '3676062...L%LL^', CI_DET),
    column_d0b802a4=sahe_add(column_8c0f2543, '3676062...L%LL^', CI_ORE), 
    column_eaf2a38b=sahe_add(column_8c0f2543, '3676062...L%LL^', CI_AES) 
WHERE 
    column_0d7f100e5 = '02cd94732c38bd9fcc105b275578c28e';