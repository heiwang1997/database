#include "testdate.h"

testdate::testdate()
{

}
void testdate::begintest()
{
    auto s = system_clock::now();
    std::cout<<sizeof(s)<<std::endl;
    std::cout<<s<<std::endl;
    for (int i=0;i<1000000;i++)
        auto temp = make_zoned(current_zone(), system_clock::now());
    auto t = make_zoned(current_zone(), system_clock::now());
    std::cout<<sizeof(t)<<std::endl;
    auto tt = t.get_local_time();
    std::cout<<tt<<' '<<sizeof(tt)<<'\n';
    std::cout<<t<<std::endl;

    std::cout<<t.get_sys_time()<<std::endl;

    auto nowt = t.get_sys_time();
    auto dp = floor<days>(nowt);
    std::cout<<dp<<'\n';
    auto ymd = year_month_day{dp};
    std::cout<<ymd<<'\n';
    auto time = make_time(nowt-dp);
    std::cout<<time<<'\n';

    int y=2015;
    auto ymd2 =year_month_day{year{y}/6/8};
    std::cout<<ymd2<<'\n';


    auto time2=make_time(hours{5},minutes{7},seconds{9},nanoseconds{22},0);
    std::cout<<time2<<'\n';

    auto cha = time2.to_duration()-time.to_duration();

    std::cout<<cha<<'\n';
    if (cha<cha.zero())
    {
        std::cout<<"time2<time1"<<std::endl;
    } else
    {
        std::cout<<"time1<time2"<<std::endl;
    }
    std::cout<<cha<<'\n';

    bool yes=ymd2<ymd;
    std::cout<<yes<<'\n';

    sys_days symd2 = ymd2;
    sys_days symd = ymd;
    auto q=(symd2-symd);

    cha += q;

    std::cout<<cha<<'\n';


    auto nowt1 = nowt+cha;

    std::cout<<nowt1<<'\n';

    auto nowt2=time2.to_duration()+symd2;
    std::cout<<nowt2<<'\n';

    std::cout<<nowt2-nowt<<'\n';

    auto totalcha = nowt2-nowt;

    //totalcha=totalcha.zero();
    //totalcha=-totalcha;
    std::cout<<(totalcha<totalcha.zero())<<std::endl;
    std::cout<<(totalcha==totalcha.zero())<<std::endl;
    std::cout<<(totalcha>totalcha.zero())<<std::endl;
}
