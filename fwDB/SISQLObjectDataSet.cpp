#include "stdafx.h"
#include "SICryptException.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

namespace fw
{
	namespace db
	{

		SISQLObjectDataSet::SISQLObjectDataSet()
		{

			m_pMaxIDDataSet = NULL;

		}


		SISQLObjectDataSet::~SISQLObjectDataSet()
		{
			clear();

		}

		SISQLObjectDataSet::SISQLObjectDataSet(CRuntimeClass* pClass)
		{

			if (NULL == pClass)
				throw SIDBException(_T("Cannot create object dataset - the type of object is unknown."));

			m_RuntimeClass = *pClass;
			m_pMaxIDDataSet = NULL;

		}

		void SISQLObjectDataSet::initialize(SIMaxIDDataSet* pMaxIdDataSet)
		{

			m_pMaxIDDataSet = pMaxIdDataSet;


		}




		int SISQLObjectDataSet::getReadQuery(CString& pQuery)
		{

			int iVarsCount = 0;

			CRuntimeClass rc = getRuntimeClass();

			ASSERT(rc.IsDerivedFrom(RUNTIME_CLASS(SISQLObject)));
			CObject* pObject = rc.CreateObject();
			if (NULL != pObject)
			{
				SISQLObject* pSQLObject = (SISQLObject*)pObject;
				if (NULL != pSQLObject)
					iVarsCount = pSQLObject->getUpdateQuery(pQuery, false, false);
				delete pObject;
				pObject = NULL;
			}

			return iVarsCount;

		}


		int SISQLObjectDataSet::getObjectsToUpdate(SQLObjectList& pObjsToUpdate)
		{

			int iCnt = 0;

			//first "regular" objects
			SQLObjectMap::iterator it;
			for (it = m_ObjectList.begin(); it != m_ObjectList.end(); it++)
			{
				if (it->second->getState() != SISQLObject::STATE_OK)
				{
					pObjsToUpdate.push_back(it->second);
					iCnt++;
				}
			}

			//then objects to delete
			for (it = m_DeletedObjectList.begin(); it != m_DeletedObjectList.end(); it++)
			{
				if (it->second->getState() != SISQLObject::STATE_OK)
				{
					pObjsToUpdate.push_back(it->second);
					iCnt++;
				}

			}


			return iCnt;


		}

		void SISQLObjectDataSet::clear()
		{

			SQLObjectMap::iterator it;
			for (it = m_ObjectList.begin(); it != m_ObjectList.end(); it++)
				delete it->second;

			m_ObjectList.clear();

			//is should never happen that when collection is being destroyed
			//there are still some objects to be deleted!!!
			if (false == m_DeletedObjectList.empty())
				TRACE(_T("ERROR: Deleted object list is NOT empty!!!!"));


		}

		bool SISQLObjectDataSet::get(const SQL_ID& pSQLId, SISQLObject* pObject) const
		{
			SQLObjectMap::const_iterator objectIt = m_ObjectList.find(pSQLId);
			if (objectIt != m_ObjectList.end())
			{
				pObject->initialize(objectIt->second);
				return true;
			}

			return false;

		}

		const SISQLObject* SISQLObjectDataSet::get(const SQL_ID& pSQLID) const
		{
			SQLObjectMap::const_iterator objectIt = m_ObjectList.find(pSQLID);
			if (objectIt != m_ObjectList.end())
				return objectIt->second;

			return NULL;

		}


		SQL_ID SISQLObjectDataSet::set(const SISQLObject* pSQLObject)
		{

			SISQLObject::StateEnum eCurrentState = pSQLObject->getState();


			//#143 if the object has already ID assigned,
			//check what is its current state in the database
			if (INVALID_SQL_ID != pSQLObject->getSQLID())
			{
				const SISQLObject* pTemp = get(pSQLObject->getSQLID());
				if (NULL != pTemp)
				{
					//we assume that it is possible that client still sees the object as new
					//while database has already saved it
					//in this case we will alter the current state to force update
					if (SISQLObject::STATE_OK == pTemp->getState() && (SISQLObject::STATE_NEW == eCurrentState))
						eCurrentState = SISQLObject::STATE_UPDATE;
				}
			}


			switch (eCurrentState)
			{
			case SISQLObject::STATE_NEW:
			case SISQLObject::STATE_DELETE:
			case SISQLObject::STATE_UPDATE:
			case SISQLObject::STATE_OK_LOADED:
				return set(pSQLObject, eCurrentState);
				break;
			case SISQLObject::STATE_OK:
				//assuming that this is request to update the object
				return set(pSQLObject, SISQLObject::STATE_UPDATE);
				break;
			default:
			{
				CString s;
				s.Format(_T("Trying to set object with unknown state (%d)."), pSQLObject->getState());
				throw SIDBException(s);
			}
			}
		}


		int SISQLObjectDataSet::countItemsToSave() const
		{

			int iCount = 0;
			SQLObjectMap::const_iterator it;
			for (it = m_ObjectList.begin(); it != m_ObjectList.end(); it++)
			{
				if ((it->second->getState() != SISQLObject::STATE_OK) && (it->second->getState() != SISQLObject::STATE_OK_LOADED))
					iCount++;
			}

			//include deleted objects as well
			iCount += (int)m_DeletedObjectList.size();

			return iCount;


		}


		SQL_ID SISQLObjectDataSet::set(const SISQLObject* pSQLObject, SISQLObject::StateEnum pState)
		{
			SQL_ID sqlID = INVALID_SQL_ID;
			bool bDoNotNotify = false;


			//sql max ids collection does not have this member set
			//so we must do exception for it!
			if ((false == pSQLObject->IsKindOf(RUNTIME_CLASS(SIMaxID))) && NULL == m_pMaxIDDataSet)
			{
				CString s;
				CString sClassName(getRuntimeClass().m_lpszClassName);
				s.Format(_T("Collection of %s has no max ID dataset initialized."), sClassName);
				throw db::SIDBException(s);
			}

			if (pSQLObject)
			{
				CObject* pp = (CObject*)pSQLObject;
				CRuntimeClass* pRC = pp->GetRuntimeClass();//pSQLObject->GetRuntimeClass();
				if (pRC)
				{
					CString sClassToCreate(pRC->m_lpszClassName);
					CString sClassOfDataSet(m_RuntimeClass.m_lpszClassName);

					if (sClassToCreate != sClassOfDataSet)
						throw db::SIDBException(_T("Trying to add an object of wrong type."));

					switch (pState)
					{
					case SISQLObject::STATE_NEW:
					{
						CObject* pObject = pRC->CreateObject();
						if (NULL != pObject)
						{
							SISQLObject* pNewSQLObject = (SISQLObject*)pObject;
							if (pNewSQLObject)
							{
								/*                if (INVALID_SQL_ID != pSQLObject->getSQLID())
												{
												  CString s;
												  s.Format(_T("New object should not have ID assigned!"));
												  throw SIDBException(s);
												} */

								try
								{
									pNewSQLObject->initialize(pSQLObject);
								}
								catch (fw::crypt::CryptException& ex)
								{
									delete pNewSQLObject;
									pNewSQLObject = NULL;
									throw ex;
								}
								//assign unique SQL id if object does not have it yet
								if (INVALID_SQL_ID == pSQLObject->getSQLID())
								{
									pNewSQLObject->setSQLID(m_pMaxIDDataSet->getNextID(CString(pNewSQLObject->GetRuntimeClass()->m_lpszClassName)));
									m_ObjectList.insert(SQLObjectMap::value_type(pNewSQLObject->getSQLID(), pNewSQLObject));
									sqlID = pNewSQLObject->getSQLID();
								}
								else
								{
									SQLObjectMap::iterator it = m_ObjectList.find(pNewSQLObject->getSQLID());
									if (it != m_ObjectList.end())
									{
										try
										{
											it->second->initialize(pNewSQLObject);
										}
										catch (fw::crypt::CryptException& ex)
										{
											delete pNewSQLObject;
											pNewSQLObject = NULL;
											throw ex;
										}

										sqlID = it->second->getSQLID();
										delete pNewSQLObject;
										pNewSQLObject = NULL;
									}
									else
									{
										CString s;
										s.Format(_T("Cannot update object with id %d, object not found."), pSQLObject->getSQLID());
										throw SIDBException(s);
									}
								}
#ifdef ALLOW_PERFORMANCE_ISSUES
								CString sClassName(getRuntimeClass().m_lpszClassName);
								fw::debug::Logger::Log(LEVEL_SIDB, "{SISQLObjectDataSet::set (STATE_NEW)} collection:%s, sql_id:%d, current items cnt:%d", fw::core::TextConv::Unicode2UTF8(sClassName).c_str(), (int)sqlID, (int)m_ObjectList.size());
#endif //ALLOW_PERFORMANCE_ISSUES
							}
							else
							{
								delete pObject;
								pObject = NULL;
							}
						}
					}
					break;
					case SISQLObject::STATE_DELETE:
					{
						//find the object in the map and move it to the "deleted" map
						SQLObjectMap::iterator it = m_ObjectList.find(pSQLObject->getSQLID());
						if (it != m_ObjectList.end())
						{
							CObject* pObject = pRC->CreateObject();
							if (NULL != pObject)
							{
								SISQLObject* pSQLObject = (SISQLObject*)pObject;
								if (NULL != pSQLObject)
								{
									try
									{
										pSQLObject->initialize(it->second);
									}
									catch (fw::crypt::CryptException& ex)
									{
										delete pSQLObject;
										pSQLObject = NULL;
										throw ex;
									}

									pSQLObject->setDeleted();
									if (m_DeletedObjectList.end() != m_DeletedObjectList.find(pSQLObject->getSQLID()))
										throw SIDBException(_T("Trying to delete the same object twice!"));

									//add copy of the object to the "deleted" map
									m_DeletedObjectList.insert(SQLObjectMap::value_type(pSQLObject->getSQLID(), pSQLObject));

									//now delete the copy from the "regular" map
									delete it->second;
									m_ObjectList.erase(it);
								}
							}
							else
							{
								delete pObject;
								pObject = NULL;
								throw SIDBException(_T("Deleting object failed (this is not an SQL object."));
							}
						}
						else
						{
							CString s;
							s.Format(_T("Cannot update object with id %d, object not found."), pSQLObject->getSQLID());
							throw SIDBException(s);
						}
					}
					break;
					case SISQLObject::STATE_UPDATE:
					{

						//SQLObjectList::iterator it = find_if(m_ObjectList.begin(), m_ObjectList.end(), sqlobjectMatch(pSQLObject->getSQLID()));
						SQLObjectMap::iterator it = m_ObjectList.find(pSQLObject->getSQLID());
						if (it != m_ObjectList.end())
						{
							it->second->initialize(pSQLObject);
							it->second->setState(SISQLObject::STATE_UPDATE);
							sqlID = it->second->getSQLID();
#ifdef ALLOW_PERFORMANCE_ISSUES
							CString sClassName(getRuntimeClass().m_lpszClassName);
							fw::debug::Logger::Log(LEVEL_SIDB, "{SISQLObjectDataSet::set (STATE_UPDATE)} collection:%s, sql_id:%d, state:%s", fw::core::TextConv::Unicode2UTF8(sClassName).c_str(), sqlID, fw::core::TextConv::Unicode2UTF8(SISQLObject::stateToString(it->second->getState())).c_str());
#endif //ALLOW_PERFORMANCE_ISSUES
						}
						else
						{
							CString s;
							s.Format(_T("Cannot update object with id %d, object not found."), pSQLObject->getSQLID());
							throw SIDBException(s);
						}

					}
					break;
					case SISQLObject::STATE_OK_LOADED:
					{

						bDoNotNotify = true;
						//the only situation when such operation is allowed is adding an object
						//that has been just loaded
						  //SQLObjectList::iterator it = find_if(m_ObjectList.begin(), m_ObjectList.end(), sqlobjectMatch(SQLObject->getSQLID()));
						SQLObjectMap::iterator it = m_ObjectList.find(pSQLObject->getSQLID());
						//if (it != m_ObjectList.end())
						//{
						//  CString s;
						//  s.Format(_T("Trying to add again object with id %d"), it->second->getSQLID());
						//  throw SIDBException(s);
						//}

						CObject* pObject = pRC->CreateObject();
						if (NULL != pObject)
						{
							SISQLObject* pSQLObjectOk = (SISQLObject*)pObject;
							if (pSQLObjectOk)
							{
								try
								{
									if (true == pSQLObjectOk->initialize(pSQLObject))
									{
										pSQLObjectOk->setState(SISQLObject::STATE_OK);
										if (it == m_ObjectList.end())
											m_ObjectList.insert(SQLObjectMap::value_type(pSQLObject->getSQLID(), pSQLObjectOk));
										else
										{
											delete it->second;
											it->second = pSQLObjectOk;
										}
										sqlID = pSQLObjectOk->getSQLID();
#ifdef ALLOW_PERFORMANCE_ISSUES
										CString sClassName(getRuntimeClass().m_lpszClassName);
										fw::debug::Logger::Log(LEVEL_SIDB, "{SISQLObjectDataSet::set (STATE_OK_LOADED)} Collection:%s [count: %d], sql_id:%d",
											fw::core::TextConv::Unicode2UTF8(sClassName).c_str(), (int)m_ObjectList.size(), (int)sqlID);
#endif //ALLOW_PEFORMANCE_ISSUES
									}
									else
									{
										delete pObject;
										pObject = NULL;
										throw SIDBException(_T("At least one object failed to initialize."));
									}
								}
								catch (fw::crypt::CryptException& ex)
								{
									delete pObject;
									pObject = NULL;
									throw ex;
								}
							}
						}
					}
					break;
					case SISQLObject::STATE_OK:
						//this should never happen!!!
					{
						CString s;
						s.Format(_T("Trying to add object (sqlid = %d) with state OK."), pSQLObject->getSQLID());
						throw SIDBException(s);
					}
					break;
					}


				}
				else throw db::SIDBException(_T("The type object is unknown."));

			}
			else
			{
				throw db::SIDBException(_T("Trying to add non existing object to collection."));
			}

			if (false == bDoNotNotify)
			{
				//notify listeners that an object has been modified
				std::list<fw::core::GenericListener*>::iterator it;
				for (it = m_Listeners.begin(); it != m_Listeners.end(); it++)
					((db::SISQLObjectDataSet_Listener*)(*it))->onObjectModified();
			}

			return sqlID;

		}


		int SISQLObjectDataSet::size() const
		{
			return (int)m_ObjectList.size();

		}


		bool SISQLObjectDataSet::initIterator() const
		{

			if (m_ObjectList.size() > 0)
			{
				m_Iterator = m_ObjectList.begin();
				return true;
			}

			m_Iterator = m_ObjectList.end();

			return false;


		}



		const SISQLObject* SISQLObjectDataSet::getNextObject() const
		{

			while (m_Iterator != m_ObjectList.end())
			{
				if (false == m_Iterator->second->isDeleted())
				{
					const db::SISQLObject* pObject = m_Iterator->second;
					m_Iterator++;
					return pObject;
				}
				m_Iterator++;
			}

			return NULL;

		}


		void SISQLObjectDataSet::erase(const SISQLObject* pSQLObject)
		{

			if (pSQLObject && (INVALID_SQL_ID != pSQLObject->getSQLID()))
			{
				//SQLObjectList::iterator it = find_if(m_ObjectList.begin(), m_ObjectList.end(), sqlobjectMatch(pSQLObject->getSQLID()));
				SQLObjectMap::iterator it = m_ObjectList.find(pSQLObject->getSQLID());
				if (it != m_ObjectList.end())
				{
					it->second->setDeleted();
					set(it->second);
				}

			}
			else
			{
				throw SIDBException(_T("Trying to remove item that has no ID!"));
			}
		}


		void SISQLObjectDataSet::setSQLID(SISQLObject* pSQLObject, const SQL_ID& pID)
		{
			pSQLObject->setSQLID(pID);

		}


		SISQLObject* SISQLObjectDataSet::createObject()
		{

			CObject* pObject = m_RuntimeClass.CreateObject();
			if (NULL != pObject)
			{
				SISQLObject* pSQLObject = (SISQLObject*)pObject;
				return pSQLObject;
			}

			return NULL;

		}



		void SISQLObjectDataSet::destroyDeletedObjects()
		{
			SQLObjectMap::iterator it;
			for (it = m_DeletedObjectList.begin(); it != m_DeletedObjectList.end(); it++)
				delete it->second;

			m_DeletedObjectList.clear();

		}


		SQL_ID SISQLObjectDataSet::getMaxIDUsed() const
		{

			//assuming that the map is sorted from smallest value to the largest
			//we just need to go to the end of the map and return the key
			if (false == m_ObjectList.empty())
			{
				SQLObjectMap::const_iterator it = m_ObjectList.end();
				it--;
				return it->first;

			}

			return INVALID_SQL_ID;


		}

		void SISQLObjectDataSet::forceFullUpdate()
		{
			SQLObjectMap::iterator it;
			for (it = m_ObjectList.begin(); it != m_ObjectList.end(); it++)
				it->second->setUpdated();
		}




	} //namespace 
}