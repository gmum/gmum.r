include ../vars.mk

CURRENT_HFILES := $(wildcard *.h)

SRC = ../../../src/
HFILES = $(wildcard $(SRC)*.hpp)
OFILES = $(patsubst $(SRC)%.cpp,$(SRC)$(OBJDIR)%.o, $(wildcard $(SRC)*.cpp))
CURRENT_SRC := $(wildcard *.cpp)
CURRENT_OBJS := $(patsubst %.cpp,%.o,$(CURRENT_SRC))
CURRENT_OBJ_FILES = ${CURRENT_OBJS:%=${OBJDIR}%} 

all: main
fresh: clean main

main: $(CURRENT_OBJ_FILES)

-include ${OBJ_FILES:%=%.d}

$(OBJDIR)%.o: %.cpp $(HFILES) $(CURRENT_HFILES)
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(I_PATH) -MT $@ -MD -MP -MF $(@:$(OBJDIR)%=$(OBJDIR)%.d) -c -o $@ $<
	

clean:
	rm -f $(OBJDIR)*.o $(OBJDIR)*.o.d

debug:
	@echo $(CURRENT_OBJ_FILES)
	@echo $(CXX)

