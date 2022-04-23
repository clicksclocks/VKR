#pragma once
#include <Horyzen.h>

#include "AppLayer.h"

class MyApp : public Horyzen::Application
{
public:

	MyApp()
		: Horyzen::Application()
	{
		AttachLayer(new AppLayer());
	}

};