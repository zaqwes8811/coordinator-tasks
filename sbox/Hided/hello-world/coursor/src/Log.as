package  
{
	/**
	 * ...
	 * @author i
	 */
	public class Log
	{
		public static var lines : ArrayList = new ArrayList();
		public static const MAX_LINES : int = 100;

		private static function logLine(line : String) : void
		{
			while (lines.length > MAX_LINES)
				lines.removeItemAt(0);
			lines.addItem({"line" : line, "time" : new Date()});
		}

		public static function logDump() : String
		{
			var ret : String = "";
			for each (var entry : Object in lines.source)
			{
				ret = (entry.time as Date).toUTCString() + " " + entry.line + "\n" + ret;
			}
			return ret;
		}

		public static function debug(...args) : void
		{
			trace(args);

			var line : String = "";
			for (var i : int = 0; i < args.length; i++)
				if (args[i] != null)
					line += args[i].toString();
			logLine(line);
		}
	}
}