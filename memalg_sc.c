/* $Id$
 * Author:	Daniel Bosk <daniel.bosk@miun.se>
 * Date:	17 Dec 2012
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

#include <stdlib.h>
#include <stdio.h>
#include "free.h"
#include "types.h"
#include "swap.h"
#include "sys.h"

static page_t *queue = NULL;
static size_t qsize = 0;
static int qhead = 0;

int
memalg_sc( page_t p, pagetable_t pt )
{
	/* do some setup the first time this function is run */
	if ( queue == NULL ) {
		qsize = sz_memory;
		queue = calloc( qsize, sizeof(page_t) );
		if ( queue == NULL )
			return ERROR;
		qhead = 0;
	}

	/* if the page is valid, do nothing as no paging is required */
	if ( pt[p].valid )
		return OK;

	fprintf(stdout, "page %lli generated page fault\n", p);

	/* if we have free frames, use one of these */
	if ( free_total() > 0 ) {
		pt[p].frame = free_getframe();
		if ( pt[p].frame == FRAME_ERR )
			return ERROR;
		fprintf(stdout, "allocated free frame %lli to page %lli\n",
				pt[p].frame, p);
	}
	/* otherwise swap out one page */
	else {
		while ( pt[queue[qhead]].valid &&
				pt[queue[qhead]].referenced ) {
			pt[queue[qhead]].referenced = 0;
			qhead = ( qhead + 1 ) % qsize;
		}
		if ( !pt[queue[qhead]].valid )
			return ERROR;

		pt[queue[qhead]].valid = 0;
		swap_out(queue[qhead]);
		
		/* allocate the newly freed frame */
		pt[p].frame = pt[queue[qhead]].frame;
	}

	/* update the queue */
	queue[qhead] = p;
	qhead = ( qhead + 1 ) % qsize;

	/* actually swap in the page */
	swap_in(p);
	pt[p].valid = 1;
	pt[p].modified = 0;
	pt[p].referenced = 0;

	return OK;
}
