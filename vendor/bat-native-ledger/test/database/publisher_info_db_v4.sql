BEGIN TRANSACTION;
DROP TABLE IF EXISTS "pending_contribution";
CREATE TABLE IF NOT EXISTS "pending_contribution" (
	"publisher_id"	LONGVARCHAR NOT NULL,
	"amount"	DOUBLE NOT NULL DEFAULT 0,
	"added_date"	INTEGER NOT NULL DEFAULT 0,
	"viewing_id"	LONGVARCHAR NOT NULL,
	"category"	INTEGER NOT NULL,
	CONSTRAINT "fk_pending_contribution_publisher_id" FOREIGN KEY("publisher_id") REFERENCES "publisher_info"("publisher_id") ON DELETE CASCADE
);
DROP TABLE IF EXISTS "recurring_donation";
CREATE TABLE IF NOT EXISTS "recurring_donation" (
	"publisher_id"	LONGVARCHAR NOT NULL UNIQUE,
	"amount"	DOUBLE NOT NULL DEFAULT 0,
	"added_date"	INTEGER NOT NULL DEFAULT 0,
	PRIMARY KEY("publisher_id"),
	CONSTRAINT "fk_recurring_donation_publisher_id" FOREIGN KEY("publisher_id") REFERENCES "publisher_info"("publisher_id") ON DELETE CASCADE
);
DROP TABLE IF EXISTS "media_publisher_info";
CREATE TABLE IF NOT EXISTS "media_publisher_info" (
	"media_key"	TEXT NOT NULL UNIQUE,
	"publisher_id"	LONGVARCHAR NOT NULL,
	PRIMARY KEY("media_key"),
	CONSTRAINT "fk_media_publisher_info_publisher_id" FOREIGN KEY("publisher_id") REFERENCES "publisher_info"("publisher_id") ON DELETE CASCADE
);
DROP TABLE IF EXISTS "activity_info";
CREATE TABLE IF NOT EXISTS "activity_info" (
	"publisher_id"	LONGVARCHAR NOT NULL,
	"duration"	INTEGER NOT NULL DEFAULT 0,
	"visits"	INTEGER NOT NULL DEFAULT 0,
	"score"	DOUBLE NOT NULL DEFAULT 0,
	"percent"	INTEGER NOT NULL DEFAULT 0,
	"weight"	DOUBLE NOT NULL DEFAULT 0,
	"month"	INTEGER NOT NULL,
	"year"	INTEGER NOT NULL,
	"reconcile_stamp"	INTEGER NOT NULL DEFAULT 0,
	CONSTRAINT "activity_unique" UNIQUE("publisher_id","month","year","reconcile_stamp"),
	CONSTRAINT "fk_activity_info_publisher_id" FOREIGN KEY("publisher_id") REFERENCES "publisher_info"("publisher_id") ON DELETE CASCADE
);
DROP TABLE IF EXISTS "contribution_info";
CREATE TABLE IF NOT EXISTS "contribution_info" (
	"publisher_id"	LONGVARCHAR,
	"probi"	TEXT "0" NOT NULL,
	"date"	INTEGER NOT NULL,
	"category"	INTEGER NOT NULL,
	"month"	INTEGER NOT NULL,
	"year"	INTEGER NOT NULL,
	CONSTRAINT "fk_contribution_info_publisher_id" FOREIGN KEY("publisher_id") REFERENCES "publisher_info"("publisher_id") ON DELETE CASCADE
);
DROP TABLE IF EXISTS "publisher_info";
CREATE TABLE IF NOT EXISTS "publisher_info" (
	"publisher_id"	LONGVARCHAR NOT NULL UNIQUE,
	"verified"	BOOLEAN NOT NULL DEFAULT 0,
	"excluded"	INTEGER NOT NULL DEFAULT 0,
	"name"	TEXT NOT NULL,
	"favIcon"	TEXT NOT NULL,
	"url"	TEXT NOT NULL,
	"provider"	TEXT NOT NULL,
	PRIMARY KEY("publisher_id")
);
DROP TABLE IF EXISTS "meta";
CREATE TABLE IF NOT EXISTS "meta" (
	"key"	LONGVARCHAR NOT NULL UNIQUE,
	"value"	LONGVARCHAR,
	PRIMARY KEY("key")
);
INSERT INTO "activity_info" ("publisher_id","duration","visits","score","percent","weight","month","year","reconcile_stamp") VALUES ('brave.com',10,0,1.12593247244573,30,29.9456389581773,2,2019,1552214829),
 ('slo-tech.com',12,0,1.23982569647756,33,32.9747774279396,2,2019,1552214829),
 ('basicattentiontoken.org',15,3,1.39416318062023,37,37.0795836138831,2,2019,1552214829);
INSERT INTO "publisher_info" ("publisher_id","verified","excluded","name","favIcon","url","provider") VALUES ('brave.com',0,0,'brave.com','','https://brave.com/',''),
 ('slo-tech.com',0,0,'slo-tech.com','','https://slo-tech.com/',''),
 ('basicattentiontoken.org',0,0,'basicattentiontoken.org','','https://basicattentiontoken.org/','');
INSERT INTO "meta" ("key","value") VALUES ('mmap_status','-1'),
 ('last_compatible_version','1'),
 ('version','4');
DROP INDEX IF EXISTS "pending_contribution_publisher_id_index";
CREATE INDEX IF NOT EXISTS "pending_contribution_publisher_id_index" ON "pending_contribution" (
	"publisher_id"
);
DROP INDEX IF EXISTS "recurring_donation_publisher_id_index";
CREATE INDEX IF NOT EXISTS "recurring_donation_publisher_id_index" ON "recurring_donation" (
	"publisher_id"
);
DROP INDEX IF EXISTS "activity_info_publisher_id_index";
CREATE INDEX IF NOT EXISTS "activity_info_publisher_id_index" ON "activity_info" (
	"publisher_id"
);
DROP INDEX IF EXISTS "contribution_info_publisher_id_index";
CREATE INDEX IF NOT EXISTS "contribution_info_publisher_id_index" ON "contribution_info" (
	"publisher_id"
);
COMMIT;
