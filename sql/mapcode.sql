CREATE OR REPLACE FUNCTION encodeLatLonToMapcodes(float8, float8, cstring) RETURNS text
    AS 'mapcode', 'coord2mc_float_float_cstring'
    LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION decodeMapcodeToLatLon(cstring, cstring) RETURNS text
    AS 'mapcode', 'mc2coord_cstring_cstring'
    LANGUAGE C STRICT;
