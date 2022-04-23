#include "pchheader.h"
#include "LayerStack.h"

#include "Layer.h"

namespace Horyzen {

	void LayerStack::PushLayer(Layer* p_layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertionIndex, p_layer);
		m_LayerInsertionIndex++;
		p_layer->OnAttach();
	}

	void LayerStack::PushOverlay(Layer* p_overlay)
	{
		m_Layers.emplace_back(p_overlay);
		p_overlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* p_layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertionIndex, p_layer);
		if (it != m_Layers.begin() + m_LayerInsertionIndex)
		{
			p_layer->OnDetach();
			m_Layers.erase(it);
			m_LayerInsertionIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* p_overlay)
	{
		auto it = std::find(m_Layers.begin() + m_LayerInsertionIndex, m_Layers.end(), p_overlay);
		if (it != m_Layers.end())
		{
			p_overlay->OnDetach();
			m_Layers.erase(it);
		}
	}

}