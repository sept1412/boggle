BIN := boggle
TEST_BOARD := test_board
TEST_TRIE := test_trie
TEST_PERF := test_perf

SRCS := \
	main.cc \
	Board.cc \
	Solver.cc \
	Trie.cc

OBJDIR := .o

DEPDIR := .d

OBJS := $(patsubst %,$(OBJDIR)/%.o,$(basename $(SRCS)))

DEPS := $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS)))

$(shell mkdir -p $(dir $(OBJS)) >/dev/null)
$(shell mkdir -p $(dir $(DEPS)) >/dev/null)

CXX := g++

LD := g++

CXXFLAGS := -std=c++17

CPPFLAGS := -g -Wall -Wextra -pedantic

LDFLAGS :=

DEPFLAGS = -MT $@ -MD -MP -MF $(DEPDIR)/$*.Td

COMPILE.cc = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) -c -o $@

LINK.o = $(LD) $(LDFLAGS) $(LDLIBS) -o $@

PRECOMPILE =

POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

all: $(BIN) $(TEST_BOARD) $(TEST_TRIE) $(TEST_PERF)

.PHONY: clean

clean:
	$(RM) -r $(OBJDIR) $(DEPDIR) $(BIN) $(TEST_BOARD) $(TEST_TRIE) $(TEST_PERF)

$(BIN): $(OBJS)
	$(LINK.o) $^

$(TEST_BOARD): $(BIN)
	ln -s $< $@

$(TEST_TRIE): $(BIN)
	ln -s $< $@

$(TEST_PERF): $(BIN)
	ln -s $< $@

$(OBJDIR)/%.o: %.cc
$(OBJDIR)/%.o: %.cc $(DEPDIR)/%.d
	$(PRECOMPILE)
	$(COMPILE.cc) $<
	$(POSTCOMPILE)

.PRECIOUS = $(DEPDIR)/%.d
$(DEPDIR)/%.d: ;

-include $(DEPS)
