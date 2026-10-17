#include <shlwapi.h>
#include "CommonData.h"
#include "CommonLibrary.h"

namespace storage_lib
{
    static std::vector<std::string> listExtDisk = {}; // List of user-friendly names for external disks
    static std::vector<RemovableDriveInfo_t> listExtDrive = {}; // Metadata for all detected removable drives

    // Checks if a specific drive is formatted and accessible by reading its file system information
    static bool CheckDriveFormatted(std::string drivePath)
    {
        char volumeName[MAX_PATH] = { 0 };
        char fileSystemName[MAX_PATH] = { 0 };
        DWORD serialNumber = 0, maxComponentLen = 0, fileSystemFlags = 0;

        if (::GetVolumeInformationA(drivePath.c_str(), volumeName, sizeof(volumeName), &serialNumber, &maxComponentLen, &fileSystemFlags, fileSystemName, sizeof(fileSystemName)))
        {
            return (strlen(fileSystemName) > 0); // Returns true if a file system (FAT, NTFS, etc.) is detected
        }

        return false; // Drive might be raw or un-formatted
    }

    // Windows API callback for monitoring file copy progress
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
        // Update global shared state with the current percentage of completion
        system_data::TransferPercent.SetValue((double)TotalBytesTransferred.QuadPart / TotalFileSize.QuadPart * STANDARD_PERCENT);

        return PROGRESS_CONTINUE;
    }

    // Retrieves human-readable free storage string (e.g., "1.2 GB") for a given drive
    std::string GetDriveFreeStorage(std::string driveLetter)
    {
        ULARGE_INTEGER freeBytesAvailableToUser = { 0 }; // Space available to current user
        ULARGE_INTEGER totalNumberOfBytes = { 0 }; // Total capacity
        ULARGE_INTEGER totalNumberOfFreeBytes = { 0 }; // Unallocated space
        wchar_t buffer[MAX_PATH] = { 0 };

        if (::GetDiskFreeSpaceEx(common_lib::ConvertStringToWString(driveLetter).c_str(), &freeBytesAvailableToUser, &totalNumberOfBytes, &totalNumberOfFreeBytes))
        {
            // Format bytes into localized units (KB, MB, GB)
            ::StrFormatByteSize(freeBytesAvailableToUser.QuadPart, buffer, MAX_PATH);
        }

        return common_lib::ConvertWStringToString(buffer);
    }

    // Iterates through all logical drives to find and collect info on removable USB storage
    void CollectExternalDrivesList()
    {
        int diskCount = 1;

        listExtDisk.clear();
        listExtDrive.clear();

        // Retrieve a bitmask of all drives currently mapped to a letter
        DWORD driveMask = ::GetLogicalDrives();

        for (int i = 0; i < ('Z' - 'A' + 1); i++)
        {
            if (driveMask & (1 << i))
            {
                char driveLetter[] = { (char)('A' + i), ':', '\\', '\0' };

                // Identify if the drive is removable and has a valid format
                if ((::GetDriveTypeA(driveLetter) == DRIVE_REMOVABLE) && CheckDriveFormatted(driveLetter))
                {
                    RemovableDriveInfo_t info;
                    info.letter = driveLetter;
                    info.label = "Disk " + std::to_string(diskCount); // Assign generic label
                    listExtDisk.push_back(info.label);
                    diskCount++;

                    // Query and format storage capacity details
                    ULARGE_INTEGER freeBytesAvailableToUser = { 0 };
                    ULARGE_INTEGER totalNumberOfBytes = { 0 };
                    ULARGE_INTEGER totalNumberOfFreeBytes = { 0 };
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

    // Returns the detailed metadata list of all removable drives
    const std::vector<RemovableDriveInfo_t>& GetExternalDrivesList()
    {
        return listExtDrive;
    }

    // Returns the simple string list of removable drive labels
    const std::vector<std::string>& GetExternalLabelList()
    {
        return listExtDisk;
    }

    // Executes a robust file copy with UI progress feedback and drive buffer flushing
    bool CopyFileWithProgress(std::wstring source, std::wstring dest)
    {
        // Copy file with OS-level progress callback
        if (::CopyFileEx(source.c_str(), dest.c_str(), (LPPROGRESS_ROUTINE)CopyProgress, NULL, FALSE, COPY_FILE_NO_BUFFERING))
        {
            // Flush destination buffers to ensure data integrity on removable media
            HANDLE hFile = ::CreateFile(dest.c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH, NULL);
            if (hFile != INVALID_HANDLE_VALUE)
            {
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
