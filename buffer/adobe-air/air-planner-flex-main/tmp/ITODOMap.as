package as_src 
{
	/***/
	public interface ITODOMap 
	{
		// Показываем всю стркутуру данных
		function ShowAll() : void;
		// работа уже созданными записями
		// данные должны быть актуальными и ?видимыми на интерфейсе пользователя
		// !для коммандной строки придется сделать полный поиск
		function getTODOValue( key:String ) : TODOMapPairValue;
		// удаляем запись по ключу
		function removeItem( key:String ) : Boolean ;
		// создаем запись по адресу
		function addItem( key:String, value:TODOMapPairValue ) : Boolean;
	}
	
}