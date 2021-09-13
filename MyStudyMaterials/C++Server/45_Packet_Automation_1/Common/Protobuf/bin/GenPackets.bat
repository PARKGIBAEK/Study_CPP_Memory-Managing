rem 은 bat 파일의 주석 문법이다
:: 또한 주석이다

pushd %~dp0 
:: pushd는 현재 디렉터리를 가상 스택에 저장한다.
:: %~dp0은 현재 bat파일의 실행폴더를 받아온다

protoc.exe -I=./ --cpp_out=./ ./Enum.proto 
protoc.exe -I=./ --cpp_out=./ ./Struct.proto
protoc.exe -I=./ --cpp_out=./ ./Protocol.proto

IF ERRORLEVEL 1 PAUSE
:: 1레벨에 해당하는 에러 발생 시 실행 정지

rem ../ 은 현재 경로를 기준으로 상위 폴더를 의미한다
rem  예) ../../../ 라면 상위의 상위의 상위 폴더를 의미
rem XCOPY는 복사 명령어이며 /Y는 덮어쓰기 옵션 적용
XCOPY /Y Enum.pb.h "../../../GameServer"
XCOPY /Y Enum.pb.cc "../../../GameServer"
XCOPY /Y Struct.pb.h "../../../GameServer"
XCOPY /Y Struct.pb.cc "../../../GameServer"
XCOPY /Y Protocol.pb.h "../../../GameServer"
XCOPY /Y Protocol.pb.cc "../../../GameServer"
:: 각파일을 상위 폴더에 복사(덮어쓰기 옵션 켜기)

XCOPY /Y Enum.pb.h "../../../DummyClient"
XCOPY /Y Enum.pb.cc "../../../DummyClient"
XCOPY /Y Struct.pb.h "../../../DummyClient"
XCOPY /Y Struct.pb.cc "../../../DummyClient"
XCOPY /Y Protocol.pb.h "../../../DummyClient"
XCOPY /Y Protocol.pb.cc "../../../DummyClient"

SET /P P=Press any key continue
:: P누르면 종료하라는 메시지를 띄우고 P누르면 종료하기