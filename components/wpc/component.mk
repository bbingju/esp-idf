#
# Component Makefile
#
ifdef CONFIG_WIREPAS_LIBRARY

COMPONENT_ADD_INCLUDEDIRS := include libwpc/source/api libwpc/source/wpc/include

COMPONENT_SRCDIRS := libwpc/source/wpc port

# Check the submodule is initialised
COMPONENT_SUBMODULES := libwpc


else
# Disable Wirepas Dual-MCU API support
COMPONENT_ADD_INCLUDEDIRS :=
COMPONENT_ADD_LDFLAGS :=
COMPONENT_SRCDIRS :=
endif


