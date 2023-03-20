//
// A simple example to get you started with the library.
// You can compile and run this example like so:
//
//   make example
//   ./example
//
//  Warning: If your compiler does not fully support C++11, some of
//  this example may require changes.
//

#include "include/codecfactory.h"
#include "include/intersection.h"

using namespace SIMDCompressionLib;


int main() {
  // We pick a CODEC
  IntegerCODEC &codec = *CODECFactory::getFromName("s4-fastpfor-d1");
  // could use others, e.g., frameofreference, ibp32, maskedvbyte, s4-bp128-d1, s4-bp128-d2, s4-bp128-d4, s4-bp128-dm, simdframeofreference, streamvbyte
  //
  // Note that some codecs compute the differential coding in-place, thus modifying part of the input, replacing it with a differentially coded version:
  //  bp32, fastpfor, s4-bp128-d1-ni, s4-bp128-d2-ni, s4-bp128-d4-ni, s4-bp128-dm-ni, s4-fastpfor-d1, s4-fastpfor-d2, s4-fastpfor-d4, s4-fastpfor-dm
  // Other codecs do the differential coding "in passing", such as 
  // for, frameofreference, ibp32, maskedvbyte, s4-bp128-d1, s4-bp128-d2, s4-bp128-d4, s4-bp128-dm, simdframeofreference, streamvbyte, varint, varintg8iu, varintgb,  vbyte
  //


  ////////////
  //
  // create a container with some integers in it
  //
  // We need the integers to be in sorted order.
  //
  // (Note: You don't need to use a vector.)
  //
  size_t N = 6;
  vector<uint32_t> mydata(N);
  for (uint32_t i = 0; i < N; ++i)
    mydata[i] = 2 * i; //0,3,6,9,12,15



  // we make a copy
  std::vector<uint32_t> original_data(mydata);
  ///////////
  //
  // You need some "output" container. You are responsible
  // for allocating enough memory.
  //
  vector<uint32_t> compressed_output(N + 1024);
  // N+1024 should be plenty
  //
  //
  size_t compressedsize = compressed_output.size();
  codec.encodeArray(mydata.data(), mydata.size(), compressed_output.data(),
                    compressedsize);
  //
  // if desired, shrink back the array:
  compressed_output.resize(compressedsize);
  compressed_output.shrink_to_fit();
  // display compression rate:
  cout << setprecision(3);
  cout << "You are using "
       << 32.0 * static_cast<double>(compressed_output.size()) /
              static_cast<double>(mydata.size())
       << " bits per integer. " << endl;
  //
  // You are done!... with the compression...
  //
  ///
  // decompressing is also easy:
  //
  vector<uint32_t> mydataback(N);
  size_t recoveredsize = mydataback.size();
  //
  codec.decodeArray(compressed_output.data(), compressed_output.size(),
                    mydataback.data(), recoveredsize);
  mydataback.resize(recoveredsize);
  //
  // That's it for compression!
  //
  if (mydataback != original_data)
    throw runtime_error("bug!");

  //
  // Next we are going to test out intersection...
  //
  size_t N2=12;
  
  vector<uint32_t> mydata2(N2);
  for (uint32_t i = 0; i < 5; ++i){
    mydata2[i] = 2 * i; //0,3,6,9,12
  }

  for (uint32_t i = 5; i < N2; ++i){
    mydata2[i] = 8 * i; //0,3,6,9,12,40,48,56,64,72,80,88 & 0,3,6,9,12,15= 0,3,6,9,12
  }

  // we make a copy
  std::vector<uint32_t> original_data2(mydata2);
  ///////////
  //

  vector<uint32_t> compressed_output2(N2 + 1024);
  // N+1024 should be plenty
  //
  //
  compressedsize = compressed_output2.size();
  codec.encodeArray(mydata2.data(), mydata2.size(), compressed_output2.data(),
                    compressedsize);
  //
  // if desired, shrink back the array:
  compressed_output2.resize(compressedsize);
  compressed_output2.shrink_to_fit();
  // display compression rate:
  cout << setprecision(3);
  cout << "You are using "
       << 32.0 * static_cast<double>(compressed_output2.size()) /
              static_cast<double>(mydata2.size())
       << " bits per integer. " << endl;


  vector<uint32_t> mydataback2(N2);
  recoveredsize = mydataback2.size();
  //
  codec.decodeArray(compressed_output2.data(), compressed_output2.size(),
                    mydataback2.data(), recoveredsize);
  mydataback2.resize(recoveredsize);
  //
  // That's it for compression!
  //
  if (mydataback2 != original_data2)
    throw runtime_error("bug!");


  intersectionfunction inter =
      IntersectionFactory::getFromName("simd"); // using SIMD intersection
  //
  // we are going to intersect mydata and mydata2 and write back
  // the result to mydata2
  //

  std::cout<<endl;
  std::cout<<endl;
  std::cout<<"mydata:"<<endl;
  for(auto n: mydata){
    std::cout<<n<<endl;
  }
  std::cout<<endl;
  std::cout<<endl;
  std::cout<<"mydata2"<<endl;
  for(auto n: mydata2){
    std::cout<<n<<endl;
  }
  std::cout<<endl;
  std::cout<<endl;

  //Working intersection for mydata and mydata2
  std::cout<<"Testing intersection on original data"<<endl;
  size_t intersize = inter(mydata2.data(), mydata2.size(), mydata.data(),
                           mydata.size(), mydata2.data());
  mydata2.resize(intersize);
  mydata2.shrink_to_fit();
  cout << "Intersection size: " << mydata2.size() << "  integers. " << endl;

  std::cout<<endl;
  std::cout<<endl;
  for(auto n: mydata2){
    std::cout<<n<<endl;
  }
  std::cout<<endl;
  std::cout<<endl;

  //Working intersection for mydataback2 and mydataback
  std::cout<<"Testing intersection on decoded data"<<endl;
  intersize = inter(mydataback2.data(), mydataback2.size(), mydataback.data(),
                          mydataback.size(), mydataback2.data());
  mydataback2.resize(intersize);
  mydataback2.shrink_to_fit();
  cout << "Intersection size: " <<  mydataback2.size() << "  integers. " << endl;

  std::cout<<endl;
  std::cout<<endl;
  for(auto n: mydataback2){
    std::cout<<n<<endl;
  }
  std::cout<<endl;
  std::cout<<endl;

  // //Working intersection for compressed_output2 and compresssed_output
  std::cout<<"Testing intersection on compressed data"<<endl;
  intersize = inter(compressed_output2.data(), compressed_output2.size(), compressed_output.data(),
                          compressed_output.size(), compressed_output2.data());
  compressed_output2.resize(intersize);
  compressed_output2.shrink_to_fit();
  cout << "Intersection size: " << intersize << "  integers. " << endl;



  vector<uint32_t> mydataback3(1024);
  recoveredsize = mydataback3.size();
  //
  codec.decodeArray(compressed_output2.data(), compressed_output2.size(),
                    mydataback3.data(), recoveredsize);
  mydataback3.resize(recoveredsize);

  std::cout<<endl;
  std::cout<<endl;
  for(auto n: mydataback3){
    std::cout<<n<<endl;
  }
  std::cout<<endl;
  std::cout<<endl;
}
