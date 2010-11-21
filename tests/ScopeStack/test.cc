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

#include <MXCore/raw_memory.h>
#include <MXCore/linear_allocator.h>
#include <MXCore/scope_stack.h>
#include <MXCore/text_output.h>

using namespace mxcore;

class Foo {
 public:
  Foo() { Print("Foo()\n"); }
  ~Foo() { Print("~Foo()\n"); }
 private:
  int32_t f_;
};

struct Bar {
  float_t b_;
};

int main()
{
  uint8_t* memory = reinterpret_cast<uint8_t*>(AllocateAligned(4096));
  LinearAllocator allocator(memory, 4096);
  Print("%p\n", allocator.marker());

  {
    Foo* foos[10];
    ScopeStack outer_scope(allocator);
    for (int32_t i = 0; i < 10; ++i) {
      foos[i] = outer_scope.AllocateWithFinalizer<Foo>();
    }

    {
      Print("entering inside\n");
      Print("%p\n", allocator.marker());

      ScopeStack inner_scope(allocator);
      Bar* bars[10];
      for (int32_t i = 0; i < 10; ++i) {
        bars[i] = inner_scope.AllocateObject<Bar>();
      }

      Print("%p\n", allocator.marker());
      Print("leaving inside\n");
    }
    Print("%p\n", allocator.marker());
  }

  Print("%p\n", allocator.marker());

  return 0;
}
