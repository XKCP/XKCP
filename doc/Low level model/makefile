BINDIR=bin
INCLUDE=-I src/ -I ../../tests/UnitTests/
CFLAGS=-g0 -O3 -Werror -Wall -Wextra -Wno-write-strings -Wno-deprecated-declarations -std=c++17
LDFLAGS=
SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(addprefix $(BINDIR)/, $(notdir $(SOURCES:.cpp=.o)))
EXECUTABLE=$(BINDIR)/Model
vpath %.cpp src

all: $(EXECUTABLE)

$(BINDIR):
	mkdir -p $(BINDIR)

$(EXECUTABLE): $(BINDIR) $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

-include $(BINDIR)/$(OBJECTS:.o=.d)

$(BINDIR)/%.o: %.cpp
	$(CXX) $(INCLUDE) -c $(CFLAGS) $< -o $(BINDIR)/$*.o
	$(CXX) $(INCLUDE) -MM $(CFLAGS) $< > $(BINDIR)/$*.d
	@mv -f $(BINDIR)/$*.d $(BINDIR)/$*.d.tmp
	@sed -e 's|.*:|$(BINDIR)/$*.o:|' < $(BINDIR)/$*.d.tmp > $(BINDIR)/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $(BINDIR)/$*.d.tmp | fmt -1 | \
	sed -e 's/^ *//' -e 's/$$/:/' >> $(BINDIR)/$*.d
	@rm -f $(BINDIR)/$*.d.tmp

clean:
	rm -fr $(BINDIR)/
