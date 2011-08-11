/*
 * Copyright (c) 1996,1999 by Internet Software Consortium.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND INTERNET SOFTWARE CONSORTIUM DISCLAIMS
 * ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL INTERNET SOFTWARE
 * CONSORTIUM BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */
/*
 * These were shamelessly ripped from EGLIBC, then modified.
 */

#include "pton.h"
#include <string.h>

#define NS_INADDRSZ 4
#define NS_IN6ADDRSZ 16

#if !UIP_CONF_IPV6

int inet_pton(const char *src, uip_ip4addr_t *dst) {
	int saw_digit, octets, ch;
	uint8_t tmp[NS_INADDRSZ], *tp;

	saw_digit = 0;
	octets = 0;
	*(tp = tmp) = 0;

	while ((ch = *src++) != '\0') {

		if (ch >= '0' && ch <= '9') {
			uint16_t new = *tp * 10 + (ch - '0');

			if (saw_digit && *tp == 0)
				return 0;

			if (new > 255)
				return 0;

			*tp = new;

			if (!saw_digit) {
				if (++octets > 4)
					return 0;

				saw_digit = 1;
			}
		}
		else if (ch == '.' && saw_digit) {
			if (octets == 4)
				return 0;

			*++tp = 0;
			saw_digit = 0;
		}
		else {
			return 0;
		}
	}

	if (octets < 4)
		return 0;

	memcpy(dst, tmp, NS_INADDRSZ);
	return 1;
}

#else // UIP_CONF_IPV6

#include <ctype.h>

int inet_pton(const char *src, uip_ip6addr_t *dst) {
	static char xdigits[] PROGMEM = "0123456789abcdef";
	uint8_t tmp[NS_IN6ADDRSZ], *tp, *endp, *colonp;
	const char *curtok;
	int ch, saw_xdigit;
	uint32_t val;

	tp = memset(tmp, '\0', NS_IN6ADDRSZ);
	endp = tp + NS_IN6ADDRSZ;
	colonp = NULL;

	/* Leading :: requires some special handling. */
	if (*src == ':')
		if (*++src != ':')
			return 0;

	curtok = src;
	saw_xdigit = 0;
	val = 0;

	while ((ch = tolower (*src++)) != '\0') {
		PGM_P pch = strchr_P(xdigits, ch);
		if (pch != NULL) {
			val <<= 4;
			val |= (pch - xdigits);
			if (val > 0xffff)
				return 0;

			saw_xdigit = 1;
			continue;
		}

		if (ch == ':') {
			curtok = src;

			if (!saw_xdigit) {
				if (colonp)
					return 0;

				colonp = tp;
				continue;
			}
			else if (*src == '\0') {
				return 0;
			}

			if (tp + sizeof(uint16_t) > endp)
				return 0;

			*tp++ = (uint8_t) (val >> 8) & 0xff;
			*tp++ = (uint8_t) val & 0xff;
			saw_xdigit = 0;
			val = 0;
			continue;
		}

		return 0;
	}

	if (saw_xdigit) {
		if (tp + sizeof(uint16_t) > endp)
			return 0;

		*tp++ = (uint8_t) (val >> 8) & 0xff;
		*tp++ = (uint8_t) val & 0xff;
	}

	if (colonp != NULL) {
		/*
		 * Since some memmove()'s erroneously fail to handle
		 * overlapping regions, we'll do the shift by hand.
		 */
		const int n = tp - colonp;
		int i;

		if (tp == endp)
			return 0;

		for (i = 1; i <= n; i++) {
			endp[- i] = colonp[n - i];
			colonp[n - i] = 0;
		}

		tp = endp;
	}

	if (tp != endp)
		return 0;

	memcpy(dst, tmp, NS_IN6ADDRSZ);
	return 1;
}

#endif // UIP_CONF_IPV6

