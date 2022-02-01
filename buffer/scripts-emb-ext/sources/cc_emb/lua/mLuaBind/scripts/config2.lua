-- RAII не работает, хотя может смартпоитер из локи может помочь
-- 1. нужно получить указатель на объект - ничего создавать не нужно!
-- 2. провести с ним операции. как быть с удалением, может смартпоинтер попробовать?
-- ZZZ создали отъект, в нем открыли интерпр. как передать указатель на объект интерпр?
--local obj = getMgr()--hello('asdf')
local obj = gameobject
local hello = obj.getHello()
--print(obj.DoIt())
print(hello.DoIt())
obj.setCore('asdfd')
obj.setCoreMock('asdfd')
