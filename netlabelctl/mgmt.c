/*
 * Management Functions
 *
 * Author: Paul Moore <paul.moore@hp.com>
 *
 */

/*
 * (c) Copyright Hewlett-Packard Development Company, L.P., 2006
 *
 * This program is free software;  you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY;  without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 * the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program;  if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <linux/netlabel.h>

#include <libnetlabel.h>

#include "netlabelctl.h"

/**
 * mgmt_protocols - Display a list of the kernel's NetLabel protocols
 *
 * Description:
 * Request the kernel's supported NetLabel protocols and display the list to
 * the user.  Returns zero on success, negative values on failure.
 *
 */
int mgmt_protocols(void)
{
  int ret_val;
  unsigned int *list;
  size_t count;
  unsigned int iter;

  ret_val = nlbl_mgmt_modules(0, &list, &count);
  if (ret_val < 0)
    return ret_val;

  if (opt_pretty)
    printf("Kernel NetLabel protocols : "); 
  for (iter = 0; iter < count; iter++) {
    switch (list[iter]) {
    case NETLBL_NLTYPE_UNLABELED:
      printf("UNLABELED");
      break;
    case NETLBL_NLTYPE_RIPSO:
      printf("RIPSO");
      break;
    case NETLBL_NLTYPE_CIPSOV4:
      printf("CIPSOv4");
      break;
    case NETLBL_NLTYPE_CIPSOV6:
      printf("CIPSOv6");
      break;
    default:
      printf("UNKNOWN(%u)", list[iter]);
      break;
    }
    if (iter + 1 < count)
      printf(" ");
  }
  printf("\n");

  free(list);
  return 0;
}

/**
 * mgmt_version - Display the kernel's NetLabel version
 *
 * Description:
 * Request the kernel's NetLabel version string and display it to the user.
 * Returns zero on success, negative values on failure.
 *
 */
int mgmt_version(void)
{
  int ret_val;
  unsigned int version;

  ret_val = nlbl_mgmt_version(0, &version);
  if (ret_val < 0)
    return ret_val;

  if (opt_pretty)
    printf("Kernel NetLabel version : "); 
  printf("%u\n", version);

  return 0;
}

/**
 * mgmt_main - Entry point for the NetLabel management functions
 * @argc: the number of arguments
 * @argv: the argument list
 *
 * Description:
 * Parses the argument list and performs the requested operation.  Returns zero
 * on success, negative values on failure.
 *
 */
int mgmt_main(int argc, char *argv[])
{
  int ret_val;

  /* sanity checks */
  if (argc <= 0 || argv == NULL || argv[0] == NULL)
    return -EINVAL;

  /* handle the request */
  if (strcmp(argv[0], "version") == 0) {
    /* kernel version */
    ret_val = mgmt_version();
  } else if (strcmp(argv[0], "protocols") == 0) {
    /* module list */
    ret_val = mgmt_protocols();
  } else {
    /* unknown request */
    fprintf(stderr, "error[mgmt]: unknown command\n");
    ret_val = -EINVAL;
  }

  return ret_val;
}
