## cosc

A Windows command-line tool for sending and receiving [OSC packets](https://opensoundcontrol.stanford.edu/).

Created so I could voice control [SongMaster](https://aurallysound.com/) with Alexa using [TRIGGERcmd](https://www.triggercmd.com).
See my [blog post](https://blog.fullmeasure.uk/2023/08/14/practice-with-alexa/).

See [Usage](https://github.com/music-practice-tools/cosc/blob/main/cosc/cosc.cpp#L18).

Based on [sendosc](https://github.com/yoggy/sendosc) by yoggy which uses [packosc](http://www.rossbencina.com/code/oscpack) by rossbencina.

## Building

Only a Windows version has be included, unlike sendosc, as that's all I needed. The code should be portable.

See [Releases](https://github.com/music-practice-tools/cosc/releases) for binary.
