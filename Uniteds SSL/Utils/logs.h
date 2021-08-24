#pragma once

#include <iostream>
#include <fstream>
//#include <shlobj_core.h>
#include <Windows.h>
#include <sstream>
#include <filesystem>
#include "errors.h"
#include "../settings.h"

namespace Colors
{
    inline int cyan = 3;
    inline int lightRed = 12;
    inline int green = 10;
    inline int defaultGray = 7;
}

namespace Logs
{
    static void LogInt(int msg, int color = Colors::defaultGray, bool bNl = true, bool err = false);
    static void Log(std::string msg, int color = Colors::defaultGray, bool bNl = true, bool debug = false, bool rainbowLog = false);
}

namespace File
{
    template <typename T>
    static void WriteToLog(T msg);
}

namespace fs = std::filesystem;

namespace Conv
{
    static const char* CharPtrToConst(char* c) { return c; }
    static std::string FloatToStr(float f) { return std::to_string(f); }
    static std::string IntToStr(int i) { return std::to_string(i); }
    //const char* StrToConst(std::string s) { return s.c_str(); }
    static std::string PathToStr(fs::path p)
    {
        std::string pstr = p.u8string();
        return pstr;
    }
    static std::string BoolToStr(bool b)
    {
        if (b) return "true";
        else if (!b) return "false";
        //return IntToStr(Errors::BOOL_TO_STR_FAILED);
    }

    static void WriteToLog(const char* msg)
    {
        std::ofstream myfile;
        myfile.open("example.txt", std::ios::app);
        myfile << msg << std::endl;
        myfile.close();
    }

}

namespace Logs
{
    static void nl()
    {
        std::cout << std::endl;
    }

    //template <typename T>
    static bool RainbowLog(std::string msg) // TODO: Add support for const char etc.
    {
        /*if (!checkType(msg, "string"))
        {
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, Colors::lightRed);
            std::cout << "[ERROR] ";
            SetConsoleTextAttribute(hConsole, Colors::defaultGray);
            std::cout << "Type was not string!";
            return false;
        } */
        int msgIndex = 0;
        for (int k = 1; k < msg.length() + 1; k++)
        {
            if (msgIndex == msg.length() + 1)
            {
                nl();
                return true;
            }
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, k);
            std::cout << msg[msgIndex];
            msgIndex++;
            if (k == 15) k = 1;
        }
        return true;
    }

    static void LogInt(int msg, int color, bool bNl, bool err)
    {
        if (err)
        {
            Log(std::string("Error Code: #" + Conv::IntToStr(msg)), Colors::lightRed, true, false, false);
            File::WriteToLog(std::string("Error Code: #" + Conv::IntToStr(msg)));
        }
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
        std::cout << msg;
        if (bNl) nl();
    }

    static void Log(std::string msg, int color, bool bNl, bool debug, bool rainbowLog) // TODO: Add back debuglog, make it inline and use if defs for debug.
    {
        if (debug)
        {
            if (Version == vProd) return;
        }
        if (rainbowLog)
        {
            std::string msgStr = msg;
            bool t = RainbowLog(msgStr);
            if (t)
            {
                if (bNl) nl();
            }
            return;
        }
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
        std::cout << msg;
        if (bNl) nl();
    }

    static void Log(const char* msg, int color = Colors::defaultGray, bool bNl = true, bool debug = false, bool rainbowLog = false)
    {
        if (debug)
        {
            if (Version == vProd) return;
        }
        if (rainbowLog)
        {
            std::string msgStr = msg;
            bool t = RainbowLog(msgStr);
            if (t)
            {
                if (bNl) nl();
            }
            return;
        }
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
        std::cout << msg;
        if (bNl) nl();
    }

    bool checkSig(uintptr_t& sig, const char* sigmsg)
    {
        if (!sig)
        {
            //Pattern Not Found
            //MessageBoxA(0, L"Failed to find Function: " + sigmsg), "United SSL Fatal Error!", MB_ICONERROR);
            return FALSE;
        }
        else
        {
            Log("Found Function: " + std::string(sigmsg));
            return TRUE;
        }
    }
}
namespace File
{
    static bool fexists(const fs::path& p)
    {
        if (exists(p)) return true;
        else return false;
    }

    static bool fdelete(const fs::path& p)
    {
        if (!fexists(p)) return false;
        remove(p);
        WriteToLog("Deleted " + Conv::PathToStr(p));
    }

    static void fcreate(const fs::path& p)
    {
        if (fexists(p)) return;
        fs::create_directory(p);
        WriteToLog(std::string("Created " + Conv::PathToStr(p)));
    }

    static const char* GetEnv(const char* envname)
    {
        // SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &path_tmp);
        char* pValue;
        size_t len;
        errno_t err = _dupenv_s(&pValue, &len, envname);
        return Conv::CharPtrToConst(pValue);
    }

    static std::string SSLPath = GetEnv("APPDATA") + std::string("\\United Network\\United SSL");

    static std::string GetSSLPath() { return SSLPath; }

    static void InitAD()
    {
        if (fexists(SSLPath)) return;
        fcreate(SSLPath);
        WriteToLog("Running InitAD..");
        return;
    }

    template <typename T>
    static void WriteToLog(T msg)
    {
        // const char* date = __DATE__ + __TIME__;
        std::ofstream outfile;
        outfile.open(SSLPath + "\\logs.txt", std::ios::out | std::ios::app); // TODO(Milxnor): name the files the time and date so the logs dont reset every time idk.
        if (!outfile.is_open()) Logs::LogInt(GetErr(Errors::FSTREAM_NOT_OPEN), Colors::defaultGray, true, true);
        outfile << msg << std::endl;
        outfile.close();
    }
}