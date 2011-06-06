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
#import "mxgfx/cocoa_window.h"

@interface MXApplication: NSApplication {
  BOOL is_open_;
}

- (BOOL)is_open;

@end

@implementation MXApplication

- (void)run {
  is_open_ = YES;
  [self finishLaunching];
}

- (void)processEvents {
  // IMPORTANT: set untilDate to distantPast to return immediately!!
  NSEvent* event = [self nextEventMatchingMask: NSAnyEventMask
      untilDate: [NSDate distantPast] inMode: NSDefaultRunLoopMode
      dequeue: YES];

  if (event != nil)
    [self sendEvent: event];

  [self updateWindows];
}

- (void)terminate: (id)sender {
  is_open_ = NO;
}

- (BOOL)is_open {
  return is_open_;
}

@end

@interface MXMainView: NSView <NSWindowDelegate> {
}

@end

@implementation MXMainView

- (void)windowWillClose: (NSNotification*)notification {
  [NSApp terminate: self];
}

@end

namespace mx {
namespace gfx {

CocoaWindow::CocoaWindow() : native_handle_(NULL) {
}

void CocoaWindow::Open(uint32_t width, uint32_t height, const char* title) {
  release_pool_ = [NSAutoreleasePool new];

  [MXApplication sharedApplication];
  [NSApp setActivationPolicy: NSApplicationActivationPolicyRegular];
  [NSApp activateIgnoringOtherApps: YES];

  CreateMenu();
  native_handle_ = CreateWindow(width, height, title);

  [NSApp run];
}

void CocoaWindow::Update() {
  [NSApp processEvents];
}

void CocoaWindow::Dispose() {
  [(NSAutoreleasePool*)release_pool_ release];
}

CocoaWindow::NativeHandleType CocoaWindow::native_handle() const {
  assert(native_handle_ != NULL);
  return native_handle_;
}

bool CocoaWindow::is_open() const {
  return [NSApp is_open];
}

void CocoaWindow::CreateMenu() {
  id menubar = [[NSMenu new] autorelease];
  id app_menuitem = [[NSMenuItem new] autorelease];
  [menubar addItem: app_menuitem];
  [NSApp setMainMenu: menubar];
  id appmenu = [[NSMenu new] autorelease];
  id appname = [[NSProcessInfo processInfo] processName];
  id quit_title = [@"Quit " stringByAppendingString: appname];
  id quit_menuitem = [[[NSMenuItem alloc] initWithTitle: quit_title
      action: @selector(terminate:) keyEquivalent: @"q"] autorelease];
  [appmenu addItem: quit_menuitem];
  [app_menuitem setSubmenu: appmenu];
}

CocoaWindow::NativeHandleType CocoaWindow::CreateWindow(uint32_t width,
                                                        uint32_t height,
                                                        const char* title) {
  NSRect view_rect = NSMakeRect(0, 0, width, height);
  MXMainView* view = [[MXMainView alloc] initWithFrame: view_rect];
  NSWindow *window = [[NSWindow alloc]
      initWithContentRect: view_rect
      styleMask: NSTitledWindowMask |
                 NSClosableWindowMask | 
                 NSMiniaturizableWindowMask
      backing: NSBackingStoreBuffered
      defer: NO];

  NSString* title_string = [NSString stringWithCString: title
      encoding: NSASCIIStringEncoding];
  [window setTitle: title_string];
  [window setDelegate: view];

  return window;
}

}  // namespace gfx
}  // namespace mx
