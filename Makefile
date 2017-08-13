NAME = snek

CODE_FILES = main.c snek.c keybInterface.c stage.c

DOC_FILES = docs
DEBUG = YES
ifeq ($(DEBUG),YES)
	D = -g
else
	D =
endif

.PHONY: all clean docs

all: $(CODE_FILES)
	gcc -lpthread -Wno-parentheses -Wextra -fsanitize=undefined -Wall -g -o $(NAME) $(CODE_FILES)

#docs: Doxyfile
#	doxygen Doxyfile

#Doxyfile:
#	doxygen -g

clean:
	rm -rf $(NAME) $(DOC_FILES) *.o
