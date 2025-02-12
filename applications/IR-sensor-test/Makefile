include ../shared.mk

SOURCES:=$(wildcard *.c)
#CFLAGS+=-Werror

CFLAGS+=$(shell pkg-config --cflags glib-2.0)
LDFLAGS+=$(shell pkg-config --libs glib-2.0)

include ../end.mk

