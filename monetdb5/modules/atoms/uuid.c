/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.  If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 1997 - July 2008 CWI, August 2008 - 2021 MonetDB B.V.
 */

/*
 * K.S. Mullender & A. de Rijke
 * The UUID module
 * The UUID module contains a wrapper for all function in
 * libuuid.
 */

#include "monetdb_config.h"
#include "mal.h"
#include "mal_exception.h"
#include "mal_atom.h"			/* for malAtomSize */
#ifndef HAVE_UUID
#ifdef HAVE_OPENSSL
# include <openssl/rand.h>
#else
#ifdef HAVE_COMMONCRYPTO
#include <CommonCrypto/CommonRandom.h>
#endif
#endif
#endif

/**
 * Returns the string representation of the given uuid value.
 * Warning: GDK function
 * Returns the length of the string
 */
static inline void
UUIDgenerateUuid_internal(uuid *u)
{
#ifdef HAVE_UUID
	uuid_generate(u->u);
#else
#ifdef HAVE_OPENSSL
	if (RAND_bytes(u->u, 16) < 0)
#else
#ifdef HAVE_COMMONCRYPTO
	if (CCRandomGenerateBytes(u->u, 16) != kCCSuccess)
#endif
#endif
		/* if it failed, use rand */
		for (int i = 0; i < UUID_SIZE;) {
			int r = rand();
			u->u[i++] = (unsigned char) (r >> 8);
			u->u[i++] = (unsigned char) r;
		}
	/* make sure this is a variant 1 UUID */
	u->u[8] = (u->u[8] & 0x3F) | 0x80;
	/* make sure this is version 4 (random UUID) */
	u->u[6] = (u->u[6] & 0x0F) | 0x40;
#endif
}

static str
UUIDgenerateUuid(uuid *retval)
{
	UUIDgenerateUuid_internal(retval);
	return MAL_SUCCEED;
}

static str
UUIDgenerateUuidInt(uuid *retval, int *d)
{
	(void)d;
	return UUIDgenerateUuid(retval);
}

static inline bit
isaUUID(const char *s)
{
	uuid u, *pu = &u;
	size_t l = UUID_SIZE;
	ssize_t res = BATatoms[TYPE_uuid].atomFromStr(s, &l, (void **) &pu, false);

	if (res > 1)
		return true;
	else if (res == 1)
		return bit_nil;
	else
		return false;
}

static str
UUIDgenerateUuidInt_bulk(bat *ret, const bat *bid)
{
	BAT *b = NULL, *bn = NULL;
	BUN n = 0;
	str msg = MAL_SUCCEED;
	uuid *restrict bnt = NULL;

	if ((b = BBPquickdesc(*bid, false)) == NULL)	{
		msg = createException(MAL, "uuid.generateuuidint_bulk", SQLSTATE(HY002) RUNTIME_OBJECT_MISSING);
		goto bailout;
	}
	n = BATcount(b);
	if ((bn = COLnew(b->hseqbase, TYPE_uuid, n, TRANSIENT)) == NULL) {
		msg = createException(MAL, "uuid.generateuuidint_bulk", SQLSTATE(HY013) MAL_MALLOC_FAIL);
		goto bailout;
	}
	bnt = Tloc(bn, 0);
	for (BUN i = 0 ; i < n ; i++)
		UUIDgenerateUuid_internal(&(bnt[i]));
	bn->tnonil = true;
	bn->tnil = false;
	BATsetcount(bn, n);
	bn->tsorted = n <= 1;
	bn->trevsorted = n <= 1;
	bn->tkey = n <= 1;

bailout:
	if (msg && bn)
		BBPreclaim(bn);
	else if (bn)
		BBPkeepref(*ret = bn->batCacheid);
	return msg;
}

static str
UUIDisaUUID(bit *retval, str *s)
{
	*retval = isaUUID(*s);
	return MAL_SUCCEED;
}

static str
UUIDisaUUID_bulk(bat *ret, const bat *bid)
{
	BAT *b = NULL, *bn = NULL;
	BUN q;
	bit *restrict dst;
	str msg = MAL_SUCCEED;
	BATiter bi;

	if ((b = BATdescriptor(*bid)) == NULL)	{
		msg = createException(MAL, "uuid.isaUUID_bulk", SQLSTATE(HY002) RUNTIME_OBJECT_MISSING);
		goto bailout;
	}
	q = BATcount(b);
	if ((bn = COLnew(b->hseqbase, TYPE_bit, q, TRANSIENT)) == NULL) {
		msg = createException(MAL, "uuid.isaUUID_bulk", SQLSTATE(HY013) MAL_MALLOC_FAIL);
		goto bailout;
	}
	dst = Tloc(bn, 0);
	bi = bat_iterator(b);
	for (BUN p = 0 ; p < q ; p++) {
		str next = BUNtail(bi, p);
		dst[p] = isaUUID(next);
	}
	bn->tnonil = b->tnonil;
	bn->tnil = b->tnil;
	BATsetcount(bn, q);
	bn->tsorted = bn->trevsorted = q < 2;
	bn->tkey = false;
bailout:
	if (b)
		BBPunfix(b->batCacheid);
	if (msg && bn)
		BBPreclaim(bn);
	else if (bn)
		BBPkeepref(*ret = bn->batCacheid);
	return msg;
}

static str
UUIDuuid2uuid(uuid *retval, uuid *i)
{
	*retval = *i;
	return MAL_SUCCEED;
}

static str
UUIDstr2uuid(uuid *retval, str *s)
{
	size_t l = UUID_SIZE;

	if (BATatoms[TYPE_uuid].atomFromStr(*s, &l, (void **) &retval, false) > 0) {
		return MAL_SUCCEED;
	}
	throw(MAL, "uuid.uuid", "Not a UUID");
}

static str
UUIDuuid2str(str *retval, const uuid *u)
{
	size_t l = 0;
	*retval = NULL;
	if (BATatoms[TYPE_uuid].atomToStr(retval, &l, u, false) < 0)
		throw(MAL, "uuid.str", GDK_EXCEPTION);
	return MAL_SUCCEED;
}

#include "mel.h"
mel_func uuid_init_funcs[] = {
 command("uuid", "new", UUIDgenerateUuid, true, "Generate a new uuid", args(1,1, arg("",uuid))),
 command("uuid", "new", UUIDgenerateUuidInt, true, "Generate a new uuid (dummy version for side effect free multiplex loop)", args(1,2, arg("",uuid),arg("d",int))),
 command("batuuid", "new", UUIDgenerateUuidInt_bulk, true, "Generate a new uuid (dummy version for side effect free multiplex loop)", args(1,2, batarg("",uuid),batarg("d",int))),
 command("uuid", "uuid", UUIDstr2uuid, false, "Coerce a string to a uuid, validating its format", args(1,2, arg("",uuid),arg("s",str))),
 command("uuid", "str", UUIDuuid2str, false, "Coerce a uuid to its string type", args(1,2, arg("",str),arg("u",uuid))),
 command("uuid", "isaUUID", UUIDisaUUID, false, "Test a string for a UUID format", args(1,2, arg("",bit),arg("u",str))),
 command("batuuid", "isaUUID", UUIDisaUUID_bulk, false, "Test a string for a UUID format", args(1,2, batarg("",bit),batarg("u",str))),
 command("calc", "uuid", UUIDstr2uuid, false, "Coerce a string to a uuid, validating its format", args(1,2, arg("",uuid),arg("s",str))),
 command("calc", "uuid", UUIDuuid2uuid, false, "", args(1,2, arg("",uuid),arg("u",uuid))),
 command("calc", "str", UUIDuuid2str, false, "Coerce a uuid to a string type", args(1,2, arg("",str),arg("s",uuid))),
 { .imp=NULL }
};
#include "mal_import.h"
#ifdef _MSC_VER
#undef read
#pragma section(".CRT$XCU",read)
#endif
LIB_STARTUP_FUNC(init_uuid_mal)
{ mal_module("uuid", NULL, uuid_init_funcs); }
