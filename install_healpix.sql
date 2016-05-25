CREATE FUNCTION ang2pix RETURNS INTEGER SONAME 'mysql_healpix.so';
CREATE FUNCTION pix2ang RETURNS REAL SONAME 'mysql_healpix.so';
CREATE FUNCTION vec2pix RETURNS INTEGER SONAME 'mysql_healpix.so';
CREATE FUNCTION pix2vec RETURNS REAL SONAME 'mysql_healpix.so';
CREATE FUNCTION neighbours RETURNS INTEGER SONAME 'mysql_healpix.so';
CREATE FUNCTION nside2npix RETURNS INTEGER SONAME 'mysql_healpix.so';
CREATE FUNCTION npix2nside RETURNS INTEGER SONAME 'mysql_healpix.so';

INSTALL PLUGIN healpix SONAME 'mysql_healpix.so';
