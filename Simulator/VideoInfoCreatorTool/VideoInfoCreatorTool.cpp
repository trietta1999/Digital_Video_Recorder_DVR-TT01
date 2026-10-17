#include "CommonDataType.h"
#include "CommonLibrary.h"

// Generates a random integer within the range [a, b)
static int RandomRange(int a, int b)
{
    return a + rand() % (b - a);
}

int main()
{
    int videoInfoCount = 0; // Number of video records to generate
    wchar_t dummyVideoPath[MAX_CHARS] = { 0 }; // Path to the source dummy MP4 file
    wchar_t outputPath[MAX_CHARS] = { 0 }; // Temporary buffer for output directory paths

    std::cout << "Input number of video info to be created: ";
    std::cin >> videoInfoCount;

    srand((unsigned int)time(0)); // Seed random generator with current time

    for (int i = 1; i <= videoInfoCount; i++)
    {
        // Generate a random timestamp within reasonable ranges
        SYSTEMTIME time = {
            (WORD)RandomRange(2000, 2100), // Year
            (WORD)RandomRange(1, 13),      // Month
            0,                             // DayOfWeek
            (WORD)RandomRange(1, 28),      // Day (capped at 28 to avoid month length issues)
            (WORD)RandomRange(0, 24),      // Hour
            (WORD)RandomRange(0, 60),      // Minute
            (WORD)RandomRange(0, 60),      // Second
            0,                             // Milliseconds
        };

        // Populate video metadata with sequential test data and unique GUIDs
        videoinfo_lib::videoinfo_t info = {
            common_lib::GenerateGUID(),
            "Event " + std::to_string(i),
            "Name " + std::to_string(i),
            "Category " + std::to_string(i),
            "Desc " + std::to_string(i),
            "Author " + std::to_string(i),
            time,
        };

        // Persist video metadata to an INI/configuration file via library
        videoinfo_lib::CreateNewData(info.videoID, info);

        // Define paths for source dummy video and target video data directory
        swprintf_s(dummyVideoPath, L"%s\\%s\\dummy_video.mp4", common_lib::GetSystemPath().c_str(), SCRIPT_DIRECTORY);
        swprintf_s(outputPath, L"%s\\video_data\\%s\\", common_lib::GetSystemPath().c_str(), common_lib::ConvertStringToWString(info.videoID).c_str());

        // Create the directory for the specific video ID
        ::CreateDirectory(outputPath, NULL);

        // Copy the dummy video file to the new directory
        ::CopyFile(dummyVideoPath, (std::wstring(outputPath) + L"\\video.mp4").c_str(), FALSE);

        debug_println("Create info #" + std::to_string(i) + " " + info.videoID);
    }

    system("pause");
}
