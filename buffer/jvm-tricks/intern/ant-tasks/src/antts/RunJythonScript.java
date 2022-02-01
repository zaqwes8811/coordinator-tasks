package antts;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Task;
import jsr.EvalFile;

public class RunJythonScript extends Task {
	private String sfile;
	public void execute() throws BuildException {
		EvalFile evalerFile = new EvalFile();
		evalerFile.runScriptNamed( this.sfile );
		 
	}
	// setter для атрибута "file" 
	public void setSFile(String sfile) {
		this.sfile = sfile;
	}
}
