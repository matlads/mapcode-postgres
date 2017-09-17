#include "postgres.h"
#include <string.h>
#include "fmgr.h"

#include "mapcode-cpp/mapcodelib/mapcoder.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

/* by value */

PG_FUNCTION_INFO_V1(mapcode_encode);

Datum mapcode_encode(PG_FUNCTION_ARGS);

Datum mapcode_encode(PG_FUNCTION_ARGS)
{
    double lat = PG_GETARG_FLOAT8(0);
    double lng = PG_GETARG_FLOAT8(1);
    const char *ccode = PG_GETARG_CSTRING(2);
    enum Territory tc = getTerritoryCode(ccode, TERRITORY_NONE);
    char result[MAX_MAPCODE_RESULT_ASCII_LEN + 1];
    text *t = NULL;
    int nrResults = encodeLatLonToSingleMapcode(result, lat, lng, tc, 0);

    if (nrResults <= 0) {
        PG_RETURN_NULL();
    }

    t = (text *) palloc(VARHDRSZ + strlen(result));
    SET_VARSIZE( t, VARHDRSZ + strlen(result));
    memcpy( VARDATA(t), result, strlen(result));

    PG_RETURN_TEXT_P( t );
}

PG_FUNCTION_INFO_V1(mapcode_decode);

Datum mapcode_decode(PG_FUNCTION_ARGS);

Datum mapcode_decode(PG_FUNCTION_ARGS)
{
    const char *mapcode = PG_GETARG_CSTRING(0);
    const char *ccode = PG_GETARG_CSTRING(1);
    char geom[128] = "";
    MapcodeElements mapcodeElements;

    enum Territory territory = getTerritoryCode(ccode, TERRITORY_NONE);
    double lat;
    double lon;
    int length = 0;
    text *t;
    enum MapcodeError ret;

    fprintf(stderr, "mapcode: mapcode_decode: mapcode=%s, ccode=%s\n", mapcode, ccode);

    ret = decodeMapcodeToLatLonUtf8(&lat, &lon, mapcode, territory, &mapcodeElements);

    if (ret != ERR_OK ) {
      PG_RETURN_NULL();
    }

    sprintf(geom, "%lf,%lf%c", lat, lon, '\0');
    fprintf(stderr, "mapcode: mc2coord_cstring_cstring: geom = %s\n", geom);

    length = strlen(geom);
    t = (text *) palloc(VARHDRSZ + length);
    SET_VARSIZE( t, VARHDRSZ + length );
    memcpy( VARDATA(t), geom, length);

    PG_RETURN_TEXT_P( t );
}

PG_FUNCTION_INFO_V1(mapcode_version);

Datum mapcode_version(PG_FUNCTION_ARGS);
Datum mapcode_version(PG_FUNCTION_ARGS)
{
    char * version = MAPCODE_C_VERSION;
    int length = strlen(version);
    text *t = (text *) palloc(VARHDRSZ + length);
    SET_VARSIZE( t, VARHDRSZ + length );
    memcpy( VARDATA(t), version, length);

    PG_RETURN_TEXT_P( t );
}
