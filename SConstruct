# Copyright 2008 Christoph Lang. All rights reserved.
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
#       copyright notice, this list of conditions and the following
#       disclaimer in the documentation and/or other materials provided
#       with the distribution.
#     * Neither the name of Christoph Lang nor the names of its
#       contributors may be used to endorse or promote products derived
#       from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import sys
import os

def CheckCommand(context, cmd):
    context.Message('Checking for %s command... ' % cmd)
    result = WhereIs(cmd)
    context.Result(result is not None)
    return result

mode = ARGUMENTS.get('mode', 'debug')
env = Environment(CPPPATH = ['#/include'], 
                  ENV = {'PATH' : os.environ['PATH']},
                  LIBPATH = ['#'])
use_clang = False

# try to use clang for debug builds on non-windows platforms
if (not sys.platform == 'win32') and mode == 'debug':
    conf = Configure(env, custom_tests = {'CheckCommand' : CheckCommand})
    if conf.CheckCommand('clang'):
        use_clang = True
        cc = 'clang'
        cxx = 'clang++'
        env['CC'] = [cc]
        env['SHCC'] = [cc]
        env['CXX'] = [cxx]
        env['SHCXX'] = [cxx]
        env['LINK'] = [cxx]
        env['SHLINK'] = [cxx]
    env = conf.Finish()

# compiler flags
if not sys.platform == 'win32':
    env['CXXFLAGS'] = ['-Wall', '-pedantic']
    if use_clang:
        env['CXXFLAGS'].append(['-fcolor-diagnostics'])

    if mode == 'debug':
        env['CXXFLAGS'].append(['-g'])
        env['CPPDEFINES'] = ['_DEBUG', '_ASSERT']
    elif mode == 'release':
        env['CXXFLAGS'].append(['-O3', '-ffast-math'])

Export('env', 'mode')
SConscript(['#/source/SConscript', '#/tests/SConscript'])
