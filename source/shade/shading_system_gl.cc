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

#include <assert.h>
#include "shade/shading_system_gl.h"

namespace mx {
namespace shade {

void ShadingSystemGL::Initialize() {
  ShadingSystem::Initialize();

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  window_ = SDL_CreateWindow("ShadingSystemGL", SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, 640, 400, 
                             SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  assert(window_ != NULL);

  gl_context_ = SDL_GL_CreateContext(window_);
  SDL_GL_SetSwapInterval(1);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void ShadingSystemGL::EndFrame() {
  glClear(GL_COLOR_BUFFER_BIT);
  
  std::vector<RenderBlock>::iterator renderblock_iterator = render_queue_.begin();
  for (; renderblock_iterator != render_queue_.end(); ++renderblock_iterator) {
  }

  SDL_GL_SwapWindow(window_);
}

void ShadingSystemGL::Dispose() {
  SDL_GL_DeleteContext(gl_context_);
  SDL_DestroyWindow(window_);
}

}  // namespace shade
}  // namespace mx
