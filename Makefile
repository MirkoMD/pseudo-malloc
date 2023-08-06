CC := gcc
CFLAGS := -c -Wall -Wextra -std=gnu99 -g -I$(SRCDIR) -DENABLE_LOGGING
AR := ar

SRCDIR := src
TESTDIR := test
OBJDIR := obj

SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))
HEADERS := $(wildcard $(SRCDIR)/*.h)
TEST_SOURCES := $(wildcard $(TESTDIR)/*.c)
TEST_OBJS := $(patsubst $(TESTDIR)/%.c, $(OBJDIR)/%.o, $(TEST_SOURCES))

TARGET := $(OBJDIR)/main
TEST_TARGETS := $(patsubst $(TESTDIR)/%.c, $(OBJDIR)/%, $(TEST_SOURCES))
LIBS := $(OBJDIR)/libbuddy.a
DEPS := $(filter-out $(OBJDIR)/main.o, $(OBJS))

all: $(TARGET) $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: $(TESTDIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

$(TARGET): $(OBJDIR)/main.o $(LIBS)
	$(CC) $^ -o $@
	$(RM) $(TARGET).o 

$(LIBS): $(DEPS)
	$(AR) rcs $@ $^

$(OBJDIR)/%: $(OBJDIR)/%.o $(DEPS)
	$(CC) $^ -o $@

test: $(TEST_TARGETS)

clean:
	$(RM) $(OBJDIR)/*.o $(TARGET) $(TEST_TARGETS) $(LIBS)
