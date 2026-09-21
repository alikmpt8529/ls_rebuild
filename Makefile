CC = gcc
CFLAGS = -Wall -Wextra -O2
OBJS = rls.o perm.o sort.o listing.o

.PHONY: rls clean docs docs-clean

rls: $(OBJS)
	$(CC) $(CFLAGS) -o rls $(OBJS)

rls.o: rls.c fileinfo.h listing.h sort.h
perm.o: perm.c perm.h
sort.o: sort.c sort.h fileinfo.h
listing.o: listing.c listing.h fileinfo.h perm.h

docs:
	@command -v doxygen >/dev/null || { echo "doxygen が必要です (brew install doxygen)"; exit 1; }
	doxygen Doxyfile

docs-clean:
	rm -rf docs

clean: docs-clean
	rm -f $(OBJS) rls
