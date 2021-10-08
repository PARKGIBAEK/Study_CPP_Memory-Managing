pushd %~dp0

:: 파이썬 패키지인 pyinstaller를 이용하여 파이썬 프로젝트를 하나의 실행파일로 만들기
pyinstaller --onefile PacketGenerator.py

:: 파일 이동하기
MOVE .\dist\PacketGenerator.exe .\GenPackets.exe

:: 폴더 삭제
@RD /S /Q .\build
@RD /S /Q .\dist

:: 파일 삭제
DEL /S /F /Q .\PacketGenerator.spec
