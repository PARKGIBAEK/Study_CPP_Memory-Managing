:: rem �� bat ������ �ּ� �����̴�
:: ���� �ּ��̴�

pushd %~dp0 
:: pushd�� ���� ���͸��� ���� ���ÿ� �����Ѵ�.
:: %~dp0�� ���� bat������ ���������� �޾ƿ´�

protoc.exe -I=./ --cpp_out=./ ./Enum.proto 
protoc.exe -I=./ --cpp_out=./ ./Struct.proto
protoc.exe -I=./ --cpp_out=./ ./Protocol.proto

IF ERRORLEVEL 1 PAUSE
:: 1������ �ش��ϴ� ���� �߻� �� ���� ����

:: ../ �� ���� ��θ� �������� ���� ������ �ǹ��Ѵ�
::  ��) ../../../ ��� ������ ������ ���� ������ �ǹ�
:: XCOPY�� ���� ��ɾ��̸� /Y�� ����� �ɼ� ����
XCOPY /Y Enum.pb.h "../../../GameServer"
XCOPY /Y Enum.pb.cc "../../../GameServer"
XCOPY /Y Struct.pb.h "../../../GameServer"
XCOPY /Y Struct.pb.cc "../../../GameServer"
XCOPY /Y Protocol.pb.h "../../../GameServer"
XCOPY /Y Protocol.pb.cc "../../../GameServer"
:: �������� ���� ������ ����(����� �ɼ� �ѱ�)

XCOPY /Y Enum.pb.h "../../../DummyClient"
XCOPY /Y Enum.pb.cc "../../../DummyClient"
XCOPY /Y Struct.pb.h "../../../DummyClient"
XCOPY /Y Struct.pb.cc "../../../DummyClient"
XCOPY /Y Protocol.pb.h "../../../DummyClient"
XCOPY /Y Protocol.pb.cc "../../../DummyClient"

:: P������ �����϶�� �޽����� ���� P������ �����ϱ�
:: SET /P P=Press any key continue
