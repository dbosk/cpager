/* $Id$
 * Author:	Daniel Bosk <daniel.bosk@miun.se>
 * Date:	15 Dec 2012
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

size_t sz_memory;

size_t sz_pagetable;
pentry_t *pagetable;

int
sys_init( size_t mem, size_t ptable )
{
	/* set up the page table */
	if ( sys_setnpages(ptable) == ERROR ) {
		sys_deinit();
		return ERROR;
	}
	/* set the number of available frames */
	else if ( sys_setnframes(mem) == ERROR ) {
		sys_deinit();
		return ERROR;
	}
	return OK;
}

void
sys_deinit( void )
{
	sz_memory = sz_pagetable = 0;
	if ( pagetable != NULL )
		free( pagetable );
	pagetable = NULL;
}

int
sys_setnpages( size_t n )
{
	int i;

	if ( n < 1 )
		return ERROR;

	sz_pagetable = n;
	if ( pagetable != NULL )
		free( pagetable );
	pagetable = calloc( sz_pagetable, sizeof(pentry_t) );

	/* at startup there will be no valid pages */
	for ( i = 0; i < sz_pagetable; i++)
		pagetable[i].valid = 0;
	
	return (pagetable != NULL);
}

int
sys_setnframes( size_t n )
{
	if ( n < 1 )
		return ERROR;
	sz_memory = n;
	return OK;
}
