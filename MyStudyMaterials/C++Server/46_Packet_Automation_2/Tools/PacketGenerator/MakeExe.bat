pushd %~dp0

rem ���̽� ��Ű���� pyinstaller�� �̿��Ͽ� ���̽� ������Ʈ�� �ϳ��� �������Ϸ� �����
pyinstaller --onefile PacketGenerator.py

rem ���� �̵��ϱ�
MOVE .\dist\PacketGenerator.exe .\GenPackets.exe

rem ���� ����
@RD /S /Q .\build
@RD /S /Q .\dist

rem ���� ����
DEL /S /F /Q .\PacketGenerator.spec
PAUSE