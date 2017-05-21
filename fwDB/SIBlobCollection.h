#ifndef BLOBCOLLECTION_H_INCLUDED
#define BLOBCOLLECTION_H_INCLUDED

#define INVALID_BLOB_ID 0

namespace fw
{
	namespace db
	{

		typedef int BLOB_ID;


		class SIBLOBCollection;


		class SIDB_DLLEXPORT SIBLOBItem
		{
			friend class SIBLOBCollection;
		public:

			SIBLOBItem();
			SIBLOBItem(const SIBLOBItem& pSIBLOBItem);
			SIBLOBItem& operator=(const SIBLOBItem& pSIBLOBItem);
			SIBLOBItem(fw::core::ByteBuffer* pData);
			virtual ~SIBLOBItem();

			BLOB_ID getID() const;
			bool isValid() const;
			fw::core::ByteBuffer* getDataBuffer() const { return m_pData; }

			//creates a buffer for data and allocates
			void create();

		protected:

			//only SIBLOBCollection can create BLOB itmes
			//SIBLOBItem();

			//only SIBLOBCollection can create BLOB itmes


	  //      void addRef();
	  //      void release();
			//int getRefCount() const { return m_iRefCount; }

			//releases the object
			void release();

			fw::core::ByteBuffer* m_pData;
			BLOB_ID m_ID;

			//static SIBLOBItem m_InvalidBLOB;

			//reference count for the item
			//int m_iRefCount;
		};


		class SIDB_DLLEXPORT
			SIBLOBCollection
		{
			friend class SIBLOBItem;
		public:
			static SIBLOBCollection* get();
			static void terminate();

			static const SIBLOBItem& invalidSIBLOBItem() { return m_InvalidBLOB; }

		protected:
			SIBLOBCollection();
			~SIBLOBCollection();

			//the collection singleton
			static SIBLOBCollection* m_pSIBLOBCollection;

			//returns next available id for BLOB item
			int getMaxID();

			//increase reference count for the specific object ID
			void addRef(BLOB_ID iID);

			//decrease reference count for the specific object ID
			//returns ref count after decreasing it
			int release(BLOB_ID iID);

		private:

			//counter to keep unique id-s of items
			BLOB_ID m_iMaxID;

			//std::list<SIBLOBItem> m_SIBLOBItems;
			//mapping BLOB id into ref count number
			std::map<BLOB_ID, int> m_BLOBRefCountMap;

			static SIBLOBItem m_InvalidBLOB;


		}; //class




	}; //namespace 
}


#endif //BLOBCOLLECTION_H_INCLUDED