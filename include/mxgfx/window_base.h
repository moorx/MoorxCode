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

#ifndef MXGFX_WINDOW_BASE_H_
#define MXGFX_WINDOW_BASE_H_

namespace mx {
namespace gfx {

template <class T>
class WindowBase {
 public:
  typedef T ImplementationType;
 
  void Open(uint32_t width, uint32_t height, const char8_t* title) {
    implementation_.Open(width, height, title);
    main_window_ = this;
  }

  void Update() {
    implementation_.Update();
  }

  void Dispose() {
    implementation_.Dispose();
  }

  static WindowBase<T>* main_window() {
    assert(main_window_ != NULL);
    return main_window_;
  }

  bool closed() const {
    return implementation_.closed();
  }

 private:
  ImplementationType implementation_;
  static WindowBase<T>* main_window_;
};

template <class T>
WindowBase<T>* WindowBase<T>::main_window_ = NULL;

}  // namespace gfx
}  // namespace mx

#endif  // MXGFX_WINDOW_BASE_H_
