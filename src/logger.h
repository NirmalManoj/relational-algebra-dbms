#ifndef LOGGER_H
#define LOGGER_H

#include<iostream>
#include<bits/stdc++.h>
#include<sys/stat.h> 
#include<fstream>

using namespace std;

class Logger{

    string logFile = "log";
    ofstream fout;
    
    public:

    Logger();
    void log(string logString);
    void log(int logInt);
};

extern Logger logger;

#endif