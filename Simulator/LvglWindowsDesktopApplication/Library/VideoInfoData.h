#ifndef _VIDEOINFO_DATA_H
#define _VIDEOINFO_DATA_H

// File names for persistence
#define VIDEOINFO_DATA L"videoinfo_data.ini" // Filename for global video information database
#define VIDEOINFO_DATA_FOLDER L"video_data" // Folder name for storing individual video metadata files

// Section names in INI files
#define VIF_TEMP_SECTION L"TempInfo" // Section for temporary video metadata storage

// Key names for video metadata attributes
#define VIF_VIDEO_ID L"VIDEO_ID" // Unique identifier for the video entry
#define VIF_VIDEO_EVENT L"VIDEO_EVENT" // Event title or subject associated with the recording
#define VIF_VIDEO_NAME L"VIDEO_NAME" // Filename or display title of the video
#define VIF_VIDEO_CATEGORY L"VIDEO_CATEGORY" // Genre or classification of the video
#define VIF_VIDEO_DESC L"VIDEO_DESC" // Detailed textual description
#define VIF_VIDEO_AUTHOR L"VIDEO_AUTHOR" // Name of the creator or operator
#define VIF_VIDEO_DAY L"VIDEO_DAY" // Recording day of the month
#define VIF_VIDEO_MONTH L"VIDEO_MONTH" // Recording month
#define VIF_VIDEO_YEAR L"VIDEO_YEAR" // Recording year
#define VIF_VIDEO_HOUR L"VIDEO_HOUR" // Recording hour (24h format)
#define VIF_VIDEO_MINUTE L"VIDEO_MINUTE" // Recording minute
#define VIF_VIDEO_SECOND L"VIDEO_SECOND" // Recording second
#define VIF_VIDEO_PATH L"VIDEO_PATH" // Absolute or relative filesystem path to the video file

#endif // _VIDEOINFO_DATA_H
