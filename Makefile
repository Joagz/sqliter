CC      = gcc
CFLAGS  = -Wall -D_FORTIFY_SOURCE=2 -Werror=implicit-function-declaration 
LFLAGS	= -L ./libs/sqlite3/ -lsqlite3
BINS    = sqliter.o commons.o libtdict.o
MAIN    = sqliter
ROOT    = *
# ============= GENERIC ACTIONS (BETTER KEEP) =============
all: $(MAIN)
$(MAIN): $(BINS)
	$(CC) $(CFLAGS) -o $(MAIN) $(BINS) $(LFLAGS)
%.o: $(ROOT)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<
clean: 
	rm -f $(BINS) $(MAIN)
