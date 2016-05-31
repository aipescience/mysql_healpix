DROP FUNCTION IF EXISTS ang2pix;
DROP FUNCTION IF EXISTS pix2ang;
DROP FUNCTION IF EXISTS vec2pix;
DROP FUNCTION IF EXISTS pix2vec;
DROP FUNCTION IF EXISTS neighbours;
DROP FUNCTION IF EXISTS nest2ring;
DROP FUNCTION IF EXISTS ring2nest;
DROP FUNCTION IF EXISTS nside2npix;
DROP FUNCTION IF EXISTS npix2nside;

USE mysql;
DELIMITER //
DROP PROCEDURE IF EXISTS UninstallPlugin;
CREATE PROCEDURE UninstallPlugin()
BEGIN
IF (SELECT 1 = 1 FROM mysql.plugin WHERE `name`='healpix') THEN UNINSTALL PLUGIN healpix;
END IF;
END //
DELIMITER ;
CALL UninstallPlugin();
DROP PROCEDURE UninstallPlugin;
