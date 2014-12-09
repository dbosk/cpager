PROGRAM_FILES=	pager.c types.h sys.h sys.c mem.h mem.c swap.h swap.c
PROGRAM_FILES+= free.h free.c
PROGRAM_FILES+=	memalg.h memalg_fifo.c memalg_sc.c memalg_esc.c memalg_arb.c
SOURCES=		${PROGRAM_FILES:%.h=}
CFLAGS=			-Wall -g

# Depends and recipe for pager program
pager: ${SOURCES:.c=.o}
	${CC} -o $@ $^

clean:
	${RM} ${SOURCES:.c=.o} pager
