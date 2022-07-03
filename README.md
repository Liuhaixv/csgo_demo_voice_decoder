# csgo_demo_voice_decoder
 Extract encoded voice from voicedata in CS:GO .dem demo file and decode it from .raw to .wav
 
## Compile
Visual Studio release x86
## Usage
1.Get encoded **`voicedata`** from your favourite **csgo demo parser**.I used [demofile](https://github.com/saul/demofile)

2.Decode voicedata using compiled **decode_voicedata.exe**, you can grab multi encoded voicedata files onto .exe and they will all be decoded into .raw files.

3.Convert .raw audio files to .wav or other audio files using [sox](http://sox.sourceforge.net/) with command line
```
-t raw -r 22050 -b 16 -c 1 -L out.raw out.wav
```
(replace out.raw with your filename)

## Credits
@ericek111 `https://gist.github.com/ericek111/abe5829f6e52e4b25b3b97a0efd0b22b`

celt-0.11.0 `https://github.com/mumble-voip/celt-0.11.0`
