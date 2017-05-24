#include "stdafx.h"
#include "SIBlobCollection.h"

namespace fw
{
	namespace db
	{

		BLOBItem BLOBCollection::m_InvalidBLOB = BLOBItem();

		BLOBItem::BLOBItem()
		{
			m_ID = INVALID_BLOB_ID;
			m_pData = NULL;

		}


		BLOBItem::BLOBItem(fw::core::ByteBuffer* pData)
		{
			m_ID = BLOBCollection::get()->getMaxID();
			m_pData = pData;
			BLOBCollection::get()->addRef(m_ID);


		}

		BLOBItem::~BLOBItem()
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


		BLOBItem::BLOBItem(const BLOBItem& pSIBLOBItem)
		{
			m_ID = INVALID_BLOB_ID;
			m_pData = NULL;

			*this = pSIBLOBItem;

		}


		BLOBItem& BLOBItem::operator=(const BLOBItem& pSIBLOBItem)
		{

			//release exiting object if any
			release();

			m_ID = pSIBLOBItem.getID();
			m_pData = pSIBLOBItem.getDataBuffer();
			BLOBCollection::get()->addRef(m_ID);

			return *this;

		}



		BLOB_ID BLOBItem::getID() const
		{
			return m_ID;
		}


		bool BLOBItem::isValid() const
		{
			return ((NULL != m_pData) && INVALID_BLOB_ID != m_ID);

		}


		void BLOBItem::create()
		{
			release();

			m_pData = new fw::core::ByteBuffer();
			m_ID = BLOBCollection::get()->getMaxID();
			BLOBCollection::get()->addRef(m_ID);
		}


		void BLOBItem::release()
		{

			if (INVALID_BLOB_ID != m_ID)
			{
				int iRefCount = BLOBCollection::get()->release(m_ID);
				if (0 == iRefCount)
				{
					delete m_pData;
					m_pData = NULL;
				}
				m_ID = INVALID_BLOB_ID;
			}

		}


		//----------------------------------------------------------------------------------


		BLOBCollection* BLOBCollection::m_pSIBLOBCollection = NULL;

		BLOBCollection::BLOBCollection()
		{
			m_iMaxID = 1;

		}


		BLOBCollection::~BLOBCollection()
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


		BLOBCollection* BLOBCollection::get()
		{
			if (NULL == m_pSIBLOBCollection)
				m_pSIBLOBCollection = new BLOBCollection;

			return m_pSIBLOBCollection;
		}


		void BLOBCollection::terminate()
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


		int BLOBCollection::getMaxID()
		{
			int iMaxID = m_iMaxID;
			m_iMaxID++;

			return iMaxID;
		}



		void BLOBCollection::addRef(BLOB_ID iID)
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

		int BLOBCollection::release(BLOB_ID iID)
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