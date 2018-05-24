# This file is part of CollectEDGARData.

# CollectEDGARData is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# CollectEDGARData is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with CollectEDGARData.  If not, see <http://www.gnu.org/licenses/>.

# see link below for make file dependency magic
#
# http://bruno.defraine.net/techtips/makefile-auto-dependencies-with-gcc/
#
MAKE=gmake

BOOSTDIR := /extra/boost/boost-1.67_gcc-8
GCCDIR := /extra/gcc/gcc-8
CPP := $(GCCDIR)/bin/g++

# If no configuration is specified, "Debug" will be used
ifndef "CFG"
	CFG := Debug
endif

#	common definitions

OUTFILE := ExtractEDGAR_XBRL

CFG_INC := -I./src -I$(BOOSTDIR) -I/extra/gcc/cpp-json/include/

RPATH_LIB := -Wl,-rpath,$(GCCDIR)/lib64 -Wl,-rpath,$(BOOSTDIR)/lib -Wl,-rpath,/usr/local/lib

SDIR1 := .
SRCS1 := $(SDIR1)/Main.cpp

SDIR2 := ../ExtractEDGAR_XBRL/src
SRCS2 := $(SDIR2)/ExtractEDGAR_XBRLApp.cpp \
		$(SDIR2)/EDGAR_XML_FileFilter.cpp \
		$(SDIR2)/SEC_Header.cpp

SRCS := $(SRCS1) $(SRCS2)

VPATH := $(SDIR1):$(SDIR2)

#
# Configuration: DEBUG
#
ifeq "$(CFG)" "Debug"

OUTDIR=Debug

CFG_LIB := -lpthread -lstdc++fs \
   		-L$(BOOSTDIR)/lib \
		-lboost_date_time-mt-d -lboost_iostreams-mt-d -lboost_regex-mt-d \
		-L/usr/local/lib -lPocoFoundationd -lPocoUtild \
		-L/usr/lib \
		-lpqxx -lpq \
		-lpugixml

OBJS1=$(addprefix $(OUTDIR)/, $(addsuffix .o, $(basename $(notdir $(SRCS1)))))
OBJS2=$(addprefix $(OUTDIR)/, $(addsuffix .o, $(basename $(notdir $(SRCS2)))))

OBJS=$(OBJS1) $(OBJS2)
DEPS=$(OBJS:.o=.d)

COMPILE=$(CPP) -c  -x c++  -O0  -g3 -std=c++17 -D_DEBUG -fPIC -o $@ $(CFG_INC) $< -march=native -MMD -MP
LINK := $(CPP)  -g -o $(OUTFILE) $(OBJS) $(CFG_LIB) -Wl,-E $(RPATH_LIB)

endif #	DEBUG configuration


#
# Configuration: Release
#
ifeq "$(CFG)" "Release"

OUTDIR=Release

CFG_LIB := -lpthread -lstdc++fs \
   		-L$(BOOSTDIR)/lib \
		-lboost_date_time-mt -lboost_iostreams-mt -lboost_regex-mt \
		-L/usr/local/lib -lPocoFoundation -lPocoUtil \
		-L/usr/lib \
		-lpqxx -lpq \
		-lpugixml


OBJS1=$(addprefix $(OUTDIR)/, $(addsuffix .o, $(basename $(notdir $(SRCS1)))))
OBJS2=$(addprefix $(OUTDIR)/, $(addsuffix .o, $(basename $(notdir $(SRCS2)))))

OBJS=$(OBJS1) $(OBJS2)
DEPS=$(OBJS:.o=.d)

# need to figure out cert handling better. Until then, turn off the SSL Cert testing.

COMPILE=$(CPP) -c  -x c++  -O3  -std=c++17 -fPIC -o $@ $(CFG_INC) $< -march=native -MMD -MP
LINK := $(CPP)  -o $(OUTFILE) $(OBJS) $(CFG_LIB) -Wl,-E $(RPATH_LIB)

endif #	RELEASE configuration

# Build rules
all: $(OUTFILE)

$(OUTDIR)/%.o : %.cpp
	$(COMPILE)

$(OUTFILE): $(OUTDIR) $(OBJS1) $(OBJS2)
	$(LINK)

-include $(DEPS)

$(OUTDIR):
	mkdir -p "$(OUTDIR)"

# Rebuild this project
rebuild: cleanall all

# Clean this project
clean:
	rm -f $(OUTFILE)
	rm -f $(OBJS)
	rm -f $(OUTDIR)/*.d
	rm -f $(OUTDIR)/*.o

# Clean this project and all dependencies
cleanall: clean