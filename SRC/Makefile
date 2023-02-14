CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I /usr/include/postgresql/ -lpq

OBJ = main.o MealMaster.o dbInfo.o
EXE = MealMasterv1

all: $(EXE)

$(EXE): $(OBJ)
	$(CXX) $(OBJ) -o $(EXE) $(CXXFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJ) $(EXE)
