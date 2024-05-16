#include "postgres.h"
#include <string.h>
#include "fmgr.h"
#include "varatt.h"
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
    text *t = NULL;
    Mapcodes results[MAX_NR_OF_MAPCODE_RESULTS];
    char mapcodes[128] = "";
    size_t length = 0;
    int nrResults = 0;

    while(lng > 180)
        lng -= 360;
    while (lng < -180)
        lng += 360;

    while (lat > 90)
        lat -= 180;

    while (lat < -90)
        lat += 180;

    nrResults = encodeLatLonToMapcodes(results, lat, lng, tc, 0);

    if (nrResults <= 0 || results->count <= 0) {
        PG_RETURN_NULL();
    }

    for (int i = 0; i < results->count; ++i) {
        // char* foundMapcode = results->mapcode[(i * 2)];
        // char* foundTerritory = results->mapcode[(i *2) + 1];
        char* foundMapcode = results->mapcode[i];

        if (i > 0) {
            sprintf(mapcodes, "%s, ", mapcodes);
        }
        sprintf(mapcodes, "%s%s", mapcodes, foundMapcode);
    }
    sprintf(mapcodes, "%s%c", mapcodes, '\0');

    length = strlen(mapcodes);
    t = (text *) palloc(VARHDRSZ + length);
    SET_VARSIZE( t, VARHDRSZ + length);
    memcpy( VARDATA(t), mapcodes, length);

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
    if (ret != 0)
        PG_RETURN_NULL();

    sprintf(geom, "%lf,%lf%c", lat, lon, '\0');

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
