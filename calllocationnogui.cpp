#include "calllocationnogui.h"

CallLocationNoGUI::CallLocationNoGUI()
{
}

CallLocationNoGUI::~CallLocationNoGUI()
{
    staVector.clear();
    grid_lat.clear();
    grid_lon.clear();
    obs.clear();
    pred.clear();
    result.clear();
    StaList.clear();
    PArrivalTime.clear();
    refStaList.clear();
    refATList.clear();
}

void CallLocationNoGUI::makeGrid(double gridlatB, double gridlatE, double gridlonB, double gridlonE, double spacing)
{
    grid_lat.clear();
    grid_lon.clear();
    while(1)
    {
        grid_lat.push_back(myRound(gridlatB, 4));
        gridlatB = gridlatB + spacing;
        gridlatB = myRound(gridlatB, 4);
        if(gridlatB > myRound(gridlatE, 4))
            break;
    }

    while(1)
    {
        grid_lon.push_back(myRound(gridlonB, 4));
        gridlonB = gridlonB + spacing;
        gridlonB = myRound(gridlonB, 4);
        if(gridlonB > myRound(gridlonE, 4))
            break;
    }
}

void CallLocationNoGUI::ready(QString pickDir, int locStanRef, QString nkInfoFile)
{
    PickDir = pickDir;
    LocStanRef = locStanRef;

    QFile file(nkInfoFile);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        QString line;
        int count = 0;

        while(!stream.atEnd())
        {
            line = stream.readLine();
            if(line.startsWith("EventName"))
            {
                if(count == LocStanRef)
                {
                    while(1)
                    {
                        line = stream.readLine();
                        if(line.startsWith("#####")) break;
                        else if(line.startsWith("OriginTime")) refOTS = line.section("=", 1, 1);
                        else if(line.startsWith("Latitude")) refLatS = line.section("=", 1, 1);
                        else if(line.startsWith("Longitude")) refLonS = line.section("=", 1, 1);
                        else if(line.startsWith("mb")) continue;
                        else
                        {
                            refStaList << line.section("=", 0, 0);
                            refATList << line.section("=", 1, 1);
                        }
                    }
                    break;
                }
                count++;
            }
        }
        file.close();
    }
}

void CallLocationNoGUI::getLoc()
{
    staVector.clear();
    grid_vector.clear();
    result_vector.clear();
    obs.clear();
    pred.clear();
    result.clear();

    QStringList staList, latList, lonList;
    staList << "YNB" << "DNH" << "KSA" << "NSN" << "YKB" << "CHNB" << "USA0B" << "BRD" << "YNG" << "MDJ" << "VLA";
    latList << "43.0007" << "43.3425" << "38.5953" << "42.0182" << "38.2254" << "38.2710" << "44.1999" << "37.9771" << "40.6811" << "44.6140" << "43.1200";
    lonList << "129.4918" << "128.2005" << "128.3516" << "125.3180" << "127.9903" << "127.1211" << "131.9887" << "124.7142" << "122.6048" << "129.5944" << "131.8933";

    QDateTime refotDT = QDateTime::fromString(refOTS, "yyyy-MM-dd hh:mm:ss.zzz");
    double refotd = refotDT.toTime_t();
    QString otmsec = "0." + refOTS.section(",", 1, 1);
    refotd = refotd + otmsec.toDouble();

    for(int i=0;i<StaList.count();i++)
    {
        STATION_FOR_LOC sta; sta.lat = 0; sta.lon = 0;
        sta.staName = StaList.at(i);
        for(int j=0;j<staList.count();j++)
        {
            if(sta.staName.startsWith(staList[j]))
            {
                sta.lat = latList[j].toDouble();
                sta.lon = lonList[j].toDouble();
                break;
            }
        }
        for(int j=0;j<refStaList.count();j++)
        {
            if(sta.staName.startsWith(refStaList[j]))
            {
                QDateTime tt = QDateTime::fromString(refATList[j], "yyyy-MM-dd hh:mm:ss.zzz");
                sta.ref_ptime = tt.toTime_t();
                QString msec = "0." + refATList[j].section(".", 1, 1);
                sta.ref_ptime = sta.ref_ptime + msec.toDouble();
                break;
            }
        }
        sta.ob_ptime = PArrivalTime.at(i);
        if(sta.lat != 0 && sta.lon != 0)
            staVector.push_back(sta);
    }

    /* caculate location */
    int rtn; int count = 0;
    double dist = 0., azim = 0.;

    for(int i=0;i<staVector.count();i++)
    {
        STATION_FOR_LOC sta;
        sta = staVector.at(i);
        sta.timeDiff = sta.ref_ptime - refotd;
        rtn = geo_to_km(sta.lat, sta.lon, refLatS.toDouble(), refLonS.toDouble(), &dist, &azim);
        sta.dist_staToevent = dist;
        sta.gv = sta.dist_staToevent / sta.timeDiff;

        staVector.replace(i, sta);
    }

    for(int i=0;i<grid_lat.count();i++)
    {
        for(int j=0;j<grid_lon.count();j++)
        {
            GRID pointOfgrid;
            pointOfgrid.lat = grid_lat.at(i);
            pointOfgrid.lon = grid_lon.at(j);

            for(int k=0;k<staVector.count();k++)
            {
                STATION_FOR_LOC sta;
                sta = staVector.at(k);
                rtn = geo_to_km(sta.lat, sta.lon, pointOfgrid.lat, pointOfgrid.lon, &dist, &azim);
                pointOfgrid.dist_staTohere[k] = dist;
                pointOfgrid.azim_staTohere[k] = azim;
                pointOfgrid.group_delay[k] = dist / sta.gv;
            }
            grid_vector.push_back(pointOfgrid);
        }
    }

    for(int i=0;i<staVector.count()-1;i++)
    {
        for(int j=i+1;j<staVector.count();j++)
        {
            STATION_FOR_LOC sta1, sta2;
            sta1 = staVector.at(i);
            sta2 = staVector.at(j);
            obs.push_back( sta1.ob_ptime - sta2.ob_ptime );
        }
    }

    for(int i=0;i<grid_vector.count();i++)
    {
        for(int j=0;j<staVector.count()-1;j++)
        {
            for(int k=j+1;k<staVector.count();k++)
            {
                GRID grid = grid_vector.at(i);
                QVector<double> temp;
                pred.push_back(temp);
                pred[i].push_back(grid.group_delay[j] - grid.group_delay[k]);
            }
        }
    }

    count = 0;
    for(int i=0;i<grid_lat.count();i++)
    {
        for(int j=0;j<grid_lon.count();j++)
        {
            RESULT_FOR_LOC resultOfgrid;
            resultOfgrid.lat = grid_lat.at(i);
            resultOfgrid.lon = grid_lon.at(j);

            double r  = 0;
            for(int k=0;k<obs.count();k++)
            {
                r = r + fabs(pred[count].at(k) - obs.at(k));
            }

            resultOfgrid.result = r;
            result.push_back(r);

            result_vector.push_back(resultOfgrid);
            count++;
        }
    }

    min = 99999;

    for(int i=0;i<result_vector.count();i++)
    {
        RESULT_FOR_LOC r = result_vector.at(i);
        if(r.result < min)
        {
            min = r.result;
            location_key = i;
            result_lat = r.lat;
            result_lon = r.lon;
        }
    }

    rtn = geo_to_km(refLatS.toDouble(), refLonS.toDouble(), result_lat, result_lon, &dist, &azim);
    emit finished(result_lat, result_lon, dist, azim);
    emit killme();
}

void CallLocationNoGUI::setup(QVector<QString> staList, QVector<double> pArrivalTime)
{
    StaList.clear();
    PArrivalTime.clear();
    StaList = staList;
    PArrivalTime = pArrivalTime;
}


