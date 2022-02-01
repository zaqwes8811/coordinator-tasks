package antts;

import java.io.*;
import java.util.*;
import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Task;


import java.text.SimpleDateFormat;

/**
 * Простое задание для сортировки файла 
 */
public class FileSorter extends Task {
    private File file, tofile;
	private String sfile;
    
    // Метод, выполняющий задание
    public void execute() throws BuildException {
		// Напечатать список файлов в директории
		ListAllFiles fileList = new ListAllFiles();
		fileList.printListFiles(".");
		
		// Тести типа входные параметров
		System.out.println( sfile );
	
		// Получить дату последнего изменения файла
		File f = new File( "./input.txt" );
        long datetime = f.lastModified();
        Date d = new Date(datetime);
        SimpleDateFormat sdf = new SimpleDateFormat("dd-MM-yyyy hh:mm:ss");
        String dateString = sdf.format(d);
        System.out.println("The file was last modified on: " + dateString);
		
		// Сортировка в файле
		System.out.println("Sorting file="+file);
        try {
            BufferedReader from =
                new BufferedReader(new FileReader(file));
            BufferedWriter to =
                new BufferedWriter(new FileWriter(tofile));
            List<String> allLines = new ArrayList<String>();
            // прочитать входной файл
            String line = from.readLine();
            while (line != null) {
                allLines.add(line);
                line = from.readLine();
            }
            from.close();
            // отсортировать список
            Collections.sort(allLines);
            // записать отсортированный список 
            for (ListIterator<String> i=allLines.listIterator(); i.hasNext(); ) {
                String s = (String)i.next();
                to.write(s); to.newLine();
            }
            to.close();
        } catch (FileNotFoundException e) {
            throw new BuildException(e);
        } catch (IOException e) {
            throw new BuildException(e);
        }
    }

    // setter для атрибута "file" 
    public void setFile(File file) {
        this.file = file;
    }
    // setter для атрибута "tofile" 
    public void setTofile(File tofile) {
        this.tofile = tofile;
    }
	
	// setter для атрибута "file" 
    public void setSFile(String sfile) {
        this.sfile = sfile;
    }
}