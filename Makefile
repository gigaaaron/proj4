CC=gcc
CXX=g++

INC_DIR=./include
SRC_DIR=./src
TESTSRC_DIR=./testsrc
BIN_DIR=./bin
OBJ_DIR=./obj

CXXFLAG=-std=gnu++17 -I $(INC_DIR)
LDFLAGS=-L /opt/local/lib -lgtest -lgtest_main -lpthread -lexpat

all: directories runtest dsvtest sinktest bustest osmtest

runtest: $(BIN_DIR)/teststrutils
	$(BIN_DIR)/teststrutils

$(BIN_DIR)/teststrutils: $(OBJ_DIR)/StringUtilsTest.o $(OBJ_DIR)/StringUtils.o
	$(CXX) -o $(BIN_DIR)/teststrutils $(OBJ_DIR)/StringUtilsTest.o $(OBJ_DIR)/StringUtils.o $(LDFLAGS)

$(OBJ_DIR)/StringUtilsTest.o: $(TESTSRC_DIR)/StringUtilsTest.cpp $(INC_DIR)/StringUtils.h
	$(CXX) -o $(OBJ_DIR)/StringUtilsTest.o $(CXXFLAG) -c $(TESTSRC_DIR)/StringUtilsTest.cpp 

$(OBJ_DIR)/StringUtils.o: $(SRC_DIR)/StringUtils.cpp $(INC_DIR)/StringUtils.h
	$(CXX) -o $(OBJ_DIR)/StringUtils.o $(CXXFLAG) -c $(SRC_DIR)/StringUtils.cpp

dsvtest: $(BIN_DIR)/dsvtest
	$(BIN_DIR)/dsvtest

$(BIN_DIR)/dsvtest: $(OBJ_DIR)/DSVTest.o $(OBJ_DIR)/DSVWriter.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringUtils.o
	$(CXX) -o $(BIN_DIR)/dsvtest $(OBJ_DIR)/DSVTest.o $(OBJ_DIR)/DSVWriter.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringUtils.o $(LDFLAGS) 

$(OBJ_DIR)/DSVTest.o: $(TESTSRC_DIR)/DSVTest.cpp $(SRC_DIR)/StringDataSink.cpp $(SRC_DIR)/StringDataSource.cpp
	$(CXX) -o $(OBJ_DIR)/DSVTest.o $(CXXFLAG) -c $(TESTSRC_DIR)/DSVTest.cpp

$(OBJ_DIR)/DSVWriter.o: $(SRC_DIR)/DSVWriter.cpp $(INC_DIR)/DSVWriter.h $(INC_DIR)/StringUtils.h
	$(CXX) -o $(OBJ_DIR)/DSVWriter.o $(CXXFLAG) -c $(SRC_DIR)/DSVWriter.cpp

$(OBJ_DIR)/DSVReader.o: $(SRC_DIR)/DSVReader.cpp $(INC_DIR)/DSVReader.h
	$(CXX) -o $(OBJ_DIR)/DSVReader.o $(CXXFLAG) -c $(SRC_DIR)/DSVReader.cpp

xmltest: $(BIN_DIR)/xmltest
	$(BIN_DIR)/xmltest

$(BIN_DIR)/xmltest: $(OBJ_DIR)/XMLTest.o $(OBJ_DIR)/XMLWriter.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/StringDataSource.o
	$(CXX) -o $(BIN_DIR)/xmltest $(OBJ_DIR)/XMLTest.o $(OBJ_DIR)/XMLWriter.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/StringDataSource.o $(LDFLAGS) 

$(OBJ_DIR)/XMLTest.o: $(TESTSRC_DIR)/XMLTest.cpp $(SRC_DIR)/StringDataSink.cpp $(SRC_DIR)/StringDataSource.cpp
	$(CXX) -o $(OBJ_DIR)/XMLTest.o $(CXXFLAG) -c $(TESTSRC_DIR)/XMLTest.cpp

$(OBJ_DIR)/XMLWriter.o: $(SRC_DIR)/XMLWriter.cpp $(INC_DIR)/XMLWriter.h
	$(CXX) -o $(OBJ_DIR)/XMLWriter.o $(CXXFLAG) -c $(SRC_DIR)/XMLWriter.cpp

$(OBJ_DIR)/XMLReader.o: $(SRC_DIR)/XMLReader.cpp $(INC_DIR)/XMLReader.h
	$(CXX) -o $(OBJ_DIR)/XMLReader.o $(CXXFLAG) -c $(SRC_DIR)/XMLReader.cpp

srctest: $(BIN_DIR)/srctest
	$(BIN_DIR)/srctest

$(BIN_DIR)/srctest: $(OBJ_DIR)/StringDataSourceTest.o $(OBJ_DIR)/StringDataSource.o
	$(CXX) -o $(BIN_DIR)/srctest $(OBJ_DIR)/StringDataSourceTest.o $(OBJ_DIR)/StringDataSource.o $(LDFLAGS)

$(OBJ_DIR)/StringDataSourceTest.o: $(TESTSRC_DIR)/StringDataSourceTest.cpp $(INC_DIR)/StringDataSource.h $(INC_DIR)/DataSource.h
	$(CXX) -o $(OBJ_DIR)/StringDataSourceTest.o $(CXXFLAG) -c ${TESTSRC_DIR}/StringDataSourceTest.cpp

$(OBJ_DIR)/StringDataSource.o: $(SRC_DIR)/StringDataSource.cpp $(INC_DIR)/StringDataSource.h $(INC_DIR)/DataSource.h
	$(CXX) -o $(OBJ_DIR)/StringDataSource.o $(CXXFLAG) -c $(SRC_DIR)/StringDataSource.cpp

sinktest: $(BIN_DIR)/sinktest
	$(BIN_DIR)/sinktest

$(BIN_DIR)/sinktest: $(OBJ_DIR)/StringDataSinkTest.o $(OBJ_DIR)/StringDataSink.o
	$(CXX) -o $(BIN_DIR)/sinktest $(OBJ_DIR)/StringDataSinkTest.o $(OBJ_DIR)/StringDataSink.o $(LDFLAGS)

$(OBJ_DIR)/StringDataSinkTest.o: $(TESTSRC_DIR)/StringDataSinkTest.cpp $(INC_DIR)/StringDataSink.h $(INC_DIR)/DataSink.h
	$(CXX) -o $(OBJ_DIR)/StringDataSinkTest.o $(CXXFLAG) -c $(TESTSRC_DIR)/StringDataSinkTest.cpp

$(OBJ_DIR)/StringDataSink.o: $(SRC_DIR)/StringDataSink.cpp $(INC_DIR)/StringDataSink.h $(INC_DIR)/DataSink.h
	$(CXX) -o $(OBJ_DIR)/StringDataSink.o $(CXXFLAG) -c $(SRC_DIR)/StringDataSink.cpp

bustest: $(BIN_DIR)/bustest
	$(BIN_DIR)/bustest

$(BIN_DIR)/bustest: $(OBJ_DIR)/BusSystemTest.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/StringUtils.o
	$(CXX) -o $(BIN_DIR)/bustest $(OBJ_DIR)/BusSystemTest.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/StringUtils.o $(LDFLAGS) 

$(OBJ_DIR)/BusSystemTest.o: $(TESTSRC_DIR)/BusSystemTest.cpp
	$(CXX) -o $(OBJ_DIR)/BusSystemTest.o $(CXXFLAG) -c ${TESTSRC_DIR}/BusSystemTest.cpp

$(OBJ_DIR)/CSVBusSystem.o: $(SRC_DIR)/CSVBusSystem.cpp $(INC_DIR)/CSVBusSystem.h $(INC_DIR)/BusSystem.h
	$(CXX) -o $(OBJ_DIR)/CSVBusSystem.o $(CXXFLAG) -c $(SRC_DIR)/CSVBusSystem.cpp


osmtest: $(BIN_DIR)/osmtest
	$(BIN_DIR)/osmtest

$(BIN_DIR)/osmtest: $(OBJ_DIR)/OSMTest.o $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/XMLReader.o
	$(CXX) -o $(BIN_DIR)/osmtest $(OBJ_DIR)/OSMTest.o $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/XMLReader.o $(LDFLAGS)

$(OBJ_DIR)/OSMTest.o: $(TESTSRC_DIR)/OSMTest.cpp
	$(CXX) -o $(OBJ_DIR)/OSMTest.o $(CXXFLAG) -c ${TESTSRC_DIR}/OSMTest.cpp

$(OBJ_DIR)/OpenStreetMap.o: $(SRC_DIR)/OpenStreetMap.cpp $(INC_DIR)/OpenStreetMap.h $(INC_DIR)/StreetMap.h
	$(CXX) -o $(OBJ_DIR)/OpenStreetMap.o $(CXXFLAG) -c $(SRC_DIR)/OpenStreetMap.cpp

directories:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)
	
clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)
