#include <stdbool.h>
#include <stdint.h>
#include "postgres.h"
#include "storage/block.h"
#include "utils/elog.h"
#include "nodes/parsenodes.h"
#include "c.h"
#include "utils/datum.h"

////////////////////////////////////////
///// utils/catalog
///////////////////////////////////////

#include "lib/stringinfo.h"

char *NameListToString(List *names);
void check_stack_depth(void);
DefElem *defWithOids(bool value);

char *NameListToString(List *names)
{
	StringInfoData string;
	ListCell *l;

	initStringInfo(&string);

	foreach (l, names)
	{
		Node *name = (Node *)lfirst(l);

		if (l != list_head(names))
			appendStringInfoChar(&string, '.');

		if (IsA(name, String))
			appendStringInfoString(&string, strVal(name));
		else if (IsA(name, A_Star))
			appendStringInfoString(&string, "*");
		else
			elog(ERROR, "unexpected node type in name list: %d",
				 (int)nodeTag(name));
	}

	return string.data;
}

//===================================
// utils/mb

#include "mb/pg_wchar.h"
size_t
pg_wchar_strlen(const pg_wchar *str)
{
	const pg_wchar *s;

	for (s = str; *s; ++s)
		;
	return (s - str);
}

//==================================
// err

void elog_start(const char *filename, int lineno, const char *funcname)
{
}
int errmsg_internal(const char *fmt, ...)
{
	fprintf(stderr, "ERROR: %s\n", fmt);
	return 0; /* return value does not matter */
}
int errcode(int sqlerrcode)
{
	return 0; /* return value does not matter */
}

void elog_finish(int elevel, const char *fmt, ...)
{
	fprintf(stderr, "ERROR: %s\n", fmt);
	//  exit(1);
}

bool errstart(int elevel, const char *filename, int lineno,
			  const char *funcname, const char *domain)
{
	return (elevel >= ERROR);
}

void errfinish(int dummy, ...)
{
	// proc_exit(1);
}

int errhint(const char *fmt, ...)
{
	fprintf(stderr, "HINT: %s\n", fmt);
	return 0; /* return value does not matter */
}

int errdetail(const char *fmt, ...)
{
	fprintf(stderr, "DETAIL: %s\n", fmt);
	return 0; /* return value does not matter */
}

// int errmsg(const char *fmt, ...)
// {
// 	fprintf(stderr, "ERR: %s\n", fmt);
// 	return 0; /* return value does not matter */
// }

//==================================
//utils/adt

Size datumGetSize(Datum value, bool typByVal, int typLen)
{
	Size size;

	if (typByVal)
	{
		/* Pass-by-value types are always fixed-length */
		Assert(typLen > 0 && typLen <= sizeof(Datum));
		size = (Size)typLen;
	}
	else
	{
		if (typLen > 0)
		{
			/* Fixed-length pass-by-ref type */
			size = (Size)typLen;
		}
		else if (typLen == -1)
		{
			/* It is a varlena datatype */
			struct varlena *s = (struct varlena *)DatumGetPointer(value);

			if (!PointerIsValid(s))
				ereport(ERROR,
						(errcode(ERRCODE_DATA_EXCEPTION),
						 errmsg("invalid Datum pointer")));

			size = (Size)VARSIZE_ANY(s);
		}
		else if (typLen == -2)
		{
			/* It is a cstring datatype */
			char *s = (char *)DatumGetPointer(value);

			if (!PointerIsValid(s))
				ereport(ERROR,
						(errcode(ERRCODE_DATA_EXCEPTION),
						 errmsg("invalid Datum pointer")));

			size = (Size)(strlen(s) + 1);
		}
		else
		{
			elog(ERROR, "invalid typLen: %d", typLen);
			size = 0; /* keep compiler quiet */
		}
	}

	return size;
}

bool datumIsEqual(Datum value1, Datum value2, bool typByVal, int typLen)
{
	bool res;

	if (typByVal)
	{
		/*
		 * just compare the two datums. NOTE: just comparing "len" bytes will
		 * not do the work, because we do not know how these bytes are aligned
		 * inside the "Datum".	We assume instead that any given datatype is
		 * consistent about how it fills extraneous bits in the Datum.
		 */
		res = (value1 == value2);
	}
	else
	{
		Size size1,
			size2;
		char *s1,
			*s2;

		/*
		 * Compare the bytes pointed by the pointers stored in the datums.
		 */
		size1 = datumGetSize(value1, typByVal, typLen);
		size2 = datumGetSize(value2, typByVal, typLen);
		if (size1 != size2)
			return false;
		s1 = (char *)DatumGetPointer(value1);
		s2 = (char *)DatumGetPointer(value2);
		res = (memcmp(s1, s2, size1) == 0);
	}
	return res;
}

Datum datumCopy(Datum value, bool typByVal, int typLen)
{
	Datum res;

	if (typByVal)
		res = value;
	else
	{
		Size realSize;
		char *s;

		if (DatumGetPointer(value) == NULL)
			return PointerGetDatum(NULL);

		realSize = datumGetSize(value, typByVal, typLen);

		s = (char *)palloc(realSize);
		memcpy(s, DatumGetPointer(value), realSize);
		res = PointerGetDatum(s);
	}
	return res;
}

char *stack_base_ptr = NULL;

void check_stack_depth(void)
{
	char stack_top_loc;
	long stack_depth;

	/*
	 * Compute distance from reference point to my local variables
	 */
	stack_depth = (long)(stack_base_ptr - &stack_top_loc);

	/*
	 * Take abs value, since stacks grow up on some machines, down on others
	 */
	if (stack_depth < 0)
		stack_depth = -stack_depth;

#if defined(__ia64__) || defined(__ia64)
	stack_depth = (long)(ia64_get_bsp() - register_stack_base_ptr);

	if (stack_depth > max_stack_depth_bytes &&
		register_stack_base_ptr != NULL)
	{
		ereport(ERROR,
				(errcode(ERRCODE_STATEMENT_TOO_COMPLEX),
				 errmsg("stack depth limit exceeded"),
				 errhint("Increase the configuration parameter \"max_stack_depth\" (currently %dkB), "
						 "after ensuring the platform's stack depth limit is adequate.",
						 max_stack_depth)));
	}
#endif /* IA64 */
}
//==========================================================================
//commands/define.c

extern DefElem *
makeDefElem(char *name, Node *arg);

DefElem *
defWithOids(bool value)
{
	return makeDefElem("oids", (Node *)makeInteger(value));
}

//========================================================================
//utils/error

static int errordata_stack_depth = -1; /* index of topmost active frame */

static ErrorData errordata[5];

int errposition(int cursorpos)
{
	ErrorData *edata = &errordata[errordata_stack_depth];

	/* we don't bother incrementing recursion_depth */
	// CHECK_STACK_DEPTH();

	edata->cursorpos = cursorpos;

	return 0; /* return value does not matter */
}
