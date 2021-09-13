pushd %~dp0

rem 파이썬 패키지인 pyinstaller를 이용하여 파이썬 프로젝트를 하나의 실행파일로 만들기
pyinstaller --onefile PacketGenerator.py

rem 파일 이동하기
MOVE .\dist\PacketGenerator.exe .\GenPackets.exe

rem 폴더 삭제
@RD /S /Q .\build
@RD /S /Q .\dist

rem 파일 삭제
DEL /S /F /Q .\PacketGenerator.spec
PAUSE