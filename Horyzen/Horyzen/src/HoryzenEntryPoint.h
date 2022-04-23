#pragma once

#include "Horyzen/Application.h"
#include "Horyzen/Logger.h"
#include "Horyzen/Profiler/Profiler.h"


// ------- Entry Point --------
#include "Horyzen/EntryPoint.h"
// ----------------------------

#define INIT_HORYZEN_APPLICATION(AppClassName) \
Horyzen::Application* CreateApplication() \
{ \
	return new AppClassName; \
}
