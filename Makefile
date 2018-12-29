############################################################################
# Makefile for the raytracer framwork created for the Computer Science
# course "Introduction Computer Graphics" taught at the University of
# Groningen by Tobias Isenberg.
############################################################################

### MACROS

# GNU (everywhere)
CPP = g++ -g -Og -Wall -Wextra
CPP+= -Wshadow # warn the user if a variable declaration shadows one from a parent context
CPP+= -Wnon-virtual-dtor # warn the user if a class with virtual functions has a
		# non-virtual destructor. This helps catch hard to
		# track down memory errors
CPP+= -Wold-style-cast # warn for c-style casts
CPP+= -Wcast-align # warn for potential performance problem casts
CPP+= -Wunused # warn on anything being unused
CPP+= -Woverloaded-virtual # warn if you overload (not override) a virtual
		# function
CPP+= -Wpedantic # warn if non-standard C++ is used
CPP+= -Wconversion # warn on type conversions that may lose data
CPP+= -Wsign-conversion # warn on sign conversions
CPP+= -Wmisleading-indentation # warn if identation implies blocks where blocks
		# do not exist
CPP+= -Wduplicated-cond # warn if if / else chain has duplicated conditions
CPP+= -Wduplicated-branches # warn if if / else branches have duplicated code
CPP+=	-Wlogical-op # warn about logical operations being used where bitwise were
		# probably wanted
CPP+=	-Wnull-dereference # warn if a null dereference is detected
CPP+=	-Wuseless-cast # warn if you perform a cast to the same type
CPP+=	-Wdouble-promotion # warn if float is implicit promoted to double
CPP+=	-Wformat=2 # warn on security issues around functions that format output (ie printf)
CPP+= -std=c++17


# GNU (faster)
#CPP = g++ -O5 -Wall -fomit-frame-pointer -ffast-math

LIBS = -lm

EXECUTABLE = ray

OBJS = main.o raytracer.o sphere.o plane.o light.o material.o camera.o \
	image.o triple.o lodepng.o scene.o


YAMLOBJS = $(subst .cpp,.o,$(wildcard yaml/*.cpp))

IMAGES = $(subst .yaml,.png,$(wildcard *.yaml))


### TARGETS

$(EXECUTABLE): $(OBJS) $(YAMLOBJS)
	$(CPP) $(OBJS) $(YAMLOBJS) $(LIBS) -o $@

run: $(IMAGES)

%.png: %.yaml $(EXECUTABLE)
	./$(EXECUTABLE) $<

depend: make.dep

clean:
	- /bin/rm -f  *.bak *~ $(OBJS) $(YAMLOBJS) $(EXECUTABLE) $(EXECUTABLE).exe

make.dep:
	gcc -MM $(OBJS:.o=.cpp) > make.dep

### RULES

.SUFFIXES: .cpp .o .yaml .png

.cpp.o:
	$(CPP) -c -o $@ $<

### DEPENDENCIES

include make.dep
