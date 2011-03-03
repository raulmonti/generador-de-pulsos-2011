/*
 * This source file is part of the bstring string library.  This code was
 * written by Paul Hsieh in 2002-2008, and is covered by the BSD open source 
 * license and the GPL. Refer to the accompanying documentation for details 
 * on usage and license.
 */

/*
 * bstrlib.c
 *
 * This file is the core module for implementing the bstring functions.
 */

#include <stdio.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bstrlib.h"

/* Optionally include a mechanism for debugging memory */

#if defined(MEMORY_DEBUG) || defined(BSTRLIB_MEMORY_DEBUG)
#include "memdbg.h"
#endif

#ifndef bstr__alloc
#define bstr__alloc(x) malloc (x)
#endif

#ifndef bstr__free
#define bstr__free(p) free (p)
#endif

#ifndef bstr__realloc
#define bstr__realloc(p,x) realloc ((p), (x))
#endif

#ifndef bstr__memcpy
#define bstr__memcpy(d,s,l) memcpy ((d), (s), (l))
#endif

#ifndef bstr__memmove
#define bstr__memmove(d,s,l) memmove ((d), (s), (l))
#endif

#ifndef bstr__memset
#define bstr__memset(d,c,l) memset ((d), (c), (l))
#endif

#ifndef bstr__memcmp
#define bstr__memcmp(d,c,l) memcmp ((d), (c), (l))
#endif

#ifndef bstr__memchr
#define bstr__memchr(s,c,l) memchr ((s), (c), (l))
#endif

/* Just a length safe wrapper for memmove. */

#define bBlockCopy(D,S,L) { if ((L) > 0) bstr__memmove ((D),(S),(L)); }

/* Compute the snapped size for a given requested size.  By snapping to powers
   of 2 like this, repeated reallocations are avoided. */
static int snapUpSize (int i) {
	if (i < 8) {
		i = 8;
	} else {
		unsigned int j;
		j = (unsigned int) i;

		j |= (j >>  1);
		j |= (j >>  2);
		j |= (j >>  4);
		j |= (j >>  8);		/* Ok, since int >= 16 bits */
#if (UINT_MAX != 0xffff)
		j |= (j >> 16);		/* For 32 bit int systems */
#if (UINT_MAX > 0xffffffffUL)
		j |= (j >> 32);		/* For 64 bit int systems */
#endif
#endif
		/* Least power of two greater than i */
		j++;
		if ((int) j >= i) i = (int) j;
	}
	return i;
}

/*  int balloc (bstring b, int len)
 *
 *  Increase the size of the memory backing the bstring b to at least len.
 */
int balloc (bstring b, int olen) {
	int len;
	if (b == NULL || b->data == NULL || b->slen < 0 || b->mlen <= 0 || 
	    b->mlen < b->slen || olen <= 0) {
		return BSTR_ERR;
	}

	if (olen >= b->mlen) {
		unsigned char * x;

		if ((len = snapUpSize (olen)) <= b->mlen) return BSTR_OK;

		/* Assume probability of a non-moving realloc is 0.125 */
		if (7 * b->mlen < 8 * b->slen) {

			/* If slen is close to mlen in size then use realloc to reduce
			   the memory defragmentation */

			reallocStrategy:;

			x = (unsigned char *) bstr__realloc (b->data, (size_t) len);
			if (x == NULL) {

				/* Since we failed, try allocating the tighest possible 
				   allocation */

				if (NULL == (x = (unsigned char *) bstr__realloc (b->data, (size_t) (len = olen)))) {
					return BSTR_ERR;
				}
			}
		} else {

			/* If slen is not close to mlen then avoid the penalty of copying
			   the extra bytes that are allocated, but not considered part of
			   the string */

			if (NULL == (x = (unsigned char *) bstr__alloc ((size_t) len))) {

				/* Perhaps there is no available memory for the two 
				   allocations to be in memory at once */

				goto reallocStrategy;

			} else {
				if (b->slen) bstr__memcpy ((char *) x, (char *) b->data, (size_t) b->slen);
				bstr__free (b->data);
			}
		}
		b->data = x;
		b->mlen = len;
		b->data[b->slen] = (unsigned char) '\0';
	}

	return BSTR_OK;
}

/*  int ballocmin (bstring b, int len)
 *
 *  Set the size of the memory backing the bstring b to len or b->slen+1,
 *  whichever is larger.  Note that repeated use of this function can degrade
 *  performance.
 */
int ballocmin (bstring b, int len) {
	unsigned char * s;

	if (b == NULL || b->data == NULL || (b->slen+1) < 0 || b->mlen <= 0 || 
	    b->mlen < b->slen || len <= 0) {
		return BSTR_ERR;
	}

	if (len < b->slen + 1) len = b->slen + 1;

	if (len != b->mlen) {
		s = (unsigned char *) bstr__realloc (b->data, (size_t) len);
		if (NULL == s) return BSTR_ERR;
		s[b->slen] = (unsigned char) '\0';
		b->data = s;
		b->mlen = len;
	}

	return BSTR_OK;
}

/*  bstring bfromcstr (const char * str)
 *
 *  Create a bstring which contains the contents of the '\0' terminated char *
 *  buffer str.
 */
bstring bfromcstr (const char * str) {
bstring b;
int i;
size_t j;

	if (str == NULL) return NULL;
	j = (strlen) (str);
	i = snapUpSize ((int) (j + (2 - (j != 0))));
	if (i <= (int) j) return NULL;

	b = (bstring) bstr__alloc (sizeof (struct tagbstring));
	if (NULL == b) return NULL;
	b->slen = (int) j;
	if (NULL == (b->data = (unsigned char *) bstr__alloc (b->mlen = i))) {
		bstr__free (b);
		return NULL;
	}

	bstr__memcpy (b->data, str, j+1);
	return b;
}

/*  bstring bfromcstralloc (int mlen, const char * str)
 *
 *  Create a bstring which contains the contents of the '\0' terminated char *
 *  buffer str.  The memory buffer backing the string is at least len 
 *  characters in length.
 */
bstring bfromcstralloc (int mlen, const char * str) {
bstring b;
int i;
size_t j;

	if (str == NULL) return NULL;
	j = (strlen) (str);
	i = snapUpSize ((int) (j + (2 - (j != 0))));
	if (i <= (int) j) return NULL;

	b = (bstring) bstr__alloc (sizeof (struct tagbstring));
	if (b == NULL) return NULL;
	b->slen = (int) j;
	if (i < mlen) i = mlen;

	if (NULL == (b->data = (unsigned char *) bstr__alloc (b->mlen = i))) {
		bstr__free (b);
		return NULL;
	}

	bstr__memcpy (b->data, str, j+1);
	return b;
}

/*  bstring blk2bstr (const void * blk, int len)
 *
 *  Create a bstring which contains the content of the block blk of length 
 *  len.
 */
bstring blk2bstr (const void * blk, int len) {
bstring b;
int i;

	if (blk == NULL || len < 0) return NULL;
	b = (bstring) bstr__alloc (sizeof (struct tagbstring));
	if (b == NULL) return NULL;
	b->slen = len;

	i = len + (2 - (len != 0));
	i = snapUpSize (i);

	b->mlen = i;

	b->data = (unsigned char *) bstr__alloc ((size_t) b->mlen);
	if (b->data == NULL) {
		bstr__free (b);
		return NULL;
	}

	if (len > 0) bstr__memcpy (b->data, blk, (size_t) len);
	b->data[len] = (unsigned char) '\0';

	return b;
}

/*  char * bstr2cstr (const_bstring s, char z)
 *
 *  Create a '\0' terminated char * buffer which is equal to the contents of 
 *  the bstring s, except that any contained '\0' characters are converted 
 *  to the character in z. This returned value should be freed with a 
 *  bcstrfree () call, by the calling application.
 */
char * bstr2cstr (const_bstring b, char z) {
int i, l;
char * r;

	if (b == NULL || b->slen < 0 || b->data == NULL) return NULL;
	l = b->slen;
	r = (char *) bstr__alloc ((size_t) (l + 1));
	if (r == NULL) return r;

	for (i=0; i < l; i ++) {
		r[i] = (char) ((b->data[i] == '\0') ? z : (char) (b->data[i]));
	}

	r[l] = (unsigned char) '\0';

	return r;
}

/*  int bcstrfree (char * s)
 *
 *  Frees a C-string generated by bstr2cstr ().  This is normally unnecessary
 *  since it just wraps a call to bstr__free (), however, if bstr__alloc () 
 *  and bstr__free () have been redefined as a macros within the bstrlib 
 *  module (via defining them in memdbg.h after defining 
 *  BSTRLIB_MEMORY_DEBUG) with some difference in behaviour from the std 
 *  library functions, then this allows a correct way of freeing the memory 
 *  that allows higher level code to be independent from these macro 
 *  redefinitions.
 */
int bcstrfree (char * s) {
	if (s) {
		bstr__free (s);
		return BSTR_OK;
	}
	return BSTR_ERR;
}

/*  int bconcat (bstring b0, const_bstring b1)
 *
 *  Concatenate the bstring b1 to the bstring b0.
 */
int bconcat (bstring b0, const_bstring b1) {
int len, d;
bstring aux = (bstring) b1;

	if (b0 == NULL || b1 == NULL || b0->data == NULL || b1->data == NULL) return BSTR_ERR;

	d = b0->slen;
	len = b1->slen;
	if ((d | (b0->mlen - d) | len | (d + len)) < 0) return BSTR_ERR;

	if (b0->mlen <= d + len + 1) {
		ptrdiff_t pd = b1->data - b0->data;
		if (0 <= pd && pd < b0->mlen) {
			if (NULL == (aux = bstrcpy (b1))) return BSTR_ERR;
		}
		if (balloc (b0, d + len + 1) != BSTR_OK) {
			if (aux != b1) bdestroy (aux);
			return BSTR_ERR;
		}
	}

	bBlockCopy (&b0->data[d], &aux->data[0], (size_t) len);
	b0->data[d + len] = (unsigned char) '\0';
	b0->slen = d + len;
	if (aux != b1) bdestroy (aux);
	return BSTR_OK;
}

/*  int bconchar (bstring b, char c)
/ *
 *  Concatenate the single character c to the bstring b.
 */
int bconchar (bstring b, char c) {
int d;

	if (b == NULL) return BSTR_ERR;
	d = b->slen;
	if ((d | (b->mlen - d)) < 0 || balloc (b, d + 2) != BSTR_OK) return BSTR_ERR;
	b->data[d] = (unsigned char) c;
	b->data[d + 1] = (unsigned char) '\0';
	b->slen++;
	return BSTR_OK;
}

/*  int bcatcstr (bstring b, const char * s)
 *
 *  Concatenate a char * string to a bstring.
 */
int bcatcstr (bstring b, const char * s) {
char * d;
int i, l;

	if (b == NULL || b->data == NULL || b->slen < 0 || b->mlen < b->slen
	 || b->mlen <= 0 || s == NULL) return BSTR_ERR;

	/* Optimistically concatenate directly */
	l = b->mlen - b->slen;
	d = (char *) &b->data[b->slen];
	for (i=0; i < l; i++) {
		if ((*d++ = *s++) == '\0') {
			b->slen += i;
			return BSTR_OK;
		}
	}
	b->slen += i;

	/* Need to explicitely resize and concatenate tail */
	return bcatblk (b, (const void *) s, (int) strlen (s));
}

/*  int bcatblk (bstring b, const void * s, int len)
 *
 *  Concatenate a fixed length buffer to a bstring.
 */
int bcatblk (bstring b, const void * s, int len) {
int nl;

	if (b == NULL || b->data == NULL || b->slen < 0 || b->mlen < b->slen
	 || b->mlen <= 0 || s == NULL || len < 0) return BSTR_ERR;

	if (0 > (nl = b->slen + len)) return BSTR_ERR; /* Overflow? */
	if (b->mlen <= nl && 0 > balloc (b, nl + 1)) return BSTR_ERR;

	bBlockCopy (&b->data[b->slen], s, (size_t) len);
	b->slen = nl;
	b->data[nl] = (unsigned char) '\0';
	return BSTR_OK;
}

/*  bstring bstrcpy (const_bstring b)
 *
 *  Create a copy of the bstring b.
 */
bstring bstrcpy (const_bstring b) {
bstring b0;
int i,j;

	/* Attempted to copy an invalid string? */
	if (b == NULL || b->slen < 0 || b->data == NULL) return NULL;

	b0 = (bstring) bstr__alloc (sizeof (struct tagbstring));
	if (b0 == NULL) {
		/* Unable to allocate memory for string header */
		return NULL;
	}

	i = b->slen;
	j = snapUpSize (i + 1);

	b0->data = (unsigned char *) bstr__alloc (j);
	if (b0->data == NULL) {
		j = i + 1;
		b0->data = (unsigned char *) bstr__alloc (j);
		if (b0->data == NULL) {
			/* Unable to allocate memory for string data */
			bstr__free (b0);
			return NULL;
		}
	}

	b0->mlen = j;
	b0->slen = i;

	if (i) bstr__memcpy ((char *) b0->data, (char *) b->data, i);
	b0->data[b0->slen] = (unsigned char) '\0';

	return b0;
}

/*  int bassign (bstring a, const_bstring b)
 *
 *  Overwrite the string a with the contents of string b.
 */
int bassign (bstring a, const_bstring b) {
	if (b == NULL || b->data == NULL || b->slen < 0)
		return BSTR_ERR;
	if (b->slen != 0) {
		if (balloc (a, b->slen) != BSTR_OK) return BSTR_ERR;
		bstr__memmove (a->data, b->data, b->slen);
	} else {
		if (a == NULL || a->data == NULL || a->mlen < a->slen || 
		    a->slen < 0 || a->mlen == 0) 
			return BSTR_ERR;
	}
	a->data[b->slen] = (unsigned char) '\0';
	a->slen = b->slen;
	return BSTR_OK;
}

/*  int bassignmidstr (bstring a, const_bstring b, int left, int len)
 *
 *  Overwrite the string a with the middle of contents of string b 
 *  starting from position left and running for a length len.  left and 
 *  len are clamped to the ends of b as with the function bmidstr.
 */
int bassignmidstr (bstring a, const_bstring b, int left, int len) {
	if (b == NULL || b->data == NULL || b->slen < 0)
		return BSTR_ERR;

	if (left < 0) {
		len += left;
		left = 0;
	}

	if (len > b->slen - left) len = b->slen - left;

	if (a == NULL || a->data == NULL || a->mlen < a->slen ||
	    a->slen < 0 || a->mlen == 0)
		return BSTR_ERR;

	if (len > 0) {
		if (balloc (a, len) != BSTR_OK) return BSTR_ERR;
		bstr__memmove (a->data, b->data + left, len);
		a->slen = len;
	} else {
		a->slen = 0;
	}
	a->data[a->slen] = (unsigned char) '\0';
	return BSTR_OK;
}

/*  int bassigncstr (bstring a, const char * str)
 *
 *  Overwrite the string a with the contents of char * string str.  Note that 
 *  the bstring a must be a well defined and writable bstring.  If an error 
 *  occurs BSTR_ERR is returned however a may be partially overwritten.
 */
int bassigncstr (bstring a, const char * str) {
int i;
size_t len;
	if (a == NULL || a->data == NULL || a->mlen < a->slen ||
	    a->slen < 0 || a->mlen == 0 || NULL == str) 
		return BSTR_ERR;

	for (i=0; i < a->mlen; i++) {
		if ('\0' == (a->data[i] = str[i])) {
			a->slen = i;
			return BSTR_OK;
		}
	}

	a->slen = i;
	len = strlen (str + i);
	if (len > INT_MAX || i + len + 1 > INT_MAX ||
	    0 > balloc (a, (int) (i + len + 1))) return BSTR_ERR;
	bBlockCopy (a->data + i, str + i, (size_t) len + 1);
	a->slen += (int) len;
	return BSTR_OK;
}

/*  int bassignblk (bstring a, const void * s, int len)
 *
 *  Overwrite the string a with the contents of the block (s, len).  Note that 
 *  the bstring a must be a well defined and writable bstring.  If an error 
 *  occurs BSTR_ERR is returned and a is not overwritten.
 */
int bassignblk (bstring a, const void * s, int len) {
	if (a == NULL || a->data == NULL || a->mlen < a->slen ||
	    a->slen < 0 || a->mlen == 0 || NULL == s || len + 1 < 1) 
		return BSTR_ERR;
	if (len + 1 > a->mlen && 0 > balloc (a, len + 1)) return BSTR_ERR;
	bBlockCopy (a->data, s, (size_t) len);
	a->data[len] = (unsigned char) '\0';
	a->slen = len;
	return BSTR_OK;
}

/*  int btrunc (bstring b, int n)
 *
 *  Truncate the bstring to at most n characters.
 */
int btrunc (bstring b, int n) {
	if (n < 0 || b == NULL || b->data == NULL || b->mlen < b->slen ||
	    b->slen < 0 || b->mlen <= 0) return BSTR_ERR;
	if (b->slen > n) {
		b->slen = n;
		b->data[n] = (unsigned char) '\0';
	}
	return BSTR_OK;
}

#define   upcase(c) (toupper ((unsigned char) c))
#define downcase(c) (tolower ((unsigned char) c))
#define   wspace(c) (isspace ((unsigned char) c))

/*  int btoupper (bstring b)
 *
 *  Convert contents of bstring to upper case.
 */
int btoupper (bstring b) {
int i, len;
	if (b == NULL || b->data == NULL || b->mlen < b->slen ||
	    b->slen < 0 || b->mlen <= 0) return BSTR_ERR;
	for (i=0, len = b->slen; i < len; i++) {
		b->data[i] = (unsigned char) upcase (b->data[i]);
	}
	return BSTR_OK;
}

/*  int btolower (bstring b)
 *
 *  Convert contents of bstring to lower case.
 */
int btolower (bstring b) {
int i, len;
	if (b == NULL || b->data == NULL || b->mlen < b->slen ||
	    b->slen < 0 || b->mlen <= 0) return BSTR_ERR;
	for (i=0, len = b->slen; i < len; i++) {
		b->data[i] = (unsigned char) downcase (b->data[i]);
	}
	return BSTR_OK;
}

/*  int bstricmp (const_bstring b0, const_bstring b1)
 *
 *  Compare two strings without differentiating between case.  The return 
 *  value is the difference of the values of the characters where the two 
 *  strings first differ after lower case transformation, otherwise 0 is 
 *  returned indicating that the strings are equal.  If the lengths are 
 *  different, then a difference from 0 is given, but if the first extra 
 *  character is '\0', then it is taken to be the value UCHAR_MAX+1.
 */
int bstricmp (const_bstring b0, const_bstring b1) {
int i, v, n;

	if (bdata (b0) == NULL || b0->slen < 0 || 
	    bdata (b1) == NULL || b1->slen < 0) return SHRT_MIN;
	if ((n = b0->slen) > b1->slen) n = b1->slen;
	else if (b0->slen == b1->slen && b0->data == b1->data) return BSTR_OK;

	for (i = 0; i < n; i ++) {
		v  = (char) downcase (b0->data[i])
		   - (char) downcase (b1->data[i]);
		if (0 != v) return v;
	}

	if (b0->slen > n) {
		v = (char) downcase (b0->data[n]);
		if (v) return v;
		return UCHAR_MAX + 1;
	}
	if (b1->slen > n) {
		v = - (char) downcase (b1->data[n]);
		if (v) return v;
		return - (int) (UCHAR_MAX + 1);
	}
	return BSTR_OK;
}

/*  int bstrnicmp (const_bstring b0, const_bstring b1, int n)
 *
 *  Compare two strings without differentiating between case for at most n
 *  characters.  If the position where the two strings first differ is
 *  before the nth position, the return value is the difference of the values
 *  of the characters, otherwise 0 is returned.  If the lengths are different
 *  and less than n characters, then a difference from 0 is given, but if the 
 *  first extra character is '\0', then it is taken to be the value 
 *  UCHAR_MAX+1.
 */
int bstrnicmp (const_bstring b0, const_bstring b1, int n) {
int i, v, m;

	if (bdata (b0) == NULL || b0->slen < 0 || 
	    bdata (b1) == NULL || b1->slen < 0 || n < 0) return SHRT_MIN;
	m = n;
	if (m > b0->slen) m = b0->slen;
	if (m > b1->slen) m = b1->slen;

	if (b0->data != b1->data) {
		for (i = 0; i < m; i ++) {
			v  = (char) downcase (b0->data[i]);
			v -= (char) downcase (b1->data[i]);
			if (v != 0) return b0->data[i] - b1->data[i];
		}
	}

	if (n == m || b0->slen == b1->slen) return BSTR_OK;

	if (b0->slen > m) {
		v = (char) downcase (b0->data[m]);
		if (v) return v;
		return UCHAR_MAX + 1;
	}

	v = - (char) downcase (b1->data[m]);
	if (v) return v;
	return - (int) (UCHAR_MAX + 1);
}

/*  int biseqcaseless (const_bstring b0, const_bstring b1)
 *
 *  Compare two strings for equality without differentiating between case.  
 *  If the strings differ other than in case, 0 is returned, if the strings 
 *  are the same, 1 is returned, if there is an error, -1 is returned.  If 
 *  the length of the strings are different, this function is O(1).  '\0' 
 *  termination characters are not treated in any special way.
 */
int biseqcaseless (const_bstring b0, const_bstring b1) {
int i, n;

	if (bdata (b0) == NULL || b0->slen < 0 || 
	    bdata (b1) == NULL || b1->slen < 0) return BSTR_ERR;
	if (b0->slen != b1->slen) return BSTR_OK;
	if (b0->data == b1->data || b0->slen == 0) return 1;
	for (i=0, n=b0->slen; i < n; i++) {
		if (b0->data[i] != b1->data[i]) {
			unsigned char c = (unsigned char) downcase (b0->data[i]);
			if (c != (unsigned char) downcase (b1->data[i])) return 0;
		}
	}
	return 1;
}

/*  int bisstemeqcaselessblk (const_bstring b0, const void * blk, int len)
 *
 *  Compare beginning of string b0 with a block of memory of length len 
 *  without differentiating between case for equality.  If the beginning of b0
 *  differs from the memory block other than in case (or if b0 is too short), 
 *  0 is returned, if the strings are the same, 1 is returned, if there is an 
 *  error, -1 is returned.  '\0' characters are not treated in any special 
 *  way.
 */
int bisstemeqcaselessblk (const_bstring b0, const void * blk, int len) {
int i;

	if (bdata (b0) == NULL || b0->slen < 0 || NULL == blk || len < 0)
		return BSTR_ERR;
	if (b0->slen < len) return BSTR_OK;
	if (b0->data == (const unsigned char *) blk || len == 0) return 1;

	for (i = 0; i < len; i ++) {
		if (b0->data[i] != ((const unsigned char *) blk)[i]) {
			if (downcase (b0->data[i]) != 
			    downcase (((const unsigned char *) blk)[i])) return 0;
		}
	}
	return 1;
}

/*
 * int bltrimws (bstring b)
 *
 * Delete whitespace contiguous from the left end of the string.
 */
int bltrimws (bstring b) {
int i, len;

	if (b == NULL || b->data == NULL || b->mlen < b->slen ||
	    b->slen < 0 || b->mlen <= 0) return BSTR_ERR;

	for (len = b->slen, i = 0; i < len; i++) {
		if (!wspace (b->data[i])) {
			return bdelete (b, 0, i);
		}
	}

	b->data[0] = (unsigned char) '\0';
	b->slen = 0;
	return BSTR_OK;
}

/*
 * int brtrimws (bstring b)
 *
 * Delete whitespace contiguous from the right end of the string.
 */
int brtrimws (bstring b) {
int i;

	if (b == NULL || b->data == NULL || b->mlen < b->slen ||
	    b->slen < 0 || b->mlen <= 0) return BSTR_ERR;

	for (i = b->slen - 1; i >= 0; i--) {
		if (!wspace (b->data[i])) {
			if (b->mlen > i) b->data[i+1] = (unsigned char) '\0';
			b->slen = i + 1;
			return BSTR_OK;
		}
	}

	b->data[0] = (unsigned char) '\0';
	b->slen = 0;
	return BSTR_OK;
}


