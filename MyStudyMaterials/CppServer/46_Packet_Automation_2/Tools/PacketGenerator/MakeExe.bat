pushd %~dp0

:: ���̽� ��Ű���� pyinstaller�� �̿��Ͽ� ���̽� ������Ʈ�� �ϳ��� �������Ϸ� �����
pyinstaller --onefile PacketGenerator.py

:: ���� �̵��ϱ�
MOVE .\dist\PacketGenerator.exe .\GenPackets.exe

:: ���� ����
@RD /S /Q .\build
@RD /S /Q .\dist

:: ���� ����
DEL /S /F /Q .\PacketGenerator.spec
