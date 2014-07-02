PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE JOB (JOB_ID integer primary key autoincrement, CUSTOMER_ID, JOB_DESC text, JOB_NEED_1 varchar(20), JOB_NEED_2 varchar(20), JOB_NEED_3 varchar(20), JOB_START_TIME date, JOB_DURATION real default 1.0, JOB_STATUS varchar(10) default 'open') engine=InnoDB;
INSERT INTO "JOB" VALUES(1,1,'Change of leg dressing','Wound Care',NULL,NULL,'2015-01-05 10:30',1.0,'open');
INSERT INTO "JOB" VALUES(2,2,'General Care',NULL,NULL,NULL,'2015-01-06 12:30',1.0,'open');
INSERT INTO "JOB" VALUES(3,1,'Bathing and Turning','Geriatric',NULL,NULL,'2014-09-10 19:00',4.0,'open');
INSERT INTO "JOB" VALUES(4,2,'Change of urinary catheter','catheterisation',NULL,NULL,'2014-12-30 19:30',1.0,'open');
INSERT INTO "JOB" VALUES(5,100,'Insertion of Ryles Tube',NULL,NULL,NULL,'2014-12-31 12:30',1.5,'open');
COMMIT;
