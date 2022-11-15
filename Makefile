SRC = testy.cpp
EXEC_NAME = exec

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