#ifndef SISQLROW_H_INCLUDED
#define SISQLROW_H_INCLUDED

namespace sidb
{

//holds a single row (result of a query) converted to Unicode strings
//and is able to return any column's value converted to specified type
//throws exceptions if column does not exist or conversion is impossible
class SIDB_DLLEXPORT SISQLRow
{

  public:

    SISQLRow();

    //adds column->value par (performs conversion from UTF8 to Unicode)
    void addValue(const char* pColName, const char* pColValue);

    //get value as int (throws is conversion cannot be done)
    int getValueAsInt(const CString& pColName) const;

    //returns value as int from the n-th column in the collection
    //this method must be used carefully since columns may not be ordered
    //in the same way as in the database
    int getValueAsInt(int iColIndex) const;

    //returns number of columns in this row
    int getColumnCount() const { return (int)m_ValuesMap.size(); }

  protected:

    //map column_name -> value
    NameValueMap m_ValuesMap;

};


//list or "raw" rows
typedef std::list<SISQLRow> SQLRowList;



}; //namespace



#endif //SISQLROW_H_INCLUDED