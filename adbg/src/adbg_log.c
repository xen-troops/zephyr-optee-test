// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * Copyright (c) 2023, EPAM Systems
 */

#include <stdlib.h>
#include <ctype.h>
#include <zephyr/kernel.h>
#include <adbg.h>
#include "security_utils_hex.h"
/******************************************************************************/
/*! @fn void Do_ADBG_Log( void* This_p, char* Format, ...)
 * @brief
 * @param [in] This_p
 * @param [in] Format
 * @param [in] ...
 * @return void
 */
/******************************************************************************/

void Do_ADBG_Log(const char *const Format, ...)
{
	va_list ap;

	va_start(ap, Format);
	vprintk(Format, ap);
	printk("\n");
	va_end(ap);
}

void Do_ADBG_HexLog(
	const void *const Buf_p,
	const size_t Size,
	const size_t Cols
	)
{
	const uint8_t *Data_p = Buf_p;
	size_t n = 0;

	for (n = 0; n < Size; n += Cols) {
		char HexBuffer[ADBG_STRING_LENGTH_MAX];
		char AsciiBuffer[ADBG_STRING_LENGTH_MAX / 3];
		size_t m, NumCols;

		(void)SecUtil_BufferToHex(Data_p + n, MIN(Cols, Size - n), NULL,
					  HexBuffer, sizeof(HexBuffer));
		NumCols = MIN(MIN(Cols, Size - n), sizeof(AsciiBuffer) - 1);
		for (m = 0; m < NumCols; m++) {
			int ch = Data_p[n + m];

			if (isprint(ch))
				AsciiBuffer[m] = (char)ch;
			else
				AsciiBuffer[m] = '.';
		}
		AsciiBuffer[m] = '\0';

		printk("  %-*s %s\n", (int)Cols * 3, HexBuffer, AsciiBuffer);
	}
}

/*************************************************************************
 * 6. Definitions of internal functions
 ************************************************************************/
