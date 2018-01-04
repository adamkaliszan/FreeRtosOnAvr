/*! \file vt100.c \brief VT100 terminal function library. */
//*****************************************************************************
//
// File Name	: 'vt100.c'
// Title		: VT100 terminal function library
// Author		: Pascal Stang - Copyright (C) 2002
// Created		: 2002.08.27
// Revised		: 2002.08.27
// Version		: 0.1
// Target MCU	: Atmel AVR Series
// Editor Tabs	: 4
//
// NOTE: This code is currently below version 1.0, and therefore is considered
// to be lacking in some functionality or documentation, or may not be fully
// tested.  Nonetheless, you can expect most functions to work.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef WIN32
	#include <avr/io.h>
	#include <avr/interrupt.h>
	#include <avr/pgmspace.h>
#endif

#include "main.h"
#include "vt100.h"
#include <stdio.h>

// Program ROM constants

// Global variables

// Functions
void vt100Init(cmdState_t *state)
{
  // initializes terminal to "power-on" settings
  // ESC c

 fprintf_P(state->myStdInOut, "\x1B\x63");
}

void vt100ClearScreen(cmdState_t *state)
{
  // ESC [ 2 J
  fprintf_P(state->myStdInOut, "\x1B[2J");
}

void vt100SetAttr(uint8_t attr, cmdState_t *state)
{
  // ESC [ Ps m
  fprintf_P(state->myStdInOut, "\x1B[%dm",attr);
}

void vt100SetCursorMode(uint8_t visible, cmdState_t *state)
{
  if(visible)
  // ESC [ ? 25 h
    fprintf_P(state->myStdInOut, "\x1B[?25h");
  else
  // ESC [ ? 25 l
    fprintf_P(state->myStdInOut, "\x1B[?25l");
}

void vt100SetCursorPos(uint8_t line, uint8_t col, cmdState_t *state)
{
  // ESC [ Pl ; Pc H
  fprintf_P(state->myStdInOut, "\x1B[%d;%dH",line,col);
}
