#include "fwCommon_pch.h"
#include "fwgenericnotifier.h"

namespace fw
{
	namespace core
	{

		GenericNotifier::GenericNotifier()
		{

		}

		void GenericNotifier::addListener(GenericListener* pListener)
		{

			//first make sure the listener is not there already
			std::list<GenericListener*>::iterator it = std::find(m_Listeners.begin(), m_Listeners.end(), pListener);
			if (it == m_Listeners.end())
				m_Listeners.push_back(pListener);
		}

		void GenericNotifier::removeListener(GenericListener* pListener)
		{
			std::list<GenericListener*>::iterator it = std::find(m_Listeners.begin(), m_Listeners.end(), pListener);
			if (it != m_Listeners.end())
				m_Listeners.erase(it);

		}

	} //namespace



}; //namespace