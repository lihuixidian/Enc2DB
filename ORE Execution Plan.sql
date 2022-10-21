
CREATE TABLE "public"."order_line1" (
  "ol_o_id" int4 NOT NULL,
  "ol_d_id" int2 NOT NULL,
  "ol_w_id" int2 NOT NULL,
  "ol_number" int2 NOT NULL,
  "ol_i_id" int4,
  "ol_supply_w_id" int2,
  "ol_delivery_d" timestamp(6),
  "ol_quantity" int2,
  "ol_amount" numeric(6,2),
  "ol_dist_info" char(24) COLLATE "pg_catalog"."default"
)
;

show seq_page_cost

-- ----------------------------
-- Indexes structure for table order_line1
-- ----------------------------
CREATE INDEX "fkey_order_line_21" ON "public"."order_line1" USING btree (
  "ol_supply_w_id" "pg_catalog"."int2_ops" ASC NULLS LAST,
  "ol_i_id" "pg_catalog"."int4_ops" ASC NULLS LAST
);

-- ----------------------------
-- Primary Key structure for table order_line1
-- ----------------------------
ALTER TABLE "public"."order_line1" ADD CONSTRAINT "order_line1_pkey" PRIMARY KEY ("ol_w_id", "ol_d_id", "ol_o_id", "ol_number");

-- ----------------------------
-- Foreign Keys structure for table order_line1
-- ----------------------------
ALTER TABLE "public"."order_line1" ADD CONSTRAINT "fkey_order_line_1_1" FOREIGN KEY ("ol_w_id", "ol_d_id", "ol_o_id") REFERENCES "public"."orders1" ("o_id", "o_d_id", "o_w_id") ON DELETE NO ACTION ON UPDATE NO ACTION;
ALTER TABLE "public"."order_line1" ADD CONSTRAINT "fkey_order_line_2_1" FOREIGN KEY ("ol_supply_w_id", "ol_i_id") REFERENCES "public"."stock1" ("s_i_id", "s_w_id") ON DELETE NO ACTION ON UPDATE NO ACTION;


explain analyze
SELECT COUNT(*)
                        FROM order_line1, stock1
                       WHERE ol_w_id = 1
                         AND ol_d_id = 4
                         AND ol_o_id < 3011
                         AND ol_o_id >= 2991
                         AND s_w_id= 1
                         AND s_i_id=ol_i_id
                         AND s_quantity < 16;

EXPLAIN analyze
SELECT COUNT(*)
                        FROM order_line1
                       WHERE ol_w_id = 1
                         AND ol_d_id = 4
                          AND ol_o_id < 3011
                         AND ol_o_id >= 2991;
                         
 Aggregate  (cost=932.34..932.35 rows=1 width=4)
   ->  Nested Loop  (cost=0.00..932.32 rows=7 width=4)
         ->  Index Scan using order_line1_pkey on order_line1  (cost=0.00..179.31 rows=98 width=4)
               Index Cond: ((ol_w_id = 1) AND (ol_d_id = 4) AND (ol_o_id < 3011) AND (ol_o_id >= 2991))
         ->  Index Scan using fkey_stock_21 on stock1  (cost=0.00..7.67 rows=1 width=4)
               Index Cond: (s_i_id = order_line1.ol_i_id)
               Filter: ((s_quantity < 16) AND (s_w_id = 1))




####################
# SOFT
####################


SELECT 
    count(column_f5f75a9cb9f34b0f (s_i_id_AES)) AS restmp_371b4205969abf23 
FROM 
    table_29c05288b22d07e2 (order_line1), table_d5aad1385e55e95e (stock1) 
WHERE 
    column_46fe678876675ee5 (f_key) (p_key) (ol_w_id_AES) = 'b6d0dbd2559eb5b975dd226c6f6cb6fd' AND    
    column_b96c3ab835c38172 (f_key) (p_key) (ol_d_id_AES) = 'ba25eac42a5e15c9a7d2acb389d04486' AND    
    udf_ore_lt(column_652c795c4a0dd9d7 (ol_o_id_ORE), '1~1!32@8*cecc0d65df0...22b8a7af2<', column_ba18eede04c98015 (f_key) (p_key)(ol_o_id_AES), '14aace62be56b8cceaa27ddfc6943633') AND 
    udf_ore_ge(column_652c795c4a0dd9d7 (ol_o_id_ORE), '1~1!32@8*ce4e08...b32c12f8bf9f2a<', column_ba18eede04c98015 (f_key) (p_key)(ol_o_id_AES), '0b2b62c7abe363ea4e037112c48ae24f') AND 
    column_75bdb093a3479e90 (s_w_id_AES) = 'b6d0dbd2559eb5b975dd226c6f6cb6fd' AND 
    column_f5f75a9cb9f34b0f (s_i_id_AES) = column_bba0ff53c6b5477b (_index) (f_key) (ol_i_id_AES) AND 
    udf_ore_lt(column_67189c5e65d495a7 (s_quantity_ORE), '1~1!32@8*ce4e0859...7c47ca07d0af<', column_14f45485dad1f10c (s_quantity_AES), '5cd1c92b57ee2629988213402a428bfd');

UDF:ORE
硬件平均运行时间=0.00us, 累计平均时间=0.00us 
软件平均运行时间=13.46us, 累计平均时间=12.87us 
硬件总计运行时间=0.00s
软件总计运行时间=0.81s
总计执行次数：SOFT=60066，HARD=0

GE: 
AvgTime=13.09us, maxTime=53.00us, CntTime=0.3924s, Called=29986

LT: 
AvgTime=13.31us, maxTime=48.00us, CntTime=0.4004s, Called=30080


 Aggregate  (cost=65.24..65.25 rows=1 width=32)
   ->  Nested Loop  (cost=0.00..65.24 rows=1 width=32)
         ->  Index Scan using table_29c05288b22d07e2 (order_line1)_pkey on table_29c05288b22d07e2 (order_line1)  (cost=0.00..48.87 rows=1 width=32)
               Index Cond: ((column_46fe678876675ee5 (f_key) (p_key) (ol_w_id_AES) = 'b6d0dbd2559eb5b975dd226c6f6cb6fd'::text) AND ((column_b96c3ab835c38172 (f_key) (p_key) (ol_d_id_AES) = 'ba25eac42a5e15c9a7d2acb389d04486'::text))
               Filter: (udf_ore_lt(column_652c795c4a0dd9d7 (ol_o_id_ORE), '1~1!32@8*ca911fe671...b8a7af2<'::text, column_ba18eede04c98015 (f_key) (p_key)(ol_o_id_AES), '14aace62be56b8cceaa27ddfc6943633'::text) AND udf_ore_ge(column_652c795c4a0dd9d7 (ol_o_id_ORE), '1~1!32@8*c08582...2979f2a<'::text, column_ba18eede04c98015 (f_key), '0b2b62c7abe363ea4e037112c48ae24f'::text))
         ->  Index Scan using table_d5aad1385e55e95e (stock1)_pkey on table_d5aad1385e55e95e (stock1)  (cost=0.00..16.37 rows=1 width=32)
               Index Cond: ((column_75bdb093a3479e90 (s_w_id_AES) = 'b6d0dbd2559eb5b975dd226c6f6cb6fd'::text) AND (column_f5f75a9cb9f34b0f (s_i_id_AES) = table_29c05288b22d07e2 (order_line1).column_bba0ff53c6b5477b (_index) (f_key) (ol_i_id_AES)))
               Filter: udf_ore_lt(column_67189c5e65d495a7 (s_quantity_ORE), '1~1!32@8*ce4e0....c47ca07d0af<'::text, column_14f45485dad1f10c (s_quantity_AES), '5cd1c92b57ee2629988213402a428bfd'::text)



 Aggregate  (cost=81763.24..81763.25 rows=1 width=33) (actual time=2415.742..2415.743 rows=1 loops=1)
   Buffers: shared hit=234451 read=29704
   ->  Nested Loop  (cost=0.00..81760.38 rows=1146 width=33) (actual time=463.560..2415.736 rows=5 loops=1)
         Buffers: shared hit=234451 read=29704
         ->  Index Scan using table_29c05288b22d07e2 (order_line1)_pkey on table_29c05288b22d07e2 (order_line1)  (cost=0.00..56218.64 rows=3437 width=33) (actual time=117.872..2410.547 rows=94 loops=1)
               Index Cond: ((column_46fe678876675ee5 (f_key) (p_key) (ol_w_id_AES) = 'b6d0dbd2559eb5b975dd226c6f6cb6fd'::text) AND (column_b96c3ab835c38172 (f_key) (p_key) (ol_d_id_AES) = 'ba25eac42a5e15c9a7d2acb389d04486'::text))
               Filter: (udf_ore_lt(column_652c795c4a0dd9d7 (ol_o_id_ORE), '1~1!32@8*ce4e...0000^9e7142...a7af2<'::text, column_ba18eede04c98015 (f_key) (p_key)(ol_o_id_AES), '14aace62be56b8cceaa27ddfc6943633'::text) AND udf_ore_ge(column_652c795c4a0dd9d7 (ol_o_id_ORE), '1~1!32@8*ce4e0...2ce000^5...9f2a<'::text, column_ba18eede04c98015 (f_key) (p_key)(ol_o_id_AES), '0b2b62c7abe363ea4e037112c48ae24f'::text))
               Rows Removed by Filter: 29892
               Buffers: shared hit=234170 read=29219
         ->  Index Scan using fkey_stock_21 on table_d5aad1385e55e95e (stock1)  (cost=0.00..7.42 rows=1 width=33) (actual time=0.055..0.055 rows=0 loops=94)
               Index Cond: (column_f5f75a9cb9f34b0f (s_i_id_AES) = table_29c05288b22d07e2 (order_line1).column_bba0ff53c6b5477b (_index) (f_key) (ol_i_id_AES))
               Filter: (udf_ore_lt(column_67189c5e65d495a7 (s_quantity_ORE), '1~1!32@8*...^...<'::text, column_14f45485dad1f10c (s_quantity_AES), '5cd1c92b57ee2629988213402a428bfd'::text) AND (column_75bdb093a3479e90 (s_w_id_AES) = 'b6d0dbd2559eb5b975dd226c6f6cb6fd'::text))
               Rows Removed by Filter: 1
               Buffers: shared hit=281 read=485
 Total runtime: 2415.782 ms


CREATE INDEX "fkey_order_line_21" ON "public"."table_29c05288b22d07e2 (order_line1)" USING btree (
  "column_619348279ea23c08" COLLATE "pg_catalog"."default" "pg_catalog"."text_ops" ASC NULLS LAST,
  "column_bba0ff53c6b5477b (_index) (f_key) (ol_i_id_AES)" COLLATE "pg_catalog"."default" "pg_catalog"."text_ops" ASC NULLS LAST
);

-- ----------------------------
-- Primary Key structure for table table_29c05288b22d07e2 (order_line1)
-- ----------------------------
ALTER TABLE "public"."table_29c05288b22d07e2 (order_line1)" ADD CONSTRAINT "table_29c05288b22d07e2 (order_line1)_pkey" PRIMARY KEY ("column_46fe678876675ee5 (f_key) (p_key) (ol_w_id_AES)", "column_b96c3ab835c38172 (f_key) (p_key) (ol_d_id_AES)", "column_ba18eede04c98015 (f_key) (p_key)(ol_o_id_AES)", "column_ada75588e8d3df83");

-- ----------------------------
-- Foreign Keys structure for table table_29c05288b22d07e2 (order_line1)
-- ----------------------------
ALTER TABLE "public"."table_29c05288b22d07e2 (order_line1)" ADD CONSTRAINT "fkey_order_line_1_1" FOREIGN KEY ("column_46fe678876675ee5 (f_key) (p_key) (ol_w_id_AES)", "column_b96c3ab835c38172 (f_key) (p_key) (ol_d_id_AES)", "column_ba18eede04c98015 (f_key) (p_key)(ol_o_id_AES)") REFERENCES "public"."table_72cd0eca73ac2251" ("column_0838fcc9cbeeb754", "column_484d1cafd715afa5", "column_ddda6c74c0b5c6e4") ON DELETE NO ACTION ON UPDATE NO ACTION;
ALTER TABLE "public"."table_29c05288b22d07e2 (order_line1)" ADD CONSTRAINT "fkey_order_line_2_1" FOREIGN KEY ("column_619348279ea23c08", "column_bba0ff53c6b5477b (_index) (f_key) (ol_i_id_AES)") REFERENCES "public"."table_d5aad1385e55e95e (stock1)" ("column_f5f75a9cb9f34b0f (s_i_id_AES)", "column_75bdb093a3479e90 (s_w_id_AES)") ON DELETE NO ACTION ON UPDATE NO ACTION;


######################
# ORE
######################

SELECT 
    count( ) AS restmp_1edb7ddc05acab47
FROM 
    table_3bb250eec23c723f (order_line1), table_42c1f78468d7cdff (stock1)
WHERE 
    column_d93c3ff73e364e7b (f_key) (p_key) (ol_w_id_AES) = 'b6d0dbd2559eb5b975dd226c6f6cb6fd' AND    
    column_38be3c6ec9d02711 (f_key) (p_key) (ol_d_id_AES) = 'ba25eac42a5e15c9a7d2acb389d04486' AND    
    udf_ore_lt('null', 'null', column_17b642933e9a36b8 (f_key) (p_key) (ol_o_id_AES), '14aace62be56b8cceaa27ddfc6943633') AND
    udf_ore_ge('null', 'null', column_17b642933e9a36b8 (f_key) (p_key) (ol_o_id_AES), '0b2b62c7abe363ea4e037112c48ae24f') AND 
    column_4577c28c98782ae9 (s_w_id_AES) = 'b6d0dbd2559eb5b975dd226c6f6cb6fd' AND 
    column_68cc8ff0df8b271f (s_i_id_AES) = column_17c157b8814c203f (f_key) (_index) (ol_i_id_AES) AND 
    udf_ore_lt('null', 'null', column_ce749e78f500c829 (f_key) (s_quantity_AES), '5cd1c92b57ee2629988213402a428bfd');


UDF:ORE
硬件平均运行时间=14.96us, 累计平均时间=7.34us 
软件平均运行时间=0.00us, 累计平均时间=0.00us 
硬件总计运行时间=10.45s
软件总计运行时间=0.00s
总计执行次数：SOFT=0，HARD=698586


GE: 
AvgTime=7.61us, maxTime=49.00us, CntTime=2.2771s, Called=299293

LT: 
AvgTime=20.77us, maxTime=5212238.00us, CntTime=8.2936s, Called=399293


Aggregate  (cost=76269.50..76269.51 rows=1 width=33)
   ->  Hash Join  (cost=22177.66..76265.85 rows=1462 width=33)
         Hash Cond: (table_3bb250eec23c723f (order_line1).column_17c157b8814c203f (f_key) (_index) (ol_i_id_AES) = table_42c1f78468d7cdff (stock1).column_68cc8ff0df8b271f (s_i_id_AES))
         ->  Seq Scan on table_3bb250eec23c723f (order_line1)  (cost=0.00..53709.66 rows=4387 width=33)
               Filter: (udf_ore_lt('null'::text, 'null'::text, column_17b642933e9a36b8 (f_key) (p_key) (ol_o_id_AES), '14aace62be56b8cceaa27ddfc6943633'::text) AND udf_ore_ge('null'::text, 'null'::text, column_17b642933e9a36b8 (f_key) (p_key) (ol_o_id_AES), '0b2b62c7abe363ea4e037112c48ae24f'::text) AND (column_d93c3ff73e364e7b (f_key) (p_key) (ol_w_id_AES) = 'b6d0dbd2559eb5b975dd226c6f6cb6fd'::text) AND (column_38be3c6ec9d02711 (f_key) (p_key) (ol_d_id_AES) = 'ba25eac42a5e15c9a7d2acb389d04486'::text))
         ->  Hash  (cost=21500.00..21500.00 rows=33333 width=33)
               ->  Seq Scan on table_42c1f78468d7cdff (stock1)  (cost=0.00..21500.00 rows=33333 width=33)
                     Filter: (udf_ore_lt('null'::text, 'null'::text, column_ce749e78f500c829 (f_key) (s_quantity_AES), '5cd1c92b57ee2629988213402a428bfd'::text) AND (column_4577c28c98782ae9 (s_w_id_AES) = 'b6d0dbd2559eb5b975dd226c6f6cb6fd'::text))


 Aggregate  (cost=76269.50..76269.51 rows=1 width=33) (actual time=26750.750..26750.751 rows=1 loops=1)
   Buffers: shared hit=5 read=67738, temp read=43 written=37
   ->  Hash Join  (cost=22177.66..76265.85 rows=1462 width=33) (actual time=26750.477..26750.747 rows=5 loops=1)
         Hash Cond: (table_3bb250eec23c723f (order_line1).column_17c157b8814c203f (f_key) (_index) (ol_i_id_AES) = table_42c1f78468d7cdff (stock1).column_68cc8ff0df8b271f (s_i_id_AES))
         Buffers: shared hit=5 read=67738, temp read=43 written=37
         ->  Seq Scan on table_3bb250eec23c723f (order_line1)  (cost=0.00..53709.66 rows=4387 width=33) (actual time=12445.768..23634.211 rows=94 loops=1)
               Filter: (udf_ore_lt('null'::text, 'null'::text, column_17b642933e9a36b8 (f_key) (p_key) (ol_o_id_AES), '14aace62be56b8cceaa27ddfc6943633'::text) AND udf_ore_ge('null'::text, 'null'::text, column_17b642933e9a36b8 (f_key) (p_key) (ol_o_id_AES), '0b2b62c7abe363ea4e037112c48ae24f'::text) AND (column_d93c3ff73e364e7b (f_key) (p_key) (ol_w_id_AES) = 'b6d0dbd2559eb5b975dd226c6f6cb6fd'::text) AND (column_38be3c6ec9d02711 (f_key) (p_key) (ol_d_id_AES) = 'ba25eac42a5e15c9a7d2acb389d04486'::text))
               Rows Removed by Filter: 299199
               Buffers: shared read=47740
         ->  Hash  (cost=21500.00..21500.00 rows=33333 width=33) (actual time=3116.013..3116.013 rows=6652 loops=1)
               Buckets: 2048  Batches: 4  Memory Usage: 112kB
               Buffers: shared hit=2 read=19998, temp written=31
               ->  Seq Scan on table_42c1f78468d7cdff (stock1)  (cost=0.00..21500.00 rows=33333 width=33) (actual time=0.298..3113.845 rows=6652 loops=1)
                     Filter: (udf_ore_lt('null'::text, 'null'::text, column_ce749e78f500c829 (f_key) (s_quantity_AES), '5cd1c92b57ee2629988213402a428bfd'::text) AND (column_4577c28c98782ae9 (s_w_id_AES) = 'b6d0dbd2559eb5b975dd226c6f6cb6fd'::text))
                     Rows Removed by Filter: 93348
                     Buffers: shared hit=2 read=19998

                     
CREATE INDEX "fkey_order_line_21" ON "public"."table_3bb250eec23c723f (order_line1)" USING btree (
  "column_ce749e78f500c829 (f_key) (_index)" COLLATE "pg_catalog"."default" "pg_catalog"."text_ops" ASC NULLS LAST,
  "column_17c157b8814c203f (f_key) (_index) (ol_i_id_AES)" COLLATE "pg_catalog"."default" "pg_catalog"."text_ops" ASC NULLS LAST
);

-- ----------------------------
-- Primary Key structure for table table_3bb250eec23c723f (order_line1)
-- ----------------------------
ALTER TABLE "public"."table_3bb250eec23c723f (order_line1)" ADD CONSTRAINT "table_3bb250eec23c723f (order_line1)_pkey" PRIMARY KEY ("column_d93c3ff73e364e7b (f_key) (p_key) (ol_w_id_AES)", "column_38be3c6ec9d02711 (f_key) (p_key) (ol_d_id_AES)", "column_17b642933e9a36b8 (f_key) (p_key) (ol_o_id_AES)", "column_da1964245d641bb8");

-- ----------------------------
-- Foreign Keys structure for table table_3bb250eec23c723f (order_line1)
-- ----------------------------
ALTER TABLE "public"."table_3bb250eec23c723f (order_line1)" ADD CONSTRAINT "fkey_order_line_1_1" FOREIGN KEY ("column_d93c3ff73e364e7b (f_key) (p_key) (ol_w_id_AES)", "column_38be3c6ec9d02711 (f_key) (p_key) (ol_d_id_AES)", "column_17b642933e9a36b8 (f_key) (p_key) (ol_o_id_AES)") REFERENCES "public"."table_7024bc3cab4e9ba5" ("column_918f92c0c5f72357", "column_4c42c1b29d4c3d4a", "column_99a0e0d781f473dd") ON DELETE NO ACTION ON UPDATE NO ACTION;
ALTER TABLE "public"."table_3bb250eec23c723f (order_line1)" ADD CONSTRAINT "fkey_order_line_2_1" FOREIGN KEY ("column_ce749e78f500c829 (f_key) (_index)", "column_17c157b8814c203f (f_key) (_index) (ol_i_id_AES)") REFERENCES "public"."table_42c1f78468d7cdff (stock1)" ("column_68cc8ff0df8b271f (s_i_id_AES)", "column_4577c28c98782ae9 (s_w_id_AES)") ON DELETE NO ACTION ON UPDATE NO ACTION;
