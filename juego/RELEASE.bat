@echo off
setlocal

set DEST=..\RELEASE

if not exist "%DEST%" mkdir "%DEST%"

echo Limpiando %DEST% (conservando jugar.bat, dosbox-x.conf, dosbox-x.exe)...
for %%F in ("%DEST%\*") do (
    if /I not "%%~nxF"=="jugar.bat" if /I not "%%~nxF"=="dosbox-x.conf" if /I not "%%~nxF"=="dosbox-x.exe" del /Q "%%F"
)

echo Copiando ejecutable...
copy /Y TIMED.EXE "%DEST%\" >nul

echo Copiando fondos de pantalla...
copy /Y PRE_P1.BMP "%DEST%\" >nul
copy /Y PRE_P2.BMP "%DEST%\" >nul
copy /Y PRE_P3.BMP "%DEST%\" >nul
copy /Y PRE_P4.BMP "%DEST%\" >nul
copy /Y PRE_P5.BMP "%DEST%\" >nul
copy /Y PRE_P6.BMP "%DEST%\" >nul
copy /Y PRE_P7.BMP "%DEST%\" >nul
copy /Y PRE_P8.BMP "%DEST%\" >nul
copy /Y PRE_P9.BMP "%DEST%\" >nul
copy /Y MED_P1.BMP "%DEST%\" >nul
copy /Y MED_P2.BMP "%DEST%\" >nul
copy /Y MED_P3.BMP "%DEST%\" >nul
copy /Y FUT_P1.BMP "%DEST%\" >nul
copy /Y FUT_P2.BMP "%DEST%\" >nul
copy /Y FUT_P3.BMP "%DEST%\" >nul

echo Copiando personajes...
copy /Y PLAYER.BMP "%DEST%\" >nul
copy /Y SHAMAN.BMP "%DEST%\" >nul
copy /Y HEAD.BMP "%DEST%\" >nul
copy /Y ERICFRM.BMP "%DEST%\" >nul
copy /Y chafrm.bmp "%DEST%\" >nul

echo Copiando enemigos...
copy /Y BEAR.BMP "%DEST%\" >nul
copy /Y BOAR.BMP "%DEST%\" >nul
copy /Y REPTILE.BMP "%DEST%\" >nul
copy /Y FISH.BMP "%DEST%\" >nul
copy /Y SPLASH.BMP "%DEST%\" >nul
copy /Y DRONE.BMP "%DEST%\" >nul

echo Copiando objetos y puzzles...
copy /Y ROCK.BMP "%DEST%\" >nul
copy /Y ROCKROLL.BMP "%DEST%\" >nul
copy /Y STICK.BMP "%DEST%\" >nul
copy /Y HUEVO.BMP "%DEST%\" >nul
copy /Y EGGICON.BMP "%DEST%\" >nul
copy /Y CUP16.BMP "%DEST%\" >nul
copy /Y CUPH16.BMP "%DEST%\" >nul
copy /Y CUPICON.BMP "%DEST%\" >nul
copy /Y CUPHICON.BMP "%DEST%\" >nul
copy /Y HONEY.BMP "%DEST%\" >nul
copy /Y FIRE.BMP "%DEST%\" >nul
copy /Y LOG.BMP "%DEST%\" >nul
copy /Y LOG_ICON.BMP "%DEST%\" >nul

echo Copiando HUD de fragmentos...
copy /Y F1DIM.BMP "%DEST%\" >nul
copy /Y F1COL.BMP "%DEST%\" >nul
copy /Y F2DIM.BMP "%DEST%\" >nul
copy /Y F2COL.BMP "%DEST%\" >nul
copy /Y F3DIM.BMP "%DEST%\" >nul
copy /Y F3COL.BMP "%DEST%\" >nul

echo Copiando menu...
copy /Y MENUS.BMP "%DEST%\" >nul
copy /Y MENUN.BMP "%DEST%\" >nul

echo Copiando logos e intro...
copy /Y contest.bmp "%DEST%\" >nul
copy /Y h3logo.bmp "%DEST%\" >nul
copy /Y timed.bmp "%DEST%\" >nul
copy /Y INTRO1.BMP "%DEST%\" >nul
copy /Y INTRO2.BMP "%DEST%\" >nul
copy /Y INTRO3.BMP "%DEST%\" >nul
copy /Y INTRO4.BMP "%DEST%\" >nul
copy /Y INTRO5.BMP "%DEST%\" >nul
copy /Y INTRO6.BMP "%DEST%\" >nul
copy /Y INTRO7.BMP "%DEST%\" >nul
copy /Y INTRO8.BMP "%DEST%\" >nul

echo Copiando instrucciones...
copy /Y INSTR1.BMP "%DEST%\" >nul
copy /Y INSTR2.BMP "%DEST%\" >nul
copy /Y INSTR3.BMP "%DEST%\" >nul
copy /Y INSTR4-1.BMP "%DEST%\" >nul
copy /Y INSTR4-2.BMP "%DEST%\" >nul
copy /Y INSTR4-3.BMP "%DEST%\" >nul

echo Copiando pantalla de game over...
copy /Y GAMEOVER.BMP "%DEST%\" >nul

echo Copiando efectos de sonido...
copy /Y jump.wav "%DEST%\" >nul
copy /Y moverock.wav "%DEST%\" >nul
copy /Y woso.wav "%DEST%\" >nul
copy /Y drops.wav "%DEST%\" >nul
copy /Y splash.wav "%DEST%\" >nul
copy /Y PICKUP.WAV "%DEST%\" >nul
copy /Y DROP.WAV "%DEST%\" >nul
copy /Y HURT.WAV "%DEST%\" >nul
copy /Y CONTEST.WAV "%DEST%\" >nul

echo Copiando musica...
copy /Y PRETHEME.XM "%DEST%\" >nul
copy /Y MEDTHEME.XM "%DEST%\" >nul
copy /Y FUTTHEME.XM "%DEST%\" >nul
copy /Y fire.xm "%DEST%\" >nul
copy /Y INTRO.XM "%DEST%\" >nul

echo.
echo Listo. Assets copiados a %DEST%
pause
