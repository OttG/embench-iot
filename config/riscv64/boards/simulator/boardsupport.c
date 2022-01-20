/* Copyright (C) 2017 Embecosm Limited and University of Bristol

   Contributor Graham Markall <graham.markall@embecosm.com>

   This file is part of Embench and was formerly part of the Bristol/Embecosm
   Embedded Benchmark Suite.

   SPDX-License-Identifier: GPL-3.0-or-later */
#include <stdio.h>
#include "encoding.h"

int cycles, instret;

void
initialise_board ()
{
}

void __attribute__ ((noinline)) __attribute__ ((externally_visible))
start_trigger ()
{
  cycles  = -read_csr(0xb00);
  instret = -read_csr(0xb02);
}

void __attribute__ ((noinline)) __attribute__ ((externally_visible))
stop_trigger ()
{
  cycles  += read_csr(0xb00);
  instret += read_csr(0xb02);
  printf("tot_cycles = %d\ninstret = %d\n", cycles, instret);
}
