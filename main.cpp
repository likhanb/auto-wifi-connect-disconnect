#include <iostream>
#include <ctime>
#include <windows.h>
#include <fstream>

using namespace std;

string TIME;

class wifi
{
public:
    string hr_con, min_con, hr_discon, min_discon, time, status_str, SSID;
    int state;
    ofstream logfile;


    //DEFAULT VALUES
    wifi()
    {
        hr_con = "00";
        min_con = "01";
        hr_discon = "05";
        min_discon = "59";
        SSID = "she ate my balls";
        state = 0;

        logfile.open("auto_wifi_log.txt", fstream::app);
        logfile<<"\n\n\n\n"<<TIME;
        logfile.close();
    }

    void default_values()
    {
        hr_con = "00";
        min_con = "01";
        hr_discon = "05";
        min_discon = "59";
        SSID = "she ate my balls";
        state = 0;
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
        if(hr_con == "0" && hr_discon == "0" && min_con == "0" && min_discon == "0") //IF ALL PARAMETERS ARE 0, DEFAULT VALUES WILL BE USED
            default_values();
        cout<<"\n\n";
    }


    void checkstatus()
    {
        string status_str;
        int count = 0;
        system("ipconfig>automation.txt");
        ifstream outfile;
        outfile.open("automation.txt");

        count = check_internet_connection();

        outfile.close();
        system("del automation.txt");
        if(count == 0/*DISCONNECTED*/&& state == 1/*FORCE_CONNECT_ON*/)
        {
            //system("NETSH WLAN CONNECT SSID=Z40 NAME=Z40"); //SSID WITHOUT BLANK SPACES
            system(("NETSH WLAN CONNECT SSID=\"" + SSID + "\" NAME=\"" + SSID + "\"").c_str()); //SSID WITH BLANK SPACES
            cout<<"FORCED CONNECTION AT "<<TIME<<endl;

            logfile.open("auto_wifi_log.txt", fstream::app);
            logfile<<"FORCED CONNETION AT "<<TIME;
            logfile.close();
        }
        else if(count == 1/*CONNECTED*/&& state == 0/*FORCE_CONNECT_OFF*/)
        {
            system("NETSH WLAN DISCONNECT");
            cout<<"FORCED DISCONNECTION AT "<<TIME<<endl;

            logfile.open("auto_wifi_log.txt", fstream::app);
            logfile<<"FORCED DISCONNETION AT "<<TIME;
            logfile.close();
        }
    }


    void connect_()
    {
        if(hr_con[0]==TIME[11]&&hr_con[1]==TIME[12]&&min_con[0]==TIME[14]&&min_con[1]==TIME[15]&&state==0)
        {
            //system("shutdown -s -t 0");//shutdown command invoked
            //system("NETSH WLAN CONNECT SSID=Z40 NAME=Z40"); SSID WITHOUT BLANK SPACES
            system(("NETSH WLAN CONNECT SSID=\"" + SSID + "\" NAME=\"" + SSID + "\"").c_str()); //SSID WITH BLANK SPACES
            cout<<TIME<<endl;
            state=1;

            logfile.open("auto_wifi_log.txt", fstream::app);
            logfile<<"NORMAL CONNECTION ESTABLISHMENT AT "<<TIME;
            logfile.close();
        }
    }


    void disconnect_()
    {
        if(hr_discon[0]==TIME[11]&&hr_discon[1]==TIME[12]&&min_discon[0]==TIME[14]&&min_discon[1]==TIME[15]&&state==1)
        {
            //system("shutdown -s -t 0");//shutdown command invoked
            system("NETSH WLAN DISCONNECT");//wlan disconnect command invoked
            cout<<TIME<<endl;
            state=0;

            logfile.open("auto_wifi_log.txt", fstream::app);
            logfile<<"NORMAL DISCONNETION AT "<<TIME;
            logfile.close();
        }
    }

    int check_internet_connection()
    {
        string status_str;
        system("ping google.com -n 1 -w 1 -l 1 >automation.txt");
        Sleep(500);
        ifstream outfile;
        outfile.open("automation.txt");
        while(outfile>>status_str)
        {
            if(status_str == "Pinging")
                {
                    return 1;
                    break;
                }
            else
                return 0;
    }
    outfile.close();
    system("del automation.txt");
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
