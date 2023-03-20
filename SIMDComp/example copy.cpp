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
  size_t N = 8;
  vector<uint32_t> mydata(N);
  for (uint32_t i = 0; i < N; ++i)
    mydata[i] = i;

  size_t N1 = 9;
  vector<uint32_t> mydata1(N1);
  for (uint32_t i = 0; i < 6; ++i)
    mydata1[i] = i;

  for (uint32_t i = 6; i < N1; ++i)
    mydata1[i] = 22+i*3;

  // we make a copy
  std::vector<uint32_t> original_data(mydata);
  ///////////
  //
    // we make a copy
  std::vector<uint32_t> original_data1(mydata1);
  ///////////
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
  cout << "The original data is "<<32.0 <<" bits per integer."<<endl;
  cout << "The compressed output size is "<<static_cast<double>(compressed_output.size())<<endl;
  cout << "You are using "
       << 32.0 * static_cast<double>(compressed_output.size()) /
              static_cast<double>(mydata.size())
       << " bits per integer. " << endl;


  vector<uint32_t> compressed_output1(N1 + 1024);
  //N+1024 should be plenty
  
  
  compressedsize = compressed_output1.size();
  codec.encodeArray(mydata1.data(), mydata1.size(), compressed_output1.data(),
                    compressedsize);
  //
  // if desired, shrink back the array:
  compressed_output1.resize(compressedsize);
  compressed_output1.shrink_to_fit();
  // display compression rate:
  cout << setprecision(3);
  cout << "The original data is "<<32.0 <<" bits per integer."<<endl;
  cout << "The compressed output size is "<<static_cast<double>(compressed_output1.size())<<endl;
  cout << "You are using "
       << 32.0 * static_cast<double>(compressed_output1.size()) /
              static_cast<double>(mydata1.size())
       << " bits per integer. " << endl;
  
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
  
  //That's it for compression!
  
  if (mydataback != original_data)
    throw runtime_error("bug!");


  vector<uint32_t> mydataback1(N1);
  recoveredsize = mydataback1.size();
  //
  codec.decodeArray(compressed_output1.data(), compressed_output1.size(),
                    mydataback1.data(), recoveredsize);
  mydataback1.resize(recoveredsize);
  std::cout<<"mydataback size "<<mydataback.size()<<endl;
  std::cout<<"mydataback1 size "<<mydataback1.size()<<endl;

  //Union of mydataback and mydataback1
  // vector<uint32_t> merged_vec;
  // merged_vec.reserve( mydataback1.size() + mydataback.size() ); // preallocate memory
  // merged_vec.insert( merged_vec.end(), mydataback.begin(), mydataback.end() );
  // merged_vec.insert( merged_vec.end(), mydataback1.begin(), mydataback1.end() );

  // vector<uint32_t> merged_vec;
  // std::merge(mydataback1.begin(), mydataback1.end(),
  //           mydataback.begin(), mydataback.end(),
  //           std::back_inserter(merged_vec));

  // vector<uint32_t>::iterator pte = std::unique(merged_vec.begin(), merged_vec.end());
  // // dups now in [pte, vecC.end()), so optionally erase:
  // merged_vec.erase(pte, merged_vec.end());

  for(auto n: mydataback1){
    cout<<"Value is "<<n<<endl;
  }
  cout<<"\n";
  cout<<"\n";
  for(auto n: mydataback){
    cout<<"Value is "<<n<<endl;
  }
  cout<<"\n";
  cout<<"\n";
  // for(auto n: merged_vec){
  //   cout<<"Value is "<<n<<endl;
  // }
  //
  // That's it for compression!
  //
  if (mydataback1 != original_data1)
    throw runtime_error("bug!");
  
  // Next we are going to test out intersection...
  //

  intersectionfunction inter =
      IntersectionFactory::getFromName("simd"); // using SIMD intersection
  // //
  // // we are going to intersect mydata and mydata2 and write back
  // // the result to mydata2
  // //
  // size_t intersize = inter(mydata1.data(), mydata1.size(), mydata.data(),
  //                          mydata.size(), mydata1.data());
  // mydata1.resize(intersize);
  // mydata1.shrink_to_fit();
  // cout << "Intersection size: " << mydata1.size() << "  integers. " << endl;
  // std::cout<<endl;
  // std::cout<<endl;
  // for(auto n:mydata1){
  //   std::cout<<"Intersect val: "<<n<<endl;
  // }

  //
  // we are going to intersect mydata and mydata2 and write back
  // the result to mydata2
  //
  size_t intersize = inter(mydataback1.data(), mydataback1.size(), mydataback.data(),
                           mydataback.size(), mydataback1.data());
  mydataback1.resize(intersize);
  mydataback1.shrink_to_fit();
  cout << "Intersection size: " << intersize << "  integers. " << endl;

  // vector<uint32_t> mydataback2(1024);
  // recoveredsize = mydataback2.size();
  
  // codec.decodeArray(mydataback1.data(), mydataback1.size(),
  //                   mydataback2.data(), recoveredsize);
  // mydataback2.resize(recoveredsize);


  std::cout<<endl;
  std::cout<<endl;
  for(auto n: mydataback1){
    std::cout<<"Intersect val: "<<n<<endl;
  }

}
