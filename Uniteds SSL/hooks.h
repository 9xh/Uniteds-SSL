// hooks.h - Contains the hooks of process event and cURL.
#pragma once


#include <Windows.h>
#include <string>
#include <regex>

#include "Structs/curldefs.h"
#include "Third Party/xorstr.hpp"
#include "settings.h"
#include "Utils/logs.h"
#include "Utils/memory.h"

typedef void CURL;

using namespace Logs;

uintptr_t CurlEasyOptP;
uintptr_t CurlSetOptP;

auto (*curl_easy_setopt_original)(CURL* cURL, uintptr_t opt, ...)->CURLcode;

auto changeeasyopt(CURL* Curl, uintptr_t opt, va_list info)->CURLcode
{
    int OFF = 0;
    switch (opt)
    {
    case CURL_NOPROXY:
        return curl_easy_setopt_original(Curl, opt, "");
        break;

    case CURL_VERIFYPEER:
        return curl_easy_setopt_original(Curl, opt, OFF);
        break;
    case CURL_VERIFYHOST:
        return curl_easy_setopt_original(Curl, opt, OFF);
        break;
    case CURL_PINNEDPUBLICKEY:
        return CURLcode::CURLE_OK;
        break;
    case CURL_URL:
        std::string url = info;
        std::regex Host(xor ("(.*).ol.epicgames.com"));
        if (bIsHybrid)
        {
            if (std::regex_search(info, std::regex(xor ("/fortnite/api/cloudstorage/system")))) {
                url = std::regex_replace(info, Host, FNhost);
                Log(xor ("Redirected Cloudstorage / System"));
            }
            else if (std::regex_search(info, std::regex(xor ("/fortnite/api/v2/versioncheck/")))) {
                url = std::regex_replace(info, Host, FNhost);
                Log(xor ("Redirected Versioncheck"));
            }
            else if (std::regex_search(info, std::regex(xor ("/fortnite/api/game/v2/profile")))) {
                url = std::regex_replace(info, Host, FNhost);
                Log(xor ("Redirected Profile"));
            }
            else if (std::regex_search(info, std::regex(xor ("/content/api/pages/fortnite-game")))) {
                url = std::regex_replace(info, Host, FNhost);
                Log(xor ("Redirected fortnite-game"));
            }
            else if (std::regex_search(info, std::regex(xor ("/affiliate/api/public/affiliates/slug")))) {
                url = std::regex_replace(info, Host, FNhost);
                Log(xor ("Redirected Affiliates"));
            }
            else if (std::regex_search(info, std::regex(xor ("/socialban/api/public/v1")))) {
                url = std::regex_replace(info, Host, FNhost);
                Log(xor ("Redirected Socialban"));
            }
            return curl_easy_setopt_original(Curl, opt, url.c_str());
            break;
        }
        else
        {
            if (std::regex_search(info, Host)) {
                url = std::regex_replace(info, Host, FNhost);
                return curl_easy_setopt_original(Curl, opt, url.c_str());
                break;
                //DebugLog(charcmb("Redirected ", Host.c_str()));
            }
        }
    }
    return curl_easy_setopt_original(Curl, opt, info);
}

/*CURLcode CurlHook(CURL* cURL, uintptr_t opt, ...)
{
    int OFF = 0;
    std::string url = info;
    std::regex Host(xor("(.*).ol.epicgames.com"));
    switch (opt)
    {
    case CURL_NOPROXY:
        return curl_easy_setopt_original(cURL, opt, "");
        break;
    case CURL_PINNEDPUBLICKEY:
        return CURLcode::CURLE_OK;
        break;
    case CURL_URL:
        if (bIsHybrid)
        {
            if (std::regex_search(info, std::regex(xor ("/fortnite/api/cloudstorage/system")))) {
                url = std::regex_replace(info, Host, FNhost);
                Log(xor ("Redirected Cloudstorage / System"));
            }
            else if (std::regex_search(info, std::regex(xor ("/fortnite/api/v2/versioncheck/")))) {
                url = std::regex_replace(info, Host, FNhost);
                Log(xor ("Redirected Versioncheck"));
            }
            else if (std::regex_search(info, std::regex(xor ("/fortnite/api/game/v2/profile")))) {
                url = std::regex_replace(info, Host, FNhost);
                Log(xor ("Redirected Profile"));
            }
            else if (std::regex_search(info, std::regex(xor ("/content/api/pages/fortnite-game")))) {
                url = std::regex_replace(info, Host, FNhost);
                Log(xor ("Redirected fortnite-game"));
            }
            else if (std::regex_search(info, std::regex(xor ("/affiliate/api/public/affiliates/slug")))) {
                url = std::regex_replace(info, Host, FNhost);
                Log(xor ("Redirected Affiliates"));
            }
            else if (std::regex_search(info, std::regex(xor ("/socialban/api/public/v1")))) {
                url = std::regex_replace(info, Host, FNhost);
                Log(xor ("Redirected Socialban"));
            }
            else if (std::regex_search(info, std::regex(xor ("platform=Windows")))) {
                if (Version == vS13)
                {
                    url = std::regex_replace(info, Host, xor ("platform=IOS"));
                    Log(xor ("Changed Windows to IOS"));
                }
            }
            return curl_easy_setopt_original(cURL, opt, url.c_str());
            break;
        }
        else
        {
            if (std::regex_search(info, Host)) {
                url = std::regex_replace(info, Host, FNhost);
                return curl_easy_setopt_original(cURL, opt, url.c_str());
                break;
                //DebugLog(charcmb("Redirected ", Host.c_str()));
            }
        }
        break;
        // case CURL_USE_SSL:
            // break;
    case CURL_VERIFYHOST:
        return curl_easy_setopt_original(cURL, opt, OFF);
        break;
    case CURL_VERIFYPEER:
        return curl_easy_setopt_original(cURL, opt, OFF);
        break;
    }
    return curl_easy_setopt_original(cURL, opt, info);
}*/

void SetupHooks()
{

    // CURL
#ifdef READFILE
    std::cin.getline(MyReadFile, myText);
    FNhost = myText;
    std::cin.getline(MyReadFile, hybridyesorno);
    if (hybridyesorno == xor ("private"))
    {
        bIsHybrid = false;
    }
    else bIsHybrid;
    MyReadFile.close();
#endif
    CurlEasyOptP = Memory::sigscan(xor ("89 54 24 10 4C 89 44 24 18 4C 89 4C 24 20 48 83 EC 28 48 85 C9"));
    CurlSetOptP = Memory::sigscan(xor ("48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 30 33 ED 49 8B F0 48 8B D9"));
    if (INGAME)
    {
        LPVOID lpCurlSetopt = reinterpret_cast<LPVOID>(CurlSetOptP);
        LONG(*CurlSetoptCast)(LPVOID, INT, va_list) = NULL;
        CurlSetoptCast = reinterpret_cast<decltype(CurlSetoptCast)>(lpCurlSetopt);
    }

}