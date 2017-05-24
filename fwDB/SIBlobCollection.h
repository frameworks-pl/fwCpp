#ifndef BLOBCOLLECTION_H_INCLUDED
#define BLOBCOLLECTION_H_INCLUDED

#define INVALID_BLOB_ID 0

namespace fw
{
	namespace db
	{

		typedef int BLOB_ID;


		class BLOBCollection;


		class SIDB_DLLEXPORT BLOBItem
		{
			friend class BLOBCollection;
		public:

			BLOBItem();
			BLOBItem(const BLOBItem& pSIBLOBItem);
			BLOBItem& operator=(const BLOBItem& pSIBLOBItem);
			BLOBItem(fw::core::ByteBuffer* pData);
			virtual ~BLOBItem();

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
			BLOBCollection
		{
			friend class BLOBItem;
		public:
			static BLOBCollection* get();
			static void terminate();

			static const BLOBItem& invalidSIBLOBItem() { return m_InvalidBLOB; }

		protected:
			BLOBCollection();
			~BLOBCollection();

			//the collection singleton
			static BLOBCollection* m_pSIBLOBCollection;

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

			static BLOBItem m_InvalidBLOB;


		}; //class




	}; //namespace 
}


#endif //BLOBCOLLECTION_H_INCLUDED