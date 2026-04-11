#include <combaseapi.h>
#include <shlwapi.h>
#include "CommonLibrary.h"
#include "CommonData.h"
#include "SystemConfig.h"

namespace config_lib
{
    std::wstring GetWStringConfig(std::wstring section, std::wstring key, std::wstring file)
    {
        wchar_t buff[MAX_CHARS] = { 0 };

        auto path = common_lib::GetFullPath(file.c_str());
        ::GetPrivateProfileString(section.c_str(), key.c_str(), L"", buff, MAX_CHARS, path.c_str());

        return buff;
    }

    unsigned int GetUIntConfig(std::wstring section, std::wstring key, std::wstring file)
    {
        auto path = common_lib::GetFullPath(file.c_str());
        auto value = ::GetPrivateProfileInt(section.c_str(), key.c_str(), 0, path.c_str());

        return value;
    }

    unsigned short GetUShortConfig(std::wstring section, std::wstring key, std::wstring file)
    {
        auto path = common_lib::GetFullPath(file.c_str());
        unsigned short value = ::GetPrivateProfileInt(section.c_str(), key.c_str(), 0, path.c_str());

        return value;
    }

    void WriteWStringConfig(std::wstring section, std::wstring key, std::wstring file, std::wstring value)
    {
        auto path = common_lib::GetFullPath(file.c_str());

        if (key == L"")
        {
            ::WritePrivateProfileString(section.c_str(), nullptr, nullptr, path.c_str());
        }
        else if (value == L"")
        {
            ::WritePrivateProfileString(section.c_str(), key.c_str(), nullptr, path.c_str());
        }
        else
        {
            ::WritePrivateProfileString(section.c_str(), key.c_str(), value.c_str(), path.c_str());
        }
    }
}

namespace common_lib
{
    bool CheckInRangeNumber(int check, int min, int max)
    {
        return ((check > min) && (check < max));
    }

    std::string JoinString(std::string delimeter, const std::vector<std::string>& list)
    {
        std::string output;

        for (int i = 0; i < list.size(); i++)
        {
            if (i < list.size() - 1)
            {
                output += list[i] + delimeter;
            }
            else
            {
                output += list[i];
            }
        }

        return output;
    }

    void GetSystemStorageSize(double& totalGB, double& usedGB)
    {
        ULARGE_INTEGER freeBytesAvailableToUser; // Available free space for the user
        ULARGE_INTEGER totalNumberOfBytes; // Total disk space
        ULARGE_INTEGER totalNumberOfFreeBytes; // Actual total free space
        char buffDate[MAX_CHARS] = { 0 };
        char buffTime[MAX_CHARS] = { 0 };

        auto drive = config_lib::GetWStringConfig(SYSTEM_SECTION, STORAGE_DRIVE, SYSTEM_CONFIG);

        if (GetDiskFreeSpaceExW(drive.c_str(), &freeBytesAvailableToUser, &totalNumberOfBytes, &totalNumberOfFreeBytes))
        {
            // Convert Bytes to GB (1 GB = 1024 * 1024 * 1024 Bytes)
            totalGB = (double)totalNumberOfBytes.QuadPart / (1024 * 1024 * 1024);
            usedGB = totalGB - (double)freeBytesAvailableToUser.QuadPart / (1024 * 1024 * 1024);
        }
    }

    SYSTEMTIME GetSystemDateTime()
    {
        SYSTEMTIME systime = { 0 };
        GetLocalTime(&systime);

        return systime;
    }

    std::wstring ConvertStringToWString(std::string input)
    {
        return std::wstring(input.begin(), input.end());
    }

    std::string ConvertWStringToString(std::wstring input)
    {
        std::string str;

        std::transform(input.begin(), input.end(), std::back_inserter(str), [](wchar_t c) {
            return (char)c;
            });

        return str;
    }

    std::string GenerateGUID()
    {
        std::string str = "";
        wchar_t wguid[MAX_CHARS] = { 0 };
        GUID guid = { 0 };

        auto result = ::CoCreateGuid(&guid);

        if (result == S_OK)
        {
            if (::StringFromGUID2(guid, wguid, ARRAYSIZE(wguid)) > 0)
            {
                str = ConvertWStringToString(wguid);
            }
        }

        return str;
    }

    std::wstring GetFullPath(std::wstring file)
    {
        wchar_t path[MAX_CHARS] = { 0 };

        ::PathAppend(path, GetSystemPath().c_str());
        ::PathAppend(path, file.c_str());

        return path;
    }

    std::wstring GetSystemPath()
    {
        wchar_t path[MAX_CHARS] = { 0 };

        ::GetModuleFileName(NULL, path, MAX_CHARS);
        ::PathRemoveFileSpec(path);

        return path;
    }
}

//std::queue<bool> queueTimerExist = { };
//
//int32_t RandomRangeNumber(int32_t a, int32_t b)
//{
//    return (a + rand() % (b + 1 - a));
//}
//
//bool CheckObjectState(lv_obj_t* obj, lv_state_t state)
//{
//    if ((lv_obj_get_state(obj) & state) == state)
//    {
//        return true;
//    }
//
//    return false;
//}
//
//std::vector<BULLET_TYPE> CreateBulletList(uint8_t maxNum)
//{
//    std::vector<BULLET_TYPE> sample(maxNum), temp(maxNum);
//
//    // Get number of Blank value
//    uint8_t blankCount = RandomRangeNumber(1, maxNum - 1); // Must have at least 1 Live value
//
//    // Assign Blank value to first element range
//    for (uint8_t i = 0; i < blankCount; i++) {
//        sample[i] = BULLET_TYPE::BLANK;
//    }
//
//    // Copy to temporary
//    temp = sample;
//
//    std::mt19937 gen(rand());
//
//    // The random set must be different from the sample
//    do
//    {
//        std::shuffle(temp.begin(), temp.end(), gen);
//    } while (temp == sample);
//
//    // Update sample
//    sample = temp;
//
//    // Fill Live value
//    for (auto& item : sample)
//    {
//        if (item == BULLET_TYPE::MIN)
//        {
//            item = BULLET_TYPE::LIVE;
//        }
//    }
//
//    return sample;
//}
//
//void GetOrdinalNumber(uint8_t num, std::string& suffix)
//{
//    if (num == 1)
//    {
//        suffix = "st";
//    }
//    else if (num == 2)
//    {
//        suffix = "nd";
//    }
//    else if (num == 3)
//    {
//        suffix = "rd";
//    }
//    else
//    {
//        suffix = "th";
//    }
//}
//
//void DelayCallback(std::function<void(const void*)> func, void* data, uint32_t ms)
//{
//    struct data_t
//    {
//        std::function<void(const void*)> func;
//        void* data = nullptr;
//    };
//
//    data_t* ldata = new data_t;
//
//    if (ldata)
//    {
//        ldata->func = func;
//        ldata->data = data;
//
//        BlockGui();
//
//        lv_timer_create([](lv_timer_t* timer) {
//            auto ldata = (data_t*)lv_timer_get_user_data(timer);
//
//            ldata->func(ldata->data);
//
//            delete ldata;
//
//            UnblockGui();
//            }, ms, ldata)->repeat_count = 1;
//    }
//}
//
//void BlockGui()
//{
//    GuiBlockState.SetValue(true);
//    queueTimerExist.push(true);
//}
//
//void UnblockGui()
//{
//    queueTimerExist.pop();
//
//    if (!queueTimerExist.size())
//    {
//        GuiBlockState.SetValue(false);
//    }
//}
//
