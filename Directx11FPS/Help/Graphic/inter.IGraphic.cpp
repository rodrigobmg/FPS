#include "inter.IGraphic.h"
#include "../App/class.Direct.h"
#include <assert.h>

IGraphicDevice::~IGraphicDevice() {
	m_direct->RemoveIGraphic(this);
}


IGraphicDeviceManager::IGraphicDeviceManager(Direct *direct) {
	m_direct = direct;
}
void IGraphicDeviceManager::AddIGraphic(IGraphicDevice* value) {
	value->m_direct = m_direct;
	m_array.Add(value);
}
void IGraphicDeviceManager::RemoveIGraphic(IGraphicDevice* value) {
	UINT index = m_array.IndexOf(value);
	assert( index == -1 );
	m_array.Remove(index);
}
void IGraphicDeviceManager::OnDestroyDevice() { 
	for(UINT i=0; i < m_array.Size(); i++) {
		m_array.GetAt(i)->OnDestroyDevice();
	}
} 
void IGraphicDeviceManager::OnCreateDevice(Direct *direct) { 
	for(UINT i=0; i < m_array.Size(); i++) {
		m_array.GetAt(i)->m_direct = direct;
		m_array.GetAt(i)->OnCreateDevice();
	}
} 