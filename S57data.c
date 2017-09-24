// S57data.c: S57 geo data
//
// Project:  OpENCview

/*
    This file is part of the OpENCview project, a viewer of ENC.
    Copyright (C) 2000-2017 Sylvain Duclos sduclos@users.sourceforge.net

    OpENCview is free software: you can redistribute it and/or modify
    it under the terms of the Lesser GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpENCview is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    Lesser GNU General Public License for more details.

    You should have received a copy of the Lesser GNU General Public License
    along with OpENCview.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "S57data.h"    // S57_geo
#include "S52utils.h"   // PRINTF()

#define _ISOC99_SOURCE
#include <math.h>       // INFINITY, NAN, nearbyint()

#ifdef S52_USE_PROJ
static projPJ      _pjsrc   = NULL;   // projection source
static projPJ      _pjdst   = NULL;   // projection destination
static char       *_pjstr   = NULL;
static int         _doInit  = TRUE;   // will set new src projection
static const char *_argssrc = "+proj=latlong +ellps=WGS84 +datum=WGS84";
//static const char *_argsdst = "+proj=merc +ellps=WGS84 +datum=WGS84 +unit=m +no_defs";
// Note: ../../../FWTools/FWTools-2.0.6/bin/gdalwarp
//       -t_srs "+proj=merc +ellps=WGS84 +datum=WGS84 +unit=m +no_defs"
//        46307260_LOD2.tif 46307260_LOD2.merc.tif
// FIXME: test POLAR ENC omerc:
//  "+proj=omerc +lat_0=4 +lonc=115 +alpha=53.31582047222222 +k=0.99984 +x_0=590476.8727431979 +y_0=442857.6545573985
//   +ellps=evrstSS +towgs84=-533.4,669.2,-52.5,0,0,4.28,9.4 +to_meter=0.3047994715386762 +no_defs ";
#endif

// MAXINT-6 is how OGR tag an UNKNOWN value
// see gdal/ogr/ogrsf_frmts/s57/s57.h:126
// it is then turn into a string in gv_properties
//#define EMPTY_NUMBER_MARKER "2147483641"  /* MAXINT-6 */

//#define UNKNOWN  (1.0/0.0)   //HUGE_VAL   // INFINITY/NAN
// FIXME: this fail in CS - why?
//#define UNKNOWN  NAN
#define UNKNOWN  FP_NAN  // OK

// object's internal ID
static unsigned int _S57ID = 1;  // start at 1, the number of object loaded

// data for glDrawArrays()
typedef struct _prim {
    int mode;
    int first;
    int count;
    //guint mode;
    //guint first;
    //guint count;
} _prim;

typedef struct _S57_prim {
    GArray *list;      // list of _prim in 'vertex'
    GArray *vertex;    // XYZ geographic coordinate (bouble or float for GLES2 since some go right in the GPU - ie line)
    guint   DList;     // display list of the above
} _S57_prim;

// S57 object geo data
#define S57_ATT_NM_LN    6   // S57 Class Attribute Name lenght
#define S57_GEO_NM_LN   13   // GDAL/OGR primitive max name length: "ConnectedNode"
typedef struct _S57_geo {
    guint        S57ID;          // record ID / S52ObjectHandle use as index in S52_obj GPtrArray
                                 // Note: must be the first member for S57GETS57ID(GEO)

    //guint        s52objID;     // optimisation: numeric value of OBCL string

    char         name[S57_GEO_NM_LN+1]; //  6 - object name    + '\0'
                                        //  8 - WOLDNM         + '\0'
                                        // 13 - ConnectedNode  + '\0'

    S57_Obj_t    obj_t;       // PL & S57 - P/L/A

    ObjExt_t     ext;

    // length of geo data (POINT, LINE, AREA) currently in buffer
    guint        geoSize;        // max is 1 point / linexyznbr / ringxyznbr[0]

    // hold coordinate before and after projection
    // FIXME: why alloc xyz*1, easy to handle like the reste, but fragment mem !?!
    geocoord    *pointxyz;    // point (alloc)

    guint        linexyznbr;  // line number of point XYZ (alloc)
    geocoord    *linexyz;     // line coordinate

    // area
    guint        ringnbr;     // number of ring
    guint       *ringxyznbr;  // number coords per ring (alloc)
    geocoord   **ringxyz;     // coords of rings        (alloc)

    // hold tessalated geographic and projected coordinate of area
    // in a format suitable for OpenGL
    S57_prim    *prim;

    GData       *attribs;

#ifdef S52_USE_C_AGGR_C_ASSO
    // point to the S57 relationship object C_AGGR / C_ASSO this S57_geo belong
    // FIXME: handle multiple relation for the same object (ex US3NY21M/US3NY21M.000, CA379035.000)
    S57_geo     *relation;
#endif

    // for CS - object "touched" by this object - ref to object local to tihs object
    // FIXME: union assume use is exclusif - check this!
    union {
    //struct {
        S57_geo *TOPMAR; // break out objet class "touched"
        S57_geo *LIGHTS; // break out objet class "touched"
        S57_geo *DEPARE; // break out objet class "touched"
        S57_geo *DEPVAL; // break out objet class "touched"
    } touch;

    double       scamin;

    // FIXME: SCAMAX

#ifdef S52_USE_SUPP_LINE_OVERLAP
    // only for object "Edge"
    gchar       *name_rcidstr;  // optimisation: point to Att NAME_RCID str value

    S57_geo     *geoOwner;      // s57 obj that use this edge

    //S57_AW_t     origAW;        // debug - original Area Winding, CW: area < 0,  CCW: area > 0
#endif

    // centroid - save current centroids of this object
    // optimisation mostly for layer 9 AREA (FIXME: exemple of centroid on layer 9 ?!)
    guint        centroidIdx;
    GArray      *centroid;

#ifdef S52_USE_WORLD
    S57_geo     *nextPoly;
#endif

    gboolean     highlight;  // highlight this geo object (cursor pick / hazard - experimental)

    //gboolean     hazard;     // TRUE if a Safety Contour / hazard - use by leglin and GUARDZONE

    // optimisation: set LOD
    //S57_setLOD(obj, *c->dsid_intustr->str);
    //char       LOD;           // optimisation: chart purpose: cell->dsid_intustr->str

} _S57_geo;

static GString *_attList = NULL;


int           _initPROJ()
// Note: corrected for PROJ 4.6.0 ("datum=WGS84")
{
    if (FALSE == _doInit)
        return FALSE;

#ifdef S52_USE_PROJ
    const char *pj_ver = pj_get_release();
    if (NULL != pj_ver)
        PRINTF("PROJ4 VERSION: %s\n", pj_ver);

    // setup source projection
    if (!(_pjsrc = pj_init_plus(_argssrc))){
        PRINTF("ERROR: init src PROJ4\n");
        S57_donePROJ();
        g_assert(0);
        return FALSE;
    }
#endif

    if (NULL == _attList)
        _attList = g_string_new("");

    // FIXME: will need resetting for different projection
    _doInit = FALSE;

    return TRUE;
}

int        S57_donePROJ(void)
{
#ifdef S52_USE_PROJ
    if (NULL != _pjsrc) pj_free(_pjsrc);
    if (NULL != _pjdst) pj_free(_pjdst);
#endif

    _pjsrc  = NULL;
    _pjdst  = NULL;
    _doInit = TRUE;

    if (NULL != _attList)
        g_string_free(_attList, TRUE);
    _attList = NULL;

    g_free(_pjstr);
    _pjstr = NULL;

    return TRUE;
}

int        S57_setMercPrj(double lat, double lon)
{
    // From: http://trac.osgeo.org/proj/wiki/GenParms (and other link from that page)
    // Note: For merc, PROJ.4 does not support a latitude of natural origin other than the equator (lat_0=0).
    // Note: true scale using the +lat_ts parameter, which is the latitude at which the scale is 1.
    // Note: +lon_wrap=180.0 convert clamp [-180..180] to clamp [0..360]


/* http://en.wikipedia.org/wiki/Web_Mercator
PROJCS["WGS 84 / Pseudo-Mercator",
    GEOGCS["WGS 84",
        DATUM["WGS_1984",
            SPHEROID["WGS 84",6378137,298.257223563,
                AUTHORITY["EPSG","7030"]],
            AUTHORITY["EPSG","6326"]],
        PRIMEM["Greenwich",0,
            AUTHORITY["EPSG","8901"]],
        UNIT["degree",0.0174532925199433,
            AUTHORITY["EPSG","9122"]],
        AUTHORITY["EPSG","4326"]],
    PROJECTION["Mercator_1SP"],
    PARAMETER["central_meridian",0],
    PARAMETER["scale_factor",1],
    PARAMETER["false_easting",0],
    PARAMETER["false_northing",0],
    UNIT["metre",1,
        AUTHORITY["EPSG","9001"]],
    AXIS["X",EAST],
    AXIS["Y",NORTH],
    EXTENSION["PROJ4","+proj=merc +a=6378137 +b=6378137 +lat_ts=0.0 +lon_0=0.0 +x_0=0.0 +y_0=0 +k=1.0 +units=m +nadgrids=@null +wktext +no_defs"],
    AUTHORITY["EPSG","3857"]]
*/

    // FIXME: POLAR ENC
    // const char *templ = "+proj=omerc +lat_0=4 +lonc=115 +alpha=53.31582047222222 +k=0.99984
    //                      +x_0=590476.8727431979 +y_0=442857.6545573985
    //                      +ellps=evrstSS +towgs84=-533.4,669.2,-52.5,0,0,4.28,9.4
    //                      +to_meter=0.3047994715386762 +no_defs ";
    //
    //FAIL: ENC skewed: const char *templ = "+proj=omerc +lat_0=%.6f +lonc=%.6f +x_0=0 +y_0=0 +alpha=45 +gamma=0 +k_0=1 +ellps=WGS84 +no_defs";

    const char *templ = "+proj=merc +lat_ts=%.6f +lon_0=%.6f +ellps=WGS84 +datum=WGS84 +unit=m +no_defs";
    // FIXME: utm tilt ENC .. why?
    //const char *templ = "+proj=utm +lat_ts=%.6f +lon_0=%.6f +ellps=WGS84 +datum=WGS84 +unit=m +no_defs";

    if (NULL != _pjstr) {
        PRINTF("WARNING: Merc projection allready set\n");
        return FALSE;
    }

    _pjstr = g_strdup_printf(templ, lat, lon);
    PRINTF("DEBUG: lat:%f, lon:%f [%s]\n", lat, lon, _pjstr);

#ifdef S52_USE_PROJ
    if (NULL != _pjdst)
        pj_free(_pjdst);

    _pjdst = pj_init_plus(_pjstr);
    if (FALSE == _pjdst) {
        PRINTF("ERROR: init pjdst PROJ4 (lat:%f) [%s]\n", lat, pj_strerrno(pj_errno));
        g_assert(0);
        return FALSE;
    }
#endif

    return TRUE;
}

GCPTR      S57_getPrjStr(void)
{
    return _pjstr;
}

projXY     S57_prj2geo(projUV uv)
// convert PROJ to geographic (LL)
{
    if (TRUE == _doInit) return uv;
    if (NULL == _pjdst)  return uv;

#ifdef S52_USE_PROJ
    uv = pj_inv(uv, _pjdst);
    if (0 != pj_errno) {
        PRINTF("ERROR: x=%f y=%f %s\n", uv.u, uv.v, pj_strerrno(pj_errno));
        g_assert(0);
        return uv;
    }

    uv.u /= DEG_TO_RAD;
    uv.v /= DEG_TO_RAD;
#endif

    return uv;
}

int        S57_geo2prj3dv(guint npt, pt3 *data)
// convert a vector of lon/lat/z (pt3) to XY(z) 'in-place'
{
#ifdef S52_USE_GV
    return TRUE;
#endif

    return_if_null(data);

    //pt3 *pt = (pt3*)data;
    pt3 *pt = data;

    if (TRUE == _doInit) {
        _initPROJ();
    }

    if (NULL == _pjdst) {
        PRINTF("WARNING: nothing to project to .. load a chart first!\n");
        return FALSE;
    }

#ifdef S52_USE_PROJ
    // deg to rad --latlon
    for (guint i=0; i<npt; ++i, ++pt) {
        pt->x *= DEG_TO_RAD;
        pt->y *= DEG_TO_RAD;
    }

    // reset to beginning
    pt = (pt3*)data;

    // rad to cartesian  --mercator
    int ret = pj_transform(_pjsrc, _pjdst, npt, 3, &pt->x, &pt->y, &pt->z);
    if (0 != ret) {
        PRINTF("WARNING: in transform (%i): %s (%f,%f)\n", ret, pj_strerrno(pj_errno), pt->x, pt->y);
        g_assert(0);
        return FALSE;
    }

    /*
    // FIXME: test heuristic to reduce the number of point (for LOD):
    // try to (and check) reduce the number of points by flushing decimal
    // then libtess should remove coincident points.
    //
    // Other trick, try to reduce more by rounding using cell scale
    // pt->x = nearbyint(pt->x / (? * 10)) / (? * 10);
    //
    // test - 1km
    pt = (pt3*)data;
    for (guint i=0; i<npt; ++i, ++pt) {
        pt->x = nearbyint(pt->x / 1000.0) * 1000.0;
        pt->y = nearbyint(pt->y / 1000.0) * 1000.0;
    }
    //*/
#endif

    return TRUE;
}

static int    _inLine(pt3 A, pt3 B, pt3 C)
// is BC inline with AC or visa-versa (not AB)
{
    // test.1: A( 0, 0) B(2,2) C( 4, 4)
    // test.2: A(-2,-2) B(0,0) C(+2,+2)
    //A.x=+2;A.y=+2;B.x=0;B.y=0;C.x=-2;C.y=-2;
    //A.x=-2;A.y=-2;B.x=0;B.y=0;C.x=+2;C.y=+2;
    //A.x=-2;A.y=-2;B.x=-11;B.y=-11;C.x=+2;C.y=+2;

//#define S57_GEO_TOLERANCE 0.0001     // *60*60 = .36'
//#define S57_GEO_TOLERANCE 0.00001    // *60*60 = .036'   ; * 1852 =
//#define S57_GEO_TOLERANCE 0.000001   // *60*60 = .0036'  ; * 1852 = 6.667 meter
//#define S57_GEO_TOLERANCE 0.0000001  // *60*60 = .00036' ; * 1852 = 0.6667 meter
#define S57_GEO_TOLERANCE 0.00000001   // *60*60 = .000036'; * 1852 = 0.06667 meter

    // from: https://stackoverflow.com/questions/17692922/check-is-a-point-x-y-is-between-two-points-drawn-on-a-straight-line/17693189
    // if AC is vertical
    //if (A.x == C.x) return B.x == C.x;
    if (ABS(A.x-C.x) < S57_GEO_TOLERANCE) return ABS(B.x-C.x) < S57_GEO_TOLERANCE;

    // if AC is horizontal
    //if (A.y == C.y) return B.y == C.y;
    if (ABS(A.y-C.y) < S57_GEO_TOLERANCE) return ABS(B.y-C.y) < S57_GEO_TOLERANCE;

    // match the gradients (BUG: maybe after edit of '/' to '*')
    //return (A.x - C.x)*(A.y - C.y) == (C.x - B.x)*(C.y - B.y);
    //ex: -4*-4 == 2*2 = 16 == 4 !?!

    // slope: (y2-y1)/(x2-x1)
    // so slope AC == slope BC
    //return (A.y-C.y)/(A.x-C.x) == (B.y-C.y)/(B.x-C.x);  // div 0, need test above
    //return (A.y-C.y)*(B.x-C.x) == (B.y-C.y)*(A.x-C.x);  // inf
    return ABS(ABS((A.y-C.y)*(B.x-C.x)) - ABS((B.y-C.y)*(A.x-C.x))) < S57_GEO_TOLERANCE;  // inf

    // determinant = (ax-cx)(by-cy) - (bx-cx)(ay-cy)
    //return (A.x - C.x)*(B.y - C.y) == (B.x - C.x)*(A.y - C.y);
    // >0 above, <0 bellow, =0 on line
    // ex: (0-4)(2-4) - (2-4)(0-4) = 8-8 = 0
}

static guint  _delInLineSeg(guint npt, double *ppt)
// remove point ON the line segment
{
    pt3 *p = (pt3*)ppt;

    //* FIXME NOT: optimisation: use pt3 newArr[npt];
    // and only one memmove(ppt, newArr, sizeof(pt3) * j);  (see fail test bellow)
    //pt3 newArr[npt]; guint ii = 0, k = 0;
    //

    guint j = npt;
    for (guint i=0; i<(npt-2); ++i) {
        // don't lose Z
        if (p[1].z != p[2].z) {
            ++p;
            continue;
        }

        //*
        if (TRUE == _inLine(p[0], p[1], p[2])) {
            // A--B--C,       3-0-2=1
            // 0--A--B--C,    4-1-2=1
            // 0--A--B--C--0, 5-1-2=2
            memmove(&p[1], &p[2], sizeof(pt3) * (npt - i - 2));
            --j;
        } else {
            ++p;
        }
        //*/

        /*
        if (TRUE == _inLine(p[0], p[k+1], p[k+2])) {
            --j;
            ++k;
        } else {
            newArr[ii++] = *p++;
            p += k;
        }
        */
    }
    //memmove(ppt, newArr, sizeof(pt3) * j);

#ifdef S52_DEBUG
    //* debug: check for duplicate vertex
    p = (pt3*)ppt;
    guint nDup = 0;
    for (guint i=1; i<j; ++i) {
        if ((p[i-1].x == p[i].x) && (p[i-1].y == p[i].y)) {
            // FIXME: check (p[i-1].z == p[i].z)
            ++nDup;
        }
    }
    if (0 != nDup) {
        PRINTF("DEBUG: dup %i\n", nDup);
        //g_assert(0);
    }
    //*/
#endif  // S52_DEBUG

    return j;


    /* FIXME: this fail
    pt3  newArr[npt];
    guint i = 0, j = 0, k = 0;
    while (i<(npt-2)) {
        if (TRUE == _inLine(p[0], p[k+1], p[k+2])) {
            ++k;
        } else {
            newArr[j++] = p[0];
            if (0 < k) {
                //newArr[j++] = p[k];
                //newArr[j++] = p[k+1];  // p[k+2]
                newArr[j++] = p[k-1];  // p[old_k]
                k = 0;
            }
        }
        ++p;
        ++i;
    }
    */
}

static   int  _simplifyGEO(_S57_geo *geo)
{
    // LINE
    if (S57_LINES_T == geo->obj_t) {

        // need at least 3 pt
        if (2 < geo->linexyznbr) {
            guint npt = _delInLineSeg(geo->linexyznbr, geo->linexyz);
            if (npt != geo->linexyznbr) {
                //PRINTF("DEBUG: line reduction: %i \t(%i\t->\t%i)\n", geo->linexyznbr - npt, geo->linexyznbr, npt);
                geo->linexyznbr = npt;
            }
        }

        // degenerate !
        if (2 > geo->linexyznbr) {
            // FIXME: delete geo!
            PRINTF("WARNING: degenerated line: %i\n", geo->linexyznbr);
            g_assert(0);
        }
    }

    // AREA
    if (S57_AREAS_T == geo->obj_t) {
        for (guint i=0; i<geo->ringnbr; ++i) {
            if (3 < geo->ringxyznbr[i]) {
                guint npt = _delInLineSeg(geo->ringxyznbr[i], geo->ringxyz[i]);
                if (npt != geo->ringxyznbr[i]) {
                    //PRINTF("DEBUG: poly reduction: %i \t(%i\t->\t%i)\n", geo->ringxyznbr[i] - npt, geo->ringxyznbr[i], npt);
                    geo->ringxyznbr[i] = npt;
                }
            }

            // degenerate !
            if (3 > geo->ringxyznbr[i]) {
                // FIXME: delete ring!
                PRINTF("WARNING: ring with less than 3 vertex (%i)\n", geo->ringxyznbr[i]);
                g_assert(0);
            }
        }
    }

    return TRUE;
}

int        S57_geo2prj(_S57_geo *geo)
{
    // useless - rbin
    //return_if_null(geo);

    // FIXME: this break line/poly missmatch
    //_simplifyGEO(geo);

    if (TRUE == _doInit)
        _initPROJ();

#ifdef S52_USE_PROJ
    guint nr = S57_getRingNbr(geo);
    for (guint i=0; i<nr; ++i) {
        guint   npt;
        double *ppt;
        if (TRUE == S57_getGeoData(geo, i, &npt, &ppt)) {
            if (FALSE == S57_geo2prj3dv(npt, (pt3*)ppt))
                return FALSE;
        }
    }
#endif  // S52_USE_PROJ

    return TRUE;
}

static int    _doneGeoData(_S57_geo *geo)
// delete the geo data it self - data from OGR is a copy
{
#ifdef S52_USE_GV
    // not GV geo data
    return TRUE;
#endif

    // POINT
    if (NULL != geo->pointxyz) {
        g_free((geocoord*)geo->pointxyz);
        geo->pointxyz = NULL;
    }

    // LINES
    if (NULL != geo->linexyz) {
        g_free((geocoord*)geo->linexyz);
        geo->linexyz = NULL;
    }

    // AREAS
    if (NULL != geo->ringxyz){
        unsigned int i;
        for(i = 0; i < geo->ringnbr; ++i) {
            if (NULL != geo->ringxyz[i])
                g_free((geocoord*)geo->ringxyz[i]);
            geo->ringxyz[i] = NULL;
        }
        g_free((geocoord*)geo->ringxyz);
        geo->ringxyz = NULL;
    }

    if (NULL != geo->ringxyznbr) {
        g_free(geo->ringxyznbr);
        geo->ringxyznbr = NULL;
    }

    geo->linexyznbr = 0;
    geo->ringnbr    = 0;

    return TRUE;
}

int        S57_doneData   (_S57_geo *geo, gpointer user_data)
{
    // quiet line overlap analysis that trigger a bunch of harmless warning
    if (NULL!=user_data && NULL==geo)
        return FALSE;

#ifdef S52_USE_WORLD
    {
        S57_geo *geoNext = NULL;
        if (NULL != (geoNext = S57_getNextPoly(geo))) {
            S57_doneData(geoNext, user_data);
        }
    }
#endif


    _doneGeoData(geo);

    S57_donePrimGeo(geo);

    if (NULL != geo->attribs)
        g_datalist_clear(&geo->attribs);

    if (NULL != geo->centroid)
        g_array_free(geo->centroid, TRUE);

    g_free(geo);

    return TRUE;
}

S57_geo   *S57_setPOINT(geocoord *xyz)
{
    return_if_null(xyz);

    _S57_geo *geo = g_new0(_S57_geo, 1);
    //_S57_geo *geo = g_try_new0(_S57_geo, 1);
    if (NULL == geo)
        g_assert(0);

    geo->S57ID    = _S57ID++;
    geo->obj_t    = S57_POINT_T;
    geo->pointxyz = xyz;

    geo->ext.W    =  INFINITY;
    geo->ext.S    =  INFINITY;
    geo->ext.E    = -INFINITY;
    geo->ext.N    = -INFINITY;

    geo->scamin   =  INFINITY;

#ifdef S52_USE_WORLD
    geo->nextPoly = NULL;
#endif

    return geo;
}

#ifdef S52_USE_SUPP_LINE_OVERLAP
// experimental
S57_geo   *S57_setGeoLine(_S57_geo *geo, guint xyznbr, geocoord *xyz)
{
    return_if_null(geo);

    geo->obj_t      = S57_LINES_T;  // because some Edge objet default to _META_T when no geo yet
    geo->linexyznbr = xyznbr;
    geo->linexyz    = xyz;

    return geo;
}
#endif  // S52_USE_SUPP_LINE_OVERLAP

S57_geo   *S57_setLINES(guint xyznbr, geocoord *xyz)
{
    // Edge might have 0 node
    //return_if_null(xyz);

    _S57_geo *geo = g_new0(_S57_geo, 1);
    //_S57_geo *geo = g_try_new0(_S57_geo, 1);
    if (NULL == geo)
        g_assert(0);

    geo->S57ID      = _S57ID++;
    geo->obj_t      = S57_LINES_T;
    geo->linexyznbr = xyznbr;
    geo->linexyz    = xyz;

    geo->ext.W      =  INFINITY;
    geo->ext.S      =  INFINITY;
    geo->ext.E      = -INFINITY;
    geo->ext.N      = -INFINITY;

    geo->scamin     =  INFINITY;


#ifdef S52_USE_WORLD
    geo->nextPoly   = NULL;
#endif

    return geo;
}

#if 0
/*
S57_geo   *S57_setMLINE(guint nLineCount, guint *linexyznbr, geocoord **linexyz)
{
    _S57_geo *geo = g_new0(_S57_geo, 1);
    //_S57_geo *geo = g_try_new0(_S57_geo, 1);
    if (NULL == geo)
        g_assert(0);

    geo->ID         = _ID++;
    geo->obj_t      = MLINE_T;
    geo->linenbr    = nLineCount;
    geo->linexyznbr = linexyznbr;
    geo->linexyz    = linexyz;

#ifdef S52_USE_WORLD
    geo->nextPoly   = NULL;
#endif

    return geo;
}
*/
#endif  // 0

//S57_geo   *S57_setAREAS(guint ringnbr, guint *ringxyznbr, geocoord **ringxyz, S57_AW_t origAW)
S57_geo   *S57_setAREAS(guint ringnbr, guint *ringxyznbr, geocoord **ringxyz)
{
    return_if_null(ringxyznbr);
    return_if_null(ringxyz);

    _S57_geo *geo = g_new0(_S57_geo, 1);
    //_S57_geo *geo = g_try_new0(_S57_geo, 1);
    if (NULL == geo)
        g_assert(0);

    geo->S57ID      = _S57ID++;
    geo->obj_t      = S57_AREAS_T;
    geo->ringnbr    = ringnbr;
    geo->ringxyznbr = ringxyznbr;
    geo->ringxyz    = ringxyz;

    geo->ext.W      =  INFINITY;
    geo->ext.S      =  INFINITY;
    geo->ext.E      = -INFINITY;
    geo->ext.N      = -INFINITY;

    geo->scamin     =  INFINITY;

#ifdef S52_USE_WORLD
    geo->nextPoly   = NULL;
#endif

//#ifdef S52_USE_SUPP_LINE_OVERLAP
//    geo->origAW     = origAW;
//#else
//    (void)origAW;
//#endif

    return geo;
}

S57_geo   *S57_set_META(void)
{
    _S57_geo *geo = g_new0(_S57_geo, 1);
    //_S57_geo *geo = g_try_new0(_S57_geo, 1);
    if (NULL == geo)
        g_assert(0);

    geo->S57ID  = _S57ID++;
    geo->obj_t  = S57__META_T;

    geo->ext.W  =  INFINITY;
    geo->ext.S  =  INFINITY;
    geo->ext.E  = -INFINITY;
    geo->ext.N  = -INFINITY;

    geo->scamin =  INFINITY;

#ifdef S52_USE_WORLD
    geo->nextPoly = NULL;
#endif

    return geo;
}

int        S57_setName(_S57_geo *geo, const char *name)
// FIXME: name come from GDAL/OGR s57objectclasses.csv
{
    return_if_null(geo);
    return_if_null(name);

    //*
    if (S57_GEO_NM_LN < strlen(name)) {
        PRINTF("DEBUG: S57_geo name overflow max S57_GEO_NM_LN : %s\n", name);
        g_assert(0);
    }
    //*/

    int len = strlen(name);
    len = (S57_GEO_NM_LN < len) ? S57_GEO_NM_LN : len;
    memcpy(geo->name, name, len);
    geo->name[len] = '\0';

    return TRUE;
}

GCPTR      S57_getName(_S57_geo *geo)
{
    return_if_null(geo);
    return_if_null(geo->name);

    return geo->name;
}

guint      S57_getRingNbr(_S57_geo *geo)
{
    return_if_null(geo);

    // since this is used with S57_getGeoData
    // META object don't need to be projected for rendering
    switch (geo->obj_t) {
        case S57_POINT_T:
        case S57_LINES_T:
            return 1;
        case S57_AREAS_T:
            return geo->ringnbr;
        default:
            return 0;
    }
}

int        S57_getGeoData(_S57_geo *geo, guint ringNo, guint *npt, double **ppt)
// helper providing uniform access to geo
// WARNING: npt is the allocated mem (capacity)
{
    return_if_null(geo);

    if  (S57_AREAS_T==geo->obj_t && geo->ringnbr<ringNo) {
        PRINTF("WARNING: invalid ring number requested! \n");
        *npt = 0;
        g_assert(0);
        return FALSE;
    }

    switch (geo->obj_t) {
        case S57__META_T: *npt = 0; break;        // meta geo stuff (ex: C_AGGR)

        case S57_POINT_T:
            if (NULL != geo->pointxyz) {
                *npt = 1;
                *ppt = geo->pointxyz;
            } else {
                *npt = 0;
            }
            break;

        case S57_LINES_T:
            if (NULL != geo->linexyz) {
                *npt = geo->linexyznbr;
                *ppt = geo->linexyz;
            } else {
                *npt = 0;
            }
            break;

        case S57_AREAS_T:
            if (NULL != geo->ringxyznbr) {
                *npt = geo->ringxyznbr[ringNo];
                *ppt = geo->ringxyz[ringNo];
            } else {
                *npt = 0;
            }

            // debug
            //if (geodata->ringnbr > 1) {
            //    PRINTF("DEBUG: AREA_T ringnbr:%i only exterior ring used\n", geodata->ringnbr);
            //}
            break;
        default:
            PRINTF("ERROR: object type invalid (%i)\n", geo->obj_t);
            g_assert(0);
            return FALSE;
    }

    // alloc'ed mem for xyz vs xyz size
    if ((0==ringNo) && (*npt<geo->geoSize)) {
        PRINTF("ERROR: geo lenght greater then npt - internal error\n");
        g_assert(0);
        return FALSE;
    }

    if (0 == *npt) {
        //PRINTF("DEBUG: npt == 0\n");
        return FALSE;
    }

    return TRUE;
}

S57_prim  *S57_initPrim(_S57_prim *prim)
// set/reset primitive holder
{
    if (NULL == prim) {
        S57_prim *p = g_new0(S57_prim, 1);
        //_S57_prim *p = g_try_new0(_S57_prim, 1);
        if (NULL == p)
            g_assert(0);

        p->list   = g_array_new(FALSE, FALSE, sizeof(_prim));
        p->vertex = g_array_new(FALSE, FALSE, sizeof(vertex_t)*3);

        return p;
    } else {
        g_array_set_size(prim->list,   0);
        g_array_set_size(prim->vertex, 0);

        return prim;
    }
}

S57_prim  *S57_donePrim(_S57_prim *prim)
{
    //return_if_null(prim);
    // some symbol (ex Mariners' Object) dont use primitive since
    // not in OpenGL retained mode .. so this warning is a false alarme

    if (NULL == prim)
        return NULL;

    if (NULL != prim->list)   g_array_free(prim->list,   TRUE);
    if (NULL != prim->vertex) g_array_free(prim->vertex, TRUE);

    // failsafe
    prim->list   = NULL;
    prim->vertex = NULL;

    g_free(prim);

    return NULL;
}

S57_prim  *S57_initPrimGeo(_S57_geo *geo)
{
    return_if_null(geo);

    geo->prim = S57_initPrim(geo->prim);

    return geo->prim;
}

S57_geo   *S57_donePrimGeo(_S57_geo *geo)
{
    return_if_null(geo);

    if (NULL != geo->prim) {
        S57_donePrim(geo->prim);
        geo->prim = NULL;
    }

    return NULL;
}

int        S57_begPrim(_S57_prim *prim, int mode)
{
    _prim p;

    return_if_null(prim);

    p.mode  = mode;
    p.first = prim->vertex->len;

    g_array_append_val(prim->list, p);

    return TRUE;
}

int        S57_endPrim(_S57_prim *prim)
{
    return_if_null(prim);

    _prim *p = &g_array_index(prim->list, _prim, prim->list->len-1);
    if (NULL == p) {
        PRINTF("ERROR: no primitive at index: %i\n", prim->list->len-1);
        g_assert(0);
        return FALSE;
    }

    p->count = prim->vertex->len - p->first;

    // debug
    //if (p->count < 0) {
    //    g_assert(0);
    //}

    return TRUE;
}

int        S57_addPrimVertex(_S57_prim *prim, vertex_t *ptr)
// add one xyz coord (3 vertex_t)
{
    return_if_null(prim);
    return_if_null(ptr);

    //g_array_append_val(prim->vertex, *ptr);
    g_array_append_vals(prim->vertex, ptr, 1);

    return TRUE;
}

S57_prim  *S57_getPrimGeo(_S57_geo *geo)
{
    return_if_null(geo);

    return geo->prim;
}

guint      S57_getPrimData(_S57_prim *prim, guint *primNbr, vertex_t **vert, guint *vertNbr, guint *vboID)
{
    return_if_null(prim);
    //return_if_null(prim->list);

    *primNbr =            prim->list->len;
    *vert    = (vertex_t*)prim->vertex->data;
    *vertNbr =            prim->vertex->len;
    *vboID   =            prim->DList;

    return TRUE;
}

GArray    *S57_getPrimVertex(_S57_prim *prim)
{
    return_if_null(prim);

    // debug
    return_if_null(prim->vertex);

    return prim->vertex;
}

int        S57_setPrimDList (_S57_prim *prim, guint DList)
{
    return_if_null(prim);

    prim->DList = DList;

    return TRUE;
}

int        S57_getPrimIdx(_S57_prim *prim, unsigned int i, int *mode, int *first, int *count)
//int        S57_getPrimIdx(_S57_prim *prim, guint i, guint *mode, guint *first, guint *count)
{
    return_if_null(prim);

    if (i>=prim->list->len)
        return FALSE;

    _prim *p = &g_array_index(prim->list, _prim, i);
    if (NULL == p) {
        PRINTF("ERROR: no primitive at index: %i\n", i);
        g_assert(0);
        return FALSE;
    }

    *mode  = p->mode;
    *first = p->first;
    *count = p->count;

    return TRUE;
}

int        S57_setExt(_S57_geo *geo, double W, double S, double E, double N)
// assume: extent canonical - W, S, E, N
{
    return_if_null(geo);

    // debug
    //if (0 == g_strncasecmp(geo->name->str, "M_COVR", 6)) {
    //    PRINTF("DEBUG: %s: %f, %f  UR: %f, %f\n", geo->name->str, W, S, E, N);
    //}

    // canonize lng
    //W = (W < -180.0) ? 0.0 : (W > 180.0) ? 0.0 : W;
    //E = (E < -180.0) ? 0.0 : (E > 180.0) ? 0.0 : E;
    // canonize lat
    //S = (S < -90.0) ? 0.0 : (S > 90.0) ? 0.0 : S;
    //N = (N < -90.0) ? 0.0 : (N > 90.0) ? 0.0 : N;

    /*
    // check prime-meridian crossing
    if ((W < 0.0) && (0.0 < E)) {
        PRINTF("DEBUG: prime-meridian crossing %s: LL: %f, %f  UR: %f, %f\n", geo->name->str, W, S, E, N);
        g_assert(0);
    }
    */

    /* newVRMEBL pass here now, useless anyway
    if (isinf(W) && isinf(E)) {
        //PRINTF("DEBUG: %s: LL: %f, %f  UR: %f, %f\n", geo->name->str, W, S, E, N);
        PRINTF("DEBUG: %s: LL: %f, %f  UR: %f, %f\n", geo->name, W, S, E, N);
        g_assert(0);
        return FALSE;
    }
    */

    geo->ext.W = W;
    geo->ext.S = S;
    geo->ext.E = E;
    geo->ext.N = N;

    return TRUE;
}

#if 0
/*
int        S57_getExt(_S57_geo *geo, double *W, double *S, double *E, double *N)
// assume: extent canonical
{
    // called from inside cull loop this check is useless
    // but other call are not
    return_if_null(geo);

    // no extent: "$CSYMB", afgves, vessel, ..
    //if (0 != isinf(geo->ext.W)) {  // inf
    if (INFINITY == geo->ext.W) {  // inf
        geo->ext.W = -INFINITY;  // W
        geo->ext.S = -INFINITY;  // S
        geo->ext.E =  INFINITY;  // E
        geo->ext.N =  INFINITY;  // N

        // filter out system generated symb (scale, unit, ..) have no extent
        if (0 != g_strcmp0(geo->name, "$CSYMB")) {
            //PRINTF("DEBUG: no extent for %s:%i\n", geo->name, geo->S57ID);
            S57_dumpData(geo, TRUE);
        }

        return FALSE;
        //return TRUE;
    }

    *W = geo->ext.W;  // W
    *S = geo->ext.S;  // S
    *E = geo->ext.E;  // E
    *N = geo->ext.N;  // N

    return TRUE;
}
*/
#endif  // 0

ObjExt_t   S57_getExt(_S57_geo *geo)
// test returning ObjExt_t extent intead of four double in param
{
    // no extent: "$CSYMB", afgves, vessel, ..
    if (0 != isinf(geo->ext.W)) {  // inf
        geo->ext.W = -INFINITY;  // W
        geo->ext.S = -INFINITY;  // S
        geo->ext.E =  INFINITY;  // E
        geo->ext.N =  INFINITY;  // N
    }

    return geo->ext;
}

S57_Obj_t  S57_getObjtype(_S57_geo *geo)
{
    if (NULL == geo)
        return S57__META_T;

    return geo->obj_t;
}

#if 0
/* return the number of attributes.
static void   _countItems(GQuark key_id, gpointer data, gpointer user_data)
{
    const gchar *attName  = g_quark_to_string(key_id);
    if (6 == strlen(attName)){
        int *cnt = (int*)user_data;
        *cnt = *cnt + 1;
    }
}

int        S57_getNumAtt(_S57_geo *geo)
{
    int cnt = 0;
    g_datalist_foreach(&geo->attribs, _countItems, &cnt);
    return cnt;
}


struct _qwerty {
    int currentIdx;
    char featureName[20];
    char **name;
    char **value;
};

static void   _getAttValues(GQuark key_id, gpointer data, gpointer user_data)
{
    struct _qwerty *attData = (struct _qwerty*)user_data;

    GString     *attValue = (GString*) data;
    const gchar *attName  = g_quark_to_string(key_id);

    if (S57_ATT_NM_LN == strlen(attName)){
        strcpy(attData->value[attData->currentIdx], attValue->str);
        strcpy(attData->name [attData->currentIdx], attName );
        PRINTF("NOTE: inserting %s %s %d", attName, attValue->str, attData->currentIdx);
        attData->currentIdx += 1;
    } else {
        ;//      PRINTF("sjov Att: %s  = %s \n",attName, attValue->str);

    }
}

// recommend you count number of attributes in advance, to allocate the
// propper amount of **. each char *name should be allocated 7 and the char
// *val 20 ????
int        S57_getAttributes(_S57_geo *geo, char **name, char **val)
{
  struct _qwerty tmp;

  tmp.currentIdx = 0;
  tmp.name       = name;
  tmp.value      = val;

  g_datalist_foreach(&geo->attribs, _getAttValues,  &tmp);
  //  strcpy(name[tmp.currentIdx], "x");
  //  strcpy(val[tmp.currentIdx], "y");
  return tmp.currentIdx;
}
*/
#endif

GString   *S57_getAttVal(_S57_geo *geo, const char *att_name)
// return attribute string value or NULL if:
//      1 - attribute name abscent
//      2 - its a mandatory attribute but its value is not define (EMPTY_NUMBER_MARKER)
{
    return_if_null(geo);
    return_if_null(att_name);

    //GString *att = (GString*) g_datalist_get_data(&geo->attribs, att_name);
    //GString *att = (GString*) g_dataset_id_get_data(&geo->attribs, g_quark_try_string(att_name));
    GQuark   q   = g_quark_from_string(att_name);
    //GQuark   q   = g_quark_from_static_string(att_name);
    GString *att = (GString*) g_datalist_id_get_data(&geo->attribs, q);

    if (NULL!=att && (0==g_strcmp0(att->str, EMPTY_NUMBER_MARKER))) {
        //PRINTF("NOTE: mandatory attribute (%s) with ommited value\n", att_name);
        return NULL;
    }

    // display this NOTE once (because of too many warning)
    static int silent = FALSE;
    if (!silent && NULL!=att && 0==att->len) {
        //PRINTF("NOTE: attribute (%s) has no value [obj:%s]\n", att_name, geo->name->str);
        PRINTF("NOTE: attribute (%s) has no value [obj:%s]\n", att_name, geo->name);
        PRINTF("NOTE: (this msg will not repeat)\n");
        silent = TRUE;
        return NULL;
    }

    /* FIXME: handle NULL in caller
    if (NULL == att) {
        // clutter
        PRINTF("DEBUG: attribute name (%s) has NULL attribute value\n", att_name);
        //S57_dumpData(geo, FALSE);
        //g_assert(0);
    }
    */

    return att;
}

static void   _string_free(gpointer data)
{
    g_string_free((GString*)data, TRUE);
}

// FIXME: returning GData is useless
GData     *S57_setAtt(_S57_geo *geo, const char *name, const char *val)
{
    return_if_null(geo);
    return_if_null(name);
    return_if_null(val);

    GQuark   qname = g_quark_from_string(name);
    GString *value = g_string_new(val);

    if (NULL == geo->attribs)
        g_datalist_init(&geo->attribs);

#ifdef S52_USE_SUPP_LINE_OVERLAP
    if ((0==g_strcmp0(geo->name, "Edge")) && (0==g_strcmp0(name, "RCID"))) {
        geo->name_rcidstr = value->str;

        // FIXME: check for substring ",...)" if found at the end
        // this mean that TEMP_BUFFER_SIZE in OGR is not large anought.
     }
#endif

    g_datalist_id_set_data_full(&geo->attribs, qname, value, _string_free);

    return geo->attribs;
}

int        S57_setTouchTOPMAR(_S57_geo *geo, S57_geo *touch)
{
    return_if_null(geo);

    //* debug
    if ((0==g_strcmp0(touch->name, "LITFLT")) ||
        (0==g_strcmp0(touch->name, "LITVES")) ||
        (0==strncmp  (touch->name, "BOY", 3)))
    {
        if (NULL != geo->touch.TOPMAR) {
            PRINTF("DEBUG: touch.TOMAR allready in use by %s\n", geo->touch.TOPMAR->name);
        }
    } else {
        PRINTF("DEBUG: not TOMAR: %s\n", touch->name);
        g_assert(0);
    }
    //*/

    geo->touch.TOPMAR = touch;

    return TRUE;
}

S57_geo   *S57_getTouchTOPMAR(_S57_geo *geo)
{
    return_if_null(geo);

    return geo->touch.TOPMAR;
}

int        S57_setTouchLIGHTS(_S57_geo *geo, S57_geo *touch)
{
    return_if_null(geo);

    // WARNING: reverse chaining

    /* debug
    if (0 == g_strcmp0(geo->name, "LIGHTS")) {
        if (NULL != touch->touch.LIGHTS) {
            PRINTF("DEBUG: touch.LIGHTS allready in use by %s\n", touch->touch.LIGHTS->name);
        }
    } else {
        PRINTF("DEBUG: not LIGHTS: %s\n", geo->name);
        g_assert(0);
    }
    //*/

    geo->touch.LIGHTS = touch;

    return TRUE;
}

S57_geo   *S57_getTouchLIGHTS(_S57_geo *geo)
{
    return_if_null(geo);

    return geo->touch.LIGHTS;
}

int        S57_setTouchDEPARE(_S57_geo *geo, S57_geo *touch)
{
    return_if_null(geo);

    /* debug
    if ((0==g_strcmp0(touch->name, "DEPARE")) ||
        (0==g_strcmp0(touch->name, "DRGARE")) ||
        (0==g_strcmp0(touch->name, "OBSTRN")) ||
        (0==g_strcmp0(touch->name, "UWTROC")) ||
        (0==g_strcmp0(touch->name, "WRECKS"))
       )
    {
        if (NULL != geo->touch.DEPARE) {
            PRINTF("DEBUG: touch.DEPARE allready in use by %s\n", geo->touch.DEPARE->name);
            //if (0 != g_strcmp0(geo->name, geo->touch.DEPARE->name)) {
            //    PRINTF("DEBUG: %s:%i touch.DEPARE allready  in use by %s:%i will be replace by %s:%i\n",
            //           geo->name, geo->S57ID, geo->touch.DEPARE->name, geo->touch.DEPARE->S57ID, touch->name, touch->S57ID);
            //}
        }
    } else {
        PRINTF("DEBUG: not DEPARE: %s\n", touch->name);
        g_assert(0);
    }
    //*/

    geo->touch.DEPARE = touch;

    return TRUE;
}

S57_geo   *S57_getTouchDEPARE(_S57_geo *geo)
{
    return_if_null(geo);

    return geo->touch.DEPARE;
}

int        S57_setTouchDEPVAL(_S57_geo *geo, S57_geo *touch)
{
    return_if_null(geo);

    /* debug
    if ((0==g_strcmp0(touch->name, "DEPARE")) ||
        (0==g_strcmp0(touch->name, "DRGARE")) ||    // not in S52!
        (0==g_strcmp0(touch->name, "UNSARE"))       // this does nothing!
       )
    {
        if (NULL != geo->touch.DEPVAL) {
            PRINTF("DEBUG: %s:%i touch.DEPVAL allready in use by %s:%i\n",
                   geo->name, geo->S57ID, geo->touch.DEPVAL->name, geo->touch.DEPVAL->S57ID);
        }
    } else {
        PRINTF("DEBUG: not DEPVAL: %s\n", touch->name);
        g_assert(0);
    }
    //*/

    geo->touch.DEPVAL = touch;

    return TRUE;
}

S57_geo   *S57_getTouchDEPVAL(_S57_geo *geo)
{
    return_if_null(geo);

    return geo->touch.DEPVAL;
}

double     S57_setScamin(_S57_geo *geo, double scamin)
{
    // test useless since the only caller allready did that
    //return_if_null(geo);

    geo->scamin = scamin;

    return geo->scamin;
}

double     S57_getScamin(_S57_geo *geo)
{
    // test useless since the only caller allready did that
    //return_if_null(geo);

    return geo->scamin;
}

double     S57_resetScamin(_S57_geo *geo)
// reset scamin from att val
{
    // test useless since the only caller allready did that
    //return_if_null(geo);

    if (NULL == geo->attribs)
        g_datalist_init(&geo->attribs);

    GString *valstr = S57_getAttVal(geo, "SCAMIN");

    //double val = (NULL==valstr) ? INFINITY : S52_atof(valstr->str);
    double val = (NULL==valstr) ? UNKNOWN : S52_atof(valstr->str);

    geo->scamin = val;

    return geo->scamin;
}

#ifdef S52_USE_C_AGGR_C_ASSO
int        S57_setRelationship(_S57_geo *geo, _S57_geo *geoRel)
// this geo has in a C_AGGR or C_ASSO (geoRel) relationship
{
    return_if_null(geo);
    return_if_null(geoRel);

    if (NULL == geo->relation) {
        geo->relation = geoRel;
    } else {
        // FIXME: US3NY21M/US3NY21M.000 has multiple relation for the same object
        // also CA379035.000
        PRINTF("DEBUG: 'geo->relation' allready in use ..\n");
        g_assert(0);

        return FALSE;
    }

    return TRUE;
}

S57_geo  * S57_getRelationship(_S57_geo *geo)
{
    return_if_null(geo);

    return geo->relation;
}
#endif  // S52_USE_C_AGGR_C_ASSO

static void   _printAttVal(GQuark key_id, gpointer data, gpointer user_data)
{
    // 'user_data' not used
    (void) user_data;

    const gchar *attName  = g_quark_to_string(key_id);

    // print only S57 attrib - assuming that OGR att are not 6 char in lenght!!
    //if (S57_ATT_NM_LN == strlen(attName)) {
        GString *attValue = (GString*) data;
        PRINTF("%s: %s\n", attName, attValue->str);
    //}
}

int        S57_dumpData(_S57_geo *geo, int dumpCoords)
// debug - if dumpCoords is TRUE dump all coordinates
{
    return_if_null(geo);

    PRINTF("----------------\n");
    PRINTF("NAME  : %s\n", geo->name);
    PRINTF("S57ID : %i\n", geo->S57ID);

    switch (geo->obj_t) {
        case S57__META_T:  PRINTF("obj_t : _META_T\n"); break;
        case S57_POINT_T:  PRINTF("obj_t : POINT_T\n"); break;
        case S57_LINES_T:  PRINTF("obj_t : LINES_T\n"); break;
        case S57_AREAS_T:  PRINTF("obj_t : AREAS_T\n"); break;
        default:
            PRINTF("WARNING: invalid object type; %i\n", geo->obj_t);
    }

    // dump Att/Val
    g_datalist_foreach(&geo->attribs, _printAttVal, NULL);

    // dump extent
    PRINTF("EXTENT: %f, %f  --  %f, %f\n", geo->ext.S, geo->ext.W, geo->ext.N, geo->ext.E);

    if (TRUE == dumpCoords) {
        guint     npt = 0;
        geocoord *ppt = NULL;
        S57_getGeoData(geo, 0, &npt, &ppt);
        PRINTF("COORDS: %i\n", npt);
        for (guint i=0; i<npt; ++i) {
            PRINTF("\t\t(%f, %f, %f)\n", ppt[0], ppt[1], ppt[2]);
            ppt += 3;
        }
        //PRINTF("\n");
    }

    return TRUE;
}

static void   _getAtt(GQuark key_id, gpointer data, gpointer user_data)
{

    const gchar *attName  = g_quark_to_string(key_id);
    GString     *attValue = (GString*) data;
    GString     *attList  = (GString*) user_data;

    /*
    // filter out OGR internal S57 info
    if (0 == g_strcmp0("MASK",      attName)) return;
    if (0 == g_strcmp0("USAG",      attName)) return;
    if (0 == g_strcmp0("ORNT",      attName)) return;
    if (0 == g_strcmp0("NAME_RCNM", attName)) return;
    if (0 == g_strcmp0("NAME_RCID", attName)) return;
    if (0 == g_strcmp0("NINFOM",    attName)) return;
    */

    // save S57 attribute + system attribute (ex vessel name - AIS)
    if (0 != attList->len)
        g_string_append(attList, ",");

    g_string_append(attList, attName);
    g_string_append_c(attList, ':');
    g_string_append(attList, attValue->str);

    //PRINTF("\t%s : %s\n", attName, (char*)attValue->str);

    return;
}

GCPTR      S57_getAtt(_S57_geo *geo)
{
    return_if_null(geo);

    //PRINTF("S57ID : %i\n", geo->S57ID);
    //PRINTF("NAME  : %s\n", geo->name);

    g_string_set_size(_attList, 0);
    g_string_printf(_attList, "%s:%i", geo->name, geo->S57ID);

    g_datalist_foreach(&geo->attribs, _getAtt, _attList);

    return _attList->str;
}

#if 0
/*
void       S57_getGeoWindowBoundary(double lat, double lng, double scale, int width, int height, double *latMin, double *latMax, double *lngMin, double *lngMax)
{

  _initPROJ();

  {
    projUV pc1, pc2;   // pixel center

    pc1.v = lat;
    pc1.u = lng;

    pc1 = S57_geo2prj(pc1); // mercator center in meters

    // lower right
    pc2.u = (width/2.  + 0.5)*scale + pc1.u;
    pc2.v = (height/2. + 0.5)*scale + pc1.v;
    pc2 = S57_prj2geo(pc2);
    *lngMax = pc2.u;
    *latMax = pc2.v;
    // upper left
    pc2.u = -((width/2. )*scale + 0.5) + pc1.u;
    pc2.v = -((height/2.)*scale + 0.5) + pc1.v;
    pc2 = S57_prj2geo(pc2);
    *lngMin = pc2.u;
    *latMin = pc2.v;
  }

  PRINTF("lat/lng: %lf/%lf scale: %lf, w/h: %d/%d lat: %lf/%lf lng: %lf/%lf\n", lat, lng, scale, width, height, *latMin, *latMax, *lngMin, *lngMax);

  //S57_donePROJ();

}

int        S57_sameChainNode(_S57_geo *geoA, _S57_geo *geoB)
{

    return_if_null(geoA);
    return_if_null(geoB);

    pt3 *pa   = (pt3 *)geoA->linexyz;
    pt3 *pb   = (pt3 *)geoB->linexyz;
    pt3 *bend = (pt3 *)&geoB->linexyz[(geoB->linexyznbr-1)*3];
    //pt3 *bend = geoB->linexyz[geoB->linexyznbr-b-1];

    // FIXME: what if a chain-node has the same point
    // at both end of the chain!!
    //if ((pb->x == bend->x) && (pb->y == bend->y))
    //    g_assert(0);

    // first point match
    //if ((pa->x == pb->x) && (pa->y == pb->y))
    //    reverse = FALSE;
    //else {
        // last point match
    //    if ((pa->x == bend->x) && (pa->y == bend->y))
    //        reverse = TRUE;
    //    else
            //no match
    //        return FALSE;
    //}

    // can't be the same if not same lenght
    if (geoA->linexyznbr != geoB->linexyznbr)
        return FALSE;

    guint i = 0;
    if ((pa->x == pb->x) && (pa->y == pb->y)) {
        pt3 *ptA = (pt3 *)geoA->linexyz;
        pt3 *ptB = (pt3 *)geoB->linexyz;

        for (i=0; i<geoA->linexyznbr; ++i) {
            if ((ptA->x != ptB->x) || (ptA->x != ptB->x))
                break;

            ++ptA;
            ++ptB;
        }
    }

    // reach the end, then the're the same
    if (i+1 >= geoA->linexyznbr)
        return TRUE;

    if ((pa->x == bend->x) && (pa->y == bend->y)) {
        pt3 *ptA = (pt3 *)geoA->linexyz;
        pt3 *ptB = (pt3 *)(geoB->linexyz + (geoB->linexyznbr-1)*3);

        for (guint i=0; i<geoA->linexyznbr; ++i) {
            if ((ptA->x != ptB->x) || (ptA->x != ptB->x))
                break;

            ++ptA;
            --ptB;
        }
    }
    // reach the end, then the're the same
    if (i+1 >= geoA->linexyznbr)
        return TRUE;

    return FALSE;
}
*/
#endif  // 0

#ifdef S52_USE_WORLD
S57_geo   *S57_setNextPoly(_S57_geo *geo, _S57_geo *nextPoly)
{
    return_if_null(geo);

    if (NULL != geo->nextPoly)
        nextPoly->nextPoly = geo->nextPoly;

    geo->nextPoly = nextPoly;

    return geo;
}


S57_geo   *S57_getNextPoly(_S57_geo *geo)
{
    return_if_null(geo);

    return geo->nextPoly;
}

S57_geo   *S57_delNextPoly(_S57_geo *geo)
// unlink Poly chain
{
    return_if_null(geo);

    while (NULL != geo) {
        S57_geo *nextPoly = geo->nextPoly;
        geo->nextPoly = NULL;
        geo = nextPoly;
    }

    return NULL;
}
#endif

//guint      S57_getS57ID(_S57_geo *geo)
//{
//    return_if_null(geo);
//    return  geo->S57ID;
//}

//int        S57_isPtInside(int npt, double *xyz, gboolean close, double x, double y)
int        S57_isPtInside(int npt, pt3 *pt, gboolean close, double x, double y)
// return TRUE if (x,y) inside area (close/open) xyz else FALSE
// FIXME: CW or CCW or work with either?
{
    //return_if_null(xyz);
    return_if_null(pt);
    if (0 == npt)
        return FALSE;

    int c = 0;
    //pt3 *v = (pt3 *)xyz;
    pt3 *v = pt;

    if (TRUE == close) {
        for (int i=0; i<npt-1; ++i) {
            pt3 p1 = v[i];
            pt3 p2 = v[i+1];

            if ( ((p1.y>y) != (p2.y>y)) && (x < (p2.x-p1.x) * (y-p1.y) / (p2.y-p1.y) + p1.x) )
                c = !c;
        }
    } else {
        for (int i=0, j=npt-1; i<npt; j=i++) {
            pt3 p1 = v[i];
            pt3 p2 = v[j];

            if ( ((p1.y>y) != (p2.y>y)) && (x < (p2.x-p1.x) * (y-p1.y) / (p2.y-p1.y) + p1.x) )
                c = !c;
        }
    }

    // debug
    //PRINTF("npt: %i inside: %s\n", npt, (c==1)?"TRUE":"FALSE");

    return c;
}

int        S57_touch(_S57_geo *geoA, _S57_geo *geoB)
// TRUE if A touch B else FALSE
{
    guint   nptA;
    double *pptA;
    guint   nptB;
    double *pptB;

    return_if_null(geoA);
    return_if_null(geoB);

    if (FALSE == S57_getGeoData(geoA, 0, &nptA, &pptA))
        return FALSE;

    if (FALSE == S57_getGeoData(geoB, 0, &nptB, &pptB))
        return FALSE;

    // FIXME: work only for point in poly not point in line
    if (S57_LINES_T == S57_getObjtype(geoB)) {
        PRINTF("FIXME: geoB is a S57_LINES_T .. this algo break on that type\n");
        return FALSE;
    }

    for (guint i=0; i<nptA; ++i, pptA+=3) {
        if (TRUE == S57_isPtInside(nptB, (pt3*)pptB, TRUE, pptA[0], pptA[1]))
            return TRUE;
    }

    return FALSE;
}

guint      S57_getGeoSize(_S57_geo *geo)
{
    return_if_null(geo);

    return geo->geoSize;
}

guint      S57_setGeoSize(_S57_geo *geo, guint size)
{
    return_if_null(geo);

    if ((S57_POINT_T==geo->obj_t) && (size > 1)) {
        PRINTF("ERROR: POINT_T size\n");
        g_assert(0);
        return FALSE;
    }
    if ((S57_LINES_T==geo->obj_t) && (size > geo->linexyznbr)) {
        PRINTF("ERROR: LINES_T size\n");
        g_assert(0);
        return FALSE;
    }
    if ((S57_AREAS_T==geo->obj_t) && (size > geo->ringxyznbr[0])) {
        PRINTF("ERROR: AREAS_T size\n");
        g_assert(0);
        return FALSE;
    }

    if (S57__META_T == geo->obj_t) {
        PRINTF("ERROR: object type invalid (%i)\n", geo->obj_t);
        g_assert(0);
        return FALSE;
    }

    return geo->geoSize = size;
}

int        S57_newCentroid(_S57_geo *geo)
// init or reset
{
    return_if_null(geo);

    // case where an object has multiple centroid (concave poly)
    if (NULL == geo->centroid)
        geo->centroid = g_array_new(FALSE, FALSE, sizeof(pt2));
    else
        geo->centroid = g_array_set_size(geo->centroid, 0);

    geo->centroidIdx = 0;

    return TRUE;
}

int        S57_addCentroid(_S57_geo *geo, double  x, double  y)
{
    return_if_null(geo);

    pt2 pt = {x, y};
    g_array_append_val(geo->centroid, pt);

    return TRUE;
}

int        S57_getNextCent(_S57_geo *geo, double *x, double *y)
{
    return_if_null(geo);
    return_if_null(geo->centroid);

    if (geo->centroidIdx < geo->centroid->len) {
        pt2 pt = g_array_index(geo->centroid, pt2, geo->centroidIdx);
        *x = pt.x;
        *y = pt.y;

        ++geo->centroidIdx;
        return TRUE;
    }


    return FALSE;
}

int        S57_hasCentroid(_S57_geo *geo)
{
    return_if_null(geo);

    if (NULL == geo->centroid) {
        S57_newCentroid(geo);
    } else {
        // reset idx for call S57_getNextCent()
        geo->centroidIdx = 0;
    }

    if (0 == geo->centroid->len)
        return FALSE;

    return TRUE;
}

#ifdef S52_USE_SUPP_LINE_OVERLAP
S57_geo   *S57_getEdgeOwner(_S57_geo *geoEdge)
{
    // not needed
    //return_if_null(geo);

    return geoEdge->geoOwner;
}

S57_geo   *S57_setEdgeOwner(_S57_geo *geoEdge, _S57_geo *geoOwner)
{
    // not needed
    //return_if_null(geo);

    geoEdge->geoOwner = geoOwner;

    return geoEdge;
}

int        S57_markOverlapGeo(_S57_geo *geo, _S57_geo *geoEdge)
// experimental: mark coordinates in geo that match the chaine-node in geoEdge
{

    return_if_null(geo);
    return_if_null(geoEdge);

    // M_COVR is used for system generated DATCOVR
    if (0 == g_strcmp0("M_COVR", geo->name)) {
        //PRINTF("DEBUG: found M_COVR, nptEdge: %i - skipped\n", nptEdge);
        return TRUE;
    }

    guint   npt = 0;
    double *ppt;
    if (FALSE == S57_getGeoData(geo, 0, &npt, &ppt)) {
        PRINTF("WARNING: S57_getGeoData() failed\n");
        g_assert(0);
        return FALSE;
    }

    guint   nptEdge = 0;
    double *pptEdge;
    if (FALSE == S57_getGeoData(geoEdge, 0, &nptEdge, &pptEdge)) {
        PRINTF("DEBUG: nptEdge: %i\n", nptEdge);
        g_assert(0);
        return FALSE;
    }

    // debug
    //if (0 == g_strcmp0("HRBARE", geo->name)) {
    //    PRINTF("DEBUG: found HRBARE, nptEdge: %i\n", nptEdge);
    //}

    // search ppt for first pptEdge
    int   next = 0;
    guint i    = 0;
    for(i=0; i<npt; ++i) {
        //if (ppt[i*3] == pptEdge[i*3] && ppt[i*3+1] == pptEdge[i*3+1]) {
        if (ppt[i*3] == pptEdge[0] && ppt[i*3+1] == pptEdge[1]) {

            if (i == (npt-1)) {
                if (ppt[(i-1)*3] == pptEdge[3] && ppt[(i-1)*3+1] == pptEdge[4]) {
                    // next if backward
                    next = -1;
                    break;
                }
                //PRINTF("ERROR: at the end\n");
                //g_assert(0);
            }

            // FIXME: rollover
            // find if folowing point is ahead
            if (ppt[(i+1)*3] == pptEdge[3] && ppt[(i+1)*3+1] == pptEdge[4]) {
                // next is ahead
                next = 1;
                break;
            } else {
                // FIXME: start at end
                if (0 == i) {
                    i = npt - 1;
                    //PRINTF("ERROR: edge mismatch\n");
                    //    g_assert(0);
                    //return FALSE;
                }
                // FIXME: rollover
                if (ppt[(i-1)*3] == pptEdge[3] && ppt[(i-1)*3+1] == pptEdge[4]) {
                    // next if backward
                    next = -1;
                    break;
                }
                // this could be due to an innir ring!
                //else {
                //    PRINTF("ERROR: edge mismatch\n");
                //    g_assert(0);
                //}
            }
        }
    }

    // FIXME: no starting point in edge match any ppt!?
    // could it be a rounding problem
    // could be that this edge is part of an inner ring of a poly
    // and S57_getGeoData() only return outer ring
    if (0 == next) {
        //GString *rcidEdgestr = S57_getAttVal(geoEdge, "RCID");
        //S57_geo *geoEdgeLinkto = geoEdge->link;
        //PRINTF("WARNING: this edge (ID:%s link to %s) starting point doesn't match any point in this geo (%s)\n",
        //       rcidEdgestr->str, S57_getName(geoEdgeLinkto), S57_getName(geo));

        /*
        // debug: check if it is a precision problem (rounding)
        guint i = 0;
        double minLat = 90.0;
        for(i=0; i<npt; ++i) {
            // find the minimum
            double tmp = ppt[i*3] - pptEdge[0];
            if (minLat > fabs(tmp))
                minLat = tmp;
        }
        //g_assert(0);
        PRINTF("minLat = %f\n", minLat);
        */

        return FALSE;
    }

    if (-1 == next) {
        int tmp = (i+1) - nptEdge;
        if ( tmp < 0) {
            PRINTF("ERROR: tmp < 0\n");
            g_assert(0);
            return FALSE;
        }
    }

    if (1 == next) {
        if (nptEdge + i > npt) {
            PRINTF("ERROR: nptEdge + i > npt\n");
            g_assert(0);
            return FALSE;
        }
    }

    // LS() use Z_CLIP_PLANE (S57_OVERLAP_GEO_Z+1) to clip overlap
    // LC() check for the value -S57_OVERLAP_GEO_Z
    for (guint j=0; j<nptEdge; ++j) {
        ppt[i*3 + 2] = -S57_OVERLAP_GEO_Z;
        i += next;
    }

    return TRUE;
}

gchar     *S57_getRCIDstr(_S57_geo *geo)
{
    return_if_null(geo);

    return geo->name_rcidstr;
}

//S57_AW_t   S57_getOrigAW(_S57_geo *geo)
//// debug
//{
//    return_if_null(geo);
//
//    return geo->origAW;
//}
#endif  // S52_USE_SUPP_LINE_OVERLAP

int        S57_setHighlight(S57_geo *geo, gboolean highlight)
{
    return_if_null(geo);

    geo->highlight = highlight;

    return TRUE;
}

gboolean   S57_getHighlight(_S57_geo *geo)
{
    return_if_null(geo);

    return geo->highlight;
}

#if 0
int        S57_setHazard(_S57_geo *geo, gboolean hazard)
{
    return_if_null(geo);

    geo->hazard = hazard;

    return TRUE;
}

int        S57_isHazard(_S57_geo *geo)
{
    return_if_null(geo);

    return geo->hazard;
}
#endif  // 0

#if 0
int        S57_setLOD(_S52_obj *obj, char LOD)
{
    return_if_null(obj);

    obj->LOD = LOD;

    return TRUE;
}

char       S57_getLOD(_S52_obj *obj)
{
    return_if_null(obj);

    return obj->LOD;
}
#endif  // 0


#if 0
int main(int argc, char** argv)
{

   return 1;
}
#endif  // 0
