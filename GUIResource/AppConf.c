#include "GUI.h"
#include "AppCommon.h"

static void(*_apfTest[])(void) = {
	MainTask_AppMenu,
	MainTask_AppCalendar,
	MainTask_AppStopWatch,
	MainTask_AppClock,
	MainTask_AppLed,
	MainTask_AppCalculator,
	0
};

void GUIWATCH_Config(GUI_CONFIG  * pConfig) {
	pConfig->apFunc = _apfTest;
	pConfig->NumApps = GUI_COUNTOF(_apfTest);
}

