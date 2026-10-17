#include <combaseapi.h>
#include <shlwapi.h>
#include <fstream>
#include <sstream>
#include "CommonLibrary.h"
#include "CommonData.h"


namespace config_lib
{
    // Reads a wide-string configuration from an INI file
    std::wstring GetWStringConfig(std::wstring section, std::wstring key, std::wstring file)
    {
        wchar_t buff[MAX_CHARS] = { 0 }; // Buffer for the retrieved string

        auto path = common_lib::GetFullPath(file.c_str()); // Resolve full file path
        ::GetPrivateProfileString(section.c_str(), key.c_str(), L"", buff, MAX_CHARS, path.c_str());

        return buff;
    }

    // Reads an unsigned integer configuration from an INI file
    unsigned int GetUIntConfig(std::wstring section, std::wstring key, std::wstring file)
    {
        auto path = common_lib::GetFullPath(file.c_str()); // Resolve full file path
        auto value = ::GetPrivateProfileInt(section.c_str(), key.c_str(), 0, path.c_str());

        return value;
    }

    // Reads an unsigned short configuration from an INI file
    unsigned short GetUShortConfig(std::wstring section, std::wstring key, std::wstring file)
    {
        auto path = common_lib::GetFullPath(file.c_str()); // Resolve full file path
        unsigned short value = ::GetPrivateProfileInt(section.c_str(), key.c_str(), 0, path.c_str());

        return value;
    }

    // Writes or deletes configuration entries in an INI file
    void WriteWStringConfig(std::wstring section, std::wstring key, std::wstring file, std::wstring value)
    {
        auto path = common_lib::GetFullPath(file.c_str()); // Resolve full file path

        if (key == L"")
        {
            // Delete the entire section
            ::WritePrivateProfileString(section.c_str(), nullptr, nullptr, path.c_str());
        }
        else if (value == L"")
        {
            // Delete the specific key
            ::WritePrivateProfileString(section.c_str(), key.c_str(), nullptr, path.c_str());
        }
        else
        {
            // Write the value
            ::WritePrivateProfileString(section.c_str(), key.c_str(), value.c_str(), path.c_str());
        }
    }
}

namespace common_lib
{
    // Checks if a number is strictly within a given range (exclusive)
    bool CheckInRangeNumber(long long check, long long min, long long max)
    {
        return ((check > min) && (check < max));
    }

    // Checks if a number is within a given range (inclusive)
    bool CheckInRangeNumberEqual(long long check, long long min, long long max)
    {
        return ((check >= min) && (check <= max));
    }

    // Joins a vector of strings into a single string with a delimiter
    std::string JoinString(std::string delimeter, const std::vector<std::string>& list)
    {
        std::string output; // Resulting string

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

    // Splits a string into a vector based on a character delimiter
    std::vector<std::string> SplitString(char delimiter, std::string input)
    {
        std::vector<std::string> result; // List of tokens
        std::stringstream ss(input); // Stream for parsing
        std::string token; // Buffer for each token

        while (std::getline(ss, token, delimiter))
        {
            result.push_back(token);
        }

        return result;
    }

    // Removes whitespace from both ends of a string
    std::string TrimString(std::string input)
    {
        const std::string whitespace = " \t\n\r"; // List of whitespace characters

        size_t start = input.find_first_not_of(whitespace); // First non-whitespace index
        if (start == std::string::npos)
        {
            return ""; // String is empty or whitespace-only
        }

        size_t end = input.find_last_not_of(whitespace); // Last non-whitespace index

        return input.substr(start, end - start + 1); // Extract trimmed portion
    }

    // Retrieves the current local system date and time
    SYSTEMTIME GetSystemDateTime()
    {
        SYSTEMTIME systime = { 0 }; // Windows SYSTEMTIME structure
        GetLocalTime(&systime);

        return systime;
    }

    // Converts a standard narrow string to a wide string (basic mapping)
    std::wstring ConvertStringToWString(std::string input)
    {
        return std::wstring(input.begin(), input.end());
    }

    // Converts a wide string to a standard narrow string (casts characters)
    std::string ConvertWStringToString(std::wstring input)
    {
        std::string str; // Target string

        std::transform(input.begin(), input.end(), std::back_inserter(str), [](wchar_t c) {
            return (char)c;
            });

        return str;
    }

    // Generates a unique GUID string using Windows COM API
    std::string GenerateGUID()
    {
        std::string str = ""; // Resulting GUID string
        wchar_t wguid[MAX_CHARS] = { 0 }; // Wide string buffer
        GUID guid = { 0 }; // GUID structure

        auto result = ::CoCreateGuid(&guid); // Create fresh GUID

        if (result == S_OK)
        {
            if (::StringFromGUID2(guid, wguid, ARRAYSIZE(wguid)) > 0)
            {
                str = ConvertWStringToString(wguid);
            }
        }

        return str;
    }

    // Combines the system execution path with a relative file path
    std::wstring GetFullPath(std::wstring file)
    {
        wchar_t path[MAX_CHARS] = { 0 }; // Final path buffer

        ::PathAppend(path, GetSystemPath().c_str());
        ::PathAppend(path, file.c_str());

        return path;
    }

    // Retrieves the directory where the current executable is located
    std::wstring GetSystemPath()
    {
        wchar_t path[MAX_CHARS] = { 0 }; // Path buffer

        ::GetModuleFileName(NULL, path, MAX_CHARS); // Get full exe path
        ::PathRemoveFileSpec(path); // Remove executable name to get directory

        return path;
    }

    // Executes a shell command and captures its standard output
    std::string ExecuteCMD(std::string cmd)
    {
        char buffer[MAX_PATH]; // Read buffer
        std::string result = ""; // Collected output

        // Open pipe to command execution
        std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd.c_str(), "r"), _pclose);
        if (!pipe)
        {
            return "";
        }

        // Read output line by line
        while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr)
        {
            result += buffer;
        }

        return result;
    }
}
