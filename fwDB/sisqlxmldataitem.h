#ifndef SISQLXMLDATAITEM_H_INCLUDED
#define SISQLXMLDATAITEM_H_INCLUDED

#include "sisqlobject.h"

namespace fw
{
	namespace crypt
	{
		class Archive;
	}
}

namespace fw
{
	namespace db
	{


		//A class of each data item that is to be stored
		//in database as encrypted XML
		class SIDB_DLLEXPORT SQLXMLDataItem : public SQLObject
		{
		public:
			SQLXMLDataItem();


			//takes encrypted chunk of data, decrypts it and tries to parse as XML
			//bool chunkToRootXML(const fw::core::ByteBuffer& pBuffer, TiXmlDocument& pXmlDocument);
			bool chunkToRootXML(const fw::core::ByteBuffer& pBuffer, XMLNode& pXmlDocument);

			//encrypts the string suinng embedded archive objec
			fw::core::ByteBuffer encryptChunk(const CString& pChunk);

			//Method that initializes the archive object used
			//to encrypt/decrypt the chunks of data
			static void initializeArchive(const CString& pPassword);

			//destroys the current archive object
			static void deinitializeArchive();

		protected:

			//static archive object that should be initialized before anyd
			//reading/writing is done to objects of this class!
			static fw::crypt::Archive* m_pArchive;

			//xml document used to parse chunks (it must be a member 
			//since we return const pointer)
			//TiXmlDocument m_XMLDocument;

		private:

			//fake CFile handle, to satisfy Archive object
			static CFile m_FakeFile;



		};


	}; //namespace 
}


#endif //SISQLXMLDATAITEM_H_INCLUDED
