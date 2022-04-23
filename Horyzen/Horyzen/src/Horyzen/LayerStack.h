#pragma once
#include "pchheader.h"
#include "Core.h"

namespace Horyzen {
	
	class Layer;
	class LayerStack
	{
	public:

		void PushLayer(Layer* p_layer);
		void PushOverlay(Layer* p_overlay);

		void PopLayer(Layer* p_layer);
		void PopOverlay(Layer* p_overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
		std::vector<Layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return m_Layers.rend(); }

		std::vector<Layer*>::const_iterator begin() const { return m_Layers.begin(); }
		std::vector<Layer*>::const_iterator end()	const { return m_Layers.end(); }
		std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
		std::vector<Layer*>::const_reverse_iterator rend() const { return m_Layers.rend(); }

	private:

		std::vector<Layer*> m_Layers;
		u64 m_LayerInsertionIndex = 0;

	};

}

