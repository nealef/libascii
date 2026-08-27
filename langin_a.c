/**
 * @file langin_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the langinfo functions.
 * 
 * Notes	:	All the procedures are name "__xxxxxxxx_a" where
 *				xxxxxxxx is the name of the standard C run-time
 *				function name. Unless otherwise noted, all functions
 * 				take the same argument,produce the same output and
 *				return the same values as the standard functions.
 */

/********************************************************************
 * Copyright:   Licensed Materials - Property of IBM.               *
 *              (C) Copyright IBM Corp. 1997.                       *
 *              All rights reserved.                                *
 ********************************************************************/

#pragma langlvl( extended )
 
#include <langinfo.h>
#include "global_a.h"
 
#pragma export(__nl_langinfo_a)
 
#pragma map(__nl_langinfo_a, "\174\174A00077")

#ifndef __SUSV3_WITHDRAWN
# define _YESSTR_E 56
# define _NOSTR_E 57
#endif

static int itemMap[] = { _D_T_FMT_E, _D_FMT_E, _T_FMT_E, 
                         _AM_STR_E, _PM_STR_E, 
                         _ABDAY_1_E, _ABDAY_2_E, _ABDAY_3_E,
                         _ABDAY_4_E, _ABDAY_5_E, _ABDAY_6_E, _ABDAY_7_E,
                         _DAY_1_E, _DAY_2_E, _DAY_3_E, _DAY_4_E,
                         _DAY_5_E, _DAY_6_E, _DAY_7_E,
                         _ABMON_1_E, _ABMON_2_E, _ABMON_3_E, _ABMON_4_E,
                         _ABMON_5_E, _ABMON_6_E, _ABMON_7_E, _ABMON_8_E,
                         _ABMON_9_E, _ABMON_10_E, _ABMON_11_E, _ABMON_12_E,
                         _MON_1_E, _MON_2_E, _MON_3_E, _MON_4_E,
                         _MON_5_E, _MON_6_E, _MON_7_E, _MON_8_E,
                         _MON_9_E, _MON_10_E, _MON_11_E, _MON_12_E,
                         _RADIXCHAR_E, _THOUSEP_E, _YESEXPR_E, _NOEXPR_E,
                         _CRNCYSTR_E, _CODESET_E, _T_FMT_AMPM_E, 
                         _ERA_E, _ERA_D_FMT_E, _ERA_D_T_FMT_E,
                         _ALT_DIGITS_E, _YESEXPR_E, _NOEXPR_E };

/*%PAGE																*/
/**
 * @brief Retrieve Locale Information
 */
char *
__nl_langinfo_a(nl_item item)
{
    int newItem = (int) item - 1;

    if (newItem >= 0 && newItem < sizeof(itemMap)) {
        return __getAstring1_a(nl_langinfo(itemMap[newItem]));
    }
    return NULL;
}
