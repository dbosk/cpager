/* $Id$
 * Author:	Daniel Bosk <daniel.bosk@miun.se>
 * Date:	16 Dec 2012
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
#include "types.h"
#include "sys.h"

static size_t sz_freemap = 0;
static int *freemap = NULL;
static size_t nfree = 0;

int
free_init( void )
{
	frame_t i;

	sz_freemap = sz_memory;
	freemap = calloc( sz_freemap, sizeof(int) );
	if ( freemap == NULL )
		return -1;

	/* at startup all frames will be free */
	nfree = sz_freemap;
	for (i=0; i<sz_freemap; i++)
		freemap[i] = 1;

	return 0;
}

void
free_deinit( void )
{
	sz_freemap = 0;
	if ( freemap != NULL )
		free( freemap );
	freemap = NULL;
}

size_t
free_total( void )
{
	return nfree;
}

frame_t
free_getframe( void )
{
	frame_t f;

	/* find the first free frame and return its number */
	for (f=0; f<sz_freemap; f++) {
		if ( freemap[f] == 1 ) {
			freemap[f] = 0;
			nfree--;
			return f;
		}
	}

	return FRAME_ERR;
}

void
free_release( frame_t f )
{
	if ( freemap[f] == 1 )
		return;

	freemap[f] = 1;
	nfree++;
}
