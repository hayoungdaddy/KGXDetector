/**
 * @file                common.h
 * @author              YunYeoWoong (KIGAM)
 * @date                12 Sep 2016
 * @brief               Define macros, structs, functions.
 */

#ifndef COMMON_H
#define COMMON_H

#include "math.h"
#include "stdio.h"

#include <QString>
#include <QVector>
#include <QDateTime>

#define VERSION 1.0

#define NUM_NK 6

#define PI 3.14159265358979323846
#define GRID_SPACING 0.0001
#define MINIMUM_SCNL_FOR_LOC 3

#define TRACE2_STA_LEN    7
#define TRACE2_NET_LEN    9
#define TRACE2_CHAN_LEN   4
#define TRACE2_LOC_LEN    3
#define FILELEN 128
#define MAX_SCNL 10                         /**< The number of station can't exceed 10. */
#define MAX_NUM_REFDATA 10                  /**< The number of reference file of one station can't exceed 10. */
#define MAX_SAMPLE_RATE 100                 /**< We will use 20, 40, 100sps. */
#define MAX_REFDATA_LENGTH 10               /**< We will use 20, 40, 100sps. */
#define MAX_DATA_SIZE 4096 * 5              /**< Max size of data array is 4096 * 5, Size of double type is 4096 * 5 * 8 bytes. */
#define FIND_MAX_COEFF_RANGE_FOR_100 10     /**< for make CC of 100sps */
#define FIND_MAX_COEFF_RANGE_FOR_20 2       /**< for make CC of 20sps */
#define MAX_KEEP_DATA 60*10*MAX_SAMPLE_RATE /**< It will keep 60000 points(600 seconds) cc graph */
#define MAX_KEEP_TIME 60*10                 /**< It will keep 10 minutes(600 seconds) cc graph */
#define TIME_INTERVAL 1000                  /**< Time Interval is 1 second, for timer */
#define MAX_TIME_GAP 85                     /**< Duration for one event */
#define HALF_BBOARD_GRAPH_LENGTH 30         /**< It will draw graph for 1 minute data on Briefing board */
#define SOH_GREEN 30                        /**< If diff. with lasttime and system time is < 30 seconds, then soh of this station is green */
#define SOH_YELLOW 60                       /**< If diff. with lasttime and system time is > 60 & < 60 seconds, thsn soh of this station if yellow */

#define NUM_DECIMAL_FOR_COEFF   2
#define NUM_DECIMAL_FOR_DIST    3
#define NUM_DECIMAL_FOR_AZIM    1

/**
 * @brief       <b>Parameters for KGXDetector</b>
 */
typedef struct
{
    QString pickDir;                        /**< Directory name for saving PICK_INFO */
    QString alarmFileName;                  /**< File name for alarm */
    int     alarmEnable;                    /**< Alarm enable, It always 1 */
    QString staName[MAX_SCNL];              /**< Station code */
    QString chanName[MAX_SCNL];             /**< Channel code */
    float   threshold[MAX_SCNL];            /**< Coefficient threshold */
    int     ccEnable[MAX_SCNL];             /**< Correlograms enable,  default: KSA, YKB, CHNB */
    double  timeShift[MAX_SCNL];            /**< Shift time for correlograms, default: KSA:45, YKB:51.73, CHNB:54.82 */
    float   ccthreshold;                    /**< Correlograms coefficient threshold, default: 0.6 */
    int     ccSps;                          /**< Sample rate for correlograms, default: 100sps */
    int     stanRef;                        /**< Standard reference, default: Max coefficient for all */
    int     NccEnable;                      /**< Number of station for correlograms, default: 3 */
    int     locStanRef;
    double  gridlatB, gridlatE, gridlonB, gridlonE;
    QString nkinfoFileName;                 /**< NK_INFO file name for mb graph and location */
    QString segmentFile;

    QString zmqServerIP;                    /**< Server Ip address for zeroMQ */
    int     zmqPortForPick;                 /**< Port number for PICK Info */
    int     zmqPortForCoeff;                /**< Port number for coeff packet */
    int     isAlived[MAX_SCNL];             /**< Status of station, 1 = alive, 0 = alive, but not realtime, -1 = dead */
    double  lastTime[MAX_SCNL];             /**< Last time of each station */
} CONFIGURE;

/**
 * @brief       <b>For multimap values</b>
 *
 * We use QMultiMap<double, COEFFMAP>
 */
typedef struct
{
    QString staName;                        /**< Station code */
    double coeff;                           /**< Max coefficient */
} COEFFMAP;

/**
 * @brief       <b>For event</b>
 */
typedef struct
{
    QString eventSta[MAX_SCNL];             /**< It is same with staName of CONFIGURETION */
    double  firstEventTime[MAX_SCNL];       /**< First event time of station, We will save coeff. array to this directory */
    double  eventTimeForLoc[MAX_SCNL];
    float   coeffForLoc[MAX_SCNL];
    int     isGettingLoc;
    int     isCompleted[MAX_SCNL];          /**< If coeff. array saved well, then it will change 0 to 1 */
    int     isEvent;                        /**< isEvent is 0: is normal mode,  isEvent is 1: is event mode */
    QDateTime systemTimeWhenEvent;          /**< System time when event occured */
    int     blink;                          /**< Correlogram detector will blink when correlogram coefficient over threshold */
} EVENT;

/**
 * @brief       <b>From pick_xcorr</b>
 */
typedef struct
{
    char    sta[TRACE2_STA_LEN];
    char    chan[TRACE2_CHAN_LEN];
    char    net[TRACE2_NET_LEN];
    char    loc[TRACE2_LOC_LEN];
    int     sps;
    float   eachCoeff[MAX_NUM_REFDATA];
    float   maxCoeff;
    int     nRefData;
    char    refFileName[MAX_NUM_REFDATA][FILELEN];
    float   threshold;
    double  coeffTime;
    int     pOffset;
    double  pArrivalTime;
    double  maxAmp;
    double  maxAmpTime;
    double  per;
    double  mb;
//    double  mb2;
//    char    formulaFormb[50];
    int     nSamp;
    float   inpData[MAX_DATA_SIZE];
} PICK_INFO;

/**
 * @brief       <b>for eventlist class</b>
 */
typedef struct
{
    QString sta;
    QString dirName;
    QString pTime;
    QString mb;
    double maxCoeff[MAX_NUM_REFDATA];
    QString threshold;
} PICK_INFO_FOR_EVENTLIST;

/**
 * @brief       <b>From makeCC</b>
 */
typedef struct
{
    char    sta[TRACE2_STA_LEN];
    char    chan[TRACE2_CHAN_LEN];
    char    net[TRACE2_NET_LEN];
    char    loc[TRACE2_LOC_LEN];
    int     sps;
    int     nSamp;
    double  startTime;
    int     nRefData;
    char    refFileName[MAX_NUM_REFDATA][FILELEN];
    float   maxCoeffArray[MAX_SAMPLE_RATE * MAX_REFDATA_LENGTH];
    //float   coeffArray[MAX_NUM_REFDATA][MAX_SAMPLE_RATE * MAX_REFDATA_LENGTH];
} COEFF;

typedef struct
{
    double lat;
    double lon;
    double dist_staTohere[MAX_SCNL];
    double azim_staTohere[MAX_SCNL];
    double group_delay[MAX_SCNL];
} GRID;

typedef struct
{
    QString         staName;
    double          lat, lon;
    double          timeDiff;
    double          ref_ptime;
    double          ob_ptime;
    double          dist_staToevent;
    double          gv;                 // (ref_ptime + ref_msec) / dist_staToevent;
} STATION_FOR_LOC;

typedef struct
{
    double lat;
    double lon;
    double x;
    double y;
    double result;
} RESULT_FOR_LOC;

/**
 * @brief       <b>Round up for double variables</b>\n\n
 *
 * this function will use epochtime with 3 decimal
 *
 * @verbatim    EX) double t = myRound(thisScnl->delta, 2); (ex: 0.0132132-> 0.01)@endverbatim
 * @params      param1 <b>double n</b> is double type variable for round up you want
 * @params      param2 <b>unsigned int c</b> is number of decimal digit
 * @return      double
 */
static double myRound(double n, unsigned int c)
{
    double marge = pow(10, c);
    double up = n * marge;
    double ret = round(up) / marge;
    return ret;
}

static int geo_to_km(double lat1,double lon1,double lat2,double lon2,double* dist,double* azm)
{
    double a, b;
    double semi_major=a=6378.160;
    double semi_minor=b=6356.775;
    double torad, todeg;
    double aa, bb, cc, dd, top, bottom, lambda12, az, temp;
    double v1, v2;
    double fl, e, e2, eps, eps0;
    double b0, x2, y2, z2, z1, u1p, u2p, xdist;
    double lat1rad, lat2rad, lon1rad, lon2rad;
    double coslon1, sinlon1, coslon2, sinlon2;
    double coslat1, sinlat1, coslat2, sinlat2;
    double tanlat1, tanlat2, cosazm, sinazm;

    double c0, c2, c4, c6;

    double c00=1.0, c01=0.25, c02=-0.046875, c03=0.01953125;
    double c21=-0.125, c22=0.03125, c23=-0.014648438;
    double c42=-0.00390625, c43=0.0029296875;
    double c63=-0.0003255208;

/*  Check for special conditions */
    if( lat1 == lat2 && lon1 == lon2 ) {
        *azm = 0.0;
        *dist= 0.0;
        return(1);
    }
/* - Initialize.             */

    torad = PI / 180.0;
    todeg = 1.0 / torad;
    fl = ( a - b ) / a;
    e2 = 2.0*fl - fl*fl;
    e  = sqrt(e2);
    eps = e2 / ( 1.0 - e2);
/*
* - Convert event location to radians.
*   (Equations are unstable for latidudes of exactly 0 degrees.)
*/
    temp=lat1;
    if(temp == 0.) temp=1.0e-08;
    lat1rad=torad*temp;
    lon1rad=torad*lon1;

    temp=lat2;
    if(temp == 0.) temp=1.0e-08;
    lat2rad=torad*temp;
    lon2rad=torad*lon2;

/*
    Compute some of the easier and often used terms.
*/
    coslon1 = cos(lon1rad);
    sinlon1 = sin(lon1rad);
    coslon2 = cos(lon2rad);
    sinlon2 = sin(lon2rad);
    tanlat1 = tan(lat1rad);
    tanlat2 = tan(lat2rad);
    sinlat1 = sin(lat1rad);
    coslat1 = cos(lat1rad);
    sinlat2 = sin(lat2rad);
    coslat2 = cos(lat2rad);
/*
    The radii of curvature are compute from an equation defined in
    GEODESY by Bomford, Appendix A (page 647).
    v = semi_major/sqrt(1-e*e*sin(lat)*sin(lat))
*/
    v1 = a / sqrt( 1.0 - e2*sinlat1*sinlat1 );  /* radii of curvature  */
    v2 = a / sqrt( 1.0 - e2*sinlat2*sinlat2 );  /* radii of curvature  */
    aa = tanlat2 / ((1.0+eps)*tanlat1);
    bb = e2*(v1*coslat1)/(v2*coslat2);
    lambda12 = aa + bb;
    top = sinlon2*coslon1 - coslon2*sinlon1;
    bottom = lambda12*sinlat1-coslon2*coslon1*sinlat1-sinlon2*sinlon1*sinlat1;
    az = atan2(top,bottom)*todeg;
    if( az < 0.0 ) az = 360 + az;
    *azm = az;
    az = az * torad;
    cosazm = cos(az);
    sinazm = sin(az);

    if( lat2rad < 0.0 ) {
        temp = lat1rad;
        lat1rad = lat2rad;
        lat2rad = temp;
        temp = lon1rad;
        lon1rad = lon2rad;
        lon2rad = temp;

        coslon1 = cos(lon1rad);
        sinlon1 = sin(lon1rad);
        coslon2 = cos(lon2rad);
        sinlon2 = sin(lon2rad);
        tanlat1 = tan(lat1rad);
        tanlat2 = tan(lat2rad);
        sinlat1 = sin(lat1rad);
        coslat1 = cos(lat1rad);
        sinlat2 = sin(lat2rad);
        coslat2 = cos(lat2rad);

        v1 = a / sqrt( 1.0 - e2*sinlat1*sinlat1 );
        v2 = a / sqrt( 1.0 - e2*sinlat2*sinlat2 );

        aa = tanlat2 / ((1.0+eps)*tanlat1);
        bb = e2*(v1*coslat1)/(v2*coslat2);
        lambda12 = aa + bb;

        top = sinlon2*coslon1 - coslon2*sinlon1;
        bottom =lambda12*sinlat1-coslon2*coslon1*sinlat1-
            sinlon2*sinlon1*sinlat1;
        az = atan2(top,bottom);
        cosazm = cos(az);
        sinazm = sin(az);

    }

    eps0 = eps * ( coslat1*coslat1*cosazm*cosazm + sinlat1*sinlat1 );
    b0 = (v1/(1.0+eps0)) * sqrt(1.0+eps*coslat1*coslat1*cosazm*cosazm);

    x2 = v2*coslat2*(coslon2*coslon1+sinlon2*sinlon1);
    y2 = v2*coslat2*(sinlon2*coslon1-coslon2*sinlon1);
    z2 = v2*(1.0-e2)*sinlat2;
    z1 = v1*(1.0-e2)*sinlat1;

    c0 = c00 + c01*eps0 + c02*eps0*eps0 + c03*eps0*eps0*eps0;
    c2 =       c21*eps0 + c22*eps0*eps0 + c23*eps0*eps0*eps0;
    c4 =                  c42*eps0*eps0 + c43*eps0*eps0*eps0;
    c6 =                                  c63*eps0*eps0*eps0;

    bottom = cosazm*sqrt(1.0+eps0);
    u1p = atan2(tanlat1,bottom);

    top = v1*sinlat1+(1.0+eps0)*(z2-z1);
    bottom = (x2*cosazm-y2*sinlat1*sinazm)*sqrt(1.0+eps0);
    u2p = atan2(top,bottom);

    aa = c0*(u2p-u1p);
    bb = c2*(sin(2.0*u2p)-sin(2.0*u1p));
    cc = c4*(sin(4.0*u2p)-sin(4.0*u1p));
    dd = c6*(sin(6.0*u2p)-sin(6.0*u1p));

    xdist = fabs(b0*(aa+bb+cc+dd));
    *dist = xdist;
    return(1);
}


#endif // COMMON_H
