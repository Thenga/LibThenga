/*!
 * Fixed Size object bitstream diff.
 * Create a diff of bitstream from fixed size objects(of same type:)). Also construct
 * them back.
 * Use cases:
 * 1. We want to save space on the medium (disk, network etc) while serializing large structures,
 *    arrays, array of large constant size object etc. Note: The sizes should be known during complile time.
 * 2. Want a quick and a superfast way to compute diffs for some good reason.
 */

#ifndef FSZDIFF_H_
#define FSZDIFF_H_

namespace thenga
{
  /*!
   *
   */
  template <typename T>
  class FSzDiff
  {

    /*!
     * @param refObj [in], the refrence object against which currObj will be compared
     * @param currObj [in], the second object, we wan't to see what bits changed here
     * @param diffResult [out], the result buffer
     * @param diffLength [out], size of the result buffer
     */
    void getDiff(const T& refObj, const T& currObj, unsigned char* diffResult, size_t* diffLength)
    {

    }
    /*!
     * @param refObj [in]
     * @param diffBuffer [in]
     * @param diffLength [in]
     * @param fillObj [out]
     */
    void getObject(const T& refObj, const unsigned char* diffBuffer, size_t diffLength, T& fillObj)
    {

    }
  private:
    size_t fzDiffLength;
    unsigned char fzDiffBuffer[sizeof(T)*2];

  };
}

#endif /* FSZDIFF_H_ */
