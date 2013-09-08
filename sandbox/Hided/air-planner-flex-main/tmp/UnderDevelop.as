package as_src {
	// Класс обеспечивает интерфейс к хранилищу заметок.
	public class UnderDevelop implements ITODODataBase {
		private var _mapKesh:TODOMap;
		private var BASE_KEY:String = "120319123400";  // 12-03-19 12:34 00
		
		// удаляет запись
		public function DeleteEntry( assumedKey:String ) : void {
			// ! Нет просто меняется флаг! Реально удаляется перед записью в 
			//   статическую базу данных. Просто при отображении пропадает из вида.
			
			// Достать элемент с нулевым окончением
			//   даже если ключ будет не нулевой. Если нулевой, то как быть при 
			//   переименовании.
			var workAssumedKey:String = assumedKey;
			
			// Узнаем число дубляжей. 
			
			// Помещаем удаляемый в конец и отмечаем как 
			//   подлежащий удалению. Нельзя так. Ключ должен остаться иначе
			//   в базе данных потеряется.
			
			//			
		}
		// создает запись
		public function CreateEntry( assumedKey:String, entity:TODOMapPairValue ) : int {
			var workAssumedKey:String = assumedKey;
			// если не хватает длины ключа - дозаполняем из базового
			// если не нули на конце замещаем нулями
			
			// пытаемся вставить запись
			// если она уже есть, то изменяем ключ
			// ..xY(Y+1)
			// нужно ли заглядывать в базу данных? будут ли 
			//  записи в базе адекватны? если учитывать заполнение 
			//  через обчное UI то должно(при подгрузке), если через коммандуную строку
			//  то не факт
			if ( _mapKesh.addItem( workAssumedKey, entity ) == false ) {
				var sliceTODO:TODOMapPairValue = _mapKesh.getTODOValue( workAssumedKey );
				// увеличиваем счетчик ключей
				sliceTODO.incNumChanges();  
				// теперь модифицируем ключ
				var numChanges:int = sliceTODO.getNumChanges();
				var tileKey:String = numChanges.toString();
				var replacedKey:String = workAssumedKey.slice(0, workAssumedKey.length - 1) + tileKey;
				if ( _mapKesh.addItem( replacedKey, entity ) == false ) 
				{
				  // фантастика	
				  // можно рекурсию включить и куда-нить приткется
				}
				_mapKesh.ShowAll();
			} 
			return 0;	
		}
	
		// Перемещение
		public function MoveEntry( srcKey:String, dstKey:String ) : void {
		  // Копируем - нужно понастоящему а не ссылку!
		  
		  // "Удаляем"
		}
		
		//
		public function test( assumedKey:String ) : void 
		{
			var entity:TODOMapPairValue = _mapKesh.getTODOValue( assumedKey );
			trace(entity.getPreority());
			trace(entity.getStringNote());
		}
		//
		public function UnderDevelop()
		{
			_mapKesh = new TODOMap();
		}
	};	
}