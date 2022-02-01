package antts;

import java.io.*;
import java.util.*;
import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Task;


import java.text.SimpleDateFormat;

/**
 * ������� ������� ��� ���������� ����� 
 */
public class FileSorter extends Task {
    private File file, tofile;
	private String sfile;
    
    // �����, ����������� �������
    public void execute() throws BuildException {
		// ���������� ������ ������ � ����������
		ListAllFiles fileList = new ListAllFiles();
		fileList.printListFiles(".");
		
		// ����� ���� ������� ����������
		System.out.println( sfile );
	
		// �������� ���� ���������� ��������� �����
		File f = new File( "./input.txt" );
        long datetime = f.lastModified();
        Date d = new Date(datetime);
        SimpleDateFormat sdf = new SimpleDateFormat("dd-MM-yyyy hh:mm:ss");
        String dateString = sdf.format(d);
        System.out.println("The file was last modified on: " + dateString);
		
		// ���������� � �����
		System.out.println("Sorting file="+file);
        try {
            BufferedReader from =
                new BufferedReader(new FileReader(file));
            BufferedWriter to =
                new BufferedWriter(new FileWriter(tofile));
            List<String> allLines = new ArrayList<String>();
            // ��������� ������� ����
            String line = from.readLine();
            while (line != null) {
                allLines.add(line);
                line = from.readLine();
            }
            from.close();
            // ������������� ������
            Collections.sort(allLines);
            // �������� ��������������� ������ 
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

    // setter ��� �������� "file" 
    public void setFile(File file) {
        this.file = file;
    }
    // setter ��� �������� "tofile" 
    public void setTofile(File tofile) {
        this.tofile = tofile;
    }
	
	// setter ��� �������� "file" 
    public void setSFile(String sfile) {
        this.sfile = sfile;
    }
}