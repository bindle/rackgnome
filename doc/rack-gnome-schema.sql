/*
 *  Rack Gnome
 *  Copyright (C) 2013 Bindle Binaries
 */
/*
 *  Run:
 *     cat rack-gnome-schema.sql | sqlite3 rackgnome.db
 */


/*
 *  Scope:         ISP
 *  Domain:        rackgnome.org
 */


/*
 *  System:        eagle.anc.ak
 *  Scope:         ISP
 *  Status:        Pre-production
 *  OS:            Linux/Slackware
 *  OS Version:    3.8.4/14.0
 *  Installed:     2013/09/28 by syzdek
 *  Patched:       2013/09/28 by syzdek
 *  Device Group:  smtp farm
 *  Guest Host:
 *  Guest Type:
 *  Primary:       syzdek
 *  Secondary:     jones
 *  Comment:
 */


CREATE TABLE version
(
   id             SERIAL PRIMARY KEY,
   uuid           UUID UNIQUE NOT NULL,
   major          INTEGER,
   minor          INTEGER,
   patch          INTEGER,
   build          INTEGER,
   tag            VARCHAR(128),
   string         VARCHAR(128)
);


CREATE TABLE os
(
   id             SERIAL PRIMARY KEY,
   uuid           UUID UNIQUE NOT NULL,
   name           VARCHAR(128) NOT NULL,
   description    VARCHAR(128),
   hasVersion     BOOL DEAFAULT TRUE,
   osVendorId     INTEGER,
   FOREIGN KEY (osVendorId) REFERENCES osVendor (id)
);


CREATE TABLE osVersion
(
   osId           INTEGER,
   versionId      INTEGER,
   PRIMARY KEY (osId, versionId)
   FOREIGN KEY (osId)      REFERENCES os      (id),
   FOREIGN KEY (versionId) REFERENCES version (id)
);


CREATE TABLE osVendor
(
   id             SERIAL PRIMARY KEY,
   uuid           UUID UNIQUE NOT NULL,
   name           VARCHAR(128) NOT NULL,
   description    VARCHAR(128),
   hasVersion     BOOL DEFAULT FALSE
);


CREATE TABLE osVendorVersion
(
   osVendorId     INTEGER,
   versionId      INTEGER,
   PRIMARY KEY (osVendorId, versionId)
   FOREIGN KEY (osVendorId) REFERENCES osVendor (id),
   FOREIGN KEY (versionId)  REFERENCES version  (id)
);


CREATE TABLE device
(
   id               SERIAL PRIMARY KEY,
   uuid             UUID UNIQUE NOT NULL,
   name             VARCHAR(128) NOT NULL,
   description      VARCHAR(1024),
   hostname         VARCHAR(128),
   uname            VARCHAR(128),
   osId             INTEGER,
   osVerId          INTEGER,
   osVendorId       INTEGER,
   osVendorVerId    INTEGER,
   FOREIGN KEY (osId)          REFERENCES os              (id),
   FOREIGN KEY (osVerId)       REFERENCES osVersion       (id),
   FOREIGN KEY (osVendorId)    REFERENCES osVendor        (id),
   FOREIGN KEY (osVendorVerId) REFERENCES osVendorVersion (id)
);


/* end of schema */
