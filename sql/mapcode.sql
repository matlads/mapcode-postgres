CREATE OR REPLACE FUNCTION mapcode_encode(float8, float8, cstring) RETURNS text
    AS 'mapcode', 'mapcode_encode'
    LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION mapcode_decode(cstring, cstring) RETURNS text
    AS 'mapcode', 'mapcode_decode'
    LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION mapcode_version() RETURNS text
    AS 'mapcode', 'mapcode_version'
    LANGUAGE C STRICT;
