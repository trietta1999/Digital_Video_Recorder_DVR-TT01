#ifndef _MAIN_SCREEN_H
#define _MAIN_SCREEN_H

#include "BaseScreen.h"

class MainScreen : public BaseScreen
{
public:
    MainScreen(SCREEN_NAME screen);
    ~MainScreen();

private:
    static void OnClickNew(lv_obj_t* obj);

    static void UpdateDate();
    static void UpdateTime();
    static void UpdateStorage();
};

#endif // _MAIN_SCREEN_H
