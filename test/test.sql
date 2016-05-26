SELECT ang2pix(16, 0.5, 1.0);
SELECT ang2pix(16, 0.5, 1.0, 'nest');

SELECT pix2ang(8, 45, 1);
SELECT pix2ang(8, 45, 2);
SELECT pix2ang(8, 45, 1, 'nest');
SELECT pix2ang(8, 45, 2, 'nest');

SELECT vec2pix(512, 1.2, 3.4, 5.6);
SELECT vec2pix(512, 1.2, 3.4, 5.6, 'nest');

SELECT pix2vec(128, 12345, 1);
SELECT pix2vec(128, 12345, 2);
SELECT pix2vec(128, 12345, 3);
SELECT pix2vec(128, 12345, 1, 'nest');
SELECT pix2vec(128, 12345, 2, 'nest');
SELECT pix2vec(128, 12345, 3, 'nest');

SELECT neighbours(64, 555, 1);
SELECT neighbours(64, 555, 2);
SELECT neighbours(64, 555, 3);
SELECT neighbours(64, 555, 4);
SELECT neighbours(64, 555, 5);
SELECT neighbours(64, 555, 6);
SELECT neighbours(64, 555, 7);
SELECT neighbours(64, 555, 8);
SELECT neighbours(64, 555, 1, 'nest');
SELECT neighbours(64, 555, 2, 'nest');
SELECT neighbours(64, 555, 3, 'nest');
SELECT neighbours(64, 555, 4, 'nest');
SELECT neighbours(64, 555, 5, 'nest');
SELECT neighbours(64, 555, 6, 'nest');
SELECT neighbours(64, 555, 7, 'nest');
SELECT neighbours(64, 555, 8, 'nest');

SELECT nside2npix(1024);
SELECT npix2nside(12582912);
