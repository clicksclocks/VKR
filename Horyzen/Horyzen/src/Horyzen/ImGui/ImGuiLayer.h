#pragma once

#include "Horyzen/Layer.h"

namespace Horyzen {

	class HORYZEN_OBJECT ImGuiLayer : public Layer
	{
	public:

		virtual void OnAttach() override;
		virtual void OnDetach() override;
//		virtual void OnUpdate() override;

		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	};

}


