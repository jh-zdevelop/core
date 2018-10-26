#
# MIT License
#
# Copyright (c) 2018 Jason Hoyt
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

# tools
CC=gcc
RM=rm
MKDIR=mkdir
RMDIR=rmdir
AR=ar

# flags
CFLAGS=-Wall -O2 -I./include/core
RMFLAGS=-f
MKDIRFLAGS=-p
RMDIRFLAGS=
ARFLAGS=-r

# outputs
LIBRARY=lib/libcore.a

# object files
OBJECTS=\
	obj/core.o \
	obj/mem.o

all: $(LIBRARY)

clean:
	$(RM) $(LIBRARY) $(OBJECTS)
	@$(RMDIR) $(RMDIRFLAGS) lib
	@$(RMDIR) $(RMDIRFLAGS) obj

$(LIBRARY): $(OBJECTS) | lib
	$(AR) $(ARFLAGS) $(LIBRARY) $(OBJECTS)

obj/%.o: src/%.c | obj
	$(CC) $(CFLAGS) -c -o $@ $<

lib:
	@$(MKDIR) $(MKDIRFLAGS) lib

obj:
	@$(MKDIR) $(MKDIRFLAGS) obj