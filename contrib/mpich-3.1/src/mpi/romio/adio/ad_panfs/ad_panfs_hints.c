/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */
/* 
 *   ad_panfs_hints.c
 *
 *   Copyright (C) 2001 University of Chicago.
 *   See COPYRIGHT notice in top-level directory.
 */

#include "ad_panfs.h"
#include <pan_fs_client_cw_mode.h>
#include "hint_fns.h"

void ADIOI_PANFS_SetInfo(ADIO_File fd, MPI_Info users_info, int *error_code)
{
#if defined(MPICH) || !defined(PRINT_ERR_MSG)
    static char myname[] = "ADIOI_PANFS_SETINFO";
#endif
    char* value;
    int flag, tmp_val = -1;
    unsigned long int concurrent_write = 0; 
    pan_fs_client_layout_agg_type_t layout_type = PAN_FS_CLIENT_LAYOUT_TYPE__DEFAULT;
    unsigned long int layout_stripe_unit = 0;
    unsigned long int layout_parity_stripe_width = 0;
    unsigned long int layout_parity_stripe_depth = 0; 
    unsigned long int layout_total_num_comps = 0;
    pan_fs_client_layout_visit_t layout_visit_policy  = PAN_FS_CLIENT_LAYOUT_VISIT__ROUND_ROBIN;
    int gen_error_code;

    *error_code = MPI_SUCCESS;

    if (fd->info == MPI_INFO_NULL) {
	    /* This must be part of the open call. can set striping parameters 
         * if necessary. 
         */ 
	    MPI_Info_create(&(fd->info));

        /* has user specified striping parameters 
               and do they have the same value on all processes? */
        if (users_info != MPI_INFO_NULL) {

	    ADIOI_Info_check_and_install_int(fd, users_info, "panfs_concurrent_write",
		    NULL, myname, error_code);

	    ADIOI_Info_check_and_install_int(fd, users_info, "panfs_layout_type",
		    NULL, myname, error_code);

	    ADIOI_Info_check_and_install_int(fd, users_info, "panfs_layout_stripe_unit",
		    NULL, myname, error_code);

	    /* strange: there was a check "layout_type ==
	     * PAN_FS_CLIENT_LAYOUT_TYPE__RAID1_5_PARITY_STRIPE, but
	     * nothing ever touched layout_type  */
	    ADIOI_Info_check_and_install_int(fd, users_info,
		    "panfs_layout_parity_stripe_width", NULL, myname, error_code);

	    ADIOI_Info_check_and_install_int(fd, users_info,
		    "panfs_layout_parity_stripe_depth", NULL, myname, error_code);

	    ADIOI_Info_check_and_install_int(fd, users_info,
		    "panfs_layout_total_num_comps", NULL, myname, error_code);
	    /* this hint used to check for
	     * PAN_FS_CLIENT_LAYOUT_TYPE__RAID1_5_PARITY_STRIPE or
	     * PAN_FS_CLIENT_LAYOUT_TYPE__RAID10, but again, layout_type never
	     * gets updated */
	    ADIOI_Info_check_and_install_int(fd, users_info,
		    "panfs_layout_visit_policy", NULL, myname, error_code);
        }
    }

    ADIOI_GEN_SetInfo(fd, users_info, &gen_error_code); 
    /* If this function is successful, use the error code returned from ADIOI_GEN_SetInfo
     * otherwise use the error_code generated by this function
     */
    if(*error_code == MPI_SUCCESS)
    {
        *error_code = gen_error_code;
    }
}
