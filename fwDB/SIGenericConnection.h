#ifndef SIGENERICCONNECTION_H_INCLUDED
#define SIGENERICCONNECTION_H_INCLUDED


namespace sidb
{

class SISQLObjectDataSet;

//base class for all different types od DB connection (basically this is just an interface)
class SIGenericConnection
{
  public:
    SIGenericConnection();

  protected:

    //destroyes objects from dataset that have been deleted
    void destroyDeletedObjects(SISQLObjectDataSet* pSQLObjectDataSet);



}; //class


}; //namespace

#endif //SIGENERICCONNECTION_H_INCLUDED