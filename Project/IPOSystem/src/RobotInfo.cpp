#include "RobotInfo.h"

RobotInfo::RobotInfo()
{
    //ctor
}

RobotInfo::~RobotInfo()
{
    //dtor
}

double RobotInfo::getDecPos()
{
    if(abs(totalDis()) > abs(acsDis())+abs(decDis())){
        return startPos+totalDis()-decDis();
    }else{
        double p = static_cast<double>(acsCnt)/(acsCnt+decCnt);
        return totalDis()*p+startPos;
    }
}

double RobotInfo::acsDis()
{
    return 0.5*desSpeed*pro*acsCnt*0.001;
}
double RobotInfo::decDis()
{
    return 0.5*desSpeed*pro*decCnt*0.001;
}

double RobotInfo::totalDis()
{
    return endPos - startPos;
}

int RobotInfo::computePointSum()
{
    return acsCnt+decCnt+static_cast<int>(abs(totalDis())*1000/abs(desSpeed)/pro);
}

bool RobotInfo::shouldEnterDec(double curPosition)
{
    return true;
}

void RobotInfo::updateInfo()
{
    timeCnt++;
    curSpeed += accelerationUp;
}
