#ifndef BYTEBUFFER_H_INCLUDED
#define BYTEBUFFER_H_INCLUDED


namespace fw 
{
    namespace core
    {


        /**
          @brief A class that wrpas the raw BYTE* pointer.
          The main purpose of this class is to assure that each 
          created buffer od data will be eventually destroyed.
        */
        class ByteBuffer 
        {
        
          public:
            
            ByteBuffer();
            ByteBuffer(int pLength);
            ByteBuffer(const ByteBuffer& pByteBuffer);
            ~ByteBuffer();
        
            /**
              @brief Returns pointer to the data 
            */
            BYTE* getBuffer() const;
        
            /**
              @brief Returns length of the buffer 
            */
            int getLength() const { return mLength-m_iOffset; }
        
            /** 
              @brief Operator= overloaded to copy contents of 
              the buffer before it gets deleted
            */
            void operator=(const ByteBuffer& pByteBuffer);
        
            /**
              @brief Destroys the current buffer and creates a new 
              one with the new length (the data stored so far is destroyed)
            */
            void resize(int pNewLength);
        
        	//sets offset in the buffer
        	void setOffset(int iOffset);
        
        
          protected:
            BYTE* mBuffer;
        
            /** @brief Number of bytes in the buffer */
            int mLength;
        
        	//offset
        	int m_iOffset;
        
		}; //class

    } //namespace core


}; //namespace



#endif //BYTEBUFFER_H_INCLUDED