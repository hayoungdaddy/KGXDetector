#include "calllocation.h"

CallLocation::CallLocation()
{
}

CallLocation::~CallLocation()
{
    staVector.clear();
    grid_lat.clear();
    grid_lon.clear();
    obs.clear();
    pred.clear();
    result.clear();
    grid_vector.clear();
    result_vector.clear();
}

void CallLocation::getLoc()
{
    staVector.clear();
    grid_lat.clear();
    grid_lon.clear();
    grid_vector.clear();
    result_vector.clear();
    obs.clear();
    pred.clear();
    result.clear();

    QVector< QVector<QString> > pickList;
    QDateTime et;
    et.setTime_t( (uint) FirstPick );
    et = et.addSecs( 3600 * 9 );

    QDir rootdir(PickDir + "/" + et.toString("yyyy"));
    QStringList dirs;
    QString filterName = "20*";
    dirs = rootdir.entryList(QStringList(filterName), QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDir::Name);

    for(int i=0;i<dirs.count();i++) // all dirs in firstPicks year
    {
        QDateTime pt = QDateTime::fromString(dirs[i].section(".", 0, 0), "yyyyMMddhhmmss");
        if(pt.toTime_t() - et.toTime_t() <= MAX_TIME_GAP)
        {
            QDir subdir(PickDir + "/" + et.toString("yyyy") + "/" + dirs[i]);
            QStringList subdirs;
            QString filterName = "*";
            subdirs = subdir.entryList(QStringList(filterName), QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDir::Name);

            if(pickList.isEmpty())
            {
                QVector<QString> pName;
                pickList.push_back(pName);
                pickList[0].push_back(subdirs[0]);      // staName_chanName
                pickList[0].push_back(dirs[i]);         // dir Name
                continue;
            }

            int newSta = 1;
            for(int j=0;j<pickList.count();j++)
            {
                if(pickList[j].indexOf(subdirs[0]) != -1)
                {
                    pickList[j].push_back(dirs[i]);
                    newSta = 0;
                    break;
                }
            }

            if(newSta == 1)
            {
                QVector<QString> pName;
                pickList.push_back(pName);
                pickList[pickList.count()-1].push_back(subdirs[0]);
                pickList[pickList.count()-1].push_back(dirs[i]);
            }
        }
    }

    QVector<PICK_INFO_FOR_EVENTLIST> pList;

    for(int i=0;i<pickList.count();i++)
    {
        QString time, time2;
        double max = 0;
        for(int j=1;j<pickList[i].count();j++)
        {
            QFile file(PickDir + "/" + pickList[i].at(j).left(4) + "/" + pickList[i].at(j) + "/" + pickList[i].at(0) + "/pickInfo");

            if( file.open( QIODevice::ReadOnly ) )
            {
                QTextStream stream(&file);
                QString line;
                line = stream.readLine();
                line = stream.readLine();
                line = stream.readLine();
                time = line.section(" ", 0, 0).section(":", 1, 1);

                for(int k=0;k<=LocStanRef;k++)
                    line = stream.readLine();

                if(max < line.section(":",1,1).toDouble())
                {
                    max = line.section(":",1,1).toDouble();
                    time2 = time;
                }

                file.close();
            }
        }

        if(max != 0)
        {
            PICK_INFO_FOR_EVENTLIST temp;
            temp.sta = pickList[i].at(0).section("_", 0, 0);
            temp.pTime = time2;
            temp.maxCoeff[0] = max;
            pList.push_back(temp);
        }
    }

    // read standard reference info.
    QString refOTS, refLatS, refLonS;
    QStringList refStaList, refATList;

    QFile file(NKInfoFile);
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

    QDateTime refotDT = QDateTime::fromString(refOTS, "yyyy-MM-dd hh:mm:ss.zzz");
    double refotd = refotDT.toTime_t();
    QString otmsec = "0." + refOTS.section(",", 1, 1);
    refotd = refotd + otmsec.toDouble();

    int rtn; int count = 0;
    double dist = 0., azim = 0.;

    QStringList staList, latList, lonList;
    staList << "YNB" << "DNH" << "KSA" << "NSN" << "YKB" << "CHNB" << "USA0B" << "BRD" << "YNG" << "MDJ" << "VLA";
    latList << "43.0007" << "43.3425" << "38.5953" << "42.0182" << "38.2254" << "38.2710" << "44.1999" << "37.9771" << "40.6811" << "44.6140" << "43.1200";
    lonList << "129.4918" << "128.2005" << "128.3516" << "125.3180" << "127.9903" << "127.1211" << "131.9887" << "124.7142" << "122.6048" << "129.5944" << "131.8933";

    for(int i=0;i<pList.count();i++)
    {
        STATION_FOR_LOC sta;
        PICK_INFO_FOR_EVENTLIST temp;
        temp = pList.at(i);

        int id1 = refStaList.indexOf(temp.sta);
        int id2 = staList.indexOf(temp.sta);

        if(id1 != -1 && id2 != -1)
        {
            sta.staName = temp.sta;
            sta.lat = latList[id2].toDouble();
            sta.lon = lonList[id2].toDouble();
            sta.ob_ptime = temp.pTime.toDouble();
            QDateTime tt = QDateTime::fromString(refATList[id1], "yyyy-MM-dd hh:mm:ss.zzz");
            sta.ref_ptime = tt.toTime_t();
            QString msec = "0." + refATList[id1].section(".", 1, 1);
            sta.ref_ptime = sta.ref_ptime + msec.toDouble();
            staVector.push_back(sta);
        }
    }

    while(1)
    {
        grid_lat.push_back(myRound(GridLatB, 4));
        GridLatB = GridLatB + GRID_SPACING;
        GridLatB = myRound(GridLatB, 4);
        if(GridLatB > myRound(GridLatE, 4))
            break;
    }

    while(1)
    {
        grid_lon.push_back(myRound(GridLonB, 4));
        GridLonB = GridLonB + GRID_SPACING;
        GridLonB = myRound(GridLonB, 4);
        if(GridLonB > myRound(GridLonE, 4))
            break;
    }

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

    min = 99999; max = -1000;
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
        if(r.result > max)
        {
            max = r.result;
        }
    }

    count = 0;

    int nx = grid_lon.count();
    int ny = grid_lat.count();

    QString firstPickMsec = QString::number(FirstPick, 'f', 3).section(".", 1, 1);
    QString saveDir = PickDir + "/" + et.toString("yyyy") + "/" + et.toString("yyyyMMddhhmmss") + "." + firstPickMsec + "/location";

    QDir d(saveDir);
    if(!d.exists()) d.mkpath(".");

    file.setFileName(saveDir + "/result.txt");
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream stream( &file );
        stream << "Location Info" << "\n";
        stream << "min:" << QString::number(min, 'f', 4) << "/locID:" << QString::number(location_key)
               << "/lat:" << QString::number(result_lat, 'f', 4)  << "/lon:" << QString::number(result_lon, 'f', 4) << "\n";
        stream << "Grid Info" << "\n";
        stream << "lat:" << QString::number(grid_lat.at(0), 'f', 4) << "-" << QString::number(GridLatE, 'f', 4) << "\n";
        stream << "lon:" << QString::number(grid_lon.at(0), 'f', 4) << "-" << QString::number(GridLonE, 'f', 4) << "\n";
        stream << "Reference Info" << "\n";
        stream << "name:NK" << QString::number(LocStanRef + 1) << "/evlat:" << refLatS << "/evlon:" << refLonS
               << "/ot:" << QString::number(refotd, 'f', 3) << "\n";
        for(int i=0;i<staVector.count();i++)
        {
            STATION_FOR_LOC sta;
            sta = staVector.at(i);
            stream << sta.staName << ":" << QString::number(sta.ref_ptime, 'f', 3) << ":" << QString::number(sta.gv, 'f', 4) << "\n";
        }
        stream << "Observed Info" << "\n";
        for(int i=0;i<staVector.count();i++)
        {
            STATION_FOR_LOC sta;
            sta = staVector.at(i);
            stream << sta.staName << ":" << QString::number(sta.ob_ptime, 'f', 3) << "\n";
        }
        stream << "Data" << "\n";
        for(int y=0;y<ny;y++)
        {
            for(int x=0;x<nx;x++)
            {
                //stream << QString::number(x) << " " << QString::number(y) << " " << QString::number(result.at(count), 'f', 4) << "\n";
                stream << QString::number(grid_lat.at(x), 'f', 4) << " " << QString::number(grid_lon.at(y), 'f', 4)
                       << " " << QString::number(result.at(count), 'f', 4) << "\n";
                count++;
            }
        }
        file.close();
    }

    emit finished();
}

void CallLocation::setup(QString pickDir, int locStanRef, double firstPick, QString nkInfoFile, double gridlatB, double gridlatE, double gridlonB, double gridlonE)
{
    PickDir = pickDir;
    LocStanRef = locStanRef;
    FirstPick =  firstPick;
    NKInfoFile = nkInfoFile;
    GridLatB = gridlatB;
    GridLatE = gridlatE;
    GridLonB = gridlonB;
    GridLonE = gridlonE;
}


