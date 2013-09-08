package  as_src
{
	// Единичная запись
	public class TODOMapPairValue 
	{
		/// /// ///
		/// 
		/// Атрибуты
		/* Служебные */
		// Число изменений нулевого ключа. Допустимый диапазон - 0-99
		public function incNumChanges() : void {  _numChanges++; _numChanges %= 100; }
		private var _numChanges : int;
		public function getNumChanges() : int { return _numChanges; }
		
		// номер записи. 0-по ключу одна запись
		// если больше одной ищим объект с максимальной величиной
		// и присваиваем рассматриваемому MAX+1
		public var _numberEntry:int;  // ->SQL
		
		/* Для пользователей */
		// Преоритетность записи
		public function setPreority ( value:int ) : void { _preority = value; }
		private var _preority : int;   // 0-2 ->SQL
		public function getPreority() : int { return _preority; }
		
		// метка удаления - фактически удаляется либо при
		// сохранении в базе данных, либо по завершению программы 
		// удалится вместе с кэшем
		public var _toRemove:Boolean;

		/// /// ///
		/// 
		/// Контент
		// Текстовая запись
		public function setStringNote( stringNote:String ) : void { _stringNote = stringNote;  }
		private var _stringNote : String; // ->SQL
		public function getStringNote() : String { return _stringNote;  }
		
		/// /// ///
		///
		/// Стандартный набор
		public function TODOMapPairValue() {
			_preority = 0; // нормальный
			_numChanges = 0;
			_numberEntry = 0;
		}
	}
}