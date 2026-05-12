#include "CommonDataType.h"
#include "CommonLibrary.h"

static int RandomRange(int a, int b)
{
    return a + rand() % (b - a);
}

int main()
{
    int videoInfoCount = 0;
    wchar_t dummyVideoPath[MAX_CHARS] = { 0 };
    wchar_t outputPath[MAX_CHARS] = { 0 };

    std::cout << "Input number of video info to be created: ";
    std::cin >> videoInfoCount;

    srand(time(0));

    for (int i = 1; i <= videoInfoCount; i++)
    {
        // Random time
        SYSTEMTIME time = {
            RandomRange(2000, 2100),
            RandomRange(1, 13),
            0,
            RandomRange(1, 32),
            RandomRange(0, 24),
            RandomRange(0, 60),
            RandomRange(0, 60),
            0,
        };

        // Set info
        videoinfo_lib::videoinfo_t info = {
            common_lib::GenerateGUID(),
            "Event " + std::to_string(i),
            "Name " + std::to_string(i),
            "Category " + std::to_string(i),
            "Desc " + std::to_string(i),
            "Author " + std::to_string(i),
            time,
        };

        // Write to ini data
        videoinfo_lib::CreateNewData(info.videoID, info);

        // Create new info folder
        swprintf_s(dummyVideoPath, L"%s\\%s\\dummy_video.mp4", common_lib::GetSystemPath().c_str(), SCRIPT_DIRECTORY);
        swprintf_s(outputPath, L"%s\\video_data\\%s\\", common_lib::GetSystemPath().c_str(), common_lib::ConvertStringToWString(info.videoID).c_str());
        ::CreateDirectory(outputPath, NULL);
        ::CopyFile(dummyVideoPath, (std::wstring(outputPath) + L"\\video.mp4").c_str(), FALSE);

        debug_println("Create info #" + std::to_string(i) + " " + info.videoID);
    }

    system("pause");
}
