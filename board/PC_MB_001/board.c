/*
 * This file is part of the PolyController firmware source code.
 * Copyright (C) 2011 Chris Boot.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#include <avr/io.h>
#include "board.h"

/*
 *	Port A:
 *	 0-7: (O,L)	Diagnostic outputs
 *
 *	Port B:
 *	 0: (I,PU)	<NC>
 *	 1: (O,L)	TP10/AVR_CLKO
 *	 2: (O,H)	!MEM_CS
 *	 3: (I)		!ETH_INT
 *	 4: (O,H)	!ETH_CS
 *	 5:	(O,L)	MOSI
 *	 6: (O,L)	MISO
 *	 7: (O,L)	SCK
 *
 *	Port C:
 *	 0:	(I)		I2C_SCL
 *	 1: (I)		I2C_SDA
 *	 2: (O,H)	OUT_OE
 *	 3: (O,L)	OUT_LATCH
 *	 4: (O,L)	OUT_DATA
 *	 5: (O,L)	OUT_CLOCK
 *	 6: (I,PU)	RTC_CLK
 *	 7: (I,PU)	<NC>
 *
 *	Port D:
 *	 0: (I,PU)	CON_RX
 *	 1: (O,L)	CON_TX
 *	 2: (I,PU)	SER_RX
 *	 3: (O,L)	SER_TX
 *	 4:	(I,PU)	GPIO_1
 *	 5:	(I,PU)	GPIO_2
 *	 6:	(I,PU)	GPIO_3
 *	 7:	(I,PU)	GPIO_4
 */

#if !__AVR_ATmega1284P__
#error Wrong MCU type selected!
#endif

// This gets called to set up the IO pins
void board_init(void) {
	/* Port A */
	DDRA = 0xff; // all outputs
	PORTA = 0x00; // all pulled low

	/* Port B */
	DDRB =
		_BV(PINB1) |
		_BV(PINB2) |
		_BV(PINB4) |
		_BV(PINB5) |
		_BV(PINB6) |
		_BV(PINB7);
	PORTB =
		_BV(PINB0) |
		_BV(PINB2) |
		_BV(PINB4);

	/* Port C */
	DDRC =
		_BV(PINC2) |
		_BV(PINC3) |
		_BV(PINC4) |
		_BV(PINC5);
	PORTC =
		_BV(PINC2) |
		_BV(PINC6) |
		_BV(PINC7);

	/* Port D */
	DDRD =
		_BV(PIND1) |
		_BV(PIND3);
	PORTD =
		_BV(PIND0) |
		_BV(PIND2) |
		_BV(PIND4) |
		_BV(PIND5) |
		_BV(PIND6) |
		_BV(PIND7);
}
