@rem Use delayed expansion or else vars evaluated at start of a () block
@setlocal EnableDelayedExpansion
@
@rem set _debug=true
@rem un rem previous line to debug 
@if [%_debug%]==[] echo off
rem [%1][%2][%3]


goto start

:usage
echo songmaster.cmd - Call Song Master OSC API
echo.
echo  play { on off }
echo  speed { 20 40 50 60 70 80 90 100 120 }
echo  pitch { up down key} { octave 1...n }
echo  mute { on off } { 1 - 5 }
echo  solo { 1 - 5 off }
echo  metronome { on off }
echo  loop { on off bar section note track }
echo  goto { start end next previous } [{bar section note }]
echo  playlist { 1...n info songs next previous }
set exitcode=1
goto exit

:start
set cosc=%~dp0\x64\Release\cosc
set what=%1
set param1=%2
set param2=%3

if [%what%] == [play] (
  if [%param1%] == [on] (
    set value=1
  ) else (
    set value=0
  )
  set cmd=/play
  set params=i !value!

) else if [%what%] == [speed] (
  if [%param1%] == [] (
    set param1=bpm
  )
  if [!param1!] == [bmp] (
    set value=1.0
  ) else if !param1! LEQ 20 (
    set value=0.2
  ) else if !param1! LEQ 40 (
    set value=0.4
  ) else if !param1! EQU 50 ( 
    set value=0.5
  ) else if !param1! LEQ 60 ( 
    set value=0.6 
  ) else if !param1! LEQ 70 ( 
    set value=0.7
  ) else if !param1! LEQ 80 ( 
    set value=0.8
  ) else if !param1! LEQ 90 ( 
    set value=0.9
  ) else if !param1! EQU 100 ( 
    set value=1.0
  ) else if !param1! LEQ 120 (
    set value=1.2
  ) else (
    set value=1.0
  )
  set cmd=/speed
  set params=f !value!

) else if [%what%] == [pitch] (
  if [%param1%] == [key] (
    set sign=+
    set semitones=0
  ) else if [%param1%] == [down] (
    set sign=-
  ) else (
    set sign=+
  )
  if [%param2%] == [octave] (
    set semitones=12
  ) else (
    set semitones=!param2!
  )
  set cmd=/pitch
  set params=i !sign!!semitones!

) else if [%what%] == [mute] (
  if [%param2%] == [] (
    goto usage
  ) else if !param2! LSS 10 (
    set track=%param2%
    if [%param1%] == [on] (
      set state=1
    ) else (
      set state=0
    )
  )
  set cmd=/stemTrack!track!Mute
  set params=i !state!

) else if [%what%] == [solo] (
  if [%param1%] == [] (
    goto usage
  ) else if !param1! LSS 10 (
    set track=%param1%
    set state=1
  ) else if [%param1%] == [off] (
    set track=1
    set state=0
  )
  set cmd=/stemTrack!track!Solo
  set params=i !state!

) else if [%what%] == [metronome] (
  if [%param1%] == [on] (
    set state=1
  ) else (
    set state=0
  )
  set cmd=/metronomeEnable
  set params=i !state!

) else if [%what%] == [loop] (
  if [%param1%] == [] (
    set param1=on
  )
  if [!param1!] == [on] (
    set action=Enable
    set state=1
  ) else if [!param1!] == [off] (
    set action=Enable
    set state=0
  ) else if [!param1!] == [bar] (
    set action=SetToCurrentBar
    set state=1
  ) else if [!param1!] == [section] (
    set action=SetToCurrentSection
    set state=1
  ) else if [!param1!] == [note] (
    set action=SetToCurrentNote
    set state=1
  )
  set cmd=/loop!action!
  set params=i !state!

) else if [%what%] == [goto] (
  if [%param1%] == [start] (
    set action=BeginningOfTrack
  ) else if [%param1%] == [end] (
    set action=EndOfTrack
  ) else if [%param1%] == [next] (    
    set action=Next
  ) else if [%param1%] == [previous] (
    set action=Prev
  )
  if [%param2%] == [bar] (
    set action=!action!Bar
  ) else if [%param2%] == [section] (
    set action=!action!Section
  ) else if [%param2%] == [note] (
    set action=!action!Note
  )
  set cmd=/goto!action!
  set params=i 1

) else if [%what%] == [playlist] (
  if [%param1] == [] (
    goto usage
  ) else if !param1! LEQ 100 (
    set action=PlayIndex
    set trackp=i !param1!
  ) else if [%param1%] == [info] (
    set action=Info
  ) else if [%param1%] == [songs] (
    set action=Songs
  ) else if [%param1%] == [next] (
    set action=PlayNext
  ) else if [%param1%] == [previous] (
    set action=PlayPrev
  )  
  
  set cmd=/playlist!action!
  set params=!trackp! i 1

) else (
  goto usage
)

set host=127.0.0.1
set txport=8000
set rxport=9000
set timeout=500
if NOT [%_debug%] == [] start "" echo [%1][%2][%3] %cosc% !host! !txport! !rxport! !timeout! !cmd! !params!
%cosc% !host! !txport! !rxport! !timeout! !cmd! !params!
set exitcode=0
goto exit

:exit
endlocal
exit /b %exitcode%