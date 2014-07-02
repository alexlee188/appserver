PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE CUSTOMER (CUSTOMER_ID integer, NAME_1 varchar(40), NAME_2 varchar(40), ADDR_BLK_NO varchar(10), ADDR_STREET_1 varchar(80), ADDR_STREET_2 varchar(80), ADDR_CITY varchar(40), ADDR_COUNTRY varchar(40) default "Singapore", PHONE varchar(16), MOBILE varchar(16), ACCOUNT_BAL real default 0.0, ADDR_POSTCODE varchar(10), lastupdatedon date) engine=InnoDB;
INSERT INTO "CUSTOMER" VALUES(1,'Chan Ah Kow','Mrs Chan','123','Woodlands Street 456','','','Singapore','61234567','91234567',0.0,'303113','2014-06-23 15:21:06');
INSERT INTO "CUSTOMER" VALUES(2,'Tan Ah Kew',NULL,'45','Clementi Road','#10-10',NULL,'Singapore',NULL,'92345678',0.0,'201333','2014-06-23 15:20:31');
INSERT INTO "CUSTOMER" VALUES(100,'Ah Bu',NULL,NULL,NULL,NULL,NULL,'Singapore',NULL,'98888888',0.0,'269101',NULL);
CREATE TRIGGER customer_update_trg after update on CUSTOMER
begin
  update CUSTOMER set lastupdatedon = datetime('NOW') where rowid = new.rowid;
end;
COMMIT;
