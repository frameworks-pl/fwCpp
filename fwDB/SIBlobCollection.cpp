#include "stdafx.h"
#include "SIBlobCollection.h"

namespace fw
{
	namespace db
	{

		SIBLOBItem SIBLOBCollection::m_InvalidBLOB = SIBLOBItem();

		SIBLOBItem::SIBLOBItem()
		{
			m_ID = INVALID_BLOB_ID;
			m_pData = NULL;

		}


		SIBLOBItem::SIBLOBItem(fw::core::ByteBuffer* pData)
		{
			m_ID = SIBLOBCollection::get()->getMaxID();
			m_pData = pData;
			SIBLOBCollection::get()->addRef(m_ID);


		}

		SIBLOBItem::~SIBLOBItem()
		{

			release();
			if (INVALID_BLOB_ID != m_ID && NULL != m_pData)
			{
				ASSERT(FALSE);
				//No ID assigned and yet some no NULL data is there ???
			}

		}

		//SIBLOBItem::SIBLOBItem()
		//{
		//    m_iRefCount = 0;
		//    m_pData = NULL;
		//    m_ID = INVALID_BLOB_ID;
		//
		//}


		SIBLOBItem::SIBLOBItem(const SIBLOBItem& pSIBLOBItem)
		{
			m_ID = INVALID_BLOB_ID;
			m_pData = NULL;

			*this = pSIBLOBItem;

		}


		SIBLOBItem& SIBLOBItem::operator=(const SIBLOBItem& pSIBLOBItem)
		{

			//release exiting object if any
			release();

			m_ID = pSIBLOBItem.getID();
			m_pData = pSIBLOBItem.getDataBuffer();
			SIBLOBCollection::get()->addRef(m_ID);

			return *this;

		}



		BLOB_ID SIBLOBItem::getID() const
		{
			return m_ID;
		}


		bool SIBLOBItem::isValid() const
		{
			return ((NULL != m_pData) && INVALID_BLOB_ID != m_ID);

		}


		void SIBLOBItem::create()
		{
			release();

			m_pData = new fw::core::ByteBuffer();
			m_ID = SIBLOBCollection::get()->getMaxID();
			SIBLOBCollection::get()->addRef(m_ID);
		}


		void SIBLOBItem::release()
		{

			if (INVALID_BLOB_ID != m_ID)
			{
				int iRefCount = SIBLOBCollection::get()->release(m_ID);
				if (0 == iRefCount)
				{
					delete m_pData;
					m_pData = NULL;
				}
				m_ID = INVALID_BLOB_ID;
			}

		}


		//----------------------------------------------------------------------------------


		SIBLOBCollection* SIBLOBCollection::m_pSIBLOBCollection = NULL;

		SIBLOBCollection::SIBLOBCollection()
		{
			m_iMaxID = 1;

		}


		SIBLOBCollection::~SIBLOBCollection()
		{
			//std::list<SIBLOBItem>::iterator it;
			//for (it = m_SIBLOBItems.begin(); it != m_SIBLOBItems.end(); it++)
			//{
			//	it->destroy();
			//}
			std::map<BLOB_ID, int>::iterator it;
			for (it = m_BLOBRefCountMap.begin(); it != m_BLOBRefCountMap.end(); it++)
			{
				if (it->second > 0)
				{
					TRACE(_T("WARNING: item with id %d in BLOB collection has reference count above zero!\n"), (int)it->first);
				}
			}
		}


		SIBLOBCollection* SIBLOBCollection::get()
		{
			if (NULL == m_pSIBLOBCollection)
				m_pSIBLOBCollection = new SIBLOBCollection;

			return m_pSIBLOBCollection;
		}


		void SIBLOBCollection::terminate()
		{
			delete m_pSIBLOBCollection;
			m_pSIBLOBCollection = NULL;
		}


		//SIBLOBItem SIBLOBCollection::addItem(const fw::core::ByteBuffer& pData)
		//{
		//    fw::core::ByteBuffer* pByteBuffer = new fw::core::ByteBuffer();
		//    *pByteBuffer = pData;
		//
		//    SIBLOBItem oSIBLOBItem(pByteBuffer, m_iMaxID);
		//    m_iMaxID++;
		//    oSIBLOBItem.addRef();
		//
		//    m_SIBLOBItems.push_back(oSIBLOBItem);
		//    return oSIBLOBItem;
		//
		//}

		//SIBLOBItem SIBLOBCollection::getItem(BLOB_ID iID)
		//{
		//
		//    std::list<SIBLOBItem>::iterator it;
		//    for (it = m_SIBLOBItems.begin(); it != m_SIBLOBItems.end(); it++)
		//    {
		//        if (iID == it->getID())
		//            break;
		//    }
		//
		//    if (m_SIBLOBItems.end() != it)
		//    {
		//        return *it;
		//    }
		//
		//    //returning invalid blob item
		//    SIBLOBItem oInvalidBLOB;
		//    return oInvalidBLOB;
		//
		//}

		//void SIBLOBCollection::releaseItem(BLOB_ID iID)
		//{
		//    std::list<SIBLOBItem>::iterator it;
		//    for (it = m_SIBLOBItems.begin(); it != m_SIBLOBItems.end(); it++)
		//    {
		//        if (iID == it->getID())
		//            break;
		//    }
		//
		//    if (m_SIBLOBItems.end() != it)
		//    {
		//        it->release();
		//        ASSERT(it->getRefCount() >= 0); //we should never get negative value here
		//        if (it->getRefCount() == 0)
		//        {
		//            it->destroy();
		//            m_SIBLOBItems.erase(it);
		//        }
		//    }
		//
		//}


		int SIBLOBCollection::getMaxID()
		{
			int iMaxID = m_iMaxID;
			m_iMaxID++;

			return iMaxID;
		}



		void SIBLOBCollection::addRef(BLOB_ID iID)
		{

			//if the blob has invalid ID we will not regiser it!!!
			if (INVALID_BLOB_ID != iID)
			{
				std::map<BLOB_ID, int>::iterator it = m_BLOBRefCountMap.find(iID);
				if (m_BLOBRefCountMap.end() != it)
				{
					it->second++;
				}
				else
				{
					m_BLOBRefCountMap.insert(std::map<BLOB_ID, int>::value_type(iID, 1));
				}
			}

		}

		int SIBLOBCollection::release(BLOB_ID iID)
		{
			std::map<BLOB_ID, int>::iterator it = m_BLOBRefCountMap.find(iID);
			if (m_BLOBRefCountMap.end() != it)
			{
				it->second--;
				if (it->second == 0)
				{
					m_BLOBRefCountMap.erase(it);
					return 0;
				}

				return it->second;
			}
			else
			{
				ASSERT(FALSE);
				//Looks like we are trying to decrease reference count on an object that is not in the map!!!
			}


			return 0;


		}





	}; //namespace 
}