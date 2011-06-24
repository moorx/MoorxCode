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

#include <vector>

namespace mx {
namespace shade {

// Abstract, high-level representation of render state and shaders. The shading
// system backend chooses the right code paths for setting the appropriate low
// level states and shaders.
class SurfaceShader {
};

// A render block encapsulates the vertices and state used to draw a piece of
// geometry.
struct RenderBlock {
  RenderBlock();
  RenderBlock(const float* vertex_buffer, const uint16_t* index_buffer,
              const SurfaceShader* surface_shader);

  float* vertex_buffer;
  uint16_t* index_buffer;
  SurfaceShader* surface_shader;
};

// A render API abstraction layer. Takes render blocks (render state + geometry)
// and queues them up. When all render blocks forming the current frame have
// been buffered, the shading system passes the abstract render states on to the
// native API.
class ShadingSystem {
 public:
  ShadingSystem();
  virtual ~ShadingSystem();

  virtual void Initialize();
  virtual void ReInitialize();
  virtual void BeginFrame();
  virtual void Render(const RenderBlock& rb);
  virtual void EndFrame() = 0;
  virtual void Dispose();

 protected:
  std::vector<RenderBlock> render_queue_;
};

}  // namespace shade
}  // namespace mx

#endif  // SHADE_SHADING_SYSTEM_H_
