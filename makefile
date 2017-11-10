CXX=g++
CXXFLAGS=-std=c++11 -Wall -pedantic -Wextra -Wno-long-long -O0 -ggdb
DIR=obj

compile: $(DIR)/main.o $(DIR)/table.o $(DIR)/language.o $(DIR)/file_save.o $(DIR)/moves.o $(DIR)/figures.o $(DIR)/pawn_black.o $(DIR)/pawn_white.o $(DIR)/rook.o $(DIR)/knight.o $(DIR)/bishop.o $(DIR)/king.o $(DIR)/queen.o $(DIR)/exeptions.o
	$(CXX) $(CXXFLAGS) $^ -o chess

%.o: ../src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: 
	./chess
obj_clean:
	rm -rf $(DIR)/*.o chess
data_clean:
	rm -rf database/*.chess
