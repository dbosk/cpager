/* $Id$
 * Author:	Daniel Bosk <daniel.bosk@miun.se>
 * Date:	14 Dec 2012
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

#ifndef _MEMALG_H_
#define _MEMALG_H_

#include "types.h"

/*
 * The following references are used in the comments in this file for
 * the various algorithms covered:
 *
 * 	[SGG09] Silberschatz, Galvin, Gagne.  Operating System Concepts.
 * 		8th edition.  John Wiley & Sons.  2009.
 *
 */

/* first-in first-out algorithm [SGG09, ch. 9.4.2] */
int memalg_fifo(page_t p, pagetable_t pt);

/* second-chance algorithm [SGG09, ch. 9.4.5.2] */
int memalg_sc(page_t p, pagetable_t pt);

/* enhanced second-chance algorithm [SGG09, ch. 9.4.5.3]
 * NOT IMPLEMENTED! */
int memalg_esc(page_t p, pagetable_t pt);

/* additional-reference-bits algorithm [SGG09, ch. 9.4.5.1]
 * NOT IMPLEMENTED! */
int memalg_arb(page_t p, pagetable_t pt);

#endif
