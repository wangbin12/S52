# ChangeLog.md: the old way to note mods
#               The 'real' ChangeLog is generated with the Perl script cvs2cl.pl
#               OR git log
#


2018JAN10
- fix depcnt touch logic when mariner's SC not in ENC

2018JAN01
- refactor scamin attribs shortcut logic
- add ISODGR str test instead of scamin test

2017DEC23
- fix replace call to _buildPatternDL by _buildSymbDL

2017DEC11
- refactor unix signal to glib signal wrapper to allow interaction with g_main_loop (if prsent)
- mod set scamin to SCAMIN or INFINITY to debug CS WRECK02 / OBSTRN04 after reset in _UDWHAZ03()

2017DEC10
- move backtrace, atomic, signal form S52.c to S52utils.c

2017DEC06
- fix algo for finding arc extend on LIGHTS05 when sector overlap (in AA5TDS05.000)

2017DEC05
- mob invariant check [0..360[ in S52_PL_setSYorient()

2017NOV16
- mod PL lookUp to match first best match rather than the last
- mod CS touch() logic for LIGHTS, BOYLAT

2017NOV12
- add macro _g_string_free() in CS to nullify var, complitness with _new()
- replace _intersectGEO() in CS with call to S57_cmpEXT()

2017NOV11
- simplify geo of ISTZNE, TSSLPT, CTNARE to optimise centroid computation
- add build option -DS52_USE_CA_ENC to debug CA ENC in PL lookUpLUP()
- fix touch logic in CS
- replace test in CS to attVal OBJL by test to S57 obj str name
- rename _intersectEXT() to _intersectCELL() to highlight special handly of anti-meridian

2017OCT28
- fix rounding in GL _ddmmss()

2017OCT27
- add S57_RESET_SCAMIN to doc magic nbr -1.0 to signal CS to reset S57 obj SCAMIN
- fix touch() logic 
- fix AP(FOULAR) SY(ISODGR) in shallow
- rename S57_isPtInArea() to S57_isPtInRing()
- add S57_isPtInArea() to fix touch() logic around island
- fix PL lookUp() for AP(FOULAR), add S57_getAttValALL() for EMPTY_NUMBER_MARKER

2017OCT24
- fix CS(OBSTRN04) remove command AP(FOULAR01);LS(DOTT,2,CHBLK) in NO valsou branch - allready in PLib
- fix PL _lookUpLUP() special case (i) separator ' ' to '\0'
- renamme PL:_chopAtEOL to _chopAtUS()
- rename #def EOL by US (0x1F - Unit Separator)
- add S57_getAttValALL() to return also unknown value for _lookUpLUP()

2017OCT16
- fix DEPCNT02 algo adding test pt in set
- refactor DEPCNT02 add debug code
- split geo ref to depare_list to depcnt_list and udwhaz_list

2017OCT02
- mod handle uniformly CS UNKNOWN depth usage
- add safe gstring_new in CS
- add stat for S57_geo attribs
- add debug code for UFT8
- clean up scamin logic

2017SEP26
- refactor SY_vessel() in GL

2017SEP22
- add major minor graticule

2017SEP20
- refactor VESSEL red/close quarter into warning/indication logic
- deprecate S52_USE_SYM_VESSEL_DNGHL
- refactor SCAMIN logic and add scaling factor

2017SEP16
- fix GPU/VBO leak at exit in delDL() text

2017SEP10
- refactor Text code path

2017SEP06
- refactor OWNSHIP/VESSEL text handling and test handling of AIS text

2017AUG28
- refactor GL begin
- refactor pick cycle
- add test AIS msg type 18
- fix NULL touchDEPARE() in CS/DEPCNT02

2017JUL22
- fix defective refactoring in S52_GL_getViewPort()

2017JUL15
- fix symb highlight on pick
- refactor S52_PL_getDListData()

2017JUN06
- fix palette validation

2017JUN02
- add _simplifyGEO() to delete redundant vertex

2017MAY11
- refactor 'double xyz[]' to 'pt3 pt[]'

2017MAY09
- refactor _drawLegend()

2017MAY07
- mod aggregate all point struct in S57data.h

2017MAY06
- refactor to Hash S52_obj list
- refactor S52_obj Aux Info to struct

2017MAY04
- refactor validation of user Mariners Parameter

2017MAY03
- mod refactor to _getCentroid() former ..Open/..Close

2017MAY02
- mod rename S52_GL_movePoint() to local func _movePoint()
- fix typo reslove to resolve
- mod lengend extract to struct
- mod lineOverlap global

2017MAY01
- mod refactor renderBin traversal
- mod rename def cchar to CCHAR

2017APR28
- mod refactor ptrArray to call free_func _delObj() on free array
- del unused scale member in cell

2017APR24
- mod refactor _delMarObj()

2017APR19
- mod check if palette has changed before re-loading RGB in S52_PL_getDListData()
- mod refactor raster code path
- mod refactor renderBin traversal
- mod refactor S52_PL_getGeo() into S52PLGETGEO(), mod S52_obj
- add _doAPP_RASTER to flags that bathy texture need be reseted

2017APR14
- fix mem leak of META obj when relinking to new PLib on loadPLib()

2017APR08
- mod kludge udwhaz04 diff into CS udwhaz03 to get approx S64 NE.000 plot
- mod refractor _revArray() use to reverse array from CCW to CW (S57 winding)

2017APR03
- fix light extend_arc_radius sector when not overlapping
- mod _setFragAttrib() to handle highlight
- add S57_setHighlight(ON/OFF) to replace HighlightON HighlightOFF
- mon replace atoi/atof by g_ascii_strtod() local agnostic

2017APR02
- refactor APP/moveObj()
- mod set to TRUE prioOveride with default LUP prio
- refactor S57 to S52 obj type convertion

2017MAR31
- mod refactor APP/overide prio
- add combine scale boundary
- add S52_PL_isPrioO()
- mod S52ObjectType S52_PL_getFTYP()
- del M_COVR test in S52_GL_isSupp()

2017MAR23
- del S52_USE_GOBJECT not use anymore
- mod S52_MAR_DISP_HODATA to S52_MAR_DISP_HODATA_UNION
- add S52_MAR_DISP_SCLBDY_UNION
- mod *Begin() to *Beg()

2017MAR22
- mod rename _moveObj() to _appMoveObj()
- mod refactor scale boundary generation
- add scale boundary union

2017MAR20
- fix use bsize param in _renderTXTAA()
- fix scale boundary to draw only M_COVR:CATCOV=1

2017MAR18
- mod rename S57_getGeoS57ID to S57_getS57ID
- mod comment format; NOTE to Note
- fix HO data limit on doneCell()
- mod leglin text bsize form 12 to 11

2017MAR03
- mod refactor S52GL.h:extent to S57data.h:ObjExt_t

2017FEB27
- fix _intersecGEO() S-S comp
- mob rename all 'geoData' var to 'geo'

2017FEB20
- fix PRINTF str prefix
- fix draw handling when no cell loaded
- fix extent uniform handling
- mod _rename setFragColor() to _setFragAttrib()
- fix _TRANSLATE handling

2017FEB07
- fix clang -fsanitize=address in _renderTXT() and S52_PL_getNextVOCmd() read past buffer

2017FEB03
- fix lcms2 code path

2017FEB02
- add port to lcms2
- mod lcms1 code to use RGB instead of sRGB profile
- add def S52_USE_LCMS2

2017JAN28
- fix regression in s52eglx
- del S52_USE_MESA3D in test/
- mod move DislayList func to _GL1.i

2017JAN26
- fix regression of s52glx s52gtk2 in matrix handling
- fix ifdef for s52gtk2gl2

2017JAN20
- modx end EGL before releasing mutex
- mod move _view_t struct to GL
- add S52_GL_getView() to GL
- fix behavior of test/s52gtkegl

2017JAN18
- fix shader regression
- fix lock mutex from AIS
- add NULL check in set/get freetypeGL
- add init done AIS in test/s52gtkrgl.c

2017JAN11
- add implementation of rotation in drawBlit()
- add basic test of drawBlit() (drag/zoom/rot) with gtk3 gesture in test/s52gtkegl.c

2016DEC28
    -mod rename S52_USE_MESA3D TO S52_USE_DUAL_MON

2016DEC27
    -mod refactor text justification

2016DEC22
    -rename S52_cmdmDef to S52_symDef
    -move freetype-gl data to _Text
    -move code for planned speed renderSY_leglin() to renderTXT()
    -mod leglin CS for correct TX string handling
    -add text V/H justification

2016DEC13
    -fix chart rotation for bouy

2016DEC12
    -fix pattern in gl2

2016DEC06
    -mod scissor when chart rotated

2016DEC04
    -mod georef layer 0 NODATA pattern
    -fix gl2 chart rotation
    -fix culling for chart rotation

2016NOV29
    -fix cppcheck 1.72 warning

2016NOV28
    -mod rename to _fixDPI_glSetScaled()
    -mod use layer 0 for NODTA group 1 area

2016NOV12
    -fix gl2 line stipple
    -add _renderLS_gl2() to _GL2.i

2016OCT23
    -fix clang --analyse warning
    -mod _DEPVAL01
    -mod libtess
    -mod freetype_gl
    -mod parson

2016OCT06
    -mod make target default to s52gtk2egl
    -del noise experiment

2016OCT04
    -clean up test/s52ui/web/js/tinyjet
    -clean up GL cycling code

2016SEP20
    -mod refactor S52_GL_begin()
    -add GLES2/EGL robustness EXT

2016SEP07
    -add proper call for GLSC2 code path - need testing if/when driver capable

2016SEP03
    -add GLSL load/save bin prog

2016AUG09
    -add test for 8MSAA
    -add doc for X macro, fix off by one for text array bound
    -start porting to GLSC2 .. need GPU support for testing
    -add GLSC2 option, del default include path, link with gcc
    -fix compiler warning in utils, update version string for GLSC2
    -add glsc2.h, clean not used include in _GL2.i
    -fix log cb

2016AUG02
    -upgrade to GCC 5
    -mod __FUCTION__ to C99 __func__ in PRINTF
    -del old AA code in GL_begin()

2016JUL28
    -mod EGL test default to 4MSAA

2016JUL05
    -mod rename to _setFragColor

2016JUL04
    -fix indirection bug in _app()/DATCVR01 repport by chenzunfeng
    -fix comparison bug at same line as above

2016JUL02
    -fix bug repported by chenzunfeng in CS OBSTRN04 cont. B, 'quapos' / 'udwhaz03' / 'valsou'
     should be mutually exclusive

2016JUN29
    -fix bug repported by chenzunfeng in CS OBSTRN04 is LS(DASH,2,CHGRD) not CHBLK
    -fix bug repported by chenzunfeng in CS OBSTRN04 add g_string_append(obstrn04str, ";SY(OBSTRN03)");
    -fix bug repported by chenzunfeng in CS OBSTRN04, if (UNKNOWN != least_depth) should be '=='

2016JUN25
    -fix bug repported by chenzunfeng in CS _DEPVAL01
    -mod rename _setFragment to _setFragAtt

2016MAY26
    -mod rename _glColor4ub to _setFragment
    -mod _setFragment call _glColor4ub
    -mod refactor _glColor4ub

2016MAY18
    -mod debug config CONF_* to CFG_*
    -add atomic abort in _suppLineOverlap()
    -fix check for short OGR_TEMP_BUFFER_SIZE
    -add check for disp cat before SCAMIN
    -fix CFG parser in utils.c

2016MAI01
    -fix call to log_cb when logging to file

2016APR29
    -mod refactor test/, unify duplicate test code

2016APR23
    -del S52_USE_LOG as log_cd is used instead
    -fix NULL DList in case of symb missing in AUX

2016APR09
    -fix line overlap for simple line (LS())

2016APR06
    -mod draw mariners' during cells rendering rather than last cell in draw()

2016MAR26
    -mod S57_getExt() return FALSE if extent not set ($CSYMB, ..)
    -add system generated CS DATCVR01-2 - HO Data Limit,
    -mod call to _newMarObj() ok if xyz=NULL
    -add system generated CS DATCVR01-3 - SCALE BOUNDARIES (sclbdy)

2016MAR20
    -add S52_MAR_GUARDZONE_ALARM to S52MarinerParameter
    -add S52_MAR_DISP_HODATA to S52MarinerParameter

2016MAR16
    -add S52_VERSION in S52.h
    -mod utils.c _version

2016MAR10
    -add glScissor experiment to clip raster at carte boundary

2016MAR08
    -fix handling CS for M_COVR:CATCOV=1 / M_COVR:CATCOV=2

2016MAR06
    -mod refactor, cleanup SUPP_LINE_OVERLAP code

2016MAR04
    -fix regession in SUPP_LINE_OVERLAP after refractoring
    -add render LC() inner ring

2016FEB26
    -add S52_USE_RASTER code path to handle bathy .tiff

2016FEB18
    -fix handling of isinf() call

2016FEB16
    -mod code clean up
    -mod update copyright year

2016FEB13
    -mod comment dead code GLib-1 and GLIB2 ifdef
    -add 'utils' to func name in S52utils

2016FEB12
    -mod comment experiment origAW
    -mod code clean up

2015DEC03
    -mod code formatting, version() in S52utils
    -mod replace g_ascii_strncasecmp() by strncmp() - FAIL in PL
    -add def S57_OVERLAP_GEO_Z, enum S57_RCNM_t in S52data.h

2015NOV24
    -mod code formatting
    -mod update GIR

2015SEP04
    -add print GDAL_RELEASE_NAME (version) to S52_init()

2015JUN02
    -mod update doc/

2015MAY31
    -mod refactor S52_getAttList() to use S52_PL_isObjValid()
    -mod refactor use S52_PL_isObjValid() instead of _getS52obj()

2015MAY29
    -fix dandling ptr - remove warning in S52.h

2015MAY23
    -fix legend rendering
    -add gard to prevent S52_drawStr before GL init
    -mod rename S52_GL_drawStr to S52_GL_drawStrWorld
    -mod rename S52_GL_drawStrWin to S52_GL_drawStr

2015APR29
    -fix resume on Android

2015FEB26
    -mod refactor call _isObjValid() replace by S52_PL_isObjValid()

2015FEB25
    -fix VRMEBL extent
    -update copyright years
    -fix S52_PL_getObjToggleState() wrong check for MAR STD + OTHER

2015JAN28
    -add CLang compiler
    -fix CLang warning
    -mod geo->id to geo->S52ID
    -mod func name to getGeoS57ID()
    -mod geo->name from GString to char[]

2015JAN18
    -fix _objList size to increment it until it is greater than S57ID
    -fix Makefile LIBVERS

2015JAN11
    -fix S52_version() to return valid JSON
    -add version check in test/s52ui

2014DEC26
    -mob enum S57_Obj_t add prefix S57_
    -add S52_obj lookup indexed with S5_geo.ID
    -mod rename S52_GL_del to S52_GL_delDL
    -del _objToDelList as Mariner obj are del imediatly
    -mod renderBin[][N_OBJ_T] to [52_N_OBJ]
    -mod refactor _delObj()
    -mod layer type max to S52_N_OBJ
    -mod refactor S52_loadPLib()
    -mod _isObjValide to check S52_PL_isObjValid()

2014DEC20
    -add S52_PL_cpyAux() to copy S52_obj Aux Info
    -mod move supp field in S57_geo to S52_obj Aux Info
    -mod move get/set supp from S57data to S52PL

2014DEC19
    -mod optimisation, cull obj at drawLast()
    -mod setViewPort in test/s52glx.c
    -del pen_w in _S52_vec / _S52_cmdDef
    -del S52_PL_getVOwidth() / S52_PL_setLCdata()

2014DEC18
    -fix unlock mutex in S52_newCLRLIN()
    -fix VP in GL1 code path

2014DEC17
    -fix regression, put back g_string_equal() in utils
    -fix use VP.h for north arrow

2015DEC14
    -mod refactor _filterVector()
    -fix _filterVector() for line width change in symb in VBO code path

2014DEC12
    -mod logic of S52_MAR_VECMRK in CS moved to GL
    -fix GL logic of VECMRK, VECPER

2014DEC10
    -mob resolve mariner SMB CS at creation time (new) rather than in _app()

2014DEC09
    -mod move libS52wiki.html to doc/ and remove doc/wiki

2014DEC08
    -mod clean up #include / deps
    -add doc/graphic/libS52_files_deps-1.152-2014DEC08.dia / .svg

2014DEC07
    -mod rename S52_getMarObjH to S52_getMarObj
    -add doc/graphic/Call_FlowChart-1.152-2014DEC07.dia / .svg
    -mod rename S52_setEGLcb to S52_setEGLCallBack
    -mod rename EGL_cb to S52_EGL_cb

2014DEC04
    -mod initialize text group to TRUE (ON)

2014DEC03
    -mod refactor _vp[] to struct x,y,w,h

2014DEC02
    -add code / doc in S52.h  to validate calls order
    -add S52_GL_INIT to GL_cycle
    -add code to guard using GL Projection Matrix before intialising it

2014DEC01
    -mod refactor and add guard from calling PROJ4 before initialising it

2014NOV29
    -del unneeded S52.c:_win2prj()
    -mod move S52_USE_PROJ to S57data.c PROJ4 stuff

2014NOV28
    -mod prevent using S52_xy2LL() / S52_LL2xy() before init Projection View
    -add code in test/s52egl.c to test LEGLIN & guard zone

2014NOV27
    -mod deprecate S52_toggleObjClass/ON/OFF

2014NOV23
    -start handling alarm / indication for GuardZone (LEGLIN)
    -del unused S52_string_equal()

2014NOV10
    -fix _tmp.tif path in GL_dumpPixles()
    -mod remove useles mutex unlock in newLEGLIN()

2014NOV09
    -fix afterglow VESSEL_DELAY time out

2014NOV05
    -fix mariner obj extent for culling
    -mod rename S52_DListData to S52_DList
    -fix ftgl for dotpitch

2014OCT27
    -fix typo in suppression, Supp, ...
    -mod skip culling mariners' separetly in _cull()
    -mod rename S52_GL_isOFFscreen() to S52_GL_isOFFview()
    -mod rename S52_PL_getToggleState() to S52_PL_getObjToggleState()
    -fix g_assert(0) to return something in case of G_DISABLE_ASSERT

2014OCT25
    -add S52_USE_LOGFILE to split LOG and LOG_to_FILE code path

2014OCT17
    -mod S52_MAR_DISP_CRSR_POS --> S52_MAR_DISP_CRSR_PICK
    -add optimisation (failed) using tess tolerance (not implemented in libtess) for LOD

2014OCT06
    -mod S52_error_cb to S52_log_cb
    -mod setViewPort at init()
    -mod move _version to utils.c

2014OCT02
    -fix update _crntMat in _glPopMatrix

2014SEP26
    -mod SPRINTF by SNPRINTF
    -mod internal version 1.x

2014SEP10
    -mob GDAL env. var move to test/*

2014SEP04
    -mod move EMPTY_NUMBER_MARKER to S57data.h
    -fix CS wreck above water

2014AUG21
    -fix s52win32 code path

2014JUN27
    -fix refactor _insertS57Obj()

2014JUN18
    -fix synthetic after glow for the GL2 code path

2014JUN16
    -fix and refactor light sector
    -add some feedback in s52ui
    -add S52_USE_GL3 / S52_USE_GLES3 / GLSL ES 3.0

2014JUN05
    -mod refactor _parseWS() in _S52.i to avoid recursion

2014JUN02
    -mod refactor socket code in _S52.i
    -fix WebSocket bug when a stream contain more than 1 msg

2014MAY29
    -mod split tess stuff in S52GL.c to _GLU.i

2014MAY28
    -add S52_USE_GLSLES for the shader in preparation of refactoring GLES2 to GL2
    -mod S52_USE_GLES2 to S52_USE_GL2 to better show the GL1/GL2 split

2014MAY26
    -del A3D font experiment (S52_USE_A3D)

2014MAY24
    -mod refactor & clean-up

2014MAY23
    -mod split socket stuff in S52.c to _S52.i

2014MAY20
    -mod start to spliting S5GL.c to _GL1.i / _GL2.i for GLx specific stuff

2014MAY19
    -mod S52_drawStr() validate pixels_x, pixels_y
    -mod skip AP on GL1.x, pattern broken

2014MAY15
    -mod S52_MAR_DISP_RASTER to S52_MAR_DISP_RADAR_LAYER

2014MAY13
    -fix LC completion

2014MAY10
    -fix target s52glx, s52gtk2 - GL1.x

2014MAY09
    -mod refactor GL2/GLES2

2014MAY08
    -fix pattern for GLES2 (no stag)
    -add optimisation merge point symbol to line as short line

2014MAY07
    -add -DS52_USE_MESA3D to get specific Mesa3D glsl version
    -add TTF label to ./test/s52.cfg file for testing

2014MAY06
    -fix UTF-8 text handling in GLES2 (freetype_gl)
    -fix delete AIS lines when delay expire

2014MAY04
    -del #ifdef in S52.h, move to S52.c
    -mod S52_MAR_NONE to S52_MAR_ERROR
    -add #version 120 in frag shader to get gl_PointCoord (Mesa3D specific)

2014MAY03
    -add def S52_USE_GL2 for reusing GLES2 code but skip some
    -add optimisation of simple line ending in an LC command

2014MAY01
    -del def S52_USE_DOTPITCH - not used anymore

2014APR29
    -add target s52gtk2gl2, GL2 use the code path of GLES2 (a subset of GL2.x)
    -add signal Mesa3D no vSync for faster drawLast() call

2014APR26
    -fix build of s52gtk2

2014APR25
    -fix build of s52glx

2014APR19
    -fix compiler warning (static mutex deprecated)
    -fix mem corruption in GL mask of pattern

2014APR17
    -add optimisation of symbol with GL_LINES
    -mod optimisation collecte all GL_POINTS of a symbol (eg. OBSTRN11)

2014APR13
    -mod stabilize the radar image
    -mob free S52_RADAR_cb if texRadius is 0

2014APR12
    -add black background in RADAR mode when and NODATA is off

2014APR11
    -add lat/lng to radar_cb,
    -mod handle more than one radar

2014APR10
    -fix text artifact using TRIANGLES instead of STRIP

2014APR09
    -add Radar, compile with S52_USE_RADAR

2014APR01
    -mod optimisation tesselate into triangles only

2014MAR25
    -fix static text length

2014MAR24
    -mod optimze DRAW cycle static text
    -mod optimise centroid to recompute only those affected when view change

2014MAR22
    -mod test/android/dist/system to test/android/dist/sysroot
    -mod lib/freetype-gl/texture-atlas.c to match diff in git glDelete..()

2014MAR16
    -add GLES2 text shadow, compile with S52_USE_TXT_SHADOW
    -mod optimisation of text rendering to STRIP instead of FAN

2014MAR12
    -update GPSD client on ARM

2014FEB17
    -add sync raster / vector projection

2014FEB13
    -add target s52eglw32, EGL/GLES2 for Win32
    -add test/s52gtk2egl.c
    -mod folder lib/tesselator to lib/libtess
    -fix tess callback for win32

2014FEB04
    -fix handle anti-meridian

2014JAN23
    -fix _initPROJ() coord switch bug

2013DEC27
    -mod join TRIANGLE_STRIP
    -mod cull face GL_BACK

2013DEC09
    -add GDAL projection to Mercator for raster (bathy)

2013NOV26
    -mod upgrade s52droid & s52ui for Android 4.4 KitKat to get chromium in WebView

2013NOV11
    -add experimental S52_USE_SYM_VESSEL_DNGHL to draw in red
    -mod remove heading line on ARPA

2013NOV05
    -mob use GLES3 for Adreno (android) to get NPOT texture in blit()
    -mod s52ui to account for devicePixelRatio for the Nexus 7

2013OCT13
    -fix Adreno swapbuffer handling while saving draw() FB to texture
    -fix eglMakeCurrent EGL_BAD_ACCESS Error msg on Android

2013OCT11
    -mod remove unneeded android uses-permission

2013OCT10
    -fix s52ui GPS with Android 4.2
    -fix cursor pick after refactoring S52_GL_mode

2013SEP28
    -fix Android rotation, do not set EGL Native Window Geometry

2013SEP22
    -fix do _initPROJ() once only

2013SEP19
    -mod s52eglarm to link directly s52ais to s52egl
    -mod s52ais to get AIS data from a GPSD running on the network
    -mod remove the folder s52droid/bin on android and the need to get root access

2013SEP18
    -add doc/s57filecollector.cpp.diff (gdal/ogr patch to fix DOS path in CATALOG.031 on Unix)

2013SEP16
    -fix unlock mutex in S52_getS57ObjClassSupp()

2013SEP15
    -add -DS52_USE_C_AGGR_C_ASSO to get experimental S-57 relationship C_AGGR & C_ASSO

2013SEP14
    -add refactor S52_GL_mode to track GL DRAW/LAST/PICK/BLIT and clarify the state/mode

2013SEP13
    -fix bug when S52_MAR_DISP_NODATA_LAYER is OFF

2013SEP10
    -mod bring up Adreno (Nexus 7)

2013SEP04
    -fix sector lights
    -add test/s52gtk3egl.c

2013AUG27
    -fix not needed inclusion of glu.h in GLES2

2013AUG26
    -fix path for world shapefile
    -fix default build flag to skip experimental stuff

2013AUG24
    -fix s52gtk2 build
    -mod refactor pattern tilling in GLES2 code

2013MAY21
    -add Raster .tif, must be allready projected to Mercator

2013APR28
    -fix rendering of accented char in TE & TX
    -add doc/ogrfeature.cpp.diff, to document the change on OGR to compute line overlap

2013APR21
    -fix cursorPick GEO filter

2013APR20
    -fix align pixels origin to OpenGL Lower Left Corner

2013APR13
    -fix defect in loadPLib() that mess up S52ObjectHandle

2013APR11
    -add resolve relationship (C_AGGR/C_ASSO) for cursor pick

2013APR10
    -fix line overlap that suppress one to may segment in a line

2013APR07
    -add PLib exposition field: LXPO/PXPO/SXPO to cursor pick

2013APR05
    -add test for cursor pick in s52ui

2013MAR26
    -add device rotation handling to s52droid (s52ui.dart)

2013MAR24
    -add device rotation handling to s52droid (s52egl.c)

2013MAR22
    -add timer to afterglow
    -mod rename project S52android to s52droid

2013MAR19
    -add S52_getView() to make s52ui Dart code simpler
    -add S52_setEGLcd() to drive EGL makecurrent/swap
    -add S52_USE_EGL in S52.c
    -add touch handling in s52ui
    -mod S52.dart refactor
    -add stopWatch in S52.dart
    -mod s52android to s52droid

2013MAR13
    -fix CSS layout (s52ui)
    -fix line supression regression

2013MAR12
    -move to S52.dart stuff that mimic S52.h
    -add test to Rot. Buoy Light in s52ui

2013MAR10
    -fix s52ui html table wrongly resized when redisplay
    -add test to continously Draw() from s52ui via vesselSelect(1) by following AIS

2013MAR03
    -add AIS list to s52ui (hence to Android also)

2013FEB18
    -fix WebSocket on Android

2013JAN23
    -fix ship's label, fix free moving VRMEBL

2013JAV18
    -fix/refactor VRMEBL on GLES2, add TX to cursor in PLAUX.DAI

2013JAN09
    -fix rotation of buoy light (g_strcmp0())

2013JAN08
    -fix S52_doneCell() from socket (s52webUI)

2013JAN05
    -fix text rotation for GLES2

2012DEC12
    -add Websocket server - exprimental
    -add dep. 'lib/parson' to handle JSON from Websocket

2012NOV20
    -fix C1 ed3.1, mariners' objects are part of the ENCs

2012NOV19
    -fix C1 ed3.1 AA5C1ABO.000 $SCODE point symb on GLES2

2012NOV18
    -mod logic to S52_MAR_DISP_CATEGORY, one call to set many OR'ed flags

2012NOV17
    -fix 'TAP/long TAP' logic on Android: 'cursor pick/vrmebl'

2012NOV14
    - from https://git.kernel.org/?p=linux/kernel/git/kay/libabc.git;a=blob_plain;f=README
           "Don't put "extern" in front of your function prototypes in headers
           - It has no effect, no effect at all."
    -remove 'extern', it has no effect, really .. (except in S52CS.h)
    -add world shapefile

2012NOV01
    -add lib parson, replace TAB with NL in JSON

2012OCT20
    -add source code to github

2012OCT05
    -mod s52/doc, add graphic/ and wiki/

2012OCT01
    -add exit(0) if env. var. S57_CSV not found, because GDAL need it
     to work propely

2012SEP26
    -add experiment increse text kerning and decrese text size
     at new line char (TAB char, since '\n' break JSON/socket string)

2012SEP25
    -add S52_MAR_DISP_LAYER_LAST_SELECT to S52_MAR_DISP_LAYER_LAST
    -add distclean target to Makefiles in S52/ S52/test/ S52/test/android
    -mod round point in GLES2 shader

2012SEP24
    -refactor display category handling

2012SEP23
    -refactor UI color xml element to a template

2012AUG26
    -refactor projection code

2012AUG21
    -add pattern rendering for GLES2
    -improve external sensor handling

2012JUL25
    -fix regression for target s52gtk2

2012JUL09
    -add renderAP() in GLES2 (preliminary)

2012JUN29
    -fix renderLC() line ending

2012JUN28
    -fix GLES2 arc
    -fix renderLC() dotpitch

2012JUN27
    -fix GLES2 symbol with points

2012JUN19
    -add doc in README
    -fix PID path for s52eglx

2012JUN18
    -add GLES2/ARM (Android)

2011OCT14
    -mod start using EGL in test/ (might be included in libS52 later on)

2011JUL26
    -mod start to use VBO instead Display List (OpenGL 1.5)

2011JUL22
    -fix get S52_MAR_DISP_CATEGORY, return number instead of char
    -add S52_getS57ObjClassSupp() / S52_setS57ObjClassSupp()
    -add S52_getAttList()
    -add dbus for new call
    -add display S57 object attributes in dashboard
    -add User Selected (mouse) in dashboard
    -mod various clean up

2011JUN24
    -add S52_dumpS57IDPixels() to help debug pixels fragments regression

2011MAY31
    -mod name S52_getObjNameList() to S52_getS57ObjClassList
    -add S52_getObjList()
    -fix missing dbus filter call

2011MAY28
    -add dbus for S52_getObjNameList() and S52_getCellNameList()

2011MAY19
    -add S52_getObjNameList() to get S57 object list in a cell
    -add S52_getCellNameList() list of loaded cell name

2011MAY15
    -add dbus handling for S52_loadCell() and S52_loadPLib()

2011MAY07
    -mod load PLib colors using palette name
    -add S52_setTextDisp() and S52_getTextDisp() to filter text

2011MAY04
    -mod init S52_MAR_NONE to ININITY
    -add XUL/xuljet dashboard for testing (experimental)
    -add dbus method for S52_getPalettesNameList() and S52_getPLibsIDList()

2011APR26
    -add S52_getPalettesNameList() that return a list of palette name loaded

2011APR22
    -fix type in param 'vescre' (Vessel report source)
    -mod rename param 'maxpts' to 'xyznbrmax' (ie same as S52_newMarObj())
    -mod para lat/lon Start to Begin in S52_newLEGLIN()
    -add some doc in S52.h

2011APR20
    -fix lights sector culling for case of same light on 2 cell out view

2011APR18
    -add S52_getPLibsID() that return a string of PLib loaded

2011APR14
    -add ChangeLog.OLD the old way to note mods
    -mod now ChangeLog generate itself with 'cvs2cl'
    -add trap SIGINT to stop rendering
    -add trap SIGSEGV to print backtrace when segfault

2011APR08
    -mod doc in S52.h for introspection annotation and doc generation
    -mod vessel to show silhoutte and symbol
    -mod select vessel symbol on silhoutte

2011MAR31
    -mod show ship silhoutte at 6mm instead of 10mm

2011MAR27
    -mod S52_newLEGLIN() add previousLEGLIN parameter

2011MAR25
    -mod rename S52_toggleObjSUP to S52_toggleDispMarObj

2011MAR17
    -mod rename S52_objType to S52ObjectType for instrospection
    -mod rename S52_objHandle to S52ObjectHandle for introspection
    -mod rename enum S52ObjectType suffix _T removed

2011MAR16
    -fix multiple text on an S52_object
    -mod S52_MAR_param_t to	S52MarinersParameter for instrospection
    -mod S52MarinersParameter emun to start with S52_MAR_ for instrospection

2011MAR15
    -add more annotation
    -mod call s52gpsd_initAIS() to test/s52gtk2.c:realize() because data start
         to come in before the main loop is up

2011MAR06
    -add test/s52clutter.js (does that same as s52gtk2 but with clutter & Javascript)
    -add annotation in S52.h to make test/S52-1.0.typelib for test/s52clutter.js

2011MAR04
    -mod S52_init() param name from pixels_.. to screen_..
    -mod draw() and drawLast() to trylock() mutex instead of lock()

2011MAR03
    -mod put test/s52gpsd.c:_gpsdClientInit() in a thread (was in main loop)

2011MAR01
    -fix curve

2011FEB28
    -mod move 'vestat' param from S52_newVESSEL() to S52_setVESSELstate()

2011FEB15
    -mod copyright years 2000-2011

2011FEB14
    -add S52_NODATA_LAYER_OFF to supress display of layer 0 (NODATA)

2011FEB13
    -fix draw arc leg at correct display priotity (mod S52_setRoute())

2011FEB09
    -fix do VESSEL CS update when calling S52_drawLast()
    -add S52_setState() for VESSEL (more param will be added as need be)

2011JAN13
    -add start code for DBus IPC

2010DEC29
    -add 'waypnt' to S52_updObjGeo() (ie object that can be updated)

2010DEC27
    -fix update text lebal at each call to S52_setVESSELlabel()

2010NOV30
    -fix cursor pick of centroid

2010NOV26
    -fix centroid

2010NOV22
    -add S52_DRGARE_PATTERN_OFF - do not display DRGARE pattern (default off)
    -fix vector for symbol WAYPNT11 (PLAUX_00.DAI)

2010NOV15
    -fix _route initialization

2010NOV09
    -fix overlapping lights sector of same size

2010NOV07
    -mod text prio for VESSEL01 & OWNSHP02 set to 11

2010NOV02
    -mod if seed is zero do not show vector
    -add S52_MAR_DISP_CALIB to dispaly calibration symbol

2010OCT17
    -add cursor pick highlight
    -fix safety contour

2010OCT02
    -add partial line overlap handling (experimental)
    -add update VRMEBL when updating OWNSHP
    -add label to ownshp
    -add for_each() loop to draw over/under radar object
    -mod rename Mariners' object call S52_ini*() to S52_new*()

2010SEP21
    -add mutex
    -add S52_DOTPITCH_MM_X, S52_DOTPITCH_MM_Y to test dotpitch problem (experimental)

2010SEP18
    -fix clipping bug that draw LC outside screen
    -fix un-rotate text
    -add S52_toggleObjSUP() to supress display of a Mariners' Object

2010SEP17
    -add CHKSYM01 to S52_iniCSYMB()
    -add BLKADJ01 to S52_iniCSYMB()
    -add OWNSHP05 corrected vector in auxiliary PLib (PLAUX_00.DAI)

2010SEP15
    -add S52_LL2xy()
    -remove dep to pango
    -move AIS code to test/s52gpsd
    -mod S52_setVRMEBL() to return brearing and range (experimental)
    -add test to display bearing and range at cursor with S52 UI color

2010SEP12
    -fix mem leak detected by valgrind

2010SEP03
    -add AIS trail - synthetic afterglow (experimental)

2010SEP02
    -add gpsd AIS feed in s52gtk2.c
    -fix mem leak in text (TE & TX)

2010AUG30
    -add S52_drawStr()
    -add example of overlay in s52gtk2.c (graphical _dumpParam())

2010AUG29
    -add example (in test/s52gtk2.c) of raw OpenGL call
    on top (after) S52_draw()
    -add FTGL for text

2010AUG28
    -add NODATA pattern on layer 0

2010AUG26
    -fix arc

2010AUG25
    -add turn on/off object base on command word type (profiling)
    -fix adding stuff on chart beteween Draw() DrawLast()

2010AUG16
    -fix dandling ref of old lights_sector

2010AUG14
    -add ENC coverage symbol

2010AUG13
    -add silhoutte on AIS
    -add S52_setDimension()
    -mod S52_iniOWNSHP()
    -add waypoint in test/s52gtk2.c
    -mod clip leg when drawing arc

2010AUG12
    -fix parsing vector for symbole OBSTRN11

2010AUG11
    -add Mariners' Object extent for culling

2010AUG10
    -mod API for VRMEBL
    -del S52_updateObjGeo()

2010JUL24
    -add more check to parameter

2010JUL18
    -fix S52_setVector()
    -fix NATSUR parse list

2010JUL17
    -add more check to parameter
    -mod PASTRK time display, ':' separator
    -add cell basic legend (need formatting)
    -add S52_toggleObjClassON(), S52_toggleObjClassOFF()

2010JUL10
    -mod category selection
    -fix Mariner selection of PLib (S52_DisCat)

2010JUL04
    -fix Pango font 0..255
    -fix no deletion of display list when deleting Mariners' Object

2010JUL03
    -mod name change S52_delObj() -> S52_delMarObj()

2010JUL02
    -fix VESSEL01 time mark setup for each target
    -fix Mariners' Object priotity selection

2010JUN30
    -fix test to S52_MAR_SHOW_TEXT is not true/false anymore
     that fix a bug in lights caracteritic
    -remove hack to get only one light caracteristic

2010JUN29
    -add prefix 't' to time-tag in past track

2010JUN13
    -add curve LEGLIN
    -add S52_xy2LL()
    -mob S52_centerAt() deprecated
    -fix s52gtk2.c to use S52_xy2LL() / S52_setView() combinaition
     intead of S52_centerAt() since the notion of view reside on the user side

2010JUN05
    -add some check for NULL pointer
    -add object 'marfea' line & point
    -add wholinDist param in S52_iniLEGLIN()

2010JUN03
    -mod deprecate S52_zoomView() and S52_moveView()

2010JUN01
    -fix S52_newMarObj() of type S52_AREAS_T
    -add example for 'marfea' area in s52gtk2.c

2010MAY31
    -mod redo CS after a call to S52_newMarObj()
    -add text label on vessel
    -add S52_iniVESSELlabel()

2010MAY30
    -add ID in msg log for S52_updateObjGeo()
    -add drawing of graticule
    -mod flush colors only if new colors are found in the PLib

2010MAY29
    -add clrlin msg log
    -fix ownshp rotation
    -fix heading line (vessel & ownshp)

2010MAY18
    -fix rotation in s52gtk2.c

2010MAY16
    -mod force S52_MAR_ANTIALIAS OFF when calling S52_pickAt()
    -mod s52gtk2.c use S52_setView() instead of S52_zoomView()
         to preserve view variable current setting
    -add S52_MAR_DISP_GRATICULE (experimental)
    -fix layer callback in s52gtk2

2010MAY15
    -add cursor position (S52_MAR_DISP_CRSR_POS)

2010MAY11
    -add buoy light rotation S52_MAR_ROT_BUOY_LIGHT (unclutter chart, experimental)
    -mod refactor 'touch' for local object

2010MAY01
    -add log msg for S52_set* and S52_add* Mariner's object
    -del param ownshpCentered

2010APR29
    -add validate S52_MAR_HEADNG_LINE
    -add check for NULL S52_objHandle
    -fix S52_delObj() double free

2010APR27
    -add check that S52_objHandle are compatible when updating object

2010APR24
    -add hook for RADAR layer
    -add code to read all CATLIT
    -fix leglin/clrlin negative cog
    -set speed box and text for leglin

18APR2010
    -add Revision CVS keyword
    -fix hazzard symbol placement

11APR2010
    -fix s52gtk2.rb and S52.rb loadCell

05APR2010
    -del S52type.h

04APR2010
    -add FIXME in _renderSY_leglin/PLNPOS02 (not implemented yet)

28MAR2010
    -fix ownshp offset when rotating
    -mod VRMEBL so that it receive mouse coordinate
    -del S52_win2prj from S52.h (useless now)

10MAR2010
    -fix compiler warning

24FEB2010
    -mod mouse handling XY when when no viewPort
    -mod S52oject handle
    -start Qt4
    -mod logging on at compile time

14FEB2010
    -fix callback S52_loadObj_cb S52_loadLayer_cb for win32

04FEB2010
    -fix s52gv2 target

02FEB2010
    -add S52_CHECK_INIT to make sure that init has been done before anything else

24JAN2009
    -mod S52_iniOWNSHP() to allow for off center outline
    -mod struct S52_view as parameter replace with actual variable
    -mod filter text according to view group

13JAN2009
    -add chart rotation

12JAN2010
    -fix alpha buffer corruption (S52_MAR_SHALLOW_PATTERN) when doing glReadPixels

05JAN2010
    -fix ruby for S52_MAR_param_t

03JAN2010
    -add CLRLIN
    -add LEGLIN
    -mod S52_MAR_param_t to enum

28DEC2009
    -add PASTRK

27DEC2009
    -add VESSEL

19DEC2009
    -fix regression in performance

14DEC2009
    -fix ENC loading via S57FileCollector
    -fix S52_drawLast on win32

06DEC2009
    -add VRMEBL

27NOV2009
    -add scale bar, noth arrow, depthunit
    -add Xerror() to s52glx

25NOV2009
    -fix load cell to GPtrArray
    -mod minimal build for s52glx

21NOV2009
    -s52glx build using glib-2.0

19NOV2009
    -fix typo in INFITITE
    -fix safety contour
    -fix layer traversal

17NOV2009
    -fix s52glx target build with glib

16NOV2009
    -fix s52gv target to enable rendering AC command from GV

13NOV2009
    -remove default build with experimental call to S57FileCollector()
    -fix g++ error and (some) warning

09NOV2009
    -fix text rendering, draw last in a SENC not all SENC

06NOV209
    -add debug option to show ALL (OTHER_ALL), ignoring suppression toggle

04NOV2009
    -fix typo in CS RESARE02
    -add check for loaded file to prevent triggering g_assert()
    -add fclose() to close ENC

01NOV2009
    -add "display overlap" mode (debug)
    -fix lights leg out of view
    -fix disp. cat. layer change

30OCT2009
    -fix Display category typo in enum
    -refactor disc. cat. check
    -fix lights sector out of view
    -add code to draw intertwine cells layer

25OCT2009
    -fix target s52glx, s52gv
    -add wrapper to glib call
    -add build config to S52_version

20OCT2009
    -fix centroid
    -optimise pattern NODATA

15OCT2009
    -fix symbol transparancy

05OCT2009
    -optimise pattern for DRGARE (glPolygonStipple)
    -add nodata pattern

30SEP2009
    -mod refactor symbol creation for stracing pattern slowness

23SEP2009
    -add QuesoGLC - OpenGL Character Renderer (GLC)

19SEP2009
    -add 'ownshp'

14SEP2009
    -fix call old CS implementation when new CS is unavailable

13SEP2009
    -fix PLib parsing
    -mod CS for missing condition
    -fix color look up

09AUG2009
    -add merge PLib
    -add get/set RGB
    -add handle multi-line (experimental)

28AUG2009
    -add LUCM text filter (view group)

27AUG2009
    -fix point in poly (centroid)

26AUG2009
    -tune lookupLUP()
    -fix sounding in CS

20AUG2009
    -fix bypass geometry of 0 lenght (npt=0, nbr of point)

19AUG2009
    -fix bug rounding in _SNDFRM02() (>100m)
    -fix digit bug in _SNDFRM02() (>100m)
    -fix text bug when more then one text label per object
    -add S52_MAR_QUAPNT01 to supress display of CS QUAPNT01()

18AUG2009
    -mod update license GPL2 to GPL3
    -mod change license from GPL3 to LGPL3

17AUG2009
    -flip Y axis in _renderLC()
    -change winding for AREA (used in LC())
    -fix blending for partial line in LC()
    -fix lookup algo

15AUG2009
    -mod call S52_draw() after any keyboard event (simpler for debugging)

14AUG2009
    -fix Lookup algo
    -tune antialias vs transparency

12AUG2009
    -add S52_MAR_ANTIALIAS (experimental anti-aliasing)

10AUG2009
    -fix cursor pick for POINT symbol
    -widen pick region
    -fix color name of lights
    -fix multi-color name of lights
    -refix rendering circular symbol
    -fix LIGHTS CATLIT areo
    -mod LITCHR remove trailling space

09AUG2009
    -start gtk cursor handling
    -start port to OpenGL ES SC
    -tune xyL --> RGB
    -refactor (various)

06AUG2009
    -fix cursor pick centroid
    -refactor PLib transparency

05AUG2009
    -remove TAB from Changelog
    -fix transparency (PLib)

03AUG2009
    -fix transparency (ALPHA bits)

31JUL2009
    -add cursor pick (experimental)

30JUL2009
    -mod move object filter from draw() to cull()
    -fix filter for lights
    -fix filter for offseted centered symbole in area

29JUL2009
    -fix point symbol on line object
    -clear screen with NODATA

28JUL2009
    -fix/refactor color handling

27JUL2009
    -add commentdanger color for S52 bug in OBSTRN04
    -fix LIGHTS05 arc & leg

26JUL2009
    -fix clip object outside viewport (but not lights)
    -fix OBSTRN

24JUL2009
    -fix SAFETY CONTOUR for datum adjustment

22JUL2009
    -fix Mercator
    -fix DEPCNT02 for datum adjustement
    -fix height for datum adjustment

17JUL2009
    -fix touch(), no need for GEOS now
    -add S52_USE_OGR_FILECOLLECTOR to load all ENC in PATH

16JUL2009
    -add SCAMIN filter

15JUL2009
    -fix light caracteristic
    -fix light symbol 'orient' parameter
    -fix font size

12JUL2009
    -fix bug in light flare angle
    -fix bug in light sector

09JUL2009
    -fix accent in text
    -fix text offset
    -expose S52_toggleObjClass()

08JUL2009
    -mod CS the account for datum adjustement
    -tentative fix of 'orient'

07JUL2009
    -add Mariner Param for displaying depth as raster (from font)
    -add Mariner Param to ajust datum
    -fix bug in _SNDFRM02() when rendering  10 < sounding < 31

06JUL2009
    -fix centroid outside poly

05JUL2009
    -fix mercator
    -fix point symbol on line
    -fix text on line

15JUN2009
    -optimize pattern

11JUN2009
    -fix pattern

08JUN2009
    -fix s52gv Makefile target

02JUN2009
    -used Ruby-FFI for X86_64

30MAY2009
    -ajuste Conditional Symbology to Marine's Parameter at run-time

20MAY2009
    -remove need to switch RenderMode (for feedback and select)

05MAY2009
    -interface to mariner parameter

29APR2009
    -zoom/pan
    -start culling

14APR2009
    -bypass feedback buffer for point symbol of point object

13APR2009
    -display list
    -add DEBUG switch
    -del various dead branch
    -mod pointer to GV XYZ coord instead of local copy

17JAN2009
    -mingw (win32)
    -update README
    -mod test/* for debuging

31DEC2008
    -move X stuff to S52X
    -mod Xcms to lcms
    -mod exit() to g_assert

22JUL2008
    -remove some conditional compilation
    -s52test.c: add command line options

15JUL2008
    -start moving test stuff to test/ folder

26JUN2008
    -move layer loading from gvS57layer.c to S57gv.c

21MAY2008
    -add thread & destroy() to s52test.c

11MAY2008
    -GTK2 port start

20FEB2008
    -rework Makefile to fix some missing deps

18FEB2008
    -test if OGR found the datasource
    -strip blank from S57 filename
    -fix chart extend guessing

12OCT2007
    -add deps to S52GL.h in S52glxsimple.c to quiet gcc warning
    -mod include path to /usr/lib instead of ../local/..
    -mod uncomment default (standard!) font instead of going for helvetica

30OCT2005
    -commit pending stuff (clean up and update)

25OCT2004
    -del useless files (S57gvgeo.c  S57src.h  data2obj.tmpl)

07OCT2004
    -add OGR/GEOS for CS obstrn04

05OCT2004
    -update new code to CVS

04OCT2004
    -rtag freeze_V-1_0

22AUG2004
    -add S52_obj handle
    -refactor code to work with S52_obj
    -change ex0 traget and code to S52glxsimple
    -add target s52osg
    -add code to create an .osg file for osgviewer
    -reorganize Makefile

21JUL2004
    -add color palette switch 'on-the-fly'

12JUL2004
    -code clean up
    -add better text handling (GL)
    -add light translation into text (CS)
    -various bug fix

01JUL2004
    -removed S52rgb
    -add display supression of object (PL)
    -add palette selection (utils)
    -add font creation in gvS57layer
    -code cleanup

27JUN2004
    -fix pattern/projection
    -code cleanup
    -fix unlink object after pick
    -set correct extend by loading M_COVR first
    -optimize tess to strip/fan

26JUN2004
    -add a bounch of update from Christian Tveen
        -indexed color
        -pick return list of S57 object
        -parse attribute list for official att
        -some work on priority handling
        -fix leak
        -add solaris switch
    -clean up some projection code
    -clean up code formatting

21JUN2004
    -move PROJ stuff in S57data

20JUN2004
    -generalise S57 loading (GV/OGR)
    -mod initGL to S52_GL_initGL
    -fix pattern

22MAI2004
    -move Christian code in main branch
    -update README
    -mod Makefile

19MAI2004
    -add Christian code to split GV/OGR dependency
        --add ex0_xwin.c, ogrS57layers.c, S57data.[ch], S57ogr.[ch]

07MAI2004
    -mod LUP table name for indexing  (PL)
    -add mariner selection for are/line (conf)
    -mod for futur object layer switching code (GL)
    -bunch of code cleanup
    -add S52_MAR_SYMPLIFIED_PNT (utils)

03MAI2004
    -started cursor pick

27APR2004
    -add glcontext switching
    -removed dependency to openev STENCIL glcontext
    -start color handling (CIE->RGB)

26APR2004
    -extend size feedback buffer (GL)
    -add Z clip plane for line removal (GL)

25APR2004
    -add GL selection for OBSTRN04 (CS)
    -add GL selection for WRECKS02 (CS)

20APR2004
    -add GL selection for DEPCNT02 (CS)

12APR2004
    -add line mask for line object and area object

09APR2004
    -merge obj attributs with obj geo

07APR2004
    -move vector parser into PL module (GL/PL)
    -code clean up

02APR2004
    -mariner parameter handling
    -attribute handling
    -start coding LIGHTS05

18MAR2004
    -fix area pattern upside down (GL)
    -add layer override logic

16MAR2004
    -add antialiase
    -fix line orientation for area object

12MAR2004
    -fix upside down complex line style (GL)

10MAR2004
    -fix typo: FSHRES51, ACHRES51, DIAMOND1 (CS)

08MAR2004
    -add symbol orientation

07MAR2004
    -add default color (PL)
    -add nature of seabed abbreviation (PL)
    -fix symbol creation (GL)
    -fix symbol tansparency

26FEB2004
    -fix symbol drawn with point (SPRING)

25FEB2004
    -fix area obstruction in AA5C1JKL.000 (CS)

20FEB2004
    -fix list number (dec to octal in CS)
    -check data type correctness w/ g++ (CS)

16FEB2004
    -fix depth symbols (CS)

15FRB2004
    -exclude stuff outside view window (GL)

10FEB2004
    -fix tess problem in Chart No 1

20JAN2004
    -patch removed since Frank commited it to OpenEV

04JAN2004
    -opaque pointer for global PLib struct
    -opaque pointer for global geo data struct

23DEC2003
    -add code for plugin,
    -add projection (mercator)
    -code clean up
