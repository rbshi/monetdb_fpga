/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.  If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 1997 - July 2008 CWI, August 2008 - 2018 MonetDB B.V.
 */

#include "monetdb_config.h"
#include "bat_logger.h"
#include "bat_utils.h"
#include "sql_types.h" /* EC_POS */

logger *bat_logger = NULL;
logger *bat_logger_shared = NULL;

/* return GDK_SUCCEED if we can handle the upgrade from oldversion to
 * newversion */
static gdk_return
bl_preversion(int oldversion, int newversion)
{
#define CATALOG_JUL2015 52200

	(void)newversion;
	if (oldversion == CATALOG_JUL2015) {
		/* upgrade to Jun2016 releases */
		catalog_version = oldversion;
		geomversion_set();
		return GDK_SUCCEED;
	}

	return GDK_FAIL;
}

static char *
N( char *buf, char *pre, char *schema, char *post)
{
	if (pre)
		snprintf(buf, 64, "%s_%s_%s", pre, schema, post);
	else
		snprintf(buf, 64, "%s_%s", schema, post);
	return buf;
}

static gdk_return
bl_postversion( void *lg) 
{
	(void)lg;

	if (catalog_version <= CATALOG_JUL2015) {
		BAT *b;
		BATiter bi;
		BAT *te, *tne;
		BUN p, q;
		int geomUpgrade = 0;
		char *s = "sys", n[64];
		geomcatalogfix_fptr func;

		te = temp_descriptor(logger_find_bat(lg, N(n, NULL, s, "types_eclass")));
		if (te == NULL)
			return GDK_FAIL;
		bi = bat_iterator(te);
		tne = COLnew(te->hseqbase, TYPE_int, BATcount(te), PERSISTENT);
		if (tne == NULL) {
			bat_destroy(te);
			return GDK_FAIL;
		}
		for (p = 0, q = BUNlast(te); p < q; p++) {
			int eclass = *(int*)BUNtail(bi, p);

			if (eclass == EC_GEOM)		/* old EC_EXTERNAL */
				eclass++;		/* shift up */
			if (BUNappend(tne, &eclass, true) != GDK_SUCCEED) {
				bat_destroy(tne);
				bat_destroy(te);
				return GDK_FAIL;
			}
		}
		bat_destroy(te);
		if (BATsetaccess(tne, BAT_READ) != GDK_SUCCEED ||
		    logger_add_bat(lg, tne, N(n, NULL, s, "types_eclass")) != GDK_SUCCEED) {
			bat_destroy(tne);
			return GDK_FAIL;
		}
		bat_destroy(tne);

		/* in the past, the args.inout column may have been
		 * incorrectly upgraded to a bit instead of a bte
		 * column */
		te = temp_descriptor(logger_find_bat(lg, N(n, NULL, s, "args_inout")));
		if (te == NULL)
			return GDK_FAIL;
		if (te->ttype == TYPE_bit) {
			bi = bat_iterator(te);
			tne = COLnew(te->hseqbase, TYPE_bte, BATcount(te), PERSISTENT);
			if (tne == NULL) {
				bat_destroy(te);
				return GDK_FAIL;
			}
			for (p = 0, q = BUNlast(te); p < q; p++) {
				bte inout = (bte) *(bit*)BUNtail(bi, p);

				if (BUNappend(tne, &inout, true) != GDK_SUCCEED) {
					bat_destroy(tne);
					bat_destroy(te);
					return GDK_FAIL;
				}
			}
			if (BATsetaccess(tne, BAT_READ) != GDK_SUCCEED ||
			    logger_add_bat(lg, tne, N(n, NULL, s, "args_inout")) != GDK_SUCCEED) {
				bat_destroy(tne);
				bat_destroy(te);
				return GDK_FAIL;
			}
			bat_destroy(tne);
		}
		bat_destroy(te);

		/* test whether the catalog contains information
		 * regarding geometry types */
		b = BATdescriptor((bat) logger_find_bat(lg, N(n, NULL, s, "types_systemname")));
		if (b == NULL)
			return GDK_FAIL;
		bi = bat_iterator(b);
		for (p = 0, q = BUNlast(b); p < q; p++) {
			char *t = toLower(BUNtail(bi, p));
			if (t == NULL) {
				bat_destroy(b);
				return GDK_FAIL;
			}
			geomUpgrade = strcmp(t, "wkb") == 0;
			GDKfree(t);
			if (geomUpgrade)
				break;
		}
		bat_destroy(b);

		if (!geomUpgrade) {
			/* test whether the catalog contains
			 * information about geometry columns */
			b = BATdescriptor((bat) logger_find_bat(lg, N(n, NULL, s, "_columns_type")));
			if (b == NULL)
				return GDK_FAIL;
			bi = bat_iterator(b);
			for (p = 0, q = BUNlast(b); p < q; p++) {
				char *t = toLower(BUNtail(bi, p));
				if (t == NULL) {
					bat_destroy(b);
					return GDK_FAIL;
				}
				geomUpgrade = strcmp(t, "point") == 0 ||
					strcmp(t, "curve") == 0 ||
					strcmp(t, "linestring") == 0 ||
					strcmp(t, "surface") == 0 ||
					strcmp(t, "polygon") == 0 ||
					strcmp(t, "multipoint") == 0 ||
					strcmp(t, "multicurve") == 0 ||
					strcmp(t, "multilinestring") == 0 ||
					strcmp(t, "multisurface") == 0 ||
					strcmp(t, "multipolygon") == 0 ||
					strcmp(t, "geometry") == 0 ||
					strcmp(t, "geometrycollection") == 0;
				GDKfree(t);
				if (geomUpgrade)
					break;
			}
			bat_destroy(b);
		}

		func = geomcatalogfix_get();
		if (func) {
			/* Either the catalog needs to be updated and
			 * the geom module has been loaded
			 * (geomUpgrade == 1), or the catalog knew
			 * nothing about geometries but the geom
			 * module is loaded (geomUpgrade == 0) */
			(*func)(lg, geomUpgrade);
		} else {
			if (geomUpgrade) {
				/* The catalog needs to be updated but
				 * the geom module has not been
				 * loaded.  The case is prohibited by
				 * the sanity check performed during
				 * initialization */
				GDKfatal("the catalogue needs to be updated but the geom module is not loaded.\n");
			}
			/* The catalog knew nothing about geometries
			 * and the geom module is not loaded: Do
			 * nothing */
		}
	}
	return GDK_SUCCEED;
}

static int 
bl_create(int debug, const char *logdir, int cat_version, int keep_persisted_log_files)
{
	if (bat_logger)
		return LOG_ERR;
	bat_logger = logger_create(debug, "sql", logdir, cat_version, bl_preversion, bl_postversion, keep_persisted_log_files);
	if (bat_logger)
		return LOG_OK;
	return LOG_ERR;
}

static int
bl_create_shared(int debug, const char *logdir, int cat_version, const char *local_logdir)
{
	if (bat_logger_shared)
		return LOG_ERR;
	bat_logger_shared = logger_create_shared(debug, "sql", logdir, local_logdir, cat_version, bl_preversion, bl_postversion);
	if (bat_logger_shared)
		return LOG_OK;
	return LOG_ERR;
}

static void 
bl_destroy(void)
{
	logger *l = bat_logger;

	bat_logger = NULL;
	if (l) {
		close_stream(l->log);
		GDKfree(l->fn);
		GDKfree(l->dir);
		GDKfree(l->local_dir);
		GDKfree(l->buf);
		GDKfree(l);
	}
}

static void
bl_destroy_shared(void)
{
	logger *l = bat_logger_shared;

	bat_logger_shared = NULL;
	if (l) {
		logger_exit(l);
		logger_destroy(l);
	}
}

static int 
bl_restart(void)
{
	if (bat_logger)
		return logger_restart(bat_logger) == GDK_SUCCEED ? LOG_OK : LOG_ERR;
	return LOG_OK;
}

static int
bl_cleanup(int keep_persisted_log_files)
{
	if (bat_logger)
		return logger_cleanup(bat_logger, keep_persisted_log_files) == GDK_SUCCEED ? LOG_OK : LOG_ERR;
	return LOG_OK;
}

static int
bl_cleanup_shared(int keep_persisted_log_files)
{
	if (bat_logger_shared)
		return logger_cleanup(bat_logger_shared, keep_persisted_log_files) == GDK_SUCCEED ? LOG_OK : LOG_ERR;
	return LOG_OK;
}

static int
bl_changes(void)
{	
	return (int) MIN(logger_changes(bat_logger), GDK_int_max);
}

static lng
bl_read_last_transaction_id_shared(void)
{
	return logger_read_last_transaction_id(bat_logger_shared, bat_logger_shared->dir, LOGFILE, bat_logger_shared->dbfarm_role);
}

static lng
bl_get_transaction_drift_shared(void)
{
	lng res = bl_read_last_transaction_id_shared();
	if (res != -1) {
		return MIN(res, GDK_int_max) - MIN(bat_logger_shared->id, GDK_int_max);
	}
	return res;
}

static int 
bl_get_sequence(int seq, lng *id)
{
	return logger_sequence(bat_logger, seq, id);
}

static int
bl_get_sequence_shared(int seq, lng *id)
{
	return logger_sequence(bat_logger_shared, seq, id);
}

static int
bl_log_isnew(void)
{
	if (BATcount(bat_logger->catalog_bid) > 10) {
		return 0;
	}
	return 1;
}

static int
bl_log_isnew_shared(void)
{
	if (BATcount(bat_logger_shared->catalog_bid) > 10) {
		return 0;
	}
	return 1;
}

static int 
bl_tstart(void)
{
	return log_tstart(bat_logger) == GDK_SUCCEED ? LOG_OK : LOG_ERR;
}

static int 
bl_tend(void)
{
	return log_tend(bat_logger) == GDK_SUCCEED ? LOG_OK : LOG_ERR;
}

static int 
bl_sequence(int seq, lng id)
{
	return log_sequence(bat_logger, seq, id) == GDK_SUCCEED ? LOG_OK : LOG_ERR;
}

static int
bl_reload_shared(void)
{
	return logger_reload(bat_logger_shared) == GDK_SUCCEED ? LOG_OK : LOG_ERR;
}

static void
snapshot_lazy_copy_file(stream *plan, char *name, long extent)
{
	mnstr_printf(plan, "c %ld %s\n", extent, name);
}

static const char*
snapshot_immediate_copy_file(stream *plan, const char *path, const char *name)
{
	const char *err = NULL;
	struct stat statbuf;
	char *buf = NULL;
	FILE *f = NULL;
	long size;
	size_t bytes_read;
	size_t bytes_written;

	if (stat(path, &statbuf) < 0) {
		err = strerror(errno);
		goto end;
	}
	size = (long)statbuf.st_size;

	buf = malloc(size);
	if (!buf) {
		err = "malloc(buf)";
		goto end;
	}

	f = fopen(path, "rb");
	if (!f) {
		err = strerror(errno);
		goto end;
	}

	bytes_read = fread(buf, 1, size, f);
	if ((long)bytes_read < size) {
		if (ferror(f))
			err = strerror(errno);
		else if (feof(f)) 
			err = "file unexpectedly short";
		else
			err = "read unexplainably truncated";
		goto end;
	}

	mnstr_printf(plan, "w %ld %s\n", size, name);
	bytes_written = mnstr_write(plan, buf, 1, bytes_read);
	if (bytes_written < bytes_read) {
		err = "write to plan truncated";
		goto end;
	}

end:
	free(buf);
	if (f)
		fclose(f);
	return err;
}

static void
snapshot_lazy_copy_heap(stream *plan, Heap *heap)
{
	long extent = heap->free;
	char *name = heap->filename;
	mnstr_printf(plan, "c %ld %s%c%s\n", extent, BATDIR, DIR_SEP, name);
}

static const char*
snapshot_wal(stream *plan, const char *db_dir)
{
	stream *log = bat_logger->log;
	char log_file[FILENAME_MAX];

	snprintf(log_file, sizeof(log_file), "%s/%s%s", db_dir, bat_logger->dir, LOGFILE);
	snapshot_immediate_copy_file(plan, log_file, log_file + strlen(db_dir) + 1);

	snprintf(log_file, sizeof(log_file), "%s%s." LLFMT, bat_logger->dir, LOGFILE, bat_logger->id);
	long pos = ftell(getFile(log));
	if (pos < 0)
		return strerror(errno);
	
	/* ASSUMPTION: the logger does not seek around in the logfile so
	 * everything after pos is currently garbage and irrelevant to this
	 * snapshot.
	 */
	snapshot_lazy_copy_file(plan, log_file, pos);

	return NULL;
}

static const char*
snapshot_bbp(stream *plan, const char *db_dir)
{
	char bbpdir[FILENAME_MAX];
	bat active_bats;
	const char *err;

	/* UH OH we probably have to obtain some sort of lock first */

	active_bats = getBBPsize();

	snprintf(bbpdir, sizeof(bbpdir), "%s%c%s%c%s", db_dir, DIR_SEP, BAKDIR, DIR_SEP, "BBP.dir");
	err = snapshot_immediate_copy_file(plan, bbpdir, bbpdir + strlen(db_dir) + 1);
	if (err)
		return err;

	for (bat id = 1; id < active_bats; id++) {
		if (BBP_status(id) & BBPPERSISTENT) {
			BAT *b = BBP_desc(id);
			snapshot_lazy_copy_heap(plan, &b->theap);
			if (b->tvheap)
				snapshot_lazy_copy_heap(plan, b->tvheap);
			if (b->torderidx)
				snapshot_lazy_copy_heap(plan, b->torderidx);
			// 
			// UH OH.. even if b->thash exists, there may not
			// be a corresponding heap file.
			// Let's skip it, the target system can probably build
			// its own hash tables if it needs them.
			// if (b->thash)
			// 	snapshot_lazy_copy_heap(plan, &b->thash->heap);
			//
			// UH OH.. definition of b->timprints not available here so
			// b->timprints->heap unreachable. Hopefully the system can 
			//reconstruct them.
		}
	}

	/* UH OH don't forget to include the contents of BBP.dir */

	return NULL;
}

static const char* 
bl_snapshot(stream *plan)
{
	const char *err;
	char *db_dir = NULL;
	size_t db_dir_len;

	// UH OH Is it always 0? It seems dbfarm is 0 and dbextra is 1
	// but that may not be carved in stone.
	db_dir = GDKfilepath(0, NULL, "", NULL);
	db_dir_len = strlen(db_dir);
	if (db_dir[db_dir_len - 1] == DIR_SEP)
		db_dir[db_dir_len - 1] = '\0';

	mnstr_printf(plan, "%s\n", db_dir);

	err = snapshot_wal(plan, db_dir);
	if (err) {
		GDKfree(db_dir);
		return err;
	}

	err = snapshot_bbp(plan, db_dir);
	if (err) {
		GDKfree(db_dir);
		return err;
	}

	return NULL;
}


void
bat_logger_init( logger_functions *lf )
{
	lf->create = bl_create;
	lf->destroy = bl_destroy;
	lf->restart = bl_restart;
	lf->cleanup = bl_cleanup;
	lf->changes = bl_changes;
	lf->get_sequence = bl_get_sequence;
	lf->snapshot = bl_snapshot;
	lf->log_isnew = bl_log_isnew;
	lf->log_tstart = bl_tstart;
	lf->log_tend = bl_tend;
	lf->log_sequence = bl_sequence;
}

void
bat_logger_init_shared( logger_functions *lf )
{
	lf->create_shared = bl_create_shared;
	lf->destroy = bl_destroy_shared;
	lf->cleanup = bl_cleanup_shared;
	lf->get_sequence = bl_get_sequence_shared;
	lf->read_last_transaction_id = bl_read_last_transaction_id_shared;
	lf->get_transaction_drift = bl_get_transaction_drift_shared;
	lf->log_isnew = bl_log_isnew_shared;
	lf->reload = bl_reload_shared;
}
