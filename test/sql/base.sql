CREATE EXTENSION mapcode;

SELECT mapcode_version();

SELECT mapcode_encode(0.341637, 32.593781, 'UGA');

SELECT mapcode_decode('N7.FR', 'UGA');
