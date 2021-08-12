#include <iostream>
#include <ctime>
#include <windows.h>
#include <fstream>
using namespace std;
string TIME;
class wifi
{
public:
    string hr_con, min_con, hr_discon, min_discon, time, status_str;
    int state;


    wifi()
    {
        hr_con="00";
        min_con="01";
        hr_discon="05";
        min_discon="59";
        state=0;
    }


    void getdata()
    {
        cout<<"Enter begin hour: ";
        cin>>hr_con;
        cout<<"Enter begin min: ";
        cin>>min_con;
        cout<<"Enter end hour: ";
        cin>>hr_discon;
        cout<<"Enter end min: ";
        cin>>min_discon;
    }


    void checkstatus()
    {
        string status_str;
        int count=0;
        system("ipconfig>automation.txt");
        ifstream myfile;
        myfile.open("automation.txt");
        while(myfile>>status_str)
        {
            if(status_str=="disconnected")
                count++;
        }
        myfile.close();
        system("del automation.txt");
        if(count==7/*DISCONNECTED*/&&state==1/*FORCE_CONNECT_ON*/)
        {
            system("NETSH WLAN CONNECT SSID=Z40 NAME=Z40");
            cout<<"Forced connection at "<<TIME<<endl;
        }
        else if(count==6/*CONNECTED*/&&state==0/*FORCE_CONNECT_OFF*/)
        {
            system("NETSH WLAN DISCONNECT");
            cout<<"Forced disconnection at "<<TIME<<endl;
        }
    }


    void connect_()
    {
        if(hr_con[0]==TIME[11]&&hr_con[1]==TIME[12]&&min_con[0]==TIME[14]&&min_con[1]==TIME[15]&&state==0)
        {
            //system("shutdown -s -t 0");//shutdown command invoked
            system("NETSH WLAN CONNECT SSID=Z40 NAME=Z40");//wlan connect command invoked
            cout<<"Connected at "<<TIME<<endl;
            state=1;
        }
    }


    void disconnect_()
    {
        if(hr_discon[0]==TIME[11]&&hr_discon[1]==TIME[12]&&min_discon[0]==TIME[14]&&min_discon[1]==TIME[15]&&state==1)
        {
            //system("shutdown -s -t 0");//shutdown command invoked
            system("NETSH WLAN DISCONNECT");//wlan disconnect command invoked
            cout<<"Disconnected at "<<TIME<<endl;
            state=0;
        }
    }
};


int main()
{
    wifi wifi;
    wifi.getdata();
    wp1:
    time_t now=time(0);//time initialized
    TIME=ctime(&now);//convert 24 hr format
    wifi.checkstatus();
    wifi.connect_();
    wifi.disconnect_();
    Sleep(10000);
    goto wp1;
    return 0;
}
