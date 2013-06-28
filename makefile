all: KeccakReference KeccakReference32BI KeccakOptimized64 KeccakInplace32BI

SOURCES_COMMON = \
    Constructions/KeccakDuplex.c \
    Constructions/KeccakSponge.c \
    Modes/KeccakHash.c \
    Tests/genKAT.c \
    Tests/main.c \
    Tests/testDuplex.c \
    Tests/testPermutationAndStateMgt.c \
    Tests/testSponge.c

SOURCES_REFERENCE = \
    $(SOURCES_COMMON) \
    KeccakF-1600/Reference/KeccakF-1600-reference.c \
    Tests/displayIntermediateValues.c

SOURCES_REFERENCE32BI = \
    $(SOURCES_COMMON) \
    KeccakF-1600/Reference/KeccakF-1600-reference32BI.c \
    Tests/displayIntermediateValues.c

SOURCES_OPTIMIZED = \
    $(SOURCES_COMMON) \
    Tests/timing.c

SOURCES_OPTIMIZED_64 = \
    $(SOURCES_OPTIMIZED) \
    KeccakF-1600/Optimized/KeccakF-1600-opt64.c

SOURCES_INPLACE32BI = \
    $(SOURCES_OPTIMIZED) \
    KeccakF-1600/Optimized/KeccakF-1600-inplace32BI.c

HEADERS_COMMON = \
    Constructions/KeccakDuplex.h \
    Constructions/KeccakSponge.h \
    KeccakF-1600/KeccakF-1600-interface.h \
    Modes/KeccakHash.h

HEADERS_REFERENCE = \
    $(HEADERS_COMMON) \
    KeccakF-1600/Reference/KeccakF-1600-reference.h \
    Tests/displayIntermediateValues.h \
    Tests/genKAT.h \
    Tests/testDuplex.h \
    Tests/testPermutationAndStateMgt.h \
    Tests/testSponge.h

HEADERS_REFERENCE32BI = $(HEADERS_REFERENCE)

HEADERS_OPTIMIZED = \
    $(HEADERS_COMMON) \
    Common/brg_endian.h \
    Tests/timing.h

HEADERS_OPTIMIZED_64 = \
    $(HEADERS_OPTIMIZED) \
    KeccakF-1600/Optimized/KeccakF-1600-opt64-settings.h \
    KeccakF-1600/Optimized/KeccakF-1600-unrolling.macros \
    KeccakF-1600/Optimized/KeccakF-1600-64.macros

HEADERS_INPLACE32BI = \
    $(HEADERS_OPTIMIZED)

BINDIR_REFERENCE = bin/reference

$(BINDIR_REFERENCE):
	mkdir -p $(BINDIR_REFERENCE)

BINDIR_REFERENCE32BI = bin/reference32bi

$(BINDIR_REFERENCE32BI):
	mkdir -p $(BINDIR_REFERENCE32BI)

BINDIR_OPTIMIZED_64 = bin/optimized64

$(BINDIR_OPTIMIZED_64):
	mkdir -p $(BINDIR_OPTIMIZED_64)

BINDIR_INPLACE32BI = bin/inplace32BI

$(BINDIR_INPLACE32BI):
	mkdir -p $(BINDIR_INPLACE32BI)

OBJECTS_REFERENCE = $(addprefix $(BINDIR_REFERENCE)/, $(notdir $(patsubst %.c,%.o,$(SOURCES_REFERENCE))))

OBJECTS_REFERENCE32BI = $(addprefix $(BINDIR_REFERENCE32BI)/, $(notdir $(patsubst %.c,%.o,$(SOURCES_REFERENCE32BI))))

OBJECTS_OPTIMIZED_64 = $(addprefix $(BINDIR_OPTIMIZED_64)/, $(notdir $(patsubst %.c,%.o,$(SOURCES_OPTIMIZED_64))))

OBJECTS_INPLACE32BI = $(addprefix $(BINDIR_INPLACE32BI)/, $(notdir $(patsubst %.c,%.o,$(SOURCES_INPLACE32BI))))

CC = gcc

CFLAGS_REFERENCE = -DKeccakReference -O

CFLAGS_REFERENCE32BI = $(CFLAGS_REFERENCE) -DKeccakReference32BI

CFLAGS_OPTIMIZED_32 = -fomit-frame-pointer -O3 -g0 -march=native -mtune=native

CFLAGS_OPTIMIZED_64 = -fomit-frame-pointer -O3 -g0 -march=native -mtune=native -m64

VPATH = Common/ Constructions/ KeccakF-1600/ KeccakF-1600/Optimized/ KeccakF-1600/Reference/ Modes/ Tests/

INCLUDES = -ICommon/ -IConstructions/ -IKeccakF-1600/ -IKeccakF-1600/Optimized/ -IKeccakF-1600/Reference/ -IModes/ -ITests/

$(BINDIR_REFERENCE)/%.o:%.c $(HEADERS_REFERENCE)
	$(CC) $(INCLUDES) $(CFLAGS_REFERENCE) -c $< -o $@

$(BINDIR_REFERENCE32BI)/%.o:%.c $(HEADERS_REFERENCE32BI)
	$(CC) $(INCLUDES) $(CFLAGS_REFERENCE32BI) -c $< -o $@

$(BINDIR_OPTIMIZED_64)/%.o:%.c $(HEADERS_OPTIMIZED_64)
	$(CC) $(INCLUDES) $(CFLAGS_OPTIMIZED_64) -c $< -o $@

$(BINDIR_INPLACE32BI)/%.o:%.c $(HEADERS_INPLACE32BI)
	$(CC) $(INCLUDES) $(CFLAGS_OPTIMIZED_32) -c $< -o $@

.PHONY: KeccakReference KeccakReference32BI KeccakOptimized64 KeccakInplace32BI

KeccakReference: bin/KeccakReference

bin/KeccakReference:  $(BINDIR_REFERENCE) $(OBJECTS_REFERENCE)  $(HEADERS_REFERENCE)
	$(CC) $(CFLAGS_REFERENCE) -o $@ $(OBJECTS_REFERENCE)

KeccakReference32BI: bin/KeccakReference32BI

bin/KeccakReference32BI:  $(BINDIR_REFERENCE32BI) $(OBJECTS_REFERENCE32BI)  $(HEADERS_REFERENCE32BI)
	$(CC) $(CFLAGS_REFERENCE32BI) -o $@ $(OBJECTS_REFERENCE32BI)

KeccakOptimized64: bin/KeccakOptimized64

bin/KeccakOptimized64:  $(BINDIR_OPTIMIZED_64) $(OBJECTS_OPTIMIZED_64)  $(HEADERS_OPTIMIZED_64)
	$(CC) $(CFLAGS_OPTIMIZED_64) -o $@ $(OBJECTS_OPTIMIZED_64)

KeccakInplace32BI: bin/KeccakInplace32BI

bin/KeccakInplace32BI:  $(BINDIR_INPLACE32BI) $(OBJECTS_INPLACE32BI)  $(HEADERS_INPLACE32BI)
	$(CC) $(CFLAGS_INPLACE32BI) -o $@ $(OBJECTS_INPLACE32BI)

.PHONY: clean

clean:
	rm -rf bin/
