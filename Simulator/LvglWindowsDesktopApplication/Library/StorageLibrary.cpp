#include <shlwapi.h>
#include "CommonData.h"
#include "CommonLibrary.h"

namespace storage_lib
{
    static std::vector<std::string> listExtDisk = {};
    static std::vector<RemovableDriveInfo_t> listExtDrive = {};

    static bool CheckDriveFormatted(std::string drivePath)
    {
        char volumeName[MAX_PATH] = { 0 };
        char fileSystemName[MAX_PATH] = { 0 };
        DWORD serialNumber = 0, maxComponentLen = 0, fileSystemFlags = 0;

        if (::GetVolumeInformationA(drivePath.c_str(), volumeName, sizeof(volumeName), &serialNumber, &maxComponentLen, &fileSystemFlags, fileSystemName, sizeof(fileSystemName)))
        {
            return (strlen(fileSystemName) > 0); // Formatted
        }

        return false; // Un-formatted
    }

    static DWORD CALLBACK CopyProgress(
        LARGE_INTEGER TotalFileSize,
        LARGE_INTEGER TotalBytesTransferred,
        LARGE_INTEGER StreamSize,
        LARGE_INTEGER StreamBytesTransferred,
        DWORD dwStreamNumber,
        DWORD dwCallbackReason,
        HANDLE hSourceFile,
        HANDLE hDestinationFile,
        LPVOID lpData)
    {
        system_data::TransferPercent.SetValue((double)TotalBytesTransferred.QuadPart / TotalFileSize.QuadPart * STANDARD_PERCENT);

        return PROGRESS_CONTINUE;
    }

    std::string GetDriveFreeStorage(std::string driveLetter)
    {
        ULARGE_INTEGER freeBytesAvailableToUser = { 0 }; // Available free space for the user
        ULARGE_INTEGER totalNumberOfBytes = { 0 }; // Total disk space
        ULARGE_INTEGER totalNumberOfFreeBytes = { 0 }; // Actual total free space
        wchar_t buffer[MAX_PATH] = { 0 };

        if (::GetDiskFreeSpaceEx(common_lib::ConvertStringToWString(driveLetter).c_str(), &freeBytesAvailableToUser, &totalNumberOfBytes, &totalNumberOfFreeBytes))
        {
            ::StrFormatByteSize(freeBytesAvailableToUser.QuadPart, buffer, MAX_PATH);
        }

        return common_lib::ConvertWStringToString(buffer);
    }

    void CollectExternalDrivesList()
    {
        int diskCount = 1;

        listExtDisk.clear();
        listExtDrive.clear();

        // Get the mask of the active drives (returns the bitmask, bit 0 is A, bit 1 is B...).
        DWORD driveMask = ::GetLogicalDrives();

        for (int i = 0; i < ('Z' - 'A' + 1); i++)
        {
            if (driveMask & (1 << i))
            {
                char driveLetter[] = { (char)('A' + i), ':', '\\', '\0' };

                // Filter only external (removable) drives
                if ((::GetDriveTypeA(driveLetter) == DRIVE_REMOVABLE) && CheckDriveFormatted(driveLetter))
                {
                    RemovableDriveInfo_t info;
                    info.letter = driveLetter;

                    // Get label
                    //char volumeName[MAX_PATH] = { 0 };
                    //if (::GetVolumeInformationA(driveLetter, volumeName, sizeof(volumeName), NULL, NULL, NULL, NULL, 0)) {
                    //    info.label = volumeName;
                    //}
                    info.label = "Disk " + std::to_string(diskCount);
                    listExtDisk.push_back(info.label);
                    diskCount++;

                    // Get storage
                    ULARGE_INTEGER freeBytesAvailableToUser = { 0 }; // Available free space for the user
                    ULARGE_INTEGER totalNumberOfBytes = { 0 }; // Total disk space
                    ULARGE_INTEGER totalNumberOfFreeBytes = { 0 }; // Actual total free space
                    wchar_t bufferTotalSpace[MAX_PATH] = { 0 };
                    wchar_t bufferFreeSpace[MAX_PATH] = { 0 };

                    if (::GetDiskFreeSpaceEx(common_lib::ConvertStringToWString(driveLetter).c_str(), &freeBytesAvailableToUser, &totalNumberOfBytes, &totalNumberOfFreeBytes))
                    {
                        ::StrFormatByteSize(totalNumberOfBytes.QuadPart, bufferTotalSpace, MAX_PATH);
                        ::StrFormatByteSize(freeBytesAvailableToUser.QuadPart, bufferFreeSpace, MAX_PATH);

                        info.totalGB = common_lib::ConvertWStringToString(bufferTotalSpace);
                        info.freeGB = common_lib::ConvertWStringToString(bufferFreeSpace);
                    }

                    listExtDrive.push_back(info);
                }
            }
        }
    }

    const std::vector<RemovableDriveInfo_t>& GetExternalDrivesList()
    {
        return listExtDrive;
    }

    const std::vector<std::string>& GetExternalLabelList()
    {
        return listExtDisk;
    }

    bool CopyFileWithProgress(std::wstring source, std::wstring dest)
    {
        if (::CopyFileEx(source.c_str(), dest.c_str(), (LPPROGRESS_ROUTINE)CopyProgress, NULL, FALSE, COPY_FILE_NO_BUFFERING))
        {
            // Open dest file
            HANDLE hFile = ::CreateFile(dest.c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH, NULL);
            if (hFile != INVALID_HANDLE_VALUE)
            {
                // Wait for sync to drive
                ::FlushFileBuffers(hFile);
                ::CloseHandle(hFile);
            }

            return true;
        }
        else
        {
            return false;
        }
    }
}
