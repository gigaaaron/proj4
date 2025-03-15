CXX=g++

INC_DIR=./include
SRC_DIR=./src
TESTSRC_DIR=./testsrc
BIN_DIR=./bin
OBJ_DIR=./obj
TO_LINK=$(OBJ_DIR)/FileDataFactory.o $(OBJ_DIR)/FileDataSink.o $(OBJ_DIR)/FileDataSource.o $(OBJ_DIR)/StandardErrorDataSink.o $(OBJ_DIR)/StandardDataSink.o $(OBJ_DIR)/StandardDataSource.o $(OBJ_DIR)/TransportationPlannerCommandLine.o $(OBJ_DIR)/DijkstraTransportationPlanner.o $(OBJ_DIR)/DijkstraPathRouter.o $(OBJ_DIR)/GeographicUtils.o $(OBJ_DIR)/BusSystemIndexer.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/DSVWriter.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringUtils.o

CXXFLAG=-std=gnu++17 -I $(INC_DIR)
LDFLAGS=-L /opt/local/lib -lgtest -lgtest_main -lgmock -lpthread -lexpat

all: directories runtest

runtest: $(BIN_DIR)/strutilstest $(BIN_DIR)/sinktest $(BIN_DIR)/srctest $(BIN_DIR)/dsvtest $(BIN_DIR)/xmltest $(BIN_DIR)/bustest $(BIN_DIR)/osmtest $(BIN_DIR)/dprtest $(BIN_DIR)/bsitest $(BIN_DIR)/tptest $(BIN_DIR)/tpcltest $(BIN_DIR)/kmltest
	-$(BIN_DIR)/strutilstest
	-$(BIN_DIR)/sinktest
	-$(BIN_DIR)/srctest
	-$(BIN_DIR)/dsvtest
	-$(BIN_DIR)/xmltest
	-$(BIN_DIR)/bustest
	-$(BIN_DIR)/osmtest
	-$(BIN_DIR)/dprtest
	-$(BIN_DIR)/bsitest
	-$(BIN_DIR)/tptest
	-$(BIN_DIR)/tpcltest
	-$(BIN_DIR)/kmltest



strutilstest: $(BIN_DIR)/strutilstest
	$(BIN_DIR)/strutilstest

$(BIN_DIR)/strutilstest: $(OBJ_DIR)/StringUtilsTest.o $(OBJ_DIR)/StringUtils.o
	$(CXX) -o $(BIN_DIR)/strutilstest $(OBJ_DIR)/StringUtilsTest.o $(OBJ_DIR)/StringUtils.o $(LDFLAGS)

$(OBJ_DIR)/StringUtilsTest.o: $(TESTSRC_DIR)/StringUtilsTest.cpp
	$(CXX) -o $(OBJ_DIR)/StringUtilsTest.o $(CXXFLAG) -c $(TESTSRC_DIR)/StringUtilsTest.cpp 

$(OBJ_DIR)/StringUtils.o: $(SRC_DIR)/StringUtils.cpp $(INC_DIR)/StringUtils.h
	$(CXX) -o $(OBJ_DIR)/StringUtils.o $(CXXFLAG) -c $(SRC_DIR)/StringUtils.cpp



srctest: $(BIN_DIR)/srctest
	$(BIN_DIR)/srctest

$(BIN_DIR)/srctest: $(OBJ_DIR)/StringDataSourceTest.o $(OBJ_DIR)/StringDataSource.o
	$(CXX) -o $(BIN_DIR)/srctest $(OBJ_DIR)/StringDataSourceTest.o $(OBJ_DIR)/StringDataSource.o $(LDFLAGS)

$(OBJ_DIR)/StringDataSourceTest.o: $(TESTSRC_DIR)/StringDataSourceTest.cpp
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



dsvtest: $(BIN_DIR)/dsvtest
	$(BIN_DIR)/dsvtest

$(BIN_DIR)/dsvtest: $(OBJ_DIR)/DSVTest.o $(OBJ_DIR)/DSVWriter.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringUtils.o
	$(CXX) -o $(BIN_DIR)/dsvtest $(OBJ_DIR)/DSVTest.o $(OBJ_DIR)/DSVWriter.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringUtils.o $(LDFLAGS) 

$(OBJ_DIR)/DSVTest.o: $(TESTSRC_DIR)/DSVTest.cpp
	$(CXX) -o $(OBJ_DIR)/DSVTest.o $(CXXFLAG) -c $(TESTSRC_DIR)/DSVTest.cpp

$(OBJ_DIR)/DSVWriter.o: $(SRC_DIR)/DSVWriter.cpp $(INC_DIR)/DSVWriter.h
	$(CXX) -o $(OBJ_DIR)/DSVWriter.o $(CXXFLAG) -c $(SRC_DIR)/DSVWriter.cpp

$(OBJ_DIR)/DSVReader.o: $(SRC_DIR)/DSVReader.cpp $(INC_DIR)/DSVReader.h
	$(CXX) -o $(OBJ_DIR)/DSVReader.o $(CXXFLAG) -c $(SRC_DIR)/DSVReader.cpp



xmltest: $(BIN_DIR)/xmltest
	$(BIN_DIR)/xmltest

$(BIN_DIR)/xmltest: $(OBJ_DIR)/XMLTest.o $(OBJ_DIR)/XMLWriter.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringUtils.o
	$(CXX) -o $(BIN_DIR)/xmltest $(OBJ_DIR)/XMLTest.o $(OBJ_DIR)/XMLWriter.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringUtils.o $(LDFLAGS) 

$(OBJ_DIR)/XMLTest.o: $(TESTSRC_DIR)/XMLTest.cpp 
	$(CXX) -o $(OBJ_DIR)/XMLTest.o $(CXXFLAG) -c $(TESTSRC_DIR)/XMLTest.cpp

$(OBJ_DIR)/XMLWriter.o: $(SRC_DIR)/XMLWriter.cpp $(INC_DIR)/XMLWriter.h
	$(CXX) -o $(OBJ_DIR)/XMLWriter.o $(CXXFLAG) -c $(SRC_DIR)/XMLWriter.cpp

$(OBJ_DIR)/XMLReader.o: $(SRC_DIR)/XMLReader.cpp $(INC_DIR)/XMLReader.h
	$(CXX) -o $(OBJ_DIR)/XMLReader.o $(CXXFLAG) -c $(SRC_DIR)/XMLReader.cpp



bustest: $(BIN_DIR)/bustest
	$(BIN_DIR)/bustest

$(BIN_DIR)/bustest: $(OBJ_DIR)/CSVBusSystemTest.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/StringUtils.o
	$(CXX) -o $(BIN_DIR)/bustest $(OBJ_DIR)/CSVBusSystemTest.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/StringUtils.o $(LDFLAGS) 

$(OBJ_DIR)/CSVBusSystemTest.o: $(TESTSRC_DIR)/CSVBusSystemTest.cpp
	$(CXX) -o $(OBJ_DIR)/CSVBusSystemTest.o $(CXXFLAG) -c ${TESTSRC_DIR}/CSVBusSystemTest.cpp

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


dprtest: $(BIN_DIR)/dprtest
	$(BIN_DIR)/dprtest

$(BIN_DIR)/dprtest: $(OBJ_DIR)/DijkstraTest.o $(OBJ_DIR)/DijkstraPathRouter.o
	$(CXX) -o $(BIN_DIR)/dprtest $(OBJ_DIR)/DijkstraTest.o $(OBJ_DIR)/DijkstraPathRouter.o $(LDFLAGS)

$(OBJ_DIR)/DijkstraTest.o: $(TESTSRC_DIR)/DijkstraTest.cpp
	$(CXX) -o $(OBJ_DIR)/DijkstraTest.o $(CXXFLAG) -c $(TESTSRC_DIR)/DijkstraTest.cpp

$(OBJ_DIR)/DijkstraPathRouter.o: $(SRC_DIR)/DijkstraPathRouter.cpp $(INC_DIR)/DijkstraPathRouter.h
	$(CXX) -o $(OBJ_DIR)/DijkstraPathRouter.o $(CXXFLAG) -c $(SRC_DIR)/DijkstraPathRouter.cpp


bsitest: $(BIN_DIR)/bsitest
	$(BIN_DIR)/bsitest

$(BIN_DIR)/bsitest: $(OBJ_DIR)/CSVBusSystemIndexerTest.o $(OBJ_DIR)/BusSystemIndexer.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/StringDataSource.o  $(OBJ_DIR)/DSVReader.o
	$(CXX) -o $(BIN_DIR)/bsitest $(OBJ_DIR)/CSVBusSystemIndexerTest.o $(OBJ_DIR)/BusSystemIndexer.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/StringDataSource.o  $(OBJ_DIR)/DSVReader.o $(LDFLAGS)

$(OBJ_DIR)/CSVBusSystemIndexerTest.o: $(TESTSRC_DIR)/CSVBusSystemIndexerTest.cpp
	$(CXX) -o $(OBJ_DIR)/CSVBusSystemIndexerTest.o $(CXXFLAG) -c $(TESTSRC_DIR)/CSVBusSystemIndexerTest.cpp

$(OBJ_DIR)/BusSystemIndexer.o: $(SRC_DIR)/BusSystemIndexer.cpp $(INC_DIR)/BusSystemIndexer.h
	$(CXX) -o $(OBJ_DIR)/BusSystemIndexer.o $(CXXFLAG) -c $(SRC_DIR)/BusSystemIndexer.cpp


tptest: $(BIN_DIR)/tptest
	$(BIN_DIR)/tptest

$(OBJ_DIR)/GeographicUtils.o: $(SRC_DIR)/GeographicUtils.cpp
	$(CXX) -o $(OBJ_DIR)/GeographicUtils.o $(CXXFLAG) -c $(SRC_DIR)/GeographicUtils.cpp

$(BIN_DIR)/tptest: $(OBJ_DIR)/CSVOSMTransportationPlannerTest.o $(OBJ_DIR)/DijkstraTransportationPlanner.o $(OBJ_DIR)/DijkstraPathRouter.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/BusSystemIndexer.o $(OBJ_DIR)/GeographicUtils.o
	$(CXX) -o $(BIN_DIR)/tptest $(OBJ_DIR)/CSVOSMTransportationPlannerTest.o $(OBJ_DIR)/DijkstraTransportationPlanner.o $(OBJ_DIR)/DijkstraPathRouter.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/BusSystemIndexer.o $(OBJ_DIR)/GeographicUtils.o $(LDFLAGS)

$(OBJ_DIR)/CSVOSMTransportationPlannerTest.o: $(TESTSRC_DIR)/CSVOSMTransportationPlannerTest.cpp
	$(CXX) -o $(OBJ_DIR)/CSVOSMTransportationPlannerTest.o $(CXXFLAG) -c $(TESTSRC_DIR)/CSVOSMTransportationPlannerTest.cpp

$(OBJ_DIR)/DijkstraTransportationPlanner.o: $(SRC_DIR)/DijkstraTransportationPlanner.cpp $(INC_DIR)/DijkstraTransportationPlanner.h
	$(CXX) -o $(OBJ_DIR)/DijkstraTransportationPlanner.o $(CXXFLAG) -c $(SRC_DIR)/DijkstraTransportationPlanner.cpp

tpcltest: $(BIN_DIR)/tpcltest
	$(BIN_DIR)/tpcltest

$(BIN_DIR)/tpcltest: $(OBJ_DIR)/TPCommandLineTest.o $(OBJ_DIR)/TransportationPlannerCommandLine.o $(OBJ_DIR)/DijkstraTransportationPlanner.o $(OBJ_DIR)/DijkstraPathRouter.o $(OBJ_DIR)/GeographicUtils.o $(OBJ_DIR)/BusSystemIndexer.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/DSVWriter.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringUtils.o
	$(CXX) -o $(BIN_DIR)/tpcltest $(OBJ_DIR)/TPCommandLineTest.o $(OBJ_DIR)/TransportationPlannerCommandLine.o $(OBJ_DIR)/DijkstraTransportationPlanner.o $(OBJ_DIR)/DijkstraPathRouter.o $(OBJ_DIR)/GeographicUtils.o $(OBJ_DIR)/BusSystemIndexer.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/DSVWriter.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringUtils.o $(LDFLAGS)

$(OBJ_DIR)/TPCommandLineTest.o: $(TESTSRC_DIR)/TPCommandLineTest.cpp
	$(CXX) -o $(OBJ_DIR)/TPCommandLineTest.o $(CXXFLAG) -c $(TESTSRC_DIR)/TPCommandLineTest.cpp

$(OBJ_DIR)/TransportationPlannerCommandLine.o: $(SRC_DIR)/TransportationPlannerCommandLine.cpp $(INC_DIR)/TransportationPlannerCommandLine.h
	$(CXX) -o $(OBJ_DIR)/TransportationPlannerCommandLine.o $(CXXFLAG) -c $(SRC_DIR)/TransportationPlannerCommandLine.cpp


kmltest: $(BIN_DIR)/kmltest
	$(BIN_DIR)/kmltest

$(BIN_DIR)/kmltest: $(OBJ_DIR)/KMLTest.o  $(OBJ_DIR)/KMLWriter.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/XMLWriter.o $(OBJ_DIR)/StringUtils.o
	$(CXX) -o $(BIN_DIR)/kmltest $(OBJ_DIR)/KMLTest.o $(OBJ_DIR)/KMLWriter.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/XMLWriter.o $(OBJ_DIR)/StringUtils.o $(LDFLAGS)

$(OBJ_DIR)/KMLTest.o: $(TESTSRC_DIR)/KMLTest.cpp
	$(CXX) -o $(OBJ_DIR)/KMLTest.o $(CXXFLAG) -c $(TESTSRC_DIR)/KMLTest.cpp

$(OBJ_DIR)/KMLWriter.o: $(SRC_DIR)/KMLWriter.cpp $(INC_DIR)/KMLWriter.h
	$(CXX) -o $(OBJ_DIR)/KMLWriter.o $(CXXFLAG) -c $(SRC_DIR)/KMLWriter.cpp


transplanner: $(BIN_DIR)/transplanner
	$(BIN_DIR)/transplanner

$(OBJ_DIR)/StandardDataSink.o: $(SRC_DIR)/StandardDataSink.cpp $(INC_DIR)/StandardDataSink.h $(INC_DIR)/DataSink.h
	$(CXX) -o $(OBJ_DIR)/StandardDataSink.o $(CXXFLAG) -c $(SRC_DIR)/StandardDataSink.cpp

$(OBJ_DIR)/StandardDataSource.o: $(SRC_DIR)/StandardDataSource.cpp $(INC_DIR)/StandardDataSource.h $(INC_DIR)/DataSink.h
	$(CXX) -o $(OBJ_DIR)/StandardDataSource.o $(CXXFLAG) -c $(SRC_DIR)/StandardDataSource.cpp

$(OBJ_DIR)/FileDataFactory.o: $(SRC_DIR)/FileDataFactory.cpp $(INC_DIR)/FileDataFactory.h $(OBJ_DIR)/FileDataSink.o $(OBJ_DIR)/FileDataSource.o
	$(CXX) -o $(OBJ_DIR)/FileDataFactory.o $(CXXFLAG) -c $(SRC_DIR)/FileDataFactory.cpp $(OBJ_DIR)/FileDataSink.o $(OBJ_DIR)/FileDataSource.o

$(OBJ_DIR)/FileDataSink.o: $(SRC_DIR)/FileDataSink.cpp $(INC_DIR)/FileDataSink.h $(INC_DIR)/DataSink.h
	$(CXX) -o $(OBJ_DIR)/FileDataSink.o $(CXXFLAG) -c $(SRC_DIR)/FileDataSink.cpp

$(OBJ_DIR)/FileDataSource.o: $(SRC_DIR)/FileDataSource.cpp $(INC_DIR)/FileDataSource.h $(INC_DIR)/DataSource.h
	$(CXX) -o $(OBJ_DIR)/FileDataSource.o $(CXXFLAG) -c $(SRC_DIR)/FileDataSource.cpp

$(BIN_DIR)/transplanner: $(OBJ_DIR)/transplanner.o $(TO_LINK)
	$(CXX) -o $(BIN_DIR)/transplanner $(OBJ_DIR)/transplanner.o $(TO_LINK) $(LDFLAGS)

$(OBJ_DIR)/transplanner.o: $(SRC_DIR)/transplanner.cpp
	$(CXX) -o $(OBJ_DIR)/transplanner.o $(CXXFLAG) -c $(SRC_DIR)/transplanner.cpp


speedtest: $(BIN_DIR)/speedtest
	$(BIN_DIR)/speedtest --verbose 10

$(OBJ_DIR)/StandardErrorDataSink.o: $(SRC_DIR)/StandardErrorDataSink.cpp $(INC_DIR)/StandardErrorDataSink.h
	$(CXX) -o $(OBJ_DIR)/StandardErrorDataSink.o $(CXXFLAG) -c $(SRC_DIR)/StandardErrorDataSink.cpp

$(BIN_DIR)/speedtest: $(OBJ_DIR)/speedtest.o $(TO_LINK)
	$(CXX) -o $(BIN_DIR)/speedtest $(OBJ_DIR)/speedtest.o $(TO_LINK) $(LDFLAGS)

$(OBJ_DIR)/speedtest.o: $(SRC_DIR)/speedtest.cpp
	$(CXX) -o $(OBJ_DIR)/speedtest.o $(CXXFLAG) -c $(SRC_DIR)/speedtest.cpp


directories:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)

.PHONY: clean

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

