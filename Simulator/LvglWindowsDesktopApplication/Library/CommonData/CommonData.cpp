#include "CommonData.h"

namespace system_data
{
    CData<HWND> WindowHandle;
    CData<std::string> CurrentDate;
    CData<std::string> CurrentTime;
}

namespace debug_data
{
    CData<std::vector<std::string>> InputParamList;
}

void CommonDataUpdateAll()
{
    system_data::WindowHandle.ResetState();
    system_data::CurrentDate.ResetState();
    system_data::CurrentTime.ResetState();
}
