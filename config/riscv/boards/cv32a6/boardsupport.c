/* Copyright (C) 2017 Embecosm Limited and University of Bristol

   Contributor Graham Markall <graham.markall@embecosm.com>

   This file is part of Embench and was formerly part of the Bristol/Embecosm
   Embedded Benchmark Suite.

   SPDX-License-Identifier: GPL-3.0-or-later */

#include <support.h>
#include "encoding.h"

#define INHIBIT_MASK 0x7FF8
int cycles, instret, icache_miss, dcache_miss, load_event, store_event, misprediction, stall_issue;
int branch_event, call_event, return_event, branch_mispredict, ras_mispredict, btb_mispredict;
void
initialise_board ()
{

  write_csr(0x320, INHIBIT_MASK); // Inhibit the counters.
  write_csr(mhpmevent3, 1); // l1 I-cache miss
  write_csr(mhpmevent4, 2); // l1 d-cache miss
  write_csr(mhpmevent5, 5); // load event
  write_csr(mhpmevent6, 6); // store event
  write_csr(mhpmevent7, 10); // branch misprediction
  write_csr(mhpmevent8, 22); // stall_issue
  write_csr(mhpmevent9, 23);  // Brach Mispredict
  write_csr(mhpmevent10, 24); // RAS Mispredict
  write_csr(mhpmevent11, 25); // BTB Mispredict
  write_csr(mhpmevent12, 9);  // Branch Event
  write_csr(mhpmevent13, 12); // Call Event
  write_csr(mhpmevent14, 13); // Returns
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
  branch_mispredict = -read_csr(mhpmcounter9);
  ras_mispredict    = -read_csr(mhpmcounter10);
  btb_mispredict    = -read_csr(mhpmcounter11);
  branch_event      = -read_csr(mhpmcounter12);
  call_event        = -read_csr(mhpmcounter13);
  return_event      = -read_csr(mhpmcounter14);
  cycles  = -rdcycle();
  instret = -rdinstret();
  write_csr(0x320, 0); // Enable the counters.
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
  branch_mispredict += read_csr(mhpmcounter9);
  ras_mispredict  += read_csr(mhpmcounter10);
  btb_mispredict  += read_csr(mhpmcounter11);
  branch_event    += read_csr(mhpmcounter12);
  call_event      += read_csr(mhpmcounter13);
  return_event    += read_csr(mhpmcounter14);

  printf("tot_cycles = %d\ninstret = %d\n", cycles, instret);
  printf("icache_miss = %d\ndcache_miss = %d\nload_event =%d\nstore_event =%d\nmisprediction =%d\nstall_issue =%d\nbranch_event =%d\ncall_event =%d\nretrun_event =%d\nbranch_mispredict =%d\nras_mispredict =%d\nbtb_mispredict =%d\n", icache_miss, dcache_miss, load_event, store_event, misprediction, stall_issue, branch_event, call_event, return_event, branch_mispredict, ras_mispredict, btb_mispredict);
}
