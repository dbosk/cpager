/* $Id$
 * Author:	Daniel Bosk <daniel.bosk@miun.se>
 * Date:	5 December 2012
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

#include "mem.h"
#include "memalg.h"
#include "sys.h"

static memalg_t memalg = memalg_fifo;

void
mem_setalg( memalg_t alg )
{
	memalg = alg;
}

frame_t
mem_read( page_t page )
{
	/* as the page number is an index to the page table,
	 * make sure we do not end up outside the page table. */
	if ( page >= sz_pagetable )
		return FRAME_ERR;
	/* if the valid bit in the page table is unset make the
	 * paging algorithm try to swap the page in */
	else if ( ! pagetable[page].valid &&
			  memalg( page, pagetable ) == ERROR )
		return FRAME_ERR;

	pagetable[page].referenced = 1;
	return pagetable[page].frame;
}

frame_t
mem_write( page_t page )
{
	/* as the page number is an index to the page table,
	 * make sure we do not end up outside the page table. */
	if ( page >= sz_pagetable )
		return FRAME_ERR;
	/* if the valid bit in the page table is unset make the
	 * paging algorithm try to swap the page in */
	else if ( ! pagetable[page].valid &&
			  memalg( page, pagetable ) == ERROR )
		return FRAME_ERR;

	pagetable[page].modified = 1;
	pagetable[page].referenced = 1;

	return pagetable[page].frame;
}
