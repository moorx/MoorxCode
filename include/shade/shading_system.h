// Copyright 2011 Christoph Lang. All rights reserved.
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

#ifndef SHADE_SHADING_SYSTEM_H_
#define SHADE_SHADING_SYSTEM_H_

#include <stdint.h>
#include <vector>

namespace mx {
namespace shade {

struct VertexBuffer {
  VertexBuffer() : data_(NULL), start_(0), size_(0) {}
  VertexBuffer(float* data, size_t start, size_t size)
      : data_(data),
        start_(start),
        size_(size) {}

  float* data_;
  size_t start_;
  size_t size_;
};

struct IndexBuffer {
  IndexBuffer() : data_(NULL), start_(0), size_(0) {}
  IndexBuffer(uint32_t* data, size_t start, size_t size)
      : data_(data),
        start_(start),
        size_(size) {}

  uint32_t* data_;
  size_t start_;
  size_t size_;
};

struct RenderState {
  float diffuse_color_[3];
  float ambient_light_[6];
};

// A render block encapsulates the vertices and state used to draw a piece of
// geometry.
struct RenderBlock {
  RenderBlock() : vertex_buffer_(NULL), index_buffer_(NULL), state_(NULL) {}
  RenderBlock(VertexBuffer* vertex_buffer, IndexBuffer* index_buffer,
              RenderState* state)
      : vertex_buffer_(vertex_buffer),
        index_buffer_(index_buffer),
        state_(state) {}

  VertexBuffer* vertex_buffer_;
  IndexBuffer* index_buffer_;
  RenderState* state_;
};

// A render API abstraction layer. Takes render blocks (render state + geometry)
// and queues them up. When all render blocks forming the current frame have
// been buffered, the shading system passes the abstract render states on to the
// native API.
class ShadingSystem {
 public:
  ShadingSystem() {}
  virtual ~ShadingSystem() {}

  virtual void Initialize();
  virtual void ReInitialize() {}
  void BeginFrame();
  void Render(const RenderBlock& rb);
  virtual void EndFrame() = 0;
  virtual void Dispose() {}

 protected:
  std::vector<RenderBlock> render_queue_;
};

}  // namespace shade
}  // namespace mx

#endif  // SHADE_SHADING_SYSTEM_H_
