#ifndef ROBOTINFO_H
#define ROBOTINFO_H


class RobotInfo
{
    public:
        RobotInfo();
        virtual ~RobotInfo();
        int timeCnt;
        int changeCnt;

        double curSpeed;
        double desSpeed;
        double changeSpeed;

        double curPos;
        double startPos;
        double endPos;
        double decPos;
        double changePos;

        double accelerationUp;
        double accelerationDown;

        int pro;
        int acsCnt;
        int decCnt;


        double acsDis();//加速到目标速度的距离
        double decDis();//减速到零的距离
        double totalDis();
        void updateInfo();
        double getDecPos();
        double abs(double value) { return value<0? -value:value;}
        int computePointSum();
    protected:
    private:
        bool shouldEnterDec(double curPosition);

};

#endif // ROBOTINFO_H
