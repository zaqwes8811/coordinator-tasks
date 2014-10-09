package as_src 
{
	
	/***/
	public interface ITODODataBase 
	{
		// Создает запись по предполагаемому адресу указанному адресу.
		function CreateEntry( assumedKey:String, entity:TODOMapPairValue ) : int;
		
		// "Удаляет" из базы.
		// Просто меняет маркер, а фактическое удаление 
		//   происходит при сохранении в базе.
		function DeleteEntry( Key:String ) : void;
		
		// Перемещение записи
		//function MoveEntry( srcKey:String, dstKey:String ) : void;
		
		// Получить запись из кэша. Должен возвращать ссылку (проверть)!
		// Должна уметь подгружать из базы данных.
		function GetEntry( Key:String ) : Array;
		
		// Обновить запись. Меняет только ключ.
		//function UpdateEntry( Key:String ) : void;
				
		// Сохраняем кэш в постоянной базе данных
		// Преимущественно будет по таймеру сохранятся(private). Но при закрытии 
		//   нужно тоже вызывать(public).
		//function SaveMapInDataBase() : void;
		
		// Загружаем в кеш данные из базы. Вот эта скроее всего будет скрытой
		//   нужно будет отсюда убрать.
		//function LoadSliceDBToMap() : void;
		
		// // //
		// Просто для тестирования
		function test( assumedKey:String ) : void;
	}
}
/**
 * Формат ключа - UNIX-дата и двузначный хвост для повторяющихся
 * по времени заметок. 10-01-12 16:12 -> 100121612
 * ! пусть пока что просто String - преждевременно не буду оптимизировать
 *   но можно использоват хеширование если будет реальный выигрыш давать
 * */