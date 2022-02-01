/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package jsr;

import java.util.logging.Level;
import java.util.logging.Logger;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 *
 * @author zaqwes
 */
public class PyCallerTest {
    /**
     * Test of run_py_function method, of class PyCaller.
     */
    @Test
    public void testPy_function() {
        System.out.println("py_function");

        // Исходные данные
        String fname = "scripts/for_test_caller.py";
        String function_name = "string_string";
        String arg = "10";
        PyCaller instance = new PyCaller();
        String expResult = "10";
        
        // Все нормально
        String result = "";
        try {
            result = instance.run_py_function(fname, function_name, arg);
            assertEquals(expResult, result);
        } catch (JSRException ex) {
            print(ex.msg);
        }

        // Не найден файл
        fname = "scripts_tmp/for_test_caller.py";
        try {
            result = instance.run_py_function(fname, function_name, arg);
        } catch (JSRException ex) {
            assertEquals(ex.msg, "error");
        }

        // Не нашлось имени функции
        function_name = "unknown";
        try {
            result = instance.run_py_function(fname, function_name, arg);
        } catch (JSRException ex) {
            assertEquals(ex.msg, "error");
        }

        function_name = "err_in_script";
        try {
            result = instance.run_py_function(fname, function_name, arg);
        } catch (JSRException ex) {
            assertEquals(ex.msg, "error");
        }
    }

    private void print(String msg) {
        System.out.println(msg);

    }
    
    /*@Test(expected = ArithmeticException.class)  
	public void divisionWithException() {  
	  int i = 1/0;
	}*/
}