/* 
 *  
 *  CATDSP
 *  
 *  Opens the the sound card (ie. /dev/dsp) sets the sample rate and 
 *  sends audio out to STDOUT.
 *  
 *  Copyright (C) 2000 Timothy Pozar pozar@lns.com
 *  
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *  
 *  $Id: catdsp.c,v 1.2 2000/02/20 01:31:51 pozar Exp $
 *  
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <machine/soundcard.h>

#define TRUE 1
#define FALSE 0
#define BUF_SIZE 4096

char device[BUF_SIZE] = "/dev/dspW";
int stereo = TRUE;
int rate = 32000;
int devfh;

int main(argc,argv)
int argc;
char *argv[];
{
char buf[BUF_SIZE];
int gotmask, setmask, i, len, test;

   /* scan command line arguments, and look for files to work on. */
   for (i = 1; i < argc; i++) {
      switch (argv[i][1]){   /* be case indepenent... */
         case 'D':   /* dsp file name... */
         case 'd':
            i++;
            strncpy(device,argv[i],BUF_SIZE);
            break;
         case 'H':   /* Help... */
         case 'h':
             banner();
            exit(0);
         case 'M':   /* Run in mono... */
         case 'm':
            stereo = FALSE;
            break;  
         case 'R':   /* sample rate... */
         case 'r':
            i++;
            rate = atoi(argv[i]);
            break;
         default:
            printf("I don't know the meaning of the command line argument: \"%s\".\n",argv[i]);
            banner();
            exit(1);
      }
   }

   if((devfh = open(device, O_RDONLY)) == -1){
      perror("opening dsp device");
      exit(1);
   }

   /* What formats does this device support? */
   if(ioctl(devfh, SNDCTL_DSP_GETFMTS, &gotmask) == -1){
      perror("get dsp mask");
      exit(1);
   }

   /* Set the number or channels (ie mono vs. stereo)...
      Always set stereo/mono first before sample rate. 
      See http://www.4front-tech.com/pguide/audio.html for details. */
   test = stereo;
   if(ioctl(devfh, SNDCTL_DSP_STEREO, &stereo) == -1){
      perror("Tried to set dsp to mono or stereo");
      exit(1);
   }
   if (stereo != test){
      if(stereo){
         perror("Tried to set dsp to mono but it only supports stereo.\n");
      } else {
         perror("Tried to set dsp to stereo but it only supports mono.\n");
      }
      exit(1);
   }

   /* Set the sample rate... */
   test = rate;
   if(ioctl( devfh, SNDCTL_DSP_SPEED, &test) == -1){
      perror("set sample rate");
      exit(1);
   }
   if(rate != test){
      printf("Could not set the sample rate to: \"%i\". \"%i\" was returned\n",
         rate, test);
   }

   /* read the device and copy to stdout (1) ... */
   while((len = read(devfh, &buf, sizeof(buf)))>0){ 
      if(write(1, &buf, len) == -1){
         perror("Trouble writing to STDOUT.");
         exit(1);
      }
   }

   return 0;
}

banner()
{
   printf("catdsp: Set \"%s\" sample rate and stereo/mono mode,\n",device);
   printf("        and then read the device and send it to STDOUT.\n",device);
   printf("   -d device   Sets device name.  Default is \"%s\".\n",device);
   printf("   -m          Sets device to run in mono.  Default is stereo.\n");
   printf("   -r rate     Sets device sample rate.  Default is \"%i\".\n",rate);
   return;
}
