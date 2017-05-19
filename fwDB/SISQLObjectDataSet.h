#ifndef SISQLOBJECTDATASET_H_INCLUDED
#define SISQLOBJECTDATASET_H_INCLUDED

#include "sisqlobject.h"

namespace sidb
{

class SIGenericConnection;

struct sqlobjectMatch 
{

    enum SQLObjectSearchType
    {
        TYPE_SQLID = 1, //matching based on sql object identifier
    };

    sqlobjectMatch(const SQL_ID& pSQLID) : m_Identifier(pSQLID) { m_iMatchType = TYPE_SQLID; };

    bool operator()(SISQLObject* pSQLObject) 
    {
        switch (m_iMatchType)
        {
            case TYPE_SQLID:
              return (pSQLObject->getSQLID() == m_Identifier);
            default:
                return false;
        }

        return false;
    }

	private:
		    SQL_ID m_Identifier;

        //the type of searching
        int m_iMatchType;
};


class SISQLObjectDataSet_Listener : public fw::core::GenericListener
{
  public:

    //notifies about change in the count of stored/loaded items
    virtual void onObjectModified() { }

}; 



class SIMaxIDDataSet;
class SIDB_DLLEXPORT SISQLObjectDataSet : public fw::core::GenericNotifier
{

  friend class SIGenericConnection;
  public:
    SISQLObjectDataSet(CRuntimeClass* pClass);
    virtual ~SISQLObjectDataSet();

    virtual void initialize(SIMaxIDDataSet* pMaxIdDataSet);

    //returns runtime class for this data set (can be used to produce objects of the kind)
    CRuntimeClass getRuntimeClass() { return m_RuntimeClass; }


    //destroys all object in dataset and clears the collection
    void clear();

    //adds new or updets an exitsing item in the dataset
    //returns ID of the object set (useful if object without an ID is being added to the list)
    virtual SQL_ID set(const SISQLObject* pSQLObject);

    //returns const reference to an object with the specified type
    //if there is no such object, returns NULL
    virtual bool get(const SQL_ID& pSQLId, SISQLObject* pObject) const;

    //returns directly pointer to the sql object with the 
    //specified sql id (or NULL if there is no such object)
    virtual const SISQLObject* get(const SQL_ID& pSQLID) const;

    //sets object as erased
    void erase(const SISQLObject* pSQLObject);

    //returns current number of objects in the dataset
    int size() const;

    //initializes iterator of the collection (teturns true if there
    //is at least one object in the dataset)
    bool initIterator() const;

    //returns the next object in dataset or NULL if there are no more
    //objects in the collection
    const SISQLObject* getNextObject() const;

    //gets the general read query for (all) objects of the type stored in the dataset
    //returns number of variables to bind
    int getReadQuery(CString& pQuery);

    //fills the list with objects that must be update,
    //returns number of objects to update
    int getObjectsToUpdate(SQLObjectList& pObjsToUpdate);

    SISQLObject* createObject();

    //returns number of items that must be stored (updated/inserted/deleted)
    int countItemsToSave() const;

    //return the maximum SQL_ID used in the collection
    SQL_ID getMaxIDUsed() const;

    //forces "updated" state in all items of the collection
    void forceFullUpdate();


  protected:

    //destroys objects marked as deleted
    void destroyDeletedObjects();

    virtual SQL_ID set(const SISQLObject* pSQLObject, SISQLObject::StateEnum pState);

    //assigns identifier to specified sql object
    void setSQLID(SISQLObject* pSQLObject, const SQL_ID& pID);

    //default controltor (only needed if deriving a new class from this
    //one)
    SISQLObjectDataSet();

    //collection of objects
    SQLObjectMap m_ObjectList;

    //collection of objects to be deleted
    SQLObjectMap m_DeletedObjectList;

  private:

    //class of objects stored in this set
    CRuntimeClass m_RuntimeClass;

    //internal iterator
    //const SQLObjectList::const_iterator m_Iterator;
    mutable SQLObjectMap::const_iterator m_Iterator;

    //pointer to the collection of max ids
    SIMaxIDDataSet* m_pMaxIDDataSet;





}; //class



}


#endif //SISQLOBJECTDATASET_H_INCLUDED