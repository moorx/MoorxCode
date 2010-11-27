// Copyright 2010 Christoph Lang. All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//     * Neither the name of Christoph Lang nor the names of its
//       contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <MXCore/mxassert.h>
#include <MXCore/array.h>
#include <MXCore/text_output.h>
#include <MXCore/raw_memory.h>
#include <MXCore/linear_allocator.h>
#include <MXCore/scope_stack.h>

using namespace mxcore;

class Foo {
 public:
  Foo() : f_(42) {
  }

  ~Foo() {
    Print(".");
  }

  int32_t f_;
};

struct Bar {
  Bar() : b_(64) {}
  uint64_t b_;
};

void TestFinalizer(LinearAllocator& allocator) {
  ScopeStack scope(allocator);
  
  Array<Foo, FinalizerArrayTraits> array(scope, 100);
  MX_ASSERT(array.size() == 100);

  for (int32_t i = 0; i < 100; ++i) {
    MX_ASSERT(array[i].f_ == 42);
  }
  
  for (int32_t i = 0; i < 100; ++i) {
    array[i].f_ = 13;
  }
  
  for (int32_t i = 0; i < 100; ++i) {
    MX_ASSERT(array[i].f_ == 13);
  }
}

void TestObject(LinearAllocator& allocator) {
  ScopeStack scope(allocator);

  Array<Bar> array(scope, 100);
  MX_ASSERT(array.size() == 100);

  for (int32_t i = 0; i < 100; ++i) {
    MX_ASSERT(array[i].b_ == 64);
  }
  
  for (int32_t i = 0; i < 100; ++i) {
    array[i].b_ = 48;
  }
  
  for (int32_t i = 0; i < 100; ++i) {
    MX_ASSERT(array[i].b_ == 48);
  }
}

void TestRaw(LinearAllocator& allocator) {
  ScopeStack scope(allocator);

  Array<int16_t, RawArrayTraits> array(scope, 100);
  MX_ASSERT(array.size() == 100);

  for (int32_t i = 0; i < 100; ++i) {
    array[i] = i;
  }
  
  for (int32_t i = 0; i < 100; ++i) {
    MX_ASSERT(array[i] == i);
  }
}

int main()
{
  uint8_t* memory = reinterpret_cast<uint8_t*>(AllocateAligned(4096));
  LinearAllocator allocator(memory, 4096);
  TestFinalizer(allocator);
  TestObject(allocator);
  TestRaw(allocator);
  FreeAligned(memory);
  return 0;
}
