#include "postgres.h"
#include <string.h>
#include "fmgr.h"

#include "mapcoder-cpp/mapcodelib/mapcoder.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

/* by value */

PG_FUNCTION_INFO_V1(coord2mc_float_float_cstring);

Datum coord2mc_float_float_cstring(PG_FUNCTION_ARGS);

Datum coord2mc_float_float_cstring(PG_FUNCTION_ARGS)
{
    double lat = PG_GETARG_FLOAT8(0);
    double lng = PG_GETARG_FLOAT8(1);
    const char *ccode = PG_GETARG_CSTRING(2);

    text *t = NULL;
    char *results[MAX_NR_OF_MAPCODE_RESULTS];
    char mapcodes[128] = "";
    size_t length = 0;
    int territoryCode, nrResults = 0;

    while(lng > 180)
        lng -= 360;
    while (lng < -180)
        lng += 360;

    while (lat > 90)
        lat -= 180;

    while (lat < -90)
        lat += 180;

    territoryCode = convertTerritoryIsoNameToCode(ccode, 0);
    nrResults = encodeLatLonToMapcodes(results, lat, lng, territoryCode, 0);

    if (nrResults <= 0) {
        PG_RETURN_NULL();
    }

    for (int i = 0; i < nrResults; ++i) {
        const char* foundMapcode = results[(i * 2)];
        const char* foundTerritory = results[(i *2) + 1];

        if (i > 0) {
            sprintf(mapcodes, "%s, ", mapcodes);
        }
        sprintf(mapcodes, "%s%s %s", mapcodes, foundTerritory, foundMapcode);
    }
    sprintf(mapcodes, "%s%c", mapcodes, '\0');

    length = strlen(mapcodes);
    t = (text *) palloc(VARHDRSZ + length);
    SET_VARSIZE( t, VARHDRSZ + length);
    memcpy( VARDATA(t), mapcodes, length);

    PG_RETURN_TEXT_P( t );
}

PG_FUNCTION_INFO_V1(mc2coord_cstring_cstring);

Datum mc2coord_cstring_cstring(PG_FUNCTION_ARGS);

Datum mc2coord_cstring_cstring(PG_FUNCTION_ARGS)
{
    const char *mapcode = PG_GETARG_CSTRING(0);
    const char *ccode = PG_GETARG_CSTRING(1);
    char geom[128] = "";

    int territoryCode = convertTerritoryIsoNameToCode(ccode, 0);
    double lat;
    double lon;
    int length = 0;
    text *t;

    int ret = decodeMapcodeToLatLon(&lat, &lon, mapcode, territoryCode);
    if (ret != 0)
        PG_RETURN_NULL();

    sprintf(geom, "%lf,%lf%c", lat, lon, '\0');

    length = strlen(geom);
    t = (text *) palloc(VARHDRSZ + length);
    SET_VARSIZE( t, VARHDRSZ + length );
    memcpy( VARDATA(t), geom, length);

    PG_RETURN_TEXT_P( t );
}
