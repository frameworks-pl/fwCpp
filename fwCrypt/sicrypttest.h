#ifndef SICRYPTTEST_H_INCLUDED
#define SICRYPTTEST_H_INCLUDED

namespace test {

/**
  @brief A class to perform automatic tests
  on all classes in SICrypt

*/

class SICryptTest {

  public:
    SICryptTest() { };

    /**
      @brief This is the main testing method
      if it does not return true it means something
      is wrong!!!
    */
    bool run();

  protected:

    /** AES PHM test */
    bool AESPHMTest();

    /** SHA256 test*/
    bool SHA256Test();


}; //class

}; //namespace


#endif //SICRYPTTEST_H_INCLUDED