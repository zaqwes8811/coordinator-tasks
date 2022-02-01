package antts;
/**
	TODO :
		рекурсивный обход директорий, попробовать вызвать jpython?
*/
import java.io.File;

public class ListAllFiles {
	public void printListFiles( String path ) {
		// Directory path here
		String files;
		File folder = new File(path);
		File[] listOfFiles = folder.listFiles(); 
		for (int i = 0; i < listOfFiles.length; i++) {
			if (listOfFiles[i].isFile()) {
				files = listOfFiles[i].getName();
				System.out.println(files);
			}
		}
	}
}