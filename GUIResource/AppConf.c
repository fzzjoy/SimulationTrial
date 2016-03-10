#include "GUI.h"
#include "AppCommon.h"

static void(*_apfTest[])(void) = {
	MainTask_AppMenu,
	MainTask_AppCalendar,
	MainTask_AppStopWatch,
	0
};

void GUIWATCH_Config(GUI_CONFIG  * pConfig) {
	pConfig->apFunc = _apfTest;
	pConfig->NumApps = GUI_COUNTOF(_apfTest);
}

