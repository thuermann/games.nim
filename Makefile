#
# $Id: Makefile,v 1.1 2013/01/09 10:43:59 urs Exp $
#

RM = rm -f

programs = nim

.PHONY: all
all: $(programs)

.PHONY: clean
clean:
	$(RM) $(programs) *.o core
