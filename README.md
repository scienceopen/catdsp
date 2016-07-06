
# catdsp

1.  Opens the the sound card (/dev/dsp or PulseAudio via `padsp`) at the
    specified sample rate
2.  send acquired waveforms to stdout.


## Build

```sh
make
```

or:

```sh
cc catdsp.c -o catdsp
```

## Usage

```sh
./catdsp
```

* `-d devicename`: Sets device name
* `-m` Sets device to run in mono. Default is stereo.
* `-r rate` Sets device sample rate. Default is 44100 Hz.

### No Sound Data!

The program default is to output to `/dev/dsp` that is the file handle used by OSS.
Since 1999, many Linux OS have moved to ALSA, PulseAudio, or the like.
If you don't get data from:

```sh
./catdsp
```

try the section applying to your OS.

### PulseAudio

Consider *replacing* this program with equivalent functionality
[pacat](http://gnu.wiki/man1/pacat.1.php):

    pacat -r

or if you really do want to use this program with PulseAudio, consider:

    padsp ./catdsp

### Mac and others not working

Perhaps try `sox` as a *replacement* for this program:

    rec -t raw -b 16 -e signed -
