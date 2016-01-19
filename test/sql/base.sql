\set ECHO 0

CREATE EXTENSION mapcode;

SELECT encodeLatLonToMapcodes(0.341637, 32.593781, 'UGA');

SELECT decodeMapcodeToLatLon('KP43.7KG', 'UGA');
