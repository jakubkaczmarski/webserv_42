SRC = testy.cpp split.cpp
EXEC_NAME = exec

PINK    := \033[0;35m
PURPLE    := \033[0;34m
RESET    := \033[0m

all:
	c++ $(SRC) -o $(EXEC_NAME)

fclean:
	rm $(EXEC_NAME)

re: fclean all

git:
	git add *
	@read -p "Enter the commit message: " halp; \
	git commit -m "$$halp"
	git push
	@echo "$(PURPLE)All addeded and commiteded and pusheded!$(RESET)"