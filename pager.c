/* $Id$
 * Author:	Daniel Bosk <daniel.bosk@miun.se>
 * Date:	6 December 2012
 */

/*
 * Copyright (c) 2012, Daniel Bosk <daniel.bosk@miun.se>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 *  * Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/* Thanks to Daniel Forsell <daniel@oblivioncreations.se> for spotting
 * some typos. */

#include <stdio.h>
#include <stddef.h>
#include <ctype.h>
#include "free.h"
#include "mem.h"
#include "memalg.h"
#include "sys.h"

int get_next_reference( char *op, page_t *page );

int
main( const int argc, const char ** const argv )
{
	page_t page;
	frame_t frame;
	char op;

	/* this is the number of available frames in the system,
	 * i.e. the limit of physical memory. */
	size_t nframes = 5;
	/* this is the number of available pages in the system,
	 * i.e. the limit of virtual memory. */
	size_t npages = 10;

	/* initialize "hardware" */
	if ( sys_init( nframes, npages ) < 0 ) {
		perror( argv[0] );
		return -1;
	}
	/* set up free memory bitmap */
	else if ( free_init() < 0 ) {
		perror( argv[0] );
		return -1;
	}

	/* set the paging algorithm to be used, see memalg.h for
	 * alternative algorithms. */
	mem_setalg( memalg_fifo );

	/* main loop reading page numbers from stdin and
	 * making memory references. */
	while ( get_next_reference( &op, &page ) > 0 ) {
		switch ( op ) {
		case 'r':
		case 'R':
			frame = mem_read( page );
			break;
		case 'w':
		case 'W':
			frame = mem_write( page );
			break;
		default:
			fprintf( stdout, "illegal memory operation: %c\n", op );
			return -1;
		}
		if ( frame == FRAME_ERR ) {
			/* there was an error, this simulates an abort trap
			 * generated by the MMU when the process tries to
			 * access nonexistent memory. */
			fprintf(stdout,
					"page %lli generated abort trap: process killed\n",
					page);
			return -1;
		}
		fprintf( stdout, "page %lli mapped to frame %lli\n", page, frame );
	}

	return 0;
}

/* XXX what happens with this string "1 w 2"? */
int
get_next_reference( char *op, page_t *page )
{
	char c;

	/* ignore all but alphanumericals. */
	while ( !isalnum( c = fgetc( stdin ) ) && c != EOF );
	if ( c == EOF )
		return -1;
	ungetc( c, stdin );

	if ( isalpha( c ) ) {
		return fscanf( stdin, "%c%lli", op, page );
	}
	/* now we have only a digit, to be backwards compatible and compatible
	 * with Silberschatz et al. the default operation is read. */
	*op = 'r';
	return fscanf( stdin, "%lli", page );
}
