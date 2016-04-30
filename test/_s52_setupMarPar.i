// _s52_setupMarPar.i: common Mariners Parameter
//
// SD 2016APR10


static int      _s52_setupMarPar(void)
{
    // -- DEPTH COLOR ------------------------------------
    S52_setMarinerParam(S52_MAR_TWO_SHADES,      0.0);   // 0.0 --> 5 shades
    //S52_setMarinerParam(S52_MAR_TWO_SHADES,      1.0);   // 1.0 --> 2 shades

    // sounding color
    //S52_setMarinerParam(S52_MAR_SAFETY_DEPTH,    10.0);
    S52_setMarinerParam(S52_MAR_SAFETY_DEPTH,    15.0);

    S52_setMarinerParam(S52_MAR_SAFETY_CONTOUR,  10.0);
    //S52_setMarinerParam(S52_MAR_SAFETY_CONTOUR,  5.0);     // for triggering symb ISODGR01 (ODD winding) at Rimouski
    //S52_setMarinerParam(S52_MAR_SAFETY_CONTOUR,  3.0);     // for white chanel in Rimouski
    //S52_setMarinerParam(S52_MAR_SAFETY_CONTOUR,  1.0);

    S52_setMarinerParam(S52_MAR_SHALLOW_CONTOUR, 10.0);
    //S52_setMarinerParam(S52_MAR_SHALLOW_CONTOUR, 5.0);

    //S52_setMarinerParam(S52_MAR_DEEP_CONTOUR,   10.0);
    //S52_setMarinerParam(S52_MAR_DEEP_CONTOUR,   11.0);
    S52_setMarinerParam(S52_MAR_DEEP_CONTOUR,   12.0);

    //S52_setMarinerParam(S52_MAR_SHALLOW_PATTERN, 0.0);  // (default off)
    S52_setMarinerParam(S52_MAR_SHALLOW_PATTERN, 1.0);  // ON (GPU expentive)
    // -- DEPTH COLOR ------------------------------------

    S52_setMarinerParam(S52_MAR_SYMBOLIZED_BND, 1.0);  // on (default) [Note: this tax the CPU/GPU]
    //S52_setMarinerParam(S52_MAR_SYMBOLIZED_BND, 0.0);  // off
    S52_setMarinerParam(S52_MAR_SYMPLIFIED_PNT,  1.0);

    S52_setMarinerParam(S52_MAR_SHIPS_OUTLINE,   1.0);
    //S52_setMarinerParam(S52_MAR_DISTANCE_TAGS,   1.0);
    S52_setMarinerParam(S52_MAR_DISTANCE_TAGS,   0.0);
    S52_setMarinerParam(S52_MAR_HEADNG_LINE,     1.0);
    S52_setMarinerParam(S52_MAR_BEAM_BRG_NM,     1.0);
    //S52_setMarinerParam(S52_MAR_FULL_SECTORS,    0.0);    // (default ON)

    //S52_setMarinerParam(S52_MAR_DISP_CATEGORY,   S52_MAR_DISP_CATEGORY_BASE);    // BASE always ON
    //S52_setMarinerParam(S52_MAR_DISP_CATEGORY,   S52_MAR_DISP_CATEGORY_STD);     // STABDARD default
    //S52_setMarinerParam(S52_MAR_DISP_CATEGORY,   S52_MAR_DISP_CATEGORY_OTHER);   // OTHER
    //S52_setMarinerParam(S52_MAR_DISP_CATEGORY,   S52_MAR_DISP_CATEGORY_BASE | S52_MAR_DISP_CATEGORY_STD | S52_MAR_DISP_CATEGORY_OTHER);
    //S52_setMarinerParam(S52_MAR_DISP_CATEGORY,   S52_MAR_DISP_CATEGORY_STD | S52_MAR_DISP_CATEGORY_OTHER);
    S52_setMarinerParam(S52_MAR_DISP_CATEGORY,   S52_MAR_DISP_CATEGORY_SELECT);

    //S52_setMarinerParam(S52_MAR_DISP_LAYER_LAST, S52_MAR_DISP_LAYER_LAST_NONE );  // none
    //S52_setMarinerParam(S52_MAR_DISP_LAYER_LAST, S52_MAR_DISP_LAYER_LAST_STD );   // Mariner Standard - default
    //S52_setMarinerParam(S52_MAR_DISP_LAYER_LAST, S52_MAR_DISP_LAYER_LAST_OTHER);  // Mariner Other (EBL VRN)
    S52_setMarinerParam(S52_MAR_DISP_LAYER_LAST, S52_MAR_DISP_LAYER_LAST_SELECT);   // All Mariner (Standard(default) + Other)

    //S52_setMarinerParam(S52_MAR_COLOR_PALETTE,   0.0);     // DAY (default)
    //S52_setMarinerParam(S52_MAR_COLOR_PALETTE,   1.0);     // DAY DARK
    S52_setMarinerParam(S52_MAR_COLOR_PALETTE,   5.0);     // DAY 60 - need plib_COLS-3.4-a.rle
    //S52_setMarinerParam(S52_MAR_COLOR_PALETTE,   6.0);     // DUSK 60 - need plib_COLS-3.4-a.rle

    //S52_setMarinerParam(S52_MAR_VECPER,         12.0);  // vecper: Vector-length time-period (min) (normaly 6 or 12)
    S52_setMarinerParam(S52_MAR_VECMRK,          1.0);  // vecmrk: Vector time-mark interval (0 - none, 1 - 1&6 min, 2 - 6 min)
    //S52_setMarinerParam(S52_MAR_VECMRK,          2.0);  // vecmrk: Vector time-mark interval (0 - none, 1 - 1&6 min, 2 - 6 min)
    //S52_setMarinerParam(S52_MAR_VECSTB,          0.0);  // vecstb: Vector Stabilization (0 - none, 1 - ground, 2 - water)

    S52_setMarinerParam(S52_MAR_DATUM_OFFSET,    0.0);
    //S52_setMarinerParam(S52_MAR_DATUM_OFFSET,    5.0);

    S52_setMarinerParam(S52_MAR_SCAMIN,          1.0);   // ON (default)
    //S52_setMarinerParam(S52_MAR_SCAMIN,          0.0);   // debug OFF - show all

    // remove QUAPNT01 symbole (black diagonal and a '?')
    S52_setMarinerParam(S52_MAR_QUAPNT01,        0.0);   // off

    S52_setMarinerParam(S52_MAR_DISP_CALIB,      1.0);

    // cell's legend
    //S52_setMarinerParam(S52_MAR_DISP_LEGEND, 1.0);   // show
    S52_setMarinerParam(S52_MAR_DISP_LEGEND, 0.0);   // hide (default)

    //S52_setMarinerParam(S52_MAR_DISP_DRGARE_PATTERN, 0.0);  // OFF
    S52_setMarinerParam(S52_MAR_DISP_DRGARE_PATTERN, 1.0);  // ON (default)

    S52_setMarinerParam(S52_MAR_ANTIALIAS,       1.0);   // on
    //S52_setMarinerParam(S52_MAR_ANTIALIAS,       0.0);     // off

    //S52_setMarinerParam(S52_MAR_DISP_CRSR_PICK, 0.0);  // none
    S52_setMarinerParam(S52_MAR_DISP_CRSR_PICK, 1.0);  // pick/highlight top object
    //S52_setMarinerParam(S52_MAR_DISP_CRSR_PICK, 2.0);  // pick stack/highlight top
    //S52_setMarinerParam(S52_MAR_DISP_CRSR_PICK, 3.0);  // pick stack+ASSOC/highlight ASSOC (compiled with -DS52_USE_C_AGGR_C_ASSO)


    // ---------------- trick to force symbole size -----------------------------
#ifdef S52_USE_TEGRA2
    // smaller on xoom so that proportion look the same
    // as a 'normal' screen - since the eye is closer to the 10" screen of the Xoom
    S52_setMarinerParam(S52_MAR_DOTPITCH_MM_X, 0.3);
    S52_setMarinerParam(S52_MAR_DOTPITCH_MM_Y, 0.3);
#endif

#ifdef S52_USE_ADRENO
    // Nexus 7 (2013) [~323 ppi]
    S52_setMarinerParam(S52_MAR_DOTPITCH_MM_X, 0.2);
    S52_setMarinerParam(S52_MAR_DOTPITCH_MM_Y, 0.2);
#endif

#if !defined(SET_SCREEN_SIZE) && !defined(S52_USE_ANDROID)
    // NOTE: S52 pixels for symb are 0.3 mm
    S52_setMarinerParam(S52_MAR_DOTPITCH_MM_X, 0.3);
    S52_setMarinerParam(S52_MAR_DOTPITCH_MM_Y, 0.3);
#endif
    // -------------------------------------------- -----------------------------

    // a delay of 0.0 to tell to not delete old AIS (default +600 sec old)
    //S52_setMarinerParam(S52_MAR_DISP_VESSEL_DELAY, 0.0);
    // older AIS - case where s52ais reconnect
    // FIXME: maybe check AIS mmsi, so that same ohjH is used!
    S52_setMarinerParam(S52_MAR_DISP_VESSEL_DELAY, 700.0);

    //S52_setMarinerParam(S52_MAR_DISP_NODATA_LAYER, 0.0); // debug: no NODATA layer
    S52_setMarinerParam(S52_MAR_DISP_NODATA_LAYER, 1.0);   // default

    //S52_setMarinerParam(S52_MAR_DISP_AFTERGLOW, 0.0);  // off (default)
    S52_setMarinerParam(S52_MAR_DISP_AFTERGLOW, 1.0);  // on

    //S52_MAR_DISP_CENTROIDS      = 41,   // display all centered symb of one area (on/off) (default off)

    //S52_MAR_DISP_WORLD          = 42,   // display World - TM_WORLD_BORDERS_SIMPL-0.2.shp - (on/off) (default off)

    //S52_MAR_DISP_RND_LN_END     = 43,   // display rounded line segment ending (on/off)

    //S52_MAR_DISP_VRMEBL_LABEL   = 44,   // display bearing / range label on VRMEBL (on/off)

    //S52_MAR_DISP_RADAR_LAYER    = 45,   // display Raster: RADAR, Bathy, ... (on/off) (default off)

    // debug: draw all layer 0, then all layer 1, then all layer 2, ...
    //S52_setMarinerParam(S52_MAR_DISP_OVERLAP, 0.0);  // default
    //S52_setMarinerParam(S52_MAR_DISP_OVERLAP, 1.0);

    // CS DATCVY01:M_COVR:CATCOV=2, "M_COVR" OTHER
    //S52_setMarinerParam(S52_MAR_DISP_HODATA, 1.0);  // draw all M_COVR individualy
    //CS DATCVY01:M_COVR:CATCOV=1, "m_covr" BASE, (LUPT in PLAUX_00.DAI)
    //S52_setMarinerParam(S52_MAR_DISP_HODATA, 0.0);  // union: combite M_COVR as one poly 'm_covr' (default)

    //S52_setMarinerParam(S52_MAR_SHOW_TEXT,       0.0);
    S52_setMarinerParam(S52_MAR_SHOW_TEXT,       1.0);  // default

    //S52_MAR_GUARDZONE_BEAM      = 46,   // Danger/Indication Highlight used by LEGLIN&Position  (meters) [0.0 - off]
    //S52_MAR_GUARDZONE_LENGTH    = 47,   // Danger/Indication Highlight used by Position (meters, user computed from speed/time or distance)
    //S52_MAR_GUARDZONE_ALARM     = 48,   // FIXME: 1&2 ON at the same time. 0 - no error, 1 - alarm, 2 - indication
                                        // -1 - display highlight

    //S52_MAR_DISP_HODATA         = 49,   // 0 - union HO data limit "m_covr"(default), 1 - all HO data limit (M_COVR+m_covr)

    //*
    // debug - use for timing rendering
    //S52_setMarinerParam(S52_CMD_WRD_FILTER, S52_CMD_WRD_FILTER_SY);
    //S52_setMarinerParam(S52_CMD_WRD_FILTER, S52_CMD_WRD_FILTER_LS);
    //S52_setMarinerParam(S52_CMD_WRD_FILTER, S52_CMD_WRD_FILTER_LC);
    //S52_setMarinerParam(S52_CMD_WRD_FILTER, S52_CMD_WRD_FILTER_AC);
    //S52_setMarinerParam(S52_CMD_WRD_FILTER, S52_CMD_WRD_FILTER_AP);
    //S52_setMarinerParam(S52_CMD_WRD_FILTER, S52_CMD_WRD_FILTER_TX);
    //*/

    return TRUE;
}

/*
static gboolean _dumpParam()
{
    g_print("S52_MAR_SHOW_TEXT         t %4.1f\n", S52_getMarinerParam(S52_MAR_SHOW_TEXT));
    g_print("S52_MAR_TWO_SHADES        w %4.1f\n", S52_getMarinerParam(S52_MAR_TWO_SHADES));
    g_print("S52_MAR_SAFETY_CONTOUR    c %4.1f\n", S52_getMarinerParam(S52_MAR_SAFETY_CONTOUR));
    g_print("S52_MAR_SAFETY_DEPTH      d %4.1f\n", S52_getMarinerParam(S52_MAR_SAFETY_DEPTH));
    g_print("S52_MAR_SHALLOW_CONTOUR   a %4.1f\n", S52_getMarinerParam(S52_MAR_SHALLOW_CONTOUR));
    g_print("S52_MAR_DEEP_CONTOUR      e %4.1f\n", S52_getMarinerParam(S52_MAR_DEEP_CONTOUR));
    g_print("S52_MAR_SHALLOW_PATTERN   s %4.1f\n", S52_getMarinerParam(S52_MAR_SHALLOW_PATTERN));
    g_print("S52_MAR_SHIPS_OUTLINE     o %4.1f\n", S52_getMarinerParam(S52_MAR_SHIPS_OUTLINE));
    g_print("S52_MAR_DISTANCE_TAGS     f %4.1f\n", S52_getMarinerParam(S52_MAR_DISTANCE_TAGS));
    g_print("S52_MAR_TIME_TAGS         g %4.1f\n", S52_getMarinerParam(S52_MAR_TIME_TAGS));
    g_print("S52_MAR_BEAM_BRG_NM       y %4.1f\n", S52_getMarinerParam(S52_MAR_BEAM_BRG_NM));
    g_print("S52_MAR_FULL_SECTORS      l %4.1f\n", S52_getMarinerParam(S52_MAR_FULL_SECTORS));
    g_print("S52_MAR_SYMBOLIZED_BND    b %4.1f\n", S52_getMarinerParam(S52_MAR_SYMBOLIZED_BND));
    g_print("S52_MAR_SYMPLIFIED_PNT    p %4.1f\n", S52_getMarinerParam(S52_MAR_SYMPLIFIED_PNT));
    g_print("S52_MAR_DISP_CATEGORY   7-0 %4.1f\n", S52_getMarinerParam(S52_MAR_DISP_CATEGORY));
    g_print("S52_MAR_COLOR_PALETTE     k %4.1f\n", S52_getMarinerParam(S52_MAR_COLOR_PALETTE));
    g_print("S52_MAR_FONT_SOUNDG       n %4.1f\n", S52_getMarinerParam(S52_MAR_FONT_SOUNDG));
    g_print("S52_MAR_DATUM_OFFSET      m %4.1f\n", S52_getMarinerParam(S52_MAR_DATUM_OFFSET));
    g_print("S52_MAR_SCAMIN            u %4.1f\n", S52_getMarinerParam(S52_MAR_SCAMIN));
    g_print("S52_MAR_ANTIALIAS         i %4.1f\n", S52_getMarinerParam(S52_MAR_ANTIALIAS));
    g_print("S52_MAR_QUAPNT01          j %4.1f\n", S52_getMarinerParam(S52_MAR_QUAPNT01));
    g_print("S52_MAR_DISP_OVERLAP      z %4.1f\n", S52_getMarinerParam(S52_MAR_DISP_OVERLAP));

    // FIXME
    //g_print("S52_MAR_DISP_LAYER_LAST   1 %4.1f\n", S52_getMarinerParam(S52_MAR_DISP_LAYER_LAST));
    g_print("S52_MAR_DISP_LAYER_LAST     %4.1f\n", S52_getMarinerParam(S52_MAR_DISP_LAYER_LAST));

    g_print("S52_MAR_ROT_BUOY_LIGHT    2 %4.1f\n", S52_getMarinerParam(S52_MAR_ROT_BUOY_LIGHT));
    g_print("S52_MAR_DISP_CRSR_PICK    3 %4.1f\n", S52_getMarinerParam(S52_MAR_DISP_CRSR_PICK));
    g_print("S52_MAR_DISP_GRATICULE    4 %4.1f\n", S52_getMarinerParam(S52_MAR_DISP_GRATICULE));
    g_print("S52_MAR_HEADNG_LINE       5 %4.1f\n", S52_getMarinerParam(S52_MAR_HEADNG_LINE));
    g_print("S52_MAR_DISP_WHOLIN       6 %4.1f\n", S52_getMarinerParam(S52_MAR_DISP_WHOLIN));
    g_print("S52_MAR_DISP_LEGEND       3 %4.1f\n", S52_getMarinerParam(S52_MAR_DISP_LEGEND));
    g_print("S52_CMD_WRD_FILTER     F1-5 %4.1f\n", S52_getMarinerParam(S52_CMD_WRD_FILTER));
    g_print("S52_MAR_DOTPITCH_MM_X    F7 %4.2f\n", S52_getMarinerParam(S52_MAR_DOTPITCH_MM_X));
    g_print("S52_MAR_DOTPITCH_MM_Y    F8 %4.2f\n", S52_getMarinerParam(S52_MAR_DOTPITCH_MM_Y));
    g_print("S52_MAR_DISP_NODATA_LAYER F9 %4.2f\n", S52_getMarinerParam(S52_MAR_DISP_NODATA_LAYER));

    int crntVal = (int) S52_getMarinerParam(S52_CMD_WRD_FILTER);

    g_print("\tFilter State:\n");
    g_print("\tF1 - S52_CMD_WRD_FILTER_SY: %s\n", (S52_CMD_WRD_FILTER_SY & crntVal) ? "TRUE" : "FALSE");
    g_print("\tF2 - S52_CMD_WRD_FILTER_LS: %s\n", (S52_CMD_WRD_FILTER_LS & crntVal) ? "TRUE" : "FALSE");
    g_print("\tF3 - S52_CMD_WRD_FILTER_LC: %s\n", (S52_CMD_WRD_FILTER_LC & crntVal) ? "TRUE" : "FALSE");
    g_print("\tF4 - S52_CMD_WRD_FILTER_AC: %s\n", (S52_CMD_WRD_FILTER_AC & crntVal) ? "TRUE" : "FALSE");
    g_print("\tF5 - S52_CMD_WRD_FILTER_AP: %s\n", (S52_CMD_WRD_FILTER_AP & crntVal) ? "TRUE" : "FALSE");
    g_print("\tF6 - S52_CMD_WRD_FILTER_TX: %s\n", (S52_CMD_WRD_FILTER_TX & crntVal) ? "TRUE" : "FALSE");

    return TRUE;
}
*/

    ////////////////////////////////////////////////////////////
    //
    // setup internal variable to decent value for debugging
    //
    /*
    S52_setMarinerParam("S52_MAR_SHOW_TEXT",       1.0);
    S52_setMarinerParam("S52_MAR_TWO_SHADES",      0.0);
    S52_setMarinerParam("S52_MAR_SAFETY_CONTOUR", 10.0);
    S52_setMarinerParam("S52_MAR_SAFETY_DEPTH",   10.0);
    S52_setMarinerParam("S52_MAR_SHALLOW_CONTOUR", 5.0);
    S52_setMarinerParam("S52_MAR_DEEP_CONTOUR",   11.0);

    S52_setMarinerParam("S52_MAR_SHALLOW_PATTERN", 0.0);
    //S52_setMarinerParam("S52_MAR_SHALLOW_PATTERN", 1.0);

    S52_setMarinerParam("S52_MAR_SHIPS_OUTLINE",   1.0);
    S52_setMarinerParam("S52_MAR_DISTANCE_TAGS",   0.0);
    S52_setMarinerParam("S52_MAR_TIME_TAGS",       0.0);
    S52_setMarinerParam("S52_MAR_BEAM_BRG_NM",     1.0);

    S52_setMarinerParam("S52_MAR_FULL_SECTORS",    1.0);
    S52_setMarinerParam("S52_MAR_SYMBOLIZED_BND",  1.0);
    S52_setMarinerParam("S52_MAR_SYMPLIFIED_PNT",  1.0);

    S52_setMarinerParam("S52_MAR_DISP_CATEGORY",   S52_MAR_DISP_CATEGORY_STD);  // STANDARD (default)

    S52_setMarinerParam("S52_MAR_COLOR_PALETTE",   0.0);  // first palette

    //S52_setMarinerParam("S52_MAR_FONT_SOUNDG",    1.0);
    S52_setMarinerParam("S52_MAR_FONT_SOUNDG",     0.0);

    S52_setMarinerParam("S52_MAR_DATUM_OFFSET",    0.0);
    //S52_setMarinerParam("S52_MAR_DATUM_OFFSET",    5.0);

    S52_setMarinerParam("S52_MAR_SCAMIN",          1.0);
    //S52_setMarinerParam("S52_MAR_SCAMIN",          0.0);

    // remove clutter
    S52_setMarinerParam("S52_MAR_QUAPNT01",        0.0);
    */

    //S52_setMarinerParam(S52_MAR_SHOW_TEXT,       0.0);
    //S52_setMarinerParam(S52_MAR_SHOW_TEXT,       1.0);

    //--------  SETTING FOR CHART NO 1 (PLib C1 3.1) --------
    // Soundings      ON
    // Text           ON
    // Depth Shades    4
    // Safety Contour 10 m
    // Safety Depth    7 m
    // Shallow         5 m
    // Deep           30 m
    /*
    S52_setMarinerParam("S52_MAR_SHOW_TEXT",        1.0);
    S52_setMarinerParam("S52_MAR_DISP_CATEGORY",    S52_MAR_DISP_CATEGORY_OTHER); // OTHER
    S52_setMarinerParam("S52_MAR_TWO_SHADES",       0.0); // Depth Shades
    S52_setMarinerParam("S52_MAR_SAFETY_CONTOUR",  10.0);
    S52_setMarinerParam("S52_MAR_SAFETY_DEPTH",     7.0);
    S52_setMarinerParam("S52_MAR_SHALLOW_CONTOUR",  5.0);
    S52_setMarinerParam("S52_MAR_DEEP_CONTOUR",    30.0);
    */
    //-------------------------------------------------------
