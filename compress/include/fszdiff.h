/*!
 * Fixed Size object bitstream diff.
 * Create a bitstream diff from fixed size objects(of same type:)). Also construct
 * them back.
 * Use cases:
 * 1. We want to save space on the medium (disk, network etc) while serializing large structures,
 *    arrays, array of large constant size object etc. Note: The sizes should be known during complile time.
 * 2. Want a quick and a superfast way to compute diffs for some good reason.
 */

#ifndef FSZDIFF_H_
#define FSZDIFF_H_

#include <bitset>
#include <iostream>
#include <tuple>
#include <string.h>

namespace thenga
{
  /*\breif Fixed Size object bitstream diff
   * @tparam Type
   */
  template <typename T>
  class FSzDiff
  {
    static const size_t fzMaxSize  = sizeof(T) * 2;
    static const size_t numFlags = 1; //atm, lets have one bit flag
    static const size_t numElements = sizeof(T) / sizeof(char); //no of bits for T
    static const size_t totalHeaderBits = numFlags + numElements;
    static constexpr size_t computebytesForHeader()
    {
      return (totalHeaderBits / 8) + (totalHeaderBits % 8 == 0 ? 0 : 1);
    }
    static const size_t headerByteCount = computebytesForHeader();


  public:
    /*!
     * @param [in] pRefObj , the pointer to a reference object against which currObj will be compared
     * @param [in] currObj, the second object, we wan't to see what bits changed here
     * @param [out] diffResult, the result buffer
     * @param [out] diffLength, size of the result buffer
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
     * @param [in] refObj, T ptr
     * @param [in] diffBuffer
     * @param [in] diffLength
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
     * test functions
     */
    size_t testGetHeaderByteCount()
    {
      return headerByteCount;
    }
    void testPrintBitHeader()
    {
      printBitStream(fzDiffBuffer,headerByteCount);
    }

    void printBitStream(char* in, size_t len)
    {
      for(size_t i = 0; i < len; i++)
      {
        std::cout << "[" << i << "]";
        char curr = in[i];
        for(int b = 7; b >= 0 ; --b)
        {
          std::cout << (curr & (0x01 << b) ? "1" : "0");
        }
      }
      std::cout << "\n";
    }


  private:
    /*!
     * @param [in] refAvailable
     */
    void setFullObject(bool refAvailable)
    {
      fzDiffBuffer[0] = (refAvailable << 7); //set bit:1 on/off,others off
    }
    /*!
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
     * @param [in] currObj
     */
    void doCopy(const T& currObj)
    {
      // first byte is the header as it contains the bit which is marked as full object
      memcpy(fzDiffBuffer + 1, (const char*)(&currObj), sizeof(T));
      fzDiffLength = sizeof(T) + 1;
    }
    /*!
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
