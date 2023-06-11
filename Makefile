
program_name := burbyleo

source_dirs  := Game UI .

include_dirs := Game UI .


# Compiler Settings
CXX = g++-9

source_dirs  := $(addprefix ./src/, $(source_dirs) )
source_files := $(wildcard $(addsuffix /*.cpp, $(source_dirs) ) )
object_files := $(notdir $(source_files) )
object_files := $(object_files:.cpp=.o)

compile_flags:=  -lSDL2 -lSDL2_ttf -lSDL2_image -lX11


all: $(program_name)

$(program_name): $(object_files)
	$(CXX) -std=c++17 $^ -o $@  $(compile_flags) 

VPATH := $(source_dirs)

%.o: %.cpp 
	$(CXX) -std=c++17 $< -c $(compile_flags) $(addprefix -I, $(include_dirs)) $(addprefix -I, $(source_dirs)) -MD 

include $(wildcard *.d) 

run:
	export DISPLAY=:0
	./burbyleo

clean:
	rm -f *.o
	rm -f *.d
	rm burbyleo

doc:
	echo "" >> ./doc/index.html

compile: $(program_name)
	rm -f *.o
	rm -f *.d

count:
	@lines_count=$$(find . -type f \( -name "*.cpp" -o -name "*.hpp" \) -exec cat {} + | wc -l); \
	echo "Total number of lines is $$lines_count"
