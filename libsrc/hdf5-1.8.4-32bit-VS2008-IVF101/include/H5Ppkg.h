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

/*
 * Programmer:	Quincey Koziol <koziol@ncsa.uiuc.edu>
 *		Friday, November 16, 2001
 *
 * Purpose:	This file contains declarations which are visible only within
 *		the H5P package.  Source files outside the H5P package should
 *		include H5Pprivate.h instead.
 */
#ifndef H5P_PACKAGE
#error "Do not include this file outside the H5P package!"
#endif

#ifndef _H5Ppkg_H
#define _H5Ppkg_H

/* Get package's private header */
#include "H5Pprivate.h"

/* Other private headers needed by this file */
#include "H5SLprivate.h"	/* Skip lists				*/

/**************************/
/* Package Private Macros */
/**************************/


/****************************/
/* Package Private Typedefs */
/****************************/

/* Define enum for type of object that property is within */
typedef enum {
    H5P_PROP_WITHIN_UNKNOWN=0,  /* Property container is unknown */
    H5P_PROP_WITHIN_LIST,       /* Property is within a list */
    H5P_PROP_WITHIN_CLASS       /* Property is within a class */
} H5P_prop_within_t;

/* Define enum for modifications to class */
typedef enum {
    H5P_MOD_ERR=(-1),   /* Indicate an error */
    H5P_MOD_INC_CLS,    /* Increment the dependant class count*/
    H5P_MOD_DEC_CLS,    /* Decrement the dependant class count*/
    H5P_MOD_INC_LST,    /* Increment the dependant list count*/
    H5P_MOD_DEC_LST,    /* Decrement the dependant list count*/
    H5P_MOD_INC_REF,    /* Increment the ID reference count*/
    H5P_MOD_DEC_REF,    /* Decrement the ID reference count*/
    H5P_MOD_MAX         /* Upper limit on class modifications */
} H5P_class_mod_t;

/* Define structure to hold property information */
typedef struct H5P_genprop_t {
    /* Values for this property */
    char *name;         /* Name of property */
    size_t size;        /* Size of property value */
    void *value;        /* Pointer to property value */
    H5P_prop_within_t type;     /* Type of object the property is within */
    unsigned shared_name;       /* Boolean: whether the name is shared or not */

    /* Callback function pointers & info */
    H5P_prp_create_func_t create;   /* Function to call when a property is created */
    H5P_prp_set_func_t set; /* Function to call when a property value is set */
    H5P_prp_get_func_t get; /* Function to call when a property value is retrieved */
    H5P_prp_delete_func_t del; /* Function to call when a property is deleted */
    H5P_prp_copy_func_t copy;  /* Function to call when a property is copied */
    H5P_prp_compare_func_t cmp; /* Function to call when a property is compared */
    H5P_prp_close_func_t close; /* Function to call when a property is closed */
} H5P_genprop_t;

/* Define structure to hold class information */
struct H5P_genclass_t {
    struct H5P_genclass_t *parent;     /* Pointer to parent class */
    char *name;         /* Name of property list class */
    size_t  nprops;     /* Number of properties in class */
    unsigned   plists;     /* Number of property lists that have been created since the last modification to the class */
    unsigned   classes;    /* Number of classes that have been derived since the last modification to the class */
    unsigned   ref_count;  /* Number of oustanding ID's open on this class object */
    unsigned   internal;   /* Whether this class is internal to the library or not */
    unsigned   deleted;    /* Whether this class has been deleted and is waiting for dependent classes & proplists to close */
    unsigned   revision;   /* Revision number of a particular class (global) */
    H5SL_t *props;      /* Skip list containing properties */

    /* Callback function pointers & info */
    H5P_cls_create_func_t create_func;  /* Function to call when a property list is created */
    void *create_data;  /* Pointer to user data to pass along to create callback */
    H5P_cls_copy_func_t copy_func;   /* Function to call when a property list is copied */
    void *copy_data;    /* Pointer to user data to pass along to copy callback */
    H5P_cls_close_func_t close_func;   /* Function to call when a property list is closed */
    void *close_data;   /* Pointer to user data to pass along to close callback */
};

/* Define structure to hold property list information */
struct H5P_genplist_t {
    H5P_genclass_t *pclass; /* Pointer to class info */
    hid_t   plist_id;       /* Copy of the property list ID (for use in close callback) */
    size_t  nprops;         /* Number of properties in class */
    unsigned   class_init:1;   /* Whether the class initialization callback finished successfully */
    H5SL_t *del;        /* Skip list containing names of deleted properties */
    H5SL_t *props;      /* Skip list containing properties */
};

/* Function pointer for library classes with properties to register */
typedef herr_t (*H5P_init_class_op_t)(H5P_genclass_t *pclass);
typedef herr_t (*H5P_reg_prop_func_t)(H5P_genclass_t *pclass);

/*
 * Each library property list class has a variable of this type that contains
 * class variables and methods used to initialize the class.
 */
typedef struct H5P_libclass_t {
    const char	*name;		        /* Class name */

    hid_t const * const par_class_id;   /* Pointer to global parent class property list class ID */
    hid_t * const class_id;             /* Pointer to global property list class ID */
    hid_t * const def_plist_id;         /* Pointer to global default property list ID */
    H5P_reg_prop_func_t reg_prop_func;  /* Register class's properties */

    /* Class callback function pointers & info */
    H5P_cls_create_func_t create_func;  /* Function to call when a property list is created */
    void *create_data;                  /* Pointer to user data to pass along to create callback */
    H5P_cls_copy_func_t copy_func;      /* Function to call when a property list is copied */
    void *copy_data;                    /* Pointer to user data to pass along to copy callback */
    H5P_cls_close_func_t close_func;    /* Function to call when a property list is closed */
    void *close_data;                   /* Pointer to user data to pass along to close callback */
} H5P_libclass_t;


/*****************************/
/* Package Private Variables */
/*****************************/


/******************************/
/* Package Private Prototypes */
/******************************/

/* Private functions, not part of the publicly documented API */
H5_DLL H5P_genclass_t *H5P_create_class(H5P_genclass_t *par_class,
    const char *name, unsigned internal,
    H5P_cls_create_func_t cls_create, void *create_data,
    H5P_cls_copy_func_t cls_copy, void *copy_data,
    H5P_cls_close_func_t cls_close, void *close_data);
H5_DLL H5P_genclass_t *H5P_copy_pclass(H5P_genclass_t *pclass);
H5_DLL herr_t H5P_add_prop(H5SL_t *props, H5P_genprop_t *prop);
H5_DLL herr_t H5P_access_class(H5P_genclass_t *pclass, H5P_class_mod_t mod);
H5_DLL htri_t H5P_exist_pclass(H5P_genclass_t *pclass, const char *name);
H5_DLL herr_t H5P_get_size_plist(H5P_genplist_t *plist, const char *name,
    size_t *size);
H5_DLL herr_t H5P_get_size_pclass(H5P_genclass_t *pclass, const char *name,
    size_t *size);
H5_DLL H5P_genclass_t *H5P_get_class(const H5P_genplist_t *plist);
H5_DLL herr_t H5P_get_nprops_plist(const H5P_genplist_t *plist, size_t *nprops);
H5_DLL int H5P_cmp_class(const H5P_genclass_t *pclass1, const H5P_genclass_t *pclass2);
H5_DLL int H5P_cmp_plist(const H5P_genplist_t *plist1, const H5P_genplist_t *plist2);
H5_DLL int H5P_iterate_plist(hid_t plist_id, int *idx, H5P_iterate_t iter_func,
    void *iter_data);
H5_DLL int H5P_iterate_pclass(hid_t pclass_id, int *idx, H5P_iterate_t iter_func,
    void *iter_data);
H5_DLL herr_t H5P_copy_prop_plist(hid_t dst_id, hid_t src_id, const char *name);
H5_DLL herr_t H5P_copy_prop_pclass(H5P_genclass_t *dst_pclass, H5P_genclass_t *src_pclass,
    const char *name);
H5_DLL herr_t H5P_unregister(H5P_genclass_t *pclass, const char *name);
H5_DLL char *H5P_get_class_path(H5P_genclass_t *pclass);
H5_DLL H5P_genclass_t *H5P_open_class_path(const char *path);
H5_DLL H5P_genclass_t *H5P_get_class_parent(const H5P_genclass_t *pclass);
H5_DLL herr_t H5P_close_class(void *_pclass);

/* Testing functions */
#ifdef H5P_TESTING
H5_DLL char *H5P_get_class_path_test(hid_t pclass_id);
H5_DLL hid_t H5P_open_class_path_test(const char *path);
H5_DLL herr_t H5P_reset_external_file_test(hid_t dcpl_id);
#endif /* H5P_TESTING */

#endif /* _H5Ppkg_H */

