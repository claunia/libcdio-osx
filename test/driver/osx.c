/* -*- C -*-
  Copyright (C) 2009, 2011 Rocky Bernstein <rocky@gnu.org>
  Copyright (C) 2012 Natalia Portillo <claunia@claunia.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* 
   Unit test for lib/driver/osx.c
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#define __CDIO_CONFIG_H__ 1
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include <cdio/cdio.h>
#include <cdio/logging.h>

int
main(int argc, const char *argv[])
{
  CdIo_t *p_cdio;
  char **ppsz_drives=NULL;
  int ret_err = 0;
  
  cdio_loglevel_default = (argc > 1) ? CDIO_LOG_DEBUG : CDIO_LOG_INFO;
  /* snprintf(psz_nrgfile, sizeof(psz_nrgfile)-1,
	     "%s/%s", TEST_DIR, cue_file[i]);
  */
  if (!cdio_have_driver(DRIVER_OSX)) return(77);
    
  printf("Starting OS X driver test.\n");
  ppsz_drives = cdio_get_devices(DRIVER_DEVICE);
  if (!ppsz_drives) {
      printf("Can't find a CD-ROM drive. Skipping test.\n");
      exit(77);
  }
  
  p_cdio = cdio_open_osx(ppsz_drives[0]);
  if (p_cdio) {
      const char *psz_source = cdio_get_arg(p_cdio, "source");
      const char *psz_access_mode = cdio_get_arg(p_cdio, "access-mode");
      if (0 != strncmp(psz_source, ppsz_drives[0],
		       strlen(ppsz_drives[0]))) {
	  fprintf(stderr, 
		  "Got %s; should get back %s, the name we opened.\n",
		  psz_source, ppsz_drives[0]);
	  exit(1);
      }
      if (0 != strncmp(psz_access_mode, "OS X", strlen("OS X"))) {
	  fprintf(stderr,
		  "Got %s; Should get back %s, the access mode requested.\n",
		  psz_access_mode, "OS X");
	  exit(2);
      }
      
      printf("Getting default device.\n");
      const char *default_device = cdio_get_default_device(p_cdio);
      if(default_device == NULL)
      {
          fprintf(stderr, "Got no default device.\n");
          exit(3);
      }
      else
          printf("Got default device: \"%s\".\n", default_device);
      
      printf("Getting device capabilities.\n");
      cdio_drive_read_cap_t  i_read_cap;
      cdio_drive_write_cap_t i_write_cap;
      cdio_drive_misc_cap_t  i_misc_cap;
      cdio_get_drive_cap(p_cdio, &i_read_cap, &i_write_cap, &i_misc_cap);
      if((i_read_cap & CDIO_DRIVE_CAP_ERROR) == CDIO_DRIVE_CAP_ERROR ||
         (i_read_cap & CDIO_DRIVE_CAP_UNKNOWN) == CDIO_DRIVE_CAP_ERROR ||
         (i_write_cap & CDIO_DRIVE_CAP_ERROR) == CDIO_DRIVE_CAP_ERROR ||
         (i_write_cap & CDIO_DRIVE_CAP_UNKNOWN) == CDIO_DRIVE_CAP_ERROR ||
         (i_misc_cap & CDIO_DRIVE_CAP_ERROR) == CDIO_DRIVE_CAP_ERROR ||
         (i_misc_cap & CDIO_DRIVE_CAP_UNKNOWN) == CDIO_DRIVE_CAP_ERROR)
      {
          fprintf(stderr, "Error getting capatibilites.\n");
          ret_err = 4;
      }
      printf("Capabilities: 0x%08X (READ), 0x%08X (WRITE), 0x%08X (MISC)\n",
             i_read_cap, i_write_cap, i_misc_cap);
      
      printf("Getting hardware information.\n");
      cdio_hwinfo_t hwinfo;
      if(!cdio_get_hwinfo(p_cdio, &hwinfo))
      {
          fprintf(stderr, "Error getting hardware infomation.\n");
          ret_err = 5;
      }
      else
          printf("Vendor \"%s\", model \"%s\", revision \"%s\".\n",
                 hwinfo.psz_vendor, hwinfo.psz_model, hwinfo.psz_revision);
      
      printf("Getting disc mode.\n");
      discmode_t i_discmode;
      i_discmode = cdio_get_discmode(p_cdio);
      if(i_discmode == CDIO_DISC_MODE_ERROR)
      {
          fprintf(stderr, "Error getting disc mode.\n");
          ret_err = 6;
      }
      else
          printf("Disc mode: 0x%08X.\n", i_discmode);
      
      printf("Getting disc MCN.\n");
      const char *disc_mcn = cdio_get_mcn(p_cdio);
      if(disc_mcn == NULL)
      {
          fprintf(stderr, "Error getting MCN, enable driver DEBUG for more information.\n");
          ret_err = 7;
      }
      else
          printf("MCN: \"%s\".\n", disc_mcn);
      
      printf("Setting drive speed (to an absurd high value).\n");
      driver_return_code_t i_return = cdio_set_speed(p_cdio, 1000000);
      if(i_return != DRIVER_OP_SUCCESS)
      {
          fprintf(stderr, "Error setting speed.\n");
          ret_err = 8;
      }
      else
          printf("Speed set to maximum (or 1,000,000x)");
      
      printf("Getting disc last LSN.\n");
      lsn_t i_lsn = cdio_get_disc_last_lsn(p_cdio);
      if(i_lsn == 0 || i_lsn == CDIO_INVALID_LSN)
      {
          fprintf(stderr, "Error getting disc lst LSN.\n");
          ret_err = 9;
      }
      else
          printf("Disc last LSN: %d.\n", i_lsn);
      
      printf("Getting first track format.\n");
      track_format_t i_track_format = cdio_get_track_format(p_cdio, 1);
      if(i_track_format == TRACK_FORMAT_ERROR)
      {
          fprintf(stderr, "Error getting track format.\n");
          ret_err = 10;
      }
      else
          printf("Track format: 0x%08X.\n", i_track_format);
      
      printf("Getting first track lba.\n");
      lba_t i_lba = cdio_get_track_lba(p_cdio, 1);
      if(i_lba == CDIO_INVALID_LBA)
      {
          fprintf(stderr, "Error getting track lba.\n");
          exit(11);
      }
      else
          printf("Track LBA: %d.\n", i_lba);
      
      if(i_track_format != TRACK_FORMAT_ERROR)
      {
          if(i_track_format == TRACK_FORMAT_AUDIO)
              printf("Going to read 1 AUDIO sector.\n");
          else
              printf("Going to read 1 DATA sector.\n");

          void *p_buf;
          p_buf = malloc(CDIO_CD_FRAMESIZE_RAW);
          if(i_track_format == TRACK_FORMAT_AUDIO)
              i_return = cdio_read_audio_sectors(p_cdio, p_buf, i_lba, 1);
          else
              i_return = cdio_read_data_sectors(p_cdio, p_buf, i_lba, CDIO_CD_FRAMESIZE, 1);
          
          if(i_return != DRIVER_OP_SUCCESS)
          {
              fprintf(stderr, "Error reading sector.\n");
              ret_err = 12;
          }
          else
              printf("Sector read correctly.\n");
      }
  }

  cdio_destroy(p_cdio);
  cdio_free_device_list(ppsz_drives);

  return ret_err;
}
