/*
 * Authors: Marios Vodas (mvodas@gmail.com).
 */

#include "h/Hermes.h"
#include "h/Math/Vector2D/Definition.h"
#include "h/Math/Vector3D/Definition.h"
#include "h/Temporal/Period/Definition.h"
#include "h/Temporal/RangeT/Definition.h"
#include "h/Spatial/PointSP/Definition.h"
#include "h/Spatial/BoxSP/Definition.h"
#include "h/Spatial/SegmentSP/Definition.h"
#include "h/Spatial/LineSP/Definition.h"
#include "h/Spatial/RangeSP/Definition.h"
#include "h/Spatial/CRS/Ellipsoid/Definition.h"
#include "h/Spatial/CRS/PointLL/Definition.h"
#include "h/Spatial/CRS/PointPLH/Definition.h"
#include "h/Spatial/CRS/PointXYZ/Definition.h"
#include "h/Spatial/CRS/PointENU/Definition.h"
#include "h/Spatial/CRS/PointXY/Definition.h"
#include "h/SpatioTemporal/PointST/Definition.h"
#include "h/SpatioTemporal/BoxST/Definition.h"
#include "h/SpatioTemporal/SegmentST/Definition.h"
#include "h/SpatioTemporal/RangeST/Definition.h"
#include "h/SpatioTemporal/Trajectory/Definition.h"
#include "h/Utilities/Utilities.h"
#include <funcapi.h>
#include <access/htup_details.h>
#include <utils/builtins.h>
#include <utils/date.h>
#include <utils/xml.h>

PGDLLEXPORT Datum hermesLicense(PG_FUNCTION_ARGS) {
	PG_RETURN_TEXT_P(cstring_to_text("\n"
	"Hermes License" "\n"
	"Version 1.0, September 2010" "\n"
	"\n"
	"TERMS AND CONDITIONS FOR USE, REPRODUCTION, AND DISTRIBUTION" "\n"
	"\n"
	"	1. Definitions." "\n"
	"\n"
	"		\"Software\" is defined as the software project Hermes (http://infolab.cs.unipi.gr/hermes - http://java.net/projects/hermes-mod) that is developed on top of PostgreSQL (http://www.postgresql.org/) as well as the software project HermesUI that relies on the previously mentioned software project Hermes." "\n"
	"\n"
	"		\"License\" is defined as the terms and conditions for use, reproduction,and distribution as defined by Sections 1 through 5 of this document." "\n"
	"\n"
	"		\"Licensor\" is defined as four specific individuals the first of whom is Nikos Pelekis (npelekis@unipi.gr), the second one is Yannis Theodoridis (ytheod@unipi.gr), the third one is Marios Vodas (mvodas@gmail.com), and the fourth one is Panagiotis Tampakis (panosbp@gmail.com). Only Nikos Pelekis, Yannis Theodoridis, Marios Vodas, and Panagiotis Tampakis can be referred as, and be, a Licensor." "\n"
	"\n"
	"		\"You\" (or \"Your\") is defined as an individual or legal entity exercising permissions granted by this License." "\n"
	"\n"
	"	2. Grant of Use. Subject to the terms and conditions of this License You are obliged to have the express written prior approval of the Licensor in order to use the Software." "\n"
	"\n"
	"	3. Grant of Reproduction. Subject to the terms and conditions of this License You are obliged to have the express written prior approval of the Licensor in order to reproduce the Software and the License." "\n"
	"\n"
	"	4. Grant of Distribution. Subject to the terms and conditions of this License You are obliged to have the express written prior approval of the Licensor in order to distribute the Software." "\n"
	"\n"
	"	5. Disclaimer of Warranty. Licensor provides the Software on an \"AS IS\" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. You are solely responsible for determining the appropriateness of using the Software and assume any risks associated with Your exercise of permissions under this License." "\n"
	"\n"
	"END OF TERMS AND CONDITIONS" "\n"));
}
PG_FUNCTION_INFO_V1(hermesLicense);

PGDLLEXPORT Datum hermesVersionMajor(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(1);
}
PG_FUNCTION_INFO_V1(hermesVersionMajor);

PGDLLEXPORT Datum hermesVersionMinor(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(8);
}
PG_FUNCTION_INFO_V1(hermesVersionMinor);

PGDLLEXPORT Datum hermesVersionPatch(PG_FUNCTION_ARGS) {
	PG_RETURN_INT32(5);
}
PG_FUNCTION_INFO_V1(hermesVersionPatch);

PGDLLEXPORT Datum hermesVersionString(PG_FUNCTION_ARGS) {
	PG_RETURN_TEXT_P(cstring_to_text("1.8.5"));
}
PG_FUNCTION_INFO_V1(hermesVersionString);

PGDLLEXPORT Datum hermesVersionCodename(PG_FUNCTION_ARGS) {
	PG_RETURN_TEXT_P(cstring_to_text("Book"));
}
PG_FUNCTION_INFO_V1(hermesVersionCodename);

PGDLLEXPORT Datum hermesVersionReleaseDate(PG_FUNCTION_ARGS) {
	PG_RETURN_DATEADT(DatumGetDateADT(DirectFunctionCall1(date_in, CStringGetDatum("2015-06-04"))));
}
PG_FUNCTION_INFO_V1(hermesVersionReleaseDate);

typedef struct {
	char c;
	Vector2D member;
} Vector2DM;

typedef struct {
	char c;
	Vector3D member;
} Vector3DM;

typedef struct {
	char c;
	Period member;
} PeriodM;

typedef struct {
	char c;
	RangeT member;
} RangeTM;

typedef struct {
	char c;
	PointSP member;
} PointSPM;

typedef struct {
	char c;
	BoxSP member;
} BoxSPM;

typedef struct {
	char c;
	SegmentSP member;
} SegmentSPM;

typedef struct {
	char c;
	LineSP member;
} LineSPM;

typedef struct {
	char c;
	RangeSP member;
} RangeSPM;

typedef struct {
	char c;
	Ellipsoid member;
} EllipsoidM;

typedef struct {
	char c;
	PointLL member;
} PointLLM;

typedef struct {
	char c;
	PointPLH member;
} PointPLHM;

typedef struct {
	char c;
	PointXYZ member;
} PointXYZM;

typedef struct {
	char c;
	PointENU member;
} PointENUM;

typedef struct {
	char c;
	PointXY member;
} PointXYM;

typedef struct {
	char c;
	PointST member;
} PointSTM;

typedef struct {
	char c;
	BoxST member;
} BoxSTM;

typedef struct {
	char c;
	SegmentST member;
} SegmentSTM;

typedef struct {
	char c;
	RangeST member;
} RangeSTM;

typedef struct {
	char c;
	Trajectory member;
} TrajectoryM;

PGDLLEXPORT Datum hermesDataTypes(PG_FUNCTION_ARGS) {
	FuncCallContext *funcctx;
	Datum result;
	
	bool nulls[3] = { false, false, false };
	Datum values[3];
	HeapTuple tuple;

	if (SRF_IS_FIRSTCALL()) {
		MemoryContext oldcontext;

		funcctx = SRF_FIRSTCALL_INIT();
		oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

		if (get_call_result_type(fcinfo, NULL, &funcctx->tuple_desc) != TYPEFUNC_COMPOSITE)
			ereport(ERROR, (errcode(ERRCODE_FEATURE_NOT_SUPPORTED), errmsg("function returning record called in context that cannot accept type record")));
		BlessTupleDesc(funcctx->tuple_desc);

		MemoryContextSwitchTo(oldcontext);
	}

	funcctx = SRF_PERCALL_SETUP();

	switch (funcctx->call_cntr) {
	case 0:
		values[0] = PointerGetDatum(cstring_to_text("Vector2D"));
		values[1] = Int32GetDatum((int32) sizeof(Vector2D));
		values[2] = Int32GetDatum((int32) offsetof(Vector2DM, member));

		break;
	case 1:
		values[0] = PointerGetDatum(cstring_to_text("Vector3D"));
		values[1] = Int32GetDatum((int32) sizeof(Vector3D));
		values[2] = Int32GetDatum((int32) offsetof(Vector3DM, member));

		break;
	case 2:
		values[0] = PointerGetDatum(cstring_to_text("Period"));
		values[1] = Int32GetDatum((int32) sizeof(Period));
		values[2] = Int32GetDatum((int32) offsetof(PeriodM, member));

		break;
	case 3:
		values[0] = PointerGetDatum(cstring_to_text("RangeT"));
		values[1] = Int32GetDatum((int32) sizeof(RangeT));
		values[2] = Int32GetDatum((int32) offsetof(RangeTM, member));

		break;
	case 4:
		values[0] = PointerGetDatum(cstring_to_text("PointSP"));
		values[1] = Int32GetDatum((int32) sizeof(PointSP));
		values[2] = Int32GetDatum((int32) offsetof(PointSPM, member));

		break;
	case 5:
		values[0] = PointerGetDatum(cstring_to_text("BoxSP"));
		values[1] = Int32GetDatum((int32) sizeof(BoxSP));
		values[2] = Int32GetDatum((int32) offsetof(BoxSPM, member));

		break;
	case 6:
		values[0] = PointerGetDatum(cstring_to_text("SegmentSP"));
		values[1] = Int32GetDatum((int32) sizeof(SegmentSP));
		values[2] = Int32GetDatum((int32) offsetof(SegmentSPM, member));

		break;
	case 7:
		values[0] = PointerGetDatum(cstring_to_text("LineSP"));
		values[1] = Int32GetDatum((int32) sizeof(LineSP));
		values[2] = Int32GetDatum((int32) offsetof(LineSPM, member));

		break;
	case 8:
		values[0] = PointerGetDatum(cstring_to_text("RangeSP"));
		values[1] = Int32GetDatum((int32) sizeof(RangeSP));
		values[2] = Int32GetDatum((int32) offsetof(RangeSPM, member));

		break;
	case 9:
		values[0] = PointerGetDatum(cstring_to_text("Ellipsoid"));
		values[1] = Int32GetDatum((int32) sizeof(Ellipsoid));
		values[2] = Int32GetDatum((int32) offsetof(EllipsoidM, member));

		break;
	case 10:
		values[0] = PointerGetDatum(cstring_to_text("PointLL"));
		values[1] = Int32GetDatum((int32) sizeof(PointLL));
		values[2] = Int32GetDatum((int32) offsetof(PointLLM, member));

		break;
	case 11:
		values[0] = PointerGetDatum(cstring_to_text("PointPLH"));
		values[1] = Int32GetDatum((int32) sizeof(PointPLH));
		values[2] = Int32GetDatum((int32) offsetof(PointPLHM, member));

		break;
	case 12:
		values[0] = PointerGetDatum(cstring_to_text("PointXYZ"));
		values[1] = Int32GetDatum((int32) sizeof(PointXYZ));
		values[2] = Int32GetDatum((int32) offsetof(PointXYZM, member));

		break;
	case 13:
		values[0] = PointerGetDatum(cstring_to_text("PointENU"));
		values[1] = Int32GetDatum((int32) sizeof(PointENU));
		values[2] = Int32GetDatum((int32) offsetof(PointENUM, member));

		break;
	case 14:
		values[0] = PointerGetDatum(cstring_to_text("PointXY"));
		values[1] = Int32GetDatum((int32) sizeof(PointXY));
		values[2] = Int32GetDatum((int32) offsetof(PointXYM, member));

		break;
	case 15:
		values[0] = PointerGetDatum(cstring_to_text("PointST"));
		values[1] = Int32GetDatum((int32) sizeof(PointST));
		values[2] = Int32GetDatum((int32) offsetof(PointSTM, member));

		break;
	case 16:
		values[0] = PointerGetDatum(cstring_to_text("BoxST"));
		values[1] = Int32GetDatum((int32) sizeof(BoxST));
		values[2] = Int32GetDatum((int32) offsetof(BoxSTM, member));

		break;
	case 17:
		values[0] = PointerGetDatum(cstring_to_text("SegmentST"));
		values[1] = Int32GetDatum((int32) sizeof(SegmentST));
		values[2] = Int32GetDatum((int32) offsetof(SegmentSTM, member));

		break;
	case 18:
		values[0] = PointerGetDatum(cstring_to_text("RangeST"));
		values[1] = Int32GetDatum((int32) sizeof(RangeST));
		values[2] = Int32GetDatum((int32) offsetof(RangeSTM, member));

		break;
	case 19:
		values[0] = PointerGetDatum(cstring_to_text("Trajectory"));
		values[1] = Int32GetDatum(-1);
		values[2] = Int32GetDatum((int32) offsetof(TrajectoryM, member));

		break;
	default:
		SRF_RETURN_DONE(funcctx);
	}

	tuple = heap_form_tuple(funcctx->tuple_desc, values, nulls);
	result = HeapTupleGetDatum(tuple);

	SRF_RETURN_NEXT(funcctx, result);
}
PG_FUNCTION_INFO_V1(hermesDataTypes);

PGDLLEXPORT Datum hermesDataTypesAsXML(PG_FUNCTION_ARGS) {
	char *tmp = NULL;
	xmltype *result = NULL;

	if (asprintfHermes(&tmp, "<HermesDataTypes>\n"
			"	<type id=\"Vector2D\">\n"
			"		<internallength>%zd</internallength>\n"
			"		<alignment>%zd</alignment>\n"
			"	</type>\n"
			"	<type id=\"Vector3D\">\n"
			"		<internallength>%zd</internallength>\n"
			"		<alignment>%zd</alignment>\n"
			"	</type>\n"
			"	<type id=\"Period\">\n"
			"		<internallength>%zd</internallength>\n"
			"		<alignment>%zd</alignment>\n"
			"	</type>\n"
			"	<type id=\"RangeT\">\n"
			"		<internallength>%zd</internallength>\n"
			"		<alignment>%zd</alignment>\n"
			"	</type>\n"
			"	<type id=\"PointSP\">\n"
			"		<internallength>%zd</internallength>\n"
			"		<alignment>%zd</alignment>\n"
			"	</type>\n"
			"	<type id=\"BoxSP\">\n"
			"		<internallength>%zd</internallength>\n"
			"		<alignment>%zd</alignment>\n"
			"	</type>\n"
			"	<type id=\"SegmentSP\">\n"
			"		<internallength>%zd</internallength>\n"
			"		<alignment>%zd</alignment>\n"
			"	</type>\n"
			"	<type id=\"LineSP\">\n"
			"		<internallength>%zd</internallength>\n"
			"		<alignment>%zd</alignment>\n"
			"	</type>\n"
			"	<type id=\"RangeSP\">\n"
			"		<internallength>%zd</internallength>\n"
			"		<alignment>%zd</alignment>\n"
			"	</type>\n"
			"	<type id=\"Ellipsoid\">\n"
			"		<internallength>%zd</internallength>\n"
			"		<alignment>%zd</alignment>\n"
			"	</type>\n"
			"	<type id=\"PointLL\">\n"
			"		<internallength>%zd</internallength>\n"
			"		<alignment>%zd</alignment>\n"
			"	</type>\n"
			"	<type id=\"PointPLH\">\n"
			"		<internallength>%zd</internallength>\n"
			"		<alignment>%zd</alignment>\n"
			"	</type>\n"
			"	<type id=\"PointXYZ\">\n"
			"		<internallength>%zd</internallength>\n"
			"		<alignment>%zd</alignment>\n"
			"	</type>\n"
			"	<type id=\"PointENU\">\n"
			"		<internallength>%zd</internallength>\n"
			"		<alignment>%zd</alignment>\n"
			"	</type>\n"
			"	<type id=\"PointXY\">\n"
			"		<internallength>%zd</internallength>\n"
			"		<alignment>%zd</alignment>\n"
			"	</type>\n"
			"	<type id=\"PointST\">\n"
			"		<internallength>%zd</internallength>\n"
			"		<alignment>%zd</alignment>\n"
			"	</type>\n"
			"	<type id=\"BoxST\">\n"
			"		<internallength>%zd</internallength>\n"
			"		<alignment>%zd</alignment>\n"
			"	</type>\n"
			"	<type id=\"SegmentST\">\n"
			"		<internallength>%zd</internallength>\n"
			"		<alignment>%zd</alignment>\n"
			"	</type>\n"
			"	<type id=\"RangeST\">\n"
			"		<internallength>%zd</internallength>\n"
			"		<alignment>%zd</alignment>\n"
			"	</type>\n"
			"	<type id=\"Trajectory\">\n"
			"		<internallength>%zd</internallength>\n"
			"		<alignment>%zd</alignment>\n"
			"	</type>\n"
			"</HermesDataTypes>", sizeof(Vector2D), offsetof(Vector2DM, member), sizeof(Vector3D), offsetof(Vector3DM, member), sizeof(Period), offsetof(PeriodM, member), sizeof(RangeT), offsetof(RangeTM, member), sizeof(PointSP), offsetof(PointSPM, member), sizeof(BoxSP), offsetof(BoxSPM, member), sizeof(SegmentSP), offsetof(SegmentSPM, member), sizeof(LineSP), offsetof(LineSPM, member), sizeof(RangeSP), offsetof(RangeSPM, member), sizeof(Ellipsoid), offsetof(EllipsoidM, member), sizeof(PointLL), offsetof(PointLLM, member), sizeof(PointPLH), offsetof(PointPLHM, member), sizeof(PointXYZ), offsetof(PointXYZM, member), sizeof(PointENU), offsetof(PointENUM, member), sizeof(PointXY), offsetof(PointXYM, member), sizeof(PointST), offsetof(PointSTM, member), sizeof(BoxST), offsetof(BoxSTM, member), sizeof(SegmentST), offsetof(SegmentSTM, member), sizeof(RangeST), offsetof(RangeSTM, member), -1, offsetof(TrajectoryM, member)) > 0) {

		result = DatumGetXmlP(DirectFunctionCall1(xml_in, CStringGetDatum(tmp)));
		pfree(tmp);

		PG_RETURN_XML_P(result);
	}

	PG_RETURN_NULL();
}
PG_FUNCTION_INFO_V1(hermesDataTypesAsXML);
