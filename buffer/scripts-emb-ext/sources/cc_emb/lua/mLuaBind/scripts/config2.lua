-- RAII �� ��������, ���� ����� ����������� �� ���� ����� ������
-- 1. ����� �������� ��������� �� ������ - ������ ��������� �� �����!
-- 2. �������� � ��� ��������. ��� ���� � ���������, ����� ������������ �����������?
-- ZZZ ������� ������, � ��� ������� �������. ��� �������� ��������� �� ������ �������?
--local obj = getMgr()--hello('asdf')
local obj = gameobject
local hello = obj.getHello()
--print(obj.DoIt())
print(hello.DoIt())
obj.setCore('asdfd')
obj.setCoreMock('asdfd')
