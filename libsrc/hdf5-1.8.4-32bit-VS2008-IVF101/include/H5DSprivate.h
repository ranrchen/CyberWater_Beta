/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF5.  The full HDF5 copyright notice, including     *
 * terms governing use, modification, and redistribution, is contained in    *
 * the files COPYING and Copyright.html.  COPYING can be found at the root   *
 * of the source code distribution tree; Copyright.html can be found at the  *
 * root level of an installed copy of the electronic HDF5 document set and   *
 * is linked from the top-level documents page.  It can also be found at     *
 * http://hdfgroup.org/HDF5/doc/Copyright.html.  If you do not have          *
 * access to either file, you may request a copy from help@hdfgroup.org.     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef _H5DSprivate_H
#define _H5DSprivate_H

/* High-level library internal header file */
#include "H5HLprivate2.h"

/* public LT prototypes			*/
#include "H5DSpublic.h"




/* attribute type of a DS dataset */
typedef struct ds_list_t {
 hobj_ref_t ref;     /* object reference  */
 int        dim_idx; /* dimension index of the dataset */
} ds_list_t;


/*-------------------------------------------------------------------------
 * private functions
 *-------------------------------------------------------------------------
 */

H5_HLDLL herr_t  H5DS_is_reserved( hid_t did);

#endif

