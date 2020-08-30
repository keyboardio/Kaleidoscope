GTEST_DIR=googletest/googletest
BIN_DIR=bin
LIB_DIR=lib
OBJ_DIR=obj

all: run-hello-test

run-hello-test: $(BIN_DIR)/hello-test
	$(BIN_DIR)/hello-test

$(BIN_DIR)/hello-test: hello-test.cpp $(LIB_DIR)/libgtest.so $(OBJ_DIR)/gtest_main.o
	mkdir -p $(BIN_DIR)
	g++ -o $(BIN_DIR)/hello-test -I$(GTEST_DIR)/include hello-test.cpp $(OBJ_DIR)/gtest_main.o $(LIB_DIR)/libgtest.so

$(OBJ_DIR)/gtest_main.o: $(GTEST_DIR)/src/gtest_main.cc $(wildcard $(GTEST_DIR)/include/gtest/*.h)
	mkdir -p $(OBJ_DIR)
	g++ -o $(OBJ_DIR)/gtest_main.o -c -pthread -I$(GTEST_DIR)/include $(GTEST_DIR)/src/gtest_main.cc

$(LIB_DIR)/libgtest.so: $(wildcard $(GTEST_DIR)/src/*.cc) $(wildcard $(GTEST_DIR)/include/gtest/*.h)
	mkdir -p $(LIB_DIR)
	g++ -o $(LIB_DIR)/libgtest.so -shared -fPIC -pthread -I$(GTEST_DIR) -I$(GTEST_DIR)/include $(GTEST_DIR)/src/gtest-all.cc

clean:
	rm $(BIN_DIR)/* $(LIB_DIR)/* $(OBJ_DIR)/*
