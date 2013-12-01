/*! \brief Byte diff creator for Fixed Size Objects.
 *
 * Use cases:
 * ----------
 * If we have some objects ObjX,ObjX',ObjX''... [of same type and fixed size(known at compile time) ]
 * then this could be useful if:
 *  1. want a superfast way to get object diffs and reconstruct them back - for some good reason.
 *  2. want to reduce the size of the objects before applying some standard compression.
 * If objects are related and only few bytes differ then its a very good case and
 * actually a very high compression ratio can be achieved.
 *
 * Example:
 * --------
 * compress/src/testfszd.cpp
 *
 * Internal rep:
 * ------------
 * +---------------------------------------+
 * |  <-- bitheader-->|                    |
 * |  <F><----DBF---->|<--diff bytes-->    |
 * |   _  _         _ |+--------------+    |
 * |  |_||_|.......|_||| ............ |    |
 * |                  |+--------------+    |
 * |  F  :- Full object bit flag.          |
 * |  DBF:- Bit flag index of diff bytes   |
 * +---------------------------------------+
 *
*/

#ifndef FSZDIFF_H_
#define FSZDIFF_H_

#include <bitset>
#include <iostream>
#include <tuple>
#include <string.h>

namespace thenga
{
  /*!
   * @tparam T, T should be of fixed size (the size if known at compile time)
   */
  template <typename T>
  class FSzDiff
  {
    static const size_t fzMaxSize  = sizeof(T) * 2;
    static const size_t numExistFlags = 1;
    static const size_t numElements = sizeof(T);
    static const size_t totalHeaderBits = numExistFlags + numElements;

    static constexpr size_t computebytesForHeader()
    {
      return (totalHeaderBits / 8) + (totalHeaderBits % 8 == 0 ? 0 : 1);
    }

    static const size_t headerByteCount = computebytesForHeader();


  public:
    /*!
     * Construct a diffbuffer
     * @param [in] pRefObj , the pointer to a reference object against which currObj will be compared.
     *   pRefObj can be nullptr (this usually will be the case for the first call as our ref
     *   object will the currObj)
     * @param [in] currObj, the second object: this object will be converted to a diffbuffer.
     *
     * @return tuple [ <0>diffbuffer <1>length of diffbuffer ]
     */
    std::tuple<char*, size_t> getDiff(const T* pRefObj, const T& currObj)
    {
      setFullObject(nullptr == pRefObj);

      if (nullptr == pRefObj)
      {
        doCopy(currObj);
      }
      else
      {
        doDiff(*pRefObj, currObj);
      }
      return(std::make_tuple(fzDiffBuffer, fzDiffLength));
    }
    /*!
     * Reconstruct the object from the diffBuffer
     * @param [in] refObj, pointer to T object, for initial object this would be nullptr and
     * @param [in] diffBuffer
     * @param [out] fillObj
     * throws std::string
     */
    void getObject(const T* refObj, const char* diffBuffer, T& fillObj)
    {
      bool isFullObj = isFullObject(diffBuffer);
      if ((nullptr == refObj) && (!isFullObj))
          throw(std::string("you are expecting a fullobject but diffBuffer doesn't have it"));

      if (isFullObj)
      {
        memcpy((char*)(&fillObj), diffBuffer + 1, sizeof(T));
      }
      else
      {
        const char* refBuff = reinterpret_cast<const char*>(refObj);
        char* outBuff = reinterpret_cast<char*>(&fillObj);

        size_t elemInd = 0;
        size_t diffDataIndex = headerByteCount;
        for (size_t byteIndex = 0; byteIndex < headerByteCount; byteIndex++)
        {
          int shift = 7;
          if (!byteIndex)
            --shift;
          for (;(shift >= 0 && (elemInd < numElements)); --shift)
          {
            if((diffBuffer[byteIndex] & (0x01 << shift)))
            {
              outBuff[elemInd] = diffBuffer[diffDataIndex++];
            }
            else
            {
              outBuff[elemInd] = refBuff[elemInd];
            }
            ++elemInd;
          }
        }
      }
    }
    /*!
     * test function - get the bytes required by header
     */
    size_t testGetHeaderByteCount()
    {
      return headerByteCount;
    }
    /*!
     * test function - print the bit header
     */
    void testPrintBitHeader()
    {
      printBitStream(fzDiffBuffer,headerByteCount);
    }
  private:

    /*!
     * Print the bits in the input buffer
     * @param [in] inBuff
     * @param [in] len
     */
    void printBitStream(const char* inBuff, size_t len)
    {
      for(size_t i = 0; i < len; i++)
      {
        std::cout << "[" << i << "]";
        char curr = inBuff[i];
        for(int b = 7; b >= 0 ; --b)
        {
          std::cout << (curr & (0x01 << b) ? "1" : "0");
        }
      }
      std::cout << "\n";
    }

    /*!
     * Set the Full object flag
     * @param [in] refAvailable
     */
    void setFullObject(bool refAvailable)
    {
      fzDiffBuffer[0] = (refAvailable << 7);
    }
    /*!
     * Check if the Full object flag is set
     * @param [in] buffer
     */
    bool isFullObject(const char* buffer)
    {
      return(buffer[0] & (0x01 << 7));
    }
    /*!
     * fill in the header from bitHeader
     */
    void fillHeader(void)
    {
      //fill byte-wise
      memset(fzDiffBuffer,0, headerByteCount);
      size_t bitIndex = 0;
      for(size_t byteIndex = 0; byteIndex < headerByteCount; byteIndex++)
      {
        for(int shift = 7; (shift >= 0  && (bitIndex < totalHeaderBits)); --shift)
        {
          fzDiffBuffer[byteIndex] |= (bitHeader[bitIndex] << shift);
          ++bitIndex;
        }
      }
    }
    /*!
     * copy the Full Object
     * @param [in] currObj
     */
    void doCopy(const T& currObj)
    {
      // first byte is the header as it contains the bit which is marked as full object
      memcpy(fzDiffBuffer + 1, (const char*)(&currObj), sizeof(T));
      fzDiffLength = sizeof(T) + 1;
    }
    /*!
     * The main diff method
     * @param [in] refObj
     * @param [in] currObj
     */
    void doDiff(const T& refObj, const T& currObj)
    {
      const char* pRef = reinterpret_cast<const char*>(&refObj);
      const char* pCurr = reinterpret_cast<const char*>(&currObj);

      fzDiffLength = headerByteCount;
      bitHeader[0] = false; // first bit is to identify a no-diff case
      for(size_t i = 0; i < numElements; ++i) //for all bytes
      {
        if (*(pRef + i) != *(pCurr + i))
        {
          bitHeader[i + 1] = true;
          fzDiffBuffer[fzDiffLength++] = *(pCurr + i);
        }
        else
        {
          bitHeader[i + 1] = false;
        }
      }
      fillHeader();
    }

  private:
    size_t fzDiffLength;
    char fzDiffBuffer[fzMaxSize];
    bool bitHeader[totalHeaderBits];
  };
}

#endif /* FSZDIFF_H_ */
