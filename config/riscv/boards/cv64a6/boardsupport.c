/* Copyright (C) 2017 Embecosm Limited and University of Bristol

   Contributor Graham Markall <graham.markall@embecosm.com>

   This file is part of Embench and was formerly part of the Bristol/Embecosm
   Embedded Benchmark Suite.

   SPDX-License-Identifier: GPL-3.0-or-later */

#include <support.h>
#include "encoding.h"

#define INHIBIT_MASK 0x1F8
int cycles, instret, icache_miss, dcache_miss, load_event, store_event, misprediction, stall_issue;

void
initialise_board ()
{

  write_csr(0x320, INHIBIT_MASK); // Inhibit the counters.
  write_csr(mhpmevent3, 1); // l1 I-cache miss
  write_csr(mhpmevent4, 2); // l1 d-cache miss
  write_csr(mhpmevent5, 5); // load event
  write_csr(mhpmevent6, 6); // store event
  write_csr(mhpmevent7, 10); // branch misprediction
  write_csr(mhpmevent8, 26); // stall_issue
}

void __attribute__ ((noinline)) __attribute__ ((externally_visible))
start_trigger ()
{
  icache_miss   = -read_csr(mhpmcounter3);
  dcache_miss   = -read_csr(mhpmcounter4);
  load_event    = -read_csr(mhpmcounter5);
  store_event   = -read_csr(mhpmcounter6);
  misprediction = -read_csr(mhpmcounter7);
  stall_issue   = -read_csr(mhpmcounter8);
  cycles  = -rdcycle();
  instret = -rdinstret();
  write_csr(0x320, 0); // Inhibit the counters.
}

void __attribute__ ((noinline)) __attribute__ ((externally_visible))
stop_trigger ()
{
  write_csr(0x320, INHIBIT_MASK);
  cycles  += rdcycle();
  instret += rdinstret();
  icache_miss   += read_csr(mhpmcounter3);
  dcache_miss   += read_csr(mhpmcounter4);
  load_event    += read_csr(mhpmcounter5);
  store_event   += read_csr(mhpmcounter6);
  misprediction += read_csr(mhpmcounter7);
  stall_issue   += read_csr(mhpmcounter8);

  printf("tot_cycles = %d\ninstret = %d\n", cycles, instret);
  printf("icache_miss = %d\ndcache_miss = %d\nload_event =%d\nstore_event =%d\nmisprediction =%d\nstall_issue =%d\n", icache_miss, dcache_miss, load_event, store_event, misprediction, stall_issue);
}
