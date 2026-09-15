CC = gcc
CFLAGS = -Wall -Wextra
OBJS = rls.o perm.o sort.o listing.o

rls: $(OBJS)
	$(CC) $(CFLAGS) -o rls $(OBJS)

rls.o: rls.c fileinfo.h listing.h sort.h
perm.o: perm.c perm.h
sort.o: sort.c sort.h fileinfo.h
listing.o: listing.c listing.h fileinfo.h perm.h

clean:
	rm -f $(OBJS) rls
