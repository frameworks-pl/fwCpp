#include "fwCommon_pch.h"
#include "fwbytebuffer.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

namespace fw 
{

	namespace core
	{

		ByteBuffer::ByteBuffer() {

			mLength = 0;
			mBuffer = NULL;
			m_iOffset = 0;

		}; //default constructor


		ByteBuffer::ByteBuffer(const ByteBuffer& pByteBuffer)
		{
			mBuffer = NULL;
			mLength = 0;
			m_iOffset = 0;

			operator=(pByteBuffer);

		}; //copy constructor


		ByteBuffer::ByteBuffer(int pLength)
		{

			m_iOffset = 0;
			mLength = pLength;
			if (mLength > 0)
				mBuffer = new BYTE[mLength];
			else
				mBuffer = NULL;

		}; //pLength


		ByteBuffer::~ByteBuffer() {

			//seeting buffer to zeros (avoiding core-dump attacks)
			if (mBuffer)
				memset(mBuffer, 0, mLength);


			delete[] mBuffer;
			mBuffer = NULL;

		}; //destructor


		void
			ByteBuffer::operator=(const ByteBuffer& pByteBuffer)
		{

			delete mBuffer;
			mBuffer = NULL;
			m_iOffset = 0;

			mLength = pByteBuffer.mLength;
			if (pByteBuffer.mLength > 0) {
				mBuffer = new BYTE[mLength];
				memcpy(mBuffer, pByteBuffer.mBuffer, mLength);
			}

		};

		void ByteBuffer::resize(int pNewLength)
		{

			delete[] mBuffer;
			mBuffer = NULL;
			mLength = pNewLength;
			if (pNewLength > 0) {
				mBuffer = new BYTE[mLength];
			}

		}; //resize;


		void ByteBuffer::setOffset(int iOffset)
		{
			ASSERT(iOffset < mLength);
			m_iOffset = iOffset;
		}


		BYTE* ByteBuffer::getBuffer() const
		{
			BYTE* pBuffer = mBuffer;
			pBuffer += m_iOffset;
			return pBuffer;
		}

	} //namespace core


}; //namespace