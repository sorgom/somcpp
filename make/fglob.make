# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=ci
endif

ifndef verbose
  SILENT = @
endif

.PHONY: clean prebuild

SHELLTYPE := posix
ifeq ($(shell echo "test"), "test")
	SHELLTYPE := msdos
endif

# Configurations
# #############################################

RESCOMP = windres
TARGETDIR = ../build
TARGET = $(TARGETDIR)/fglob
INCLUDES += -I../include
FORCE_INCLUDE +=
ALL_CPPFLAGS += $(CPPFLAGS) -MD -MP $(DEFINES) $(INCLUDES)
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -O3 -std=c++17 -O3 -pedantic-errors -Wall
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -O3 -std=c++17 -O3 -pedantic-errors -Wall
ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
LIBS +=
LDDEPS +=
ALL_LDFLAGS += $(LDFLAGS) -s
LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
define PREBUILDCMDS
endef
define PRELINKCMDS
endef
define POSTBUILDCMDS
endef

ifeq ($(config),ci)
OBJDIR = ../build/linux/ci/fglob
DEFINES += -DNDEBUG

else ifeq ($(config),trace_on)
OBJDIR = ../build/linux/trace_on/fglob
DEFINES += -DNDEBUG -DTRACE_ON

else ifeq ($(config),trace_all)
OBJDIR = ../build/linux/trace_all/fglob
DEFINES += -DNDEBUG -DTRACE_ALL

else ifeq ($(config),glob_linux)
OBJDIR = ../build/linux/glob_linux/fglob
DEFINES += -DNDEBUG -DGLOB_LINUX

else ifeq ($(config),glob_linux_trace_all)
OBJDIR = ../build/linux/glob_linux_trace_all/fglob
DEFINES += -DNDEBUG -DGLOB_LINUX -DTRACE_ALL

endif

# Per File Configurations
# #############################################


# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/fglob.o
GENERATED += $(OBJDIR)/fglobMain.o
OBJECTS += $(OBJDIR)/fglob.o
OBJECTS += $(OBJDIR)/fglobMain.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking fglob
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning fglob
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(GENERATED)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(GENERATED)) del /s /q $(subst /,\\,$(GENERATED))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild: | $(OBJDIR)
	$(PREBUILDCMDS)

ifneq (,$(PCH))
$(OBJECTS): $(GCH) | $(PCH_PLACEHOLDER)
$(GCH): $(PCH) | prebuild
	@echo $(notdir $<)
	$(SILENT) $(CXX) -x c++-header $(ALL_CXXFLAGS) -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
$(PCH_PLACEHOLDER): $(GCH) | $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) touch "$@"
else
	$(SILENT) echo $null >> "$@"
endif
else
$(OBJECTS): | prebuild
endif


# File Rules
# #############################################

$(OBJDIR)/fglobMain.o: ../runtime/fglobMain.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/fglob.o: ../src/fglob.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif