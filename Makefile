
program_name := game

source_dirs  := Game UI .

include_dirs := Game UI .

  

source_dirs  := $(addprefix ./src/, $(source_dirs) )
source_files := $(wildcard $(addsuffix /*.cpp, $(source_dirs) ) )
object_files := $(notdir $(source_files) )
object_files := $(object_files:.cpp=.o)

compile_flags:=  -lSDL2 -lSDL2_ttf -lSDL2_image -lX11

all: $(program_name)

$(program_name): $(object_files)
	g++-9 -std=c++17 $^ -o $@  $(compile_flags) 

VPATH := $(source_dirs)

%.o: %.cpp 
	g++-9 -std=c++17 $< -c $(compile_flags) $(addprefix -I, $(include_dirs)) $(addprefix -I, $(source_dirs)) -MD 

include $(wildcard *.d) 

run:
	./$(program_name)


clean:
	-rm -f $(program_name)
	-rm -Rf burbyleo

doc:
	#
	# Documentation statically generated in directory "doc".
	# Open Xming
	# export DISPLAY=:0
	# xeyes for testing

compile: $(program_name)
	mkdir -p ./burbyleo
	mkdir -p ./burbyleo/src
	cp -R ./src/SavedGames ./burbyleo/src
	cp -R ./src/Textures ./burbyleo/src
	cp ./game ./burbyleo/game
	cp ./src/Coffee_Extra.ttf ./burbyleo/src/Coffee_Extra.ttf
	rm -f *.o
	rm -f *.d