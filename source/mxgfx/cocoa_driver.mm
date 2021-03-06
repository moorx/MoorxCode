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

#import <Cocoa/Cocoa.h>
#import <OpenGL/OpenGL.h>

#import "mxgfx/cocoa_driver.h"

@interface MXOpenGLView: NSOpenGLView {
  NSRect frame_rect_;
}

@end

@implementation MXOpenGLView

- (id)initWithFrame: (NSRect)frame {
  int32_t attribs[] = {
    NSOpenGLPFAAccelerated,
    NSOpenGLPFADoubleBuffer,
    NSOpenGLPFADepthSize, 24,
    NSOpenGLPFAAlphaSize, 8,
    NSOpenGLPFAColorSize, 32,
    NSOpenGLPFANoRecovery,
    // kCGLPFASampleBuffers, 1,
    // kCGLPFASamples, 4,
    0
  };

  NSOpenGLPixelFormat* format = [[NSOpenGLPixelFormat alloc]
      initWithAttributes: (NSOpenGLPixelFormatAttribute*) attribs];

  self = [self initWithFrame: frame pixelFormat: format];
  [format release];
  frame_rect_ = frame;

  //int32_t vsync = 1;
  //[[self openGLContext] setValues: &vsync forParameter: NSOpenGLCPSwapInterval];

  return self;
}

@end

namespace mx {
namespace gfx {

void CocoaDriver::Initialize(const Window* window) {
  window_ = window;
  SwitchToWindow();
}

void CocoaDriver::Dispose() {
  [(MXOpenGLView*)opengl_view_ release];
}

void CocoaDriver::Present() {
  [[(MXOpenGLView*)opengl_view_ openGLContext] makeCurrentContext];

  // TODO: process render queue
  glClear(GL_COLOR_BUFFER_BIT);

  [[(MXOpenGLView*)opengl_view_ openGLContext] flushBuffer];
  [NSOpenGLContext clearCurrentContext];
}

void CocoaDriver::SwitchToFullscreen() {
  NSRect view_rect = [[NSScreen mainScreen] frame];
  NSWindow* window = (NSWindow*)window_->native_handle();
  [window setStyleMask: NSBorderlessWindowMask];
  [window setFrame: view_rect display: YES animate: YES];
  [window setLevel: NSMainMenuWindowLevel + 1];
  [(MXOpenGLView*)opengl_view_ release];
  opengl_view_ = [[MXOpenGLView alloc] initWithFrame: view_rect];
  [window setContentView: (MXOpenGLView*)opengl_view_];
  [window makeKeyAndOrderFront: window];
}

void CocoaDriver::SwitchToWindow() {
  NSRect view_rect = NSMakeRect(0, 0, window_->width(), window_->height());
  NSWindow* window = (NSWindow*)window_->native_handle();
  [window setStyleMask: NSTitledWindowMask | NSClosableWindowMask |
      NSMiniaturizableWindowMask];
  [window setFrame: view_rect display: YES];
  [window setLevel: NSMainMenuWindowLevel];
  [(MXOpenGLView*)opengl_view_ release];
  opengl_view_ = [[MXOpenGLView alloc] initWithFrame: view_rect];
  [window setContentView: (MXOpenGLView*)opengl_view_];
  [window makeKeyAndOrderFront: window];
  [window center];
}

void CocoaDriver::set_fullscreen(bool fullscreen) {
  if (fullscreen) {
    SwitchToFullscreen();
  } else {
    SwitchToWindow();
  }

  fullscreen_ = fullscreen;
}

}  // namespace gfx
}  // namespace mx
