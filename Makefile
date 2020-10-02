CXX=g++
CXXFLAGS=
OBJS=src/chash/chash.o \
	src/chash/crc/CCRC16.o \
	src/chash/crc/CCRC32.o \
	src/chash/crc/CCRC64.o \
	src/chash/md/CMD5.o \
	src/chash/ripemd/CRipeMD160.o \
	src/chash/sha/CSHA256.o \
	src/chash/sha/CSHA384.o \
	src/chash/sha/CSHA512.o
TARGET=libchash.so

all: $(TARGET)

clean:
	rm -rf *.o
	rm -f $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) --shared -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) -fPIC -c $< -I src -o $@

