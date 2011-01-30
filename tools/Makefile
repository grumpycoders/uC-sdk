CC = gcc
LD = gcc
CPPFLAGS = -O3

TARGETS = mkromfs
MKROMFS_SRCS = mkromfs.c
MKROMFS_OBJS = $(addsuffix .o, $(basename $(MKROMFS_SRCS)))

all: $(TARGETS)

mkromfs: $(MKROMFS_OBJS)
	$(LD) -o mkromfs $(MKROMFS_OBJS)

clean:
	rm -rf $(MKROMFS_OBJS) $(TARGETS)
